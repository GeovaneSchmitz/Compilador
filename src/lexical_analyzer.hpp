#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace glexa {

class Transition {
    public:
        Transition(unsigned int next_state, bool default_transition);
        ~Transition();
    private:
        unordered_set<char> *symbols;
        unsigned int next_state;
        bool default_transition;

}

class Automata {
   private:
    unsigned int initial_state;
    unsigned int *final_states;
    unsigned int *rollback_states;

   public:
    Automata(unsigned int initial_state, unsigned int *final_states, unsigned int *rollback_states);
    ~Automata();
};

template <typename T>
class Transition {
   private:
    char *key;
    T *value;

   public:
    Transition<T>(unsigned int next_state, T *action);
    ~Transition();
};

}  // namespace glexa
