from copy import deepcopy


class Grammar:
    def __init__(self, grammar_filepath):
        self.grammar = dict()
        self.load_grammar(grammar_filepath)
        self.non_terminals = self.get_non_terminals()
        self.initial = self.non_terminals[0]
        self.terminals = self.get_terminals()

    def load_grammar(self, filepath) -> dict:
        opened_file = open(filepath)

        while True:
            line = opened_file.readline()
            if not line:
                break
            if line.find("->") > 0:

                cabeca, corpo = line.split("->")

                corpo = corpo.strip()
                cabeca = cabeca.strip()

                if cabeca not in self.grammar:
                    self.grammar[cabeca] = [[x.strip() for x in corpo.split()]]
                else:
                    self.grammar[cabeca].append([x.strip() for x in corpo.split()])

        return self.grammar

    def get_non_terminals(self) -> list:
        return list(self.grammar.keys())

    def get_terminals(self) -> list:
        terminals = []
        for corpos in self.grammar.values():

            for corpo in corpos:
                for simbolo in corpo:
                    if simbolo not in self.non_terminals and simbolo not in terminals:
                        terminals.append(simbolo)
        return terminals
    
    def __str__(self) -> str:
        data = ""
        for cabeca, corpos in self.grammar.items():
            data += f"{cabeca} -> {' | '.join([' '.join(corpo) for corpo in corpos])}\n"
        return data[:-1]

    def get_firsts(self) -> dict:
        firsts = {NT: set() for NT in self.non_terminals}
        old_firsts = dict()
        
        """
            Repete até não haver alterações (resolve gramáticas com
            recursividade a esquerda)
        """
        while old_firsts != firsts:
            old_firsts = deepcopy(firsts)
            
            """
                Para cada corpo de produção, para cada simbolo no corpo: 
                o First da cabeça tem o First do simbolo (se n terminal) ou
                o próprio simbolo (se terminal). Se o First do simbolo tem
                &, então adiciona o First do próximo, e assim por diante. Caso
                percorra todo o corpo, adiciona & ao First da cabeça.
            """
            for cabeca, corpos in self.grammar.items():
                for corpo in corpos:
                    for i in range(len(corpo)):
        
                        simbolo = corpo[i]

                        if simbolo in self.terminals:
                            firsts[cabeca].add(simbolo)
                            break
        
                        if simbolo in self.non_terminals:
                            f = firsts[simbolo]
                            firsts[cabeca] = firsts[cabeca].union(f.difference({"&"}))
                            
                            if "&" not in f:
                                break
                            
                            if i == len(corpo) - 1:
                                firsts[cabeca].add("&")
        
        return firsts

    def get_follows(self, firsts) -> dict:
        follows = {NT: set() for NT in self.non_terminals}
        old_follows = deepcopy(follows)
        
        """1. Add $ pro follow do inicial;"""
        follows[self.initial].add("$")

        """
           Repete até não haver alterações
        """
        while follows != old_follows:
            old_follows = deepcopy(follows)

            """
                2. Para cada corpo de produção, para cada simbolo no corpo: Se 
                o simbolo for um n-terminal, o seu follow tem o first do próximo 
                simbolo (se n-terminal) ou o próprio simbolo (se terminal). Caso 
                o first do próximo tenha &, o first do seguinte é adicionado tbm,
                e assim por diante.
            """
            for _, corpos in self.grammar.items():
                for corpo in corpos:
                    for i in range(len(corpo)-1):
                        if corpo[i] in self.non_terminals:
                            for j in range(i+1, len(corpo)):
                                if corpo[j] in self.non_terminals:
                                    first_do_next = firsts[corpo[j]].difference({"&"})
                                    
                                    follows[corpo[i]] = follows[corpo[i]].union(first_do_next)
                                    
                                    if "&" not in firsts[corpo[j]]:
                                        break
                                    
                                elif corpo[j] in self.terminals:
                                    follows[corpo[i]].add(corpo[j])
                                    break

            """ 
                3. Para cada corpo de produção, para cada simbolo a partir do 
                fim do corpo: o Follow da cabeça, é adicionado no Follow do último 
                simbolo (se um n-terminal). Caso o último simbolo tenha & no 
                First, adiciona o Follow da cabeça no Follow do simbolo anterior 
                também, e assim por diante.
            """
            for cabeca, corpos in self.grammar.items():
                for corpo in corpos:
                    for i in range(len(corpo)-1,-1,-1):
                        if corpo[i] in self.non_terminals and corpo[i] != cabeca:
                            follows[corpo[i]] = follows[corpo[i]].union(follows[cabeca])
        
                        if corpo[i] in self.terminals or "&" not in firsts[corpo[i]]:
                            break

        return follows

    def is_ll1(self) -> bool:

        def get_first_body(body: list):
            first = set()
            for i in range(len(body)):

                simbolo = body[i]

                if simbolo in self.terminals:
                    first.add(simbolo)
                    break

                if simbolo in self.non_terminals:
                    f = firsts[simbolo]
                    first = first.union(f.difference({"&"}))
                    
                    if "&" not in f:
                        break
                    
                    if i == len(body) - 1:
                        firsts.add("&")
            return first

        if self.is_left_recursive():
            print("Erro: essa gramática não é LL1. Recursiva à esquerda")
            return False
        
        is_ll1 = True
        firsts = self.get_firsts()
        follows = self.get_follows(firsts)

        for cabeca, corpos in self.grammar.items():
            #print("------------")
            #print(f"{cabeca} -> {' | '.join([' '.join(corpo) for corpo in corpos])}\n")
            for i1 in range(len(corpos)-1):
                for i2 in range(i1+1,len(corpos)):
                    alfa = corpos[i1]
                    beta = corpos[i2]
                    first_alfa = get_first_body(alfa)
                    first_beta = get_first_body(beta)

                    if first_alfa.intersection(first_beta) != set():
                        print("Erro: essa gramática não é LL1\n" + \
                             f"\t{cabeca} -> {" ".join(alfa)} | {" ".join(beta)}\n" + \
                             f"\tFirst({alfa}) = {first_alfa}\n" + \
                             f"\tFirst({beta}) = {first_beta}\n" + \
                             f"\tFirst({alfa}) ∩ First({beta}) = {first_alfa.intersection(first_beta)}")
                        is_ll1 = False
                    
                    if "&" in first_alfa:
                        if first_alfa.intersection(follows[cabeca]) != set():
                            print("Erro: essa gramática não é LL1\n" + \
                                 f"\t{cabeca} -> {" ".join(alfa)} | {" ".join(beta)}\n" + \
                                 f"\tFirst({alfa}) = {first_alfa}\n" + \
                                 f"\tFollow({cabeca}) = {follow[cabeca]}\n" + \
                                 f"\tFirst({alfa}) ∩ Follow({cabeca}) = {first_alfa.intersection(follows[cabeca])}")
                            is_ll1 = False
                    
                    if "&" in first_beta:
                        if first_beta.intersection(follows[cabeca]) != set():
                            print("Erro: essa gramática não é LL1\n" + \
                                 f"\t{cabeca} -> {" ".join(alfa)} | {" ".join(beta)}\n" + \
                                 f"\tFirst({beta}) = {first_beta}\n" + \
                                 f"\tFollow({cabeca}) = {follow[cabeca]}\n" + \
                                 f"\tFirst({beta}) ∩ Follow({cabeca}) = {first_beta.intersection(follows[cabeca])}")
                            is_ll1 = False
        
        # for _, corpos in self.grammar.items():
        #     for corpo in corpos:
        #         print(f"FIRST({corpo}): {get_firsts_body(corpo)}")

        return is_ll1

    def is_left_recursive(self) -> bool:
        
        def aux_busca_prof(self, NT, visitados):
            if NT in visitados:
                return True

            visitados.append(NT)
            
            for corpo in self.grammar[NT]:
                if corpo[0] in self.non_terminals:
                    if aux_busca_prof(self, corpo[0], visitados[:]):
                        return True
            
            return False
        cabeca_recurs = []
        for cabeca in self.non_terminals:
            cabeca_recurs.append(aux_busca_prof(self, cabeca, []))

        return any(cabeca_recurs)

if __name__ == "__main__":
    gramatica = Grammar("file.txt")
    print(gramatica)
    print("\n=============================================================")
    print("NON TERMINALS: ", gramatica.non_terminals)
    print("\nTERMINALS    : ", gramatica.terminals)
    print("\n=============================================================")
    print("FIRSTS\n")
    firsts = gramatica.get_firsts()
    for nt, first in {NT: list(first) for NT, first in firsts.items()}.items():
        first.sort()
        print(f"{nt:17} : {first}")

    print("\n=============================================================")
    print("FOLLOWS\n")
    follows = gramatica.get_follows(firsts)
    for nt, follow in {NT: list(follow) for NT, follow in follows.items()}.items():
        follow.sort()
        print(f"{nt:17} : {follow}")
    print("\n=============================================================")
    print("CHECKING IF IS LL1\n")
    print(gramatica.is_ll1())

