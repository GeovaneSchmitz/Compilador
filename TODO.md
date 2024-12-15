# TO-DO

## Analisador Léxico
### - Ler caractere por caractere da entrada OK
### - Baseado em diagramas de transição OK
### - Implementar tabela de símbolos OK
Uma entrada para cada token **Identificador**, com uma lista de ocorrências do identificador no arquivo (linha, coluna). 

## Analisador Sintático
### - A gramática associada deve estar em LL(1) OK
### - Deve construir uma tabela de reconhecimento uma única vez OK
### - Calcular então pertinência da entrada (fonte) na linguagem gerada pela gramática associada OK 
### - Sugestões do Professor:
#### 1. Colocar CC-2024-2 (a gramática dada) na forma convencional, obtendo então ConvCC-2024-2. Os próximos passos trabalham nesta gramática obtida OK
#### 2. Remover recursão à esquerda OK
#### 3. Tornar a gramática LL(1), adicionando novos terminais caso necessário. Deve-se demonstrar que de fato a gramática está em LL(1), usando o teorema visto em aula 
#### 4. Construir a tabela de reconhecimento sintático (podemos usar qualquer ferramenta) OK

## Analisador Semântico
### - Usar regras semânticas para:
#### 1. Construir uma árvore de expressão T (somente com operadores e operandos nos nós) QUASE OK
#### 2. Inserir os tipos das variáveis na tabela de símbolos
### - Também devemos tratar:
#### 1. Verificação de tipos em expressões
#### 2. Verificação de identificadores por escopo
#### 3. Verificação de comandos *break* em laços de repetição.
### Nota: Verificar arquivo do projeto no moodle para detalhes.

## Geração de Código Intermediário

### - Código intermediário deve ser o de três endereços apresentado em aula
### - Para isso, devemos:
#### 1. Construir SDD L-atribuída para ConvCC-2024-2-LL1 com regras semânticas para GCI
#### 2. Construir uma SDT para esta SDD
#### 3. Usar essa SDT para gerar código intermediário