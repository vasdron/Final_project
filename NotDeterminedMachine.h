#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <utility>
#include <map>
#include <set>
#include <iterator>
#include <optional>


class NotDeterminedMachine {
public:
    struct Transition {
        size_t from;
        size_t to;
        size_t by;
    };
    //struct State{
    //  int NumOfState;
    //bool IsFinal;
    //};
    typedef std::vector<std::pair<size_t, bool>> States;
    typedef std::vector<std::vector<std::set<std::string>>> StatesTransitions;
    //typedef bool IsFinal;

    States states;
    StatesTransitions states_transitions;
    int start_state;

    explicit NotDeterminedMachine(std::string& s) : states({ {0,0},{1,1} }), start_state(0),
                                                    states_transitions(2, std::vector<std::set<std::string>>(2)) {
        states_transitions[0][1].insert(s);

    }

    explicit NotDeterminedMachine() : states({ {0,0} }), start_state(0),
                                      states_transitions(2, std::vector<std::set<std::string>>(2)) {
    }



    ~NotDeterminedMachine() = default;

    [[nodiscard]] std::optional<Transition> EdgeIsRegex() const {
        size_t i = 0;
        size_t j = 0;
        size_t k = 0;
        for (; i < states_transitions.size(); ++i) {
            for (j = 0; j < states_transitions[i].size(); ++j) {
                //std::cout << j << std::endl;
                k = 0;
                for (auto& transition : states_transitions[i][j]) {
                    //std::cout << transition << std::endl;
                    if (transition.size() > 1) {
                        return { {i, j, k} };
                    }
                    ++k;
                }
            }
        }
        return std::nullopt;
    }

    void AddState() {
        states.push_back({ states.size(), false });
        for (auto& line : states_transitions) {
            line.emplace_back();
        }
        states_transitions.emplace_back(states.size());
    }

    void AddFinalState() {
        states.push_back({ states.size(), true });
        for (auto& line : states_transitions) {
            line.emplace_back();
        }
        states_transitions.emplace_back(states.size());
    }
    void MakeFinal(int i) {
        states[i].second = true;
    }
    void MakeUnFinal(int i) {
        states[i].second = false;
    }

    void AddTransition(const std::string& str, size_t i, size_t j) {
        states_transitions[i][j].insert(str);
    }

    void DeleteTransition(size_t i, size_t j, /*std::vector<std::vector<std::set<std::string>>>::iterator*/ auto vi) {
        states_transitions[i][j].erase(vi);
    }

    void make_machine() {
        int N;
        int k;
        std::cout << "enter the number of states\n";
        std::cin >> N;
        for (int i = 1; i < N; i++) {
            AddState();
        }
        std::cout << "enter transitions\n";
        std::string c;
        int from, to;
        while (true) {
            std::cin >> c;
            if (c != "end") {
                std::cin >> from;
                std::cin >> to;
                states_transitions[from][to].insert(c);
            }
            else {
                break;
            }
        }
        std::cout << "enter the number of final states\n";
        std::cin >> k;
        std::cout << "enter the numbers of the final states\n";
        for (int i = 0; i < k; i++) {
            int st;
            std::cin >> st;
            MakeFinal(st);
        }
    }

    void print_states() {
        for (int i = 0; i < states.size(); i++) {
            std::cout << states[i].first << ' ';
        }
        std::cout << std::endl;
    }

    void print_final_states() {
        for (int i = 0; i < states.size(); i++) {
            if (states[i].second == true) {
                std::cout << states[i].first << ' ';
            }
        }
        std::cout << std::endl;
    }
    void print_transition_table() {
        for (int i = 0; i < states.size(); i++) {
            for (int j = 0; j < states.size(); j++) {
                if (states_transitions[i][j].empty() == 0) {
                    for (auto& it : states_transitions[i][j]) {
                        std::cout << it << " " << i << " " << j << " ";
                        std::cout << std::endl;
                    }
                }
            }

        }
    }

    void update_NotDetM() {

        while (auto edge = EdgeIsRegex()) {
            auto i = edge.value().from;
            auto j = edge.value().to;
            auto k = edge.value().by;
            //std::cout << i << j << k << std::endl;
            auto vi = states_transitions[i][j].begin();
           // std::cout << (*vi).size() << std::endl;
            //for(auto it = states_transitions[i][j].begin(); it !=states_transitions[i][j].end() ; ++it){
            // std::cout << (*it) << std::endl;
            //}
            std::advance(vi, k);
            // std::cout << (*vi).size() << std::endl;
            size_t bracket_open = 0;
            size_t bracket_close = 0;
            size_t counter = 1;
            for (; counter < (*vi).size() - 1; ++counter) {
                //std::cout << (*vi).size() << std::endl;
                if ((*vi)[counter] == '(') {
                    ++bracket_open;
                }
                if ((*vi)[counter] == ')') {
                    ++bracket_close;
                }
                if (bracket_open == bracket_close) {
                    if ((*vi)[counter] >= 'a' && (*vi)[counter] <= 'z') {
                        std::string str1;
                        str1 = (*vi)[counter];
                        DeleteTransition(i, j, vi);
                        AddTransition(str1, i, j);
                        break;
                    }
                    if ((*vi)[counter + 1] == '|') {
                        std::string str1, str2;
                        str1 = (*vi).substr(1, counter);
                        //std::cout << (*vi).size() << std::endl;
                        //std::cout << counter << std::endl;
                        str2 = (*vi).substr(counter + 2, (*vi).size() - counter - 3);
                        DeleteTransition(i, j, vi);
                        AddTransition(str1, i, j);
                        AddTransition(str2, i, j);
                        break;
                    }
                    if ((*vi)[counter + 1] == '.') {
                        std::string str1, str2;
                        AddState();
                        str1 = (*vi).substr(1, counter);
                        str2 = (*vi).substr(counter + 2, (*vi).size() - counter - 3);
                        DeleteTransition(i, j, vi);
                        AddTransition(str1, i, states.size() - 1);
                        AddTransition(str2, states.size() - 1, j);
                        break;
                    }
                    if ((*vi)[counter + 1] == '*') {
                        std::string str1;
                        AddState();
                        AddState();
                        str1 = (*vi).substr(1, counter);
                        DeleteTransition(i, j, vi);
                        AddTransition(str1, states.size() - 2, states.size() - 1);
                        std::string empty_word;
                        empty_word = '%';
                        AddTransition(empty_word, i, states.size() - 2);
                        AddTransition(empty_word, states.size() - 1, j);
                        AddTransition(empty_word, j, i);
                        break;
                    }
                }
            }
        }
    }
    void Reverse() {
        StatesTransitions s = states_transitions;
        for (int i = 0; i < states.size(); i++) {
            for (int j = 0; j < states.size(); j++) {
                states_transitions[j][i].erase(states_transitions[j][i].begin(), states_transitions[j][i].end());
                for (auto& transition : states_transitions[i][j]) {
                    s[j][i].insert(transition);
                }
            }
        }
        states_transitions = s;
        AddState();
        for (int i = 0; i < states.size(); i++) {
            if (states[i].second == true) {
                std::string empty_word = "%";
                AddTransition(empty_word, states.size() - 1, i);
                MakeUnFinal(i);
            }
        }
        MakeFinal(start_state);
        start_state = states.size();
    }
    bool string_fits(std::string s, int cur) {
        if (s == "" and states[cur].second == 1) {
            return true;
        }
        else {
            for (int i = 0; i < states.size(); i++) {
                if (states_transitions[cur][i].empty() == 0) {
                    for (auto& it : states_transitions[cur][i]) {
                        if (it == s.substr(0, 1)) {
                            if (string_fits(s.substr(1), i)) {
                                return true;
                            }
                        }
                        else if (it == "%") {
                            if (string_fits(s, i)) {
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }
    }

    States eps_closure(int k) {
        States eps_clos;
        for (int i = 0; i < states.size(); i++) {
            for (auto& it : states_transitions[k][i]) {
                if (it == "%") {
                    eps_clos.push_back(states[i]);
                }
            }
        }
        return eps_clos;
    }


    void NTM_to_DTM(){
        std::vector<States> storage;
        States states_of_dtm;
        storage[0].push_back(states[start_state]);
        for(int i = 0; i< eps_closure(start_state).size(); i++){
            storage[0].push_back(eps_closure(start_state)[i]);
        }
        std::vector<int> queue;
        queue.push_back(0);

        while(queue.empty() == 0){
            for(int i = 0; i < storage[queue[0]].size(); i++){
                for(int j = 0; j < states.size(); j++){
                    for(auto& it : states_transitions[i][j]){
                        if( it == "a"){

                        }
                    }
                }
            }

        }
    }
    /*void DFAbyNFA() {
        NotDeterminedMachine M;
        std::vector<std::set<int>>;
        st[0].push_back(states[start_state]);
        for (int i = 0; i < eps_closure(start_state).size(); i++) {
            st[0].push_back(eps_closure(start_state)[i]);
        }
        M.AddState();
        M.start_state = 0;

        for (int i = 1; i < states.size(); i++) {
            AddState();
        }


        for (int i = 0; i < st.size(); i++) {
            for (int j = 0; j < st[i].size(); j++) {
                for(int k = 0; j<states.size(); k++){
                    for (auto& it : states_transitions[st[i][j].first][k]) {
                        if (it == "a") {
                            st[i + 1].push_back(states[k]);
                        }
                        for (int x = 0; x < eps_closure(k).size(); i++) {
                            st[i + 1].push_back(eps_closure(k)[x]);
                        }
                        if (M.states_transitions[i][i + 1].empty() == 0) {
                            M.AddTransition("a", i, i + 1);
                        }
                    }

                }
            }
            for (int j = 0; j < st[i].size(); j++) {
                for (int k = 0; j < states.size(); k++) {
                    for (auto& it : states_transitions[st[i][j].first][k]) {
                        if (it == "b") {
                            st[i + 1].push_back(states[k]);
                        }
                        for (int x = 0; x < eps_closure(k).size(); i++) {
                            st[i + 1].push_back(eps_closure(k)[x]);
                        }
                    }

                }
            }
        }



    }*/

};