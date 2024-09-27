input_str = """
def f (int a) {
    x = x + a;
    return;
}
"""

letra = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
digito = set("0123456789")


initial_state = "0"
final_states = ["2"]
rollback_states = ["2"]
transitions = {
    "0": [{"to": "1", "symbols": letra}],
    "1": [
        {"to": "1", "symbols": letra | digito},
        {
            "to": "2",
            "default": True,
        },
    ],
}


def analyze_input(input_str: str, index = 0):
    current_state = initial_state
    i = index
    while (i < len(input_str) + 1):
        if i == len(input_str):
            char = "EOF"
        else:
            char = input_str[i]
        if current_state in final_states:
            return (True, i, index, input_str[index:min(i, len(input_str))])
        best_transition = None
        default_transition = None
        for transition in transitions[current_state]:
            if "symbols" in transition and char in transition["symbols"]:
                best_transition = transition
            if "default" in transition:
                default_transition = transition
                
        if best_transition is None: 
            best_transition = default_transition
        if best_transition is None:
                return (False,)

        current_state = best_transition["to"]
        if current_state not in rollback_states:
            i += 1
            

IDENT = "IDENT"
OUTRO = "OUTRO"
WHITESPACE_SET = set(" \n\t")
token_list = []
i = 0
while i < len(input_str):
    result = analyze_input(input_str, i)
    if (result[0]):
        token_list.append(IDENT)
        i = result[1]
    else:
        current_char = input_str[i]
        if current_char not in WHITESPACE_SET:
            token_list.append(OUTRO)
        i += 1
        
print(" ".join(token_list))

class TransitionDiagram:
    def __init__(
        self,
        token: str,
        initial_state: str,
        states: list,
        transitions: dict,
        final_states: list,
        rollback_states: list,
    ):
        self.__token = token
        self.__initial_state = initial_state
        self.__states = states
        self.__transitions = transitions
        self.__fin



