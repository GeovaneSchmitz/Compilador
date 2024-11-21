
#include <bits/stdc++.h>
#include <limits.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
std::unordered_map<uint64_t, std::string> state_to_string = {
    {1, "Ident"},
    {2, "Int constant"},
    {3, "Float constant"},
    {4, "String constant"},
    {5, "Comparator"},
    {6, "High priority operator"},
    {7, "Low priority operator"},
    {8, "Semicolon"},
    {9, "Assignment"},
    {10, "Comma"},
    {11, "Open curly brace"},
    {12, "Close curly brace"},
    {13, "Open square bracket"},
    {14, "Close square bracket"},
    {15, "Open parenthesis"},
    {16, "Close parenthesis"},
    {17, "Reserved word def"},
    {18, "Reserved word break"},
    {19, "Reserved word new"},
    {20, "Reserved word return"},
    {21, "Reserved word read"},
    {22, "Reserved word print"},
    {23, "Reserved word else"},
    {24, "Reserved word if"},
    {25, "Reserved word for"},
    {26, "Reserved word int"},
    {27, "Reserved word float"},
    {28, "Reserved word string"},

};

class StateMachine {
   public:
    explicit StateMachine();
    StateMachine(const StateMachine &other);
    StateMachine(const std::vector<StateMachine> &machines);
    StateMachine(std::string str, bool is_rollback);
    ~StateMachine();
    uint64_t add_state(bool is_initial = false, bool is_final = false, bool is_rollback = false,
                       uint64_t id = UINT_MAX);
    static StateMachine state_machine_rollback(std::string str);

    uint64_t add_initial_state();
    uint64_t add_final_state(uint64_t id = UINT_MAX);
    uint64_t add_rollback_state();

    void add_transition_set(uint64_t state, const std::unordered_set<char> &symbols,
                            uint64_t next_state);

    void add_transition(uint64_t state, uint64_t next_state);
    void add_transition(uint64_t state, uint64_t symbol, uint64_t next_state);
    void add_epson_transition(uint64_t state, uint64_t next_state);

    std::unordered_map<uint64_t, std::unordered_set<uint64_t> *> *get_transitions(uint64_t state) {
        if (transitions.find(state) == transitions.end()) {
            return nullptr;
        }
        return transitions[state];
    }

    uint64_t get_state_count() { return state_count; }

    bool is_state_initial(uint64_t state) { return state == initial_state_; }
    bool is_state_final(uint64_t state) { return final_states.find(state) != final_states.end(); }
    bool is_state_rollback(uint64_t state) {
        return rollback_states.find(state) != rollback_states.end();
    }

    std::unordered_set<uint64_t> get_final_state_ids(uint64_t state) {
        auto it = final_states.find(state);
        if (it == final_states.end()) {
            return std::unordered_set<uint64_t>();
        }
        return it->second;
    }

    void add_final_state_id(uint64_t state, uint64_t id) {
        auto it = final_states.find(state);
        if (it == final_states.end()) {
            std::unordered_set<uint64_t> set;
            set.insert(id);
            final_states[state] = set;
        } else {
            it->second.insert(id);
        }
    }

    static uint64_t convert_symbol_to_map(char symbol) { return static_cast<uint64_t>(symbol); }

    static char convert_map_to_symbol(uint64_t symbol) { return static_cast<char>(symbol); }

    static uint64_t default_symbol() { return 1 << 8; }

    static uint64_t epson_symbol() { return 1 << 9; }

    static std::unordered_set<char> digit_set() { return char_to_set("0123456789"); }

    static std::unordered_set<char> lowercase_letter_set() {
        return char_to_set("abcdefghijklmnopqrstuvwxyz");
    }

    static std::unordered_set<char> uppercase_letter_set() {
        return char_to_set("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

    static std::unordered_set<char> letter_set() {
        return char_to_set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

    static std::unordered_set<char> alphanumeric_set() {
        return char_to_set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    }

    static const std::unordered_set<char> char_to_set(const char *str) {
        std::unordered_set<char> set;
        const char *p = str;
        while (*p) {
            set.insert(*p);
            p++;
        }
        return set;
    }

    const uint64_t initial_state() const { return this->initial_state_; }

    void print_state_props(uint64_t state) {
        bool open_bracket = false;
        if (is_state_initial(state)) {
            if (!open_bracket) {
                open_bracket = true;
                std::cout << "{";
            }
            std::cout << " Initial";
        }
        if (is_state_rollback(state)) {
            if (!open_bracket) {
                open_bracket = true;
                std::cout << "{";
            }
            std::cout << " Rollback";
        }
        if (is_state_final(state)) {
            if (!open_bracket) {
                open_bracket = true;
                std::cout << "{";
            }
            std::cout << " Final";
        }
        if (open_bracket) {
            std::cout << " }";
        }
    }

    void print_state(uint64_t state) {
        std::cout << "## State: " << state << " ";
        print_state_props(state);
        std::cout << std::endl;
        if (is_state_final(state)) {
            std::cout << "- **Final ID**: ";
            for (auto id : final_states[state]) {
                std::cout << state_to_string[id] << " ";
            }
            std::cout << std::endl;
        }
        auto transitions = get_transitions(state);
        if (transitions != nullptr) {
            std::cout << std::setw(15) << "### Transitions:" << std::endl;
            std::unordered_map<uint64_t, std::vector<std::uint64_t>> transition_state_to_symbol;

            for (auto transition : *transitions) {
                for (auto state : *transition.second) {
                    auto it = transition_state_to_symbol.find(state);
                    if (it == transition_state_to_symbol.end()) {
                        transition_state_to_symbol.insert({state, std::vector<uint64_t>()});
                    }
                    transition_state_to_symbol[state].push_back(transition.first);
                }
            }
            for (auto transition : transition_state_to_symbol) {
                std::sort(transition.second.begin(), transition.second.end());

                char first_symbol = convert_map_to_symbol(transition.second[0]);
                char old_symbol = convert_map_to_symbol(transition.second[0]);
                std::cout << "- **";
                bool is_first = true;
                for (auto symbol : transition.second) {
                    if (symbol == default_symbol()) {
                        if (!is_first) {
                            std::cout << " ";
                        } else {
                            is_first = false;
                        }
                        std::cout << "Default";
                    } else if (symbol == epson_symbol()) {
                        if (!is_first) {
                            std::cout << " ";
                        } else {
                            is_first = false;
                        }
                        std::cout << "Epson";
                    } else {
                        if (symbol - old_symbol > 1) {
                            if (first_symbol != old_symbol) {
                                if (!is_first) {
                                    std::cout << " ";
                                } else {
                                    is_first = false;
                                }
                                std::cout << first_symbol << "-" << old_symbol;
                            } else {
                                if (!is_first) {
                                    std::cout << " ";
                                } else {
                                    is_first = false;
                                }
                                std::cout << old_symbol;
                            }
                            first_symbol = convert_map_to_symbol(symbol);
                        }
                        old_symbol = convert_map_to_symbol(symbol);
                    }
                }

                if (old_symbol != first_symbol) {
                    if (!is_first) {
                        std::cout << " ";
                    } else {
                        is_first = false;
                    }
                    std::cout << first_symbol << "-" << old_symbol;
                } else if (old_symbol != convert_map_to_symbol(default_symbol()) &&
                           old_symbol != convert_map_to_symbol(epson_symbol())) {
                    if (!is_first) {
                        std::cout << " ";
                    } else {
                        is_first = false;
                    }
                    std::cout << old_symbol;
                }

                std::cout << "** -> State " << transition.first << std::endl;
            }
        }

        std::cout << std::endl;
    }

    void print() {
        for (uint64_t i = 0; i < state_count; i++) {
            print_state(i);
        }
    }

    static StateMachine union_machine(const std::vector<StateMachine> &machines) {
        StateMachine result;
        uint64_t machine_id = 0;
        auto initial_state = result.add_initial_state();
        for (auto machine : machines) {
            machine_id++;
            std::unordered_map<uint64_t, uint64_t> state_map;
            std::unordered_map<uint64_t, bool> visited;
            std::vector<uint64_t> states_to_visit;
            states_to_visit.push_back(machine.initial_state());

            while (!states_to_visit.empty()) {
                auto state = states_to_visit.back();
                states_to_visit.pop_back();

                if (visited[state]) {
                    continue;
                }

                visited[state] = true;

                if (state_map.find(state) == state_map.end()) {
                    bool is_final = machine.is_state_final(state);
                    bool is_rollback = machine.is_state_rollback(state);
                    auto new_state = result.add_state(false, is_final, is_rollback, machine_id);
                    state_map[state] = new_state;
                }

                auto new_state = state_map[state];

                if (machine.is_state_initial(state)) {
                    result.add_epson_transition(initial_state, new_state);
                }
                auto transitions = machine.get_transitions(state);
                if (transitions != nullptr) {
                    for (auto symbol_transition : *transitions) {
                        auto next_states = symbol_transition.second;
                        for (auto next_state : *next_states) {
                            if (visited.find(next_state) == visited.end()) {
                                states_to_visit.push_back(next_state);
                            }
                            if (state_map.find(next_state) == state_map.end()) {
                                bool is_final = machine.is_state_final(next_state);
                                bool is_rollback = machine.is_state_rollback(next_state);
                                auto new_state_dest =
                                    result.add_state(false, is_final, is_rollback, machine_id);
                                state_map[next_state] = new_state_dest;
                            }

                            result.add_transition(new_state, symbol_transition.first,
                                                  state_map[next_state]);
                        }
                    }
                }
            }
        }
        return result;
    }

    static StateMachine determinize_machine(StateMachine &machine) {
        StateMachine result;

        std::unordered_map<uint64_t, std::unordered_set<uint64_t>> epson_closure;
        bool has_change = true;
        while (has_change) {
            has_change = false;
            for (uint64_t i = 0; i < machine.get_state_count(); i++) {
                std::unordered_set<uint64_t> closure;
                closure.insert(i);
                auto transitions = machine.get_transitions(i);
                if (transitions != nullptr) {
                    auto epson_transitions = transitions->find(StateMachine::epson_symbol());
                    if (epson_transitions != transitions->end()) {
                        for (auto next_state : *epson_transitions->second) {
                            closure.insert(next_state);
                        }
                    }
                }
                auto current_closure = epson_closure.find(i);
                if (current_closure == epson_closure.end()) {
                    epson_closure[i] = closure;
                    has_change = true;
                } else {
                    auto initial_size = current_closure->second.size();
                    for (auto state : closure) {
                        current_closure->second.insert(state);
                    }
                    if (initial_size != current_closure->second.size()) {
                        has_change = true;
                    }
                }
            }
        }

        std::vector<std::unordered_set<uint64_t>> new_states;
        std::unordered_map<std::size_t, uint64_t> state_map;
        std::vector<std::size_t> states_to_visit;
        std::unordered_set<std::size_t> visited;

        auto initial_state = result.add_initial_state();
        new_states.push_back(epson_closure[machine.initial_state()]);
        state_map[new_states.size() - 1] = initial_state;
        states_to_visit.push_back(new_states.size() - 1);

        while (!states_to_visit.empty()) {
            auto state = states_to_visit.back();
            states_to_visit.pop_back();
            if (visited.find(state) != visited.end()) {
                continue;
            }
            visited.insert(state);

            std::unordered_set<uint64_t> closure;
            for (auto state : new_states[state]) {
                for (auto next_state : epson_closure[state]) {
                    closure.insert(next_state);
                }
            }

            std::unordered_map<uint64_t, std::unordered_set<uint64_t> *> current_transitions;
            for (auto state : closure) {
                auto transitions = machine.get_transitions(state);
                if (transitions != nullptr) {
                    for (auto symbol_transition : *transitions) {
                        if (symbol_transition.first == StateMachine::epson_symbol()) {
                            continue;
                        }

                        auto set_it = current_transitions.find(symbol_transition.first);
                        if (set_it == current_transitions.end()) {
                            auto new_set = new std::unordered_set<uint64_t>();
                            current_transitions[symbol_transition.first] = new_set;
                            set_it = current_transitions.find(symbol_transition.first);
                        }
                        for (auto next_state : *symbol_transition.second) {
                            for (auto next_state_closure : epson_closure[next_state]) {
                                set_it->second->insert(next_state_closure);
                            }
                        }
                    }
                }
            }

            for (auto it : current_transitions) {
                auto next_state = it.second;
                std::size_t next_state_index = 0;
                bool found = false;

                for (std::size_t i = 0; i < new_states.size(); i++) {
                    if (new_states[i] == *next_state) {
                        next_state_index = i;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    new_states.push_back(*next_state);
                    next_state_index = new_states.size() - 1;

                    bool is_rollback = false;
                    for (auto state : *next_state) {
                        if (machine.is_state_rollback(state)) {
                            is_rollback = true;
                            break;
                        }
                    }

                    auto new_state = result.add_state(false, false, is_rollback);
                    state_map[next_state_index] = new_state;
                    for (auto state : *next_state) {
                        if (machine.is_state_final(state)) {
                            auto ids = machine.get_final_state_ids(state);
                            for (auto id : ids) {
                                result.add_final_state_id(new_state, id);
                            }
                        }
                    }
                    states_to_visit.push_back(next_state_index);
                }

                result.add_transition(state_map[state], it.first, state_map[next_state_index]);
            }

            for (auto it : current_transitions) {
                delete it.second;
            }
        }
        return result;
    }

   private:
    uint64_t initial_state_;
    uint64_t state_count;
    std::unordered_map<uint64_t, std::unordered_set<uint64_t>> final_states;
    std::unordered_set<uint64_t> rollback_states;
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, std::unordered_set<uint64_t> *> *>
        transitions;
};

StateMachine::StateMachine() { this->initial_state_ = UINT_MAX; }

StateMachine::~StateMachine() {
    for (auto transition : this->transitions) {
        for (auto symbol_transition : *transition.second) {
            delete symbol_transition.second;
        }
        delete transition.second;
    }
}

uint64_t StateMachine::add_state(bool is_initial, bool is_final, bool is_rollback, uint64_t id) {
    uint64_t state = this->state_count++;
    if (is_initial) {
        if (this->initial_state_ != UINT_MAX) {
            throw std::invalid_argument("Initial state already exists");
        }
        this->initial_state_ = state;
    }
    if (is_final) {
        if (this->final_states.find(state) == this->final_states.end()) {
            std::unordered_set<uint64_t> final_states;
            final_states.insert(id);
            this->final_states.insert({state, final_states});
        } else {
            this->final_states[state].insert(id);
        }
    }

    if (is_rollback) {
        this->rollback_states.insert(state);
    }
    return state;
}

StateMachine::StateMachine(const StateMachine &other) {
    this->initial_state_ = other.initial_state_;
    this->state_count = other.state_count;
    this->final_states = other.final_states;

    this->rollback_states = other.rollback_states;

    for (auto transition : other.transitions) {
        auto transition_map = new std::unordered_map<uint64_t, std::unordered_set<uint64_t> *>();
        for (auto symbol_transition : *transition.second) {
            auto next_states = new std::unordered_set<uint64_t>();
            for (auto next_state : *symbol_transition.second) {
                next_states->insert(next_state);
            }
            transition_map->insert({symbol_transition.first, next_states});
        }
        this->transitions[transition.first] = transition_map;
    }
}

uint64_t StateMachine::add_initial_state() { return this->add_state(true, false, false); }

uint64_t StateMachine::add_final_state(uint64_t id) {
    return this->add_state(false, true, false, id);
}

uint64_t StateMachine::add_rollback_state() { return this->add_state(false, false, true); }

void StateMachine::add_transition(uint64_t state, uint64_t symbol, uint64_t next_state) {
    auto transition_map_iterator = transitions.find(state);

    std::unordered_map<uint64_t, std::unordered_set<uint64_t> *> *transition_map;

    if (transition_map_iterator == transitions.end()) {
        transition_map = new std::unordered_map<uint64_t, std::unordered_set<uint64_t> *>();
        transitions[state] = transition_map;
    } else {
        transition_map = transition_map_iterator->second;
    }

    std::unordered_set<uint64_t> *next_states = nullptr;
    auto next_states_iterator = transition_map->find(symbol);
    if (next_states_iterator == transition_map->end()) {
        next_states = new std::unordered_set<uint64_t>();
        transition_map->insert({symbol, next_states});
    } else {
        next_states = next_states_iterator->second;
    }
    next_states->insert(next_state);
}

void StateMachine::add_transition_set(uint64_t state, const std::unordered_set<char> &symbols,
                                      uint64_t next_state) {
    for (auto symbol : symbols) {
        add_transition(state, StateMachine::convert_symbol_to_map(symbol), next_state);
    }
}

void StateMachine::add_epson_transition(uint64_t state, uint64_t next_state) {
    add_transition(state, StateMachine::epson_symbol(), next_state);
}

void StateMachine::add_transition(uint64_t state, uint64_t next_state) {
    add_transition(state, StateMachine::default_symbol(), next_state);
}

StateMachine::StateMachine(std::string str, bool rollback) {
    this->initial_state_ = UINT_MAX;
    const char *p = str.c_str();

    if (p == nullptr) {
        return;
    }

    const char *p_next = p + 1;

    auto state = this->add_initial_state();

    while (p != nullptr && p_next != nullptr && *p_next != '\0' && *p != '\0') {
        auto new_state = this->add_state();
        this->add_transition(state, StateMachine::convert_symbol_to_map(*p), new_state);
        state = new_state;
        p = p_next;
        p_next++;
    }

    if (p != nullptr) {
        this->add_transition(state, StateMachine::convert_symbol_to_map(*p),
                             this->add_state(false, true, rollback));
    }
}

int main() {
    StateMachine ident;

    auto ident_state_0 = ident.add_initial_state();
    auto ident_state_1 = ident.add_state();
    auto ident_state_2 = ident.add_state(false, true, true);

    ident.add_transition_set(ident_state_0, StateMachine::letter_set(), ident_state_1);
    ident.add_transition_set(ident_state_1, StateMachine::alphanumeric_set(), ident_state_1);
    ident.add_transition(ident_state_1, ident_state_2);

    StateMachine int_constant;

    auto int_constant_state_0 = int_constant.add_initial_state();
    auto int_constant_state_1 = int_constant.add_state();
    auto int_constant_state_2 = int_constant.add_state(false, true, true);

    int_constant.add_transition_set(int_constant_state_0, StateMachine::digit_set(),
                                    int_constant_state_1);
    int_constant.add_transition_set(int_constant_state_1, StateMachine::digit_set(),
                                    int_constant_state_1);
    int_constant.add_transition(int_constant_state_1, int_constant_state_2);

    StateMachine float_constant;

    auto float_constant_state_0 = float_constant.add_initial_state();
    auto float_constant_state_1 = float_constant.add_state();
    auto float_constant_state_2 = float_constant.add_state();
    auto float_constant_state_3 = float_constant.add_state();
    auto float_constant_state_4 = float_constant.add_state(false, true, true);

    float_constant.add_transition_set(float_constant_state_0, StateMachine::digit_set(),
                                      float_constant_state_1);
    float_constant.add_transition_set(float_constant_state_1, StateMachine::digit_set(),
                                      float_constant_state_1);
    float_constant.add_transition_set(float_constant_state_1, {'.'}, float_constant_state_2);
    float_constant.add_transition_set(float_constant_state_2, StateMachine::digit_set(),
                                      float_constant_state_3);
    float_constant.add_transition_set(float_constant_state_3, StateMachine::digit_set(),
                                      float_constant_state_3);
    float_constant.add_transition(float_constant_state_3, float_constant_state_4);

    StateMachine string_constant;

    auto string_constant_state_0 = string_constant.add_initial_state();
    auto string_constant_state_1 = string_constant.add_state();
    auto string_constant_state_2 = string_constant.add_state(false, true, false);

    string_constant.add_transition_set(string_constant_state_0, {'"'}, string_constant_state_1);
    string_constant.add_transition_set(string_constant_state_1, StateMachine::alphanumeric_set(),
                                       string_constant_state_1);
    string_constant.add_transition_set(string_constant_state_1, {'"'}, string_constant_state_2);

    StateMachine comparator;
    auto comparator_state_0 = comparator.add_initial_state();
    auto comparator_state_1 = comparator.add_state();
    auto comparator_state_2 = comparator.add_final_state();
    auto comparator_state_3 = comparator.add_state();
    auto comparator_state_4 = comparator.add_final_state();
    auto comparator_state_5 = comparator.add_state();
    auto comparator_state_6 = comparator.add_final_state();
    auto comparator_state_7 = comparator.add_state(false, true, true);
    auto comparator_state_8 = comparator.add_final_state();
    auto comparator_state_9 = comparator.add_state();
    auto comparator_state_10 = comparator.add_state(false, true, true);

    comparator.add_transition_set(comparator_state_0, {'!'}, comparator_state_1);
    comparator.add_transition_set(comparator_state_1, {'='}, comparator_state_2);

    comparator.add_transition_set(comparator_state_0, {'='}, comparator_state_3);
    comparator.add_transition_set(comparator_state_3, {'='}, comparator_state_4);

    comparator.add_transition_set(comparator_state_0, {'<'}, comparator_state_5);
    comparator.add_transition_set(comparator_state_5, {'='}, comparator_state_6);
    comparator.add_transition(comparator_state_5, comparator_state_7);

    comparator.add_transition_set(comparator_state_0, {'>'}, comparator_state_8);
    comparator.add_transition_set(comparator_state_8, {'='}, comparator_state_9);
    comparator.add_transition(comparator_state_8, comparator_state_10);

    StateMachine high_priority_op;

    auto high_priority_op_state_0 = high_priority_op.add_initial_state();
    auto high_priority_op_state_1 = high_priority_op.add_final_state();

    high_priority_op.add_transition_set(high_priority_op_state_0, {'*', '/', '%'},
                                        high_priority_op_state_1);

    StateMachine low_priority_op;
    auto low_priority_op_state_0 = low_priority_op.add_initial_state();
    auto low_priority_op_state_1 = low_priority_op.add_final_state();

    low_priority_op.add_transition_set(low_priority_op_state_0, {'+', '-'},
                                       low_priority_op_state_1);

    StateMachine semicolon(";", false);

    StateMachine assignment("=", false);

    StateMachine comma(",", false);

    StateMachine open_curly_brace("{", false);

    StateMachine close_curly_brace("}", false);

    StateMachine open_square_bracket("[", false);

    StateMachine close_square_bracket("]", false);

    StateMachine open_parenthesis("(", false);

    StateMachine close_parenthesis(")", false);

    std::vector<std::string> reserved_words = {"def",  "break", "new", "return", "read",  "print",
                                               "else", "if",    "for", "int",    "float", "string"};
    std::vector<StateMachine> reserved_words_machines;
    for (auto word : reserved_words) {
        reserved_words_machines.push_back(StateMachine(word, true));
    }

    std::vector<StateMachine> machines = {ident,
                                          int_constant,
                                          float_constant,
                                          string_constant,
                                          comparator,
                                          high_priority_op,
                                          low_priority_op,
                                          semicolon,
                                          assignment,
                                          comma,
                                          open_curly_brace,
                                          close_curly_brace,
                                          open_square_bracket,
                                          close_square_bracket,
                                          open_parenthesis,
                                          close_parenthesis};

    std::vector<StateMachine> all_machines;
    all_machines.insert(all_machines.end(), machines.begin(), machines.end());
    all_machines.insert(all_machines.end(), reserved_words_machines.begin(),
                        reserved_words_machines.end());
    StateMachine union_machine = StateMachine::union_machine(all_machines);

    StateMachine::determinize_machine(union_machine).print();

    return 0;
}