#include <iostream>
#include "NotDeterminedMachine.h"
#include <string>


int main() {
    std::string str1;
    std:: getline(std:: cin ,str1);
    NotDeterminedMachine NTM (str1);
    NTM.update_NotDetM();
    size_t i = 0;
    while(i < NTM.states.size()){
        std::cout << NTM.states[i].first << std::endl;
        ++i;
    }
    std::cout << std::endl;
    NTM.print_transition_table();
    std::cout << std::endl;
    NTM.Reverse();
    NTM.print_transition_table();


    return 0;
}

