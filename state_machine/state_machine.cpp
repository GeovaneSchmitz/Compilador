#pragma once
#include <limits.h>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

class StateMachine {
   public:
    StateMachine();
    ~StateMachine();
    unsigned int add_state(bool is_initial = false, bool is_final = false,
                           bool is_rollback = false);
    unsigned int add_state();

    unsigned int add_initial_state();
    unsigned int add_final_state();
    unsigned int add_rollback_state();

    void add_transition(unsigned int state, const std::unordered_set<char> &symbols,
                        unsigned int next_state);

    static const std::unordered_set<char> &char_to_set(const char *str) {
        std::unordered_set<char> set;
        const char *p = str;
        while (*p) {
            set.insert(*p);
            p++;
        }
        return set;
    }

    static const std::unordered_set<char> &digit_set() { return char_to_set("0123456789"); }

    static const std::unordered_set<char> &lowercase_letter_set() {
        return char_to_set("abcdefghijklmnopqrstuvwxyz");
    }

    static const std::unordered_set<char> &uppercase_letter_set() {
        return char_to_set("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

   private:
    unsigned int initial_state;
    unsigned int state_count;
    std::unordered_set<unsigned int> final_states;
    std::unordered_set<unsigned int> rollback_states;
    std::unordered_map<unsigned int, std::unordered_map<char, unsigned int> *> transitions;
}

StateMachine::StateMachine() {
    this->initial_state = UINT_MAX;
}

unsigned int StateMachine::add_state(bool is_initial, bool is_final, bool is_rollback) {
    unsigned int state = this->state_count++;
    if (is_initial) {
        if (this->initial_state != UINT_MAX) {
            throw std::invalid_argument("Initial state already exists");
        }
        this->initial_state = state;
    }
    if (is_final) {
        this->final_states.insert(state);
    }

    if (is_rollback) {
        this->rollback_states.insert(state);
    }
    return state;
}

unsigned int StateMachine::add_state() { return this->add_state(false, false, false); }

unsigned int StateMachine::add_initial_state() { return this->add_state(true, false, false); }

unsigned int StateMachine::add_final_state() { return this->add_state(false, true, false); }

unsigned int StateMachine::add_rollback_state() { return this->add_state(false, false, true); }

void StateMachine::add_transition(unsigned int state, const std::unordered_set<char> &symbols,
                                  unsigned int next_state) {
    auto transition_map_iterator = transitions.find(state);
    std::unordered_map<char, unsigned int> *transition_map = nullptr;
    if (transition_map_iterator == transitions.end()) {
        transition_map = new std::unordered_map<char, unsigned int>();
        transitions[state] = transition_map;
    } else {
        transition_map = transition_map_iterator->second;
    }

    for (auto symbol : symbols) {
        transition_map->insert({symbol, next_state});
    }
}

int main() {
    StateMachine ident();

    auto state_0 = ident.add_initial_state();
    auto state_1 = ident.add_state();
    auto state_2 = ident.add_state(false, true, true);
    ident.add_transition(state_0, StateMachine.lowercase_letter_set, state_1);
    ident.add_transition(state_1, StateMachine.lowercase_letter_set, state_1);
    

    return 0;
}