# Final_project
REGEX_TO_NFA
This project allows you to build an NFA using a regular expression. Inside, functions are also implemented that allow you to parse a regular expression into components, get an automaton inverse to this one, enter the automaton you need and run a word acceptance check by this automaton.
To enter a regular expression, note that each subexpression must be in parentheses. For example (((A)*)|(B)).
To create an automaton manually, you need to call the make_machine() function. After the launch, you will be asked to enter the number of states, transitions (in the symbol, from, to format) and final states.
The signal to stop entering new transitions or new words for checking is the line "end"
