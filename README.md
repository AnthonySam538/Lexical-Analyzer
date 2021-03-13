# Problem Statement
This is a lexical analyzer. It parses through a file character by character and displays the relevant tokens that it finds. The list of relevant tokens are as follows: keyword, identifier, integer, float, separator, and operator.

# How to Use the Program
This C++ program will prompt you for the name of a source code file. After giving it a file, the program will display the relevant tokens that it finds in the console window.

# Design of the Program
This lexical analyzer is a finite-state machine. There is a finite set of states and the program moves between them as it reads in characters from your input file. The next state that the program will move to is defined by the current input character that it's looking at. This is implemented by a 2-dimensional matrix, where the row number is the current state, and the column number is the current input character. By using both of these, we can locate the next state to move to. When the program moves into an accepting state, then the program will have found a token, and will reset back to the starting state to look for another token. Below is the initial non-deterministic finite-state machine that this finite-state machine was built from.
![alt text](https://i.imgur.com/Zs1kScz.jpg)
And then the above non-deterministic finite-state machine simplifies down to the below deterministic finite-state machine
Current State | letter | digit | _ or $ | . | other separators | operator | whitespace | ! | any other character | Stay on character?
------------- | ------ | ----- | ------ | - | ---------------- | -------- | ---------- | - | ------------------- | ------------------
0 | 1 | 2 | 12 | 9 | 9 | 10 | 0 | 5 | 12 | 
1 | 1 | 1 | 1 | 6 | 6 | 6 | 6 | 6 | 6 | 
2 | 12 | 2 | 12 | 3 | 7 | 7 | 7 | 7 | 7 | 
3 | 12 | 4 | 12 | 12 | 12 | 12 | 12 | 12 | 12 | 
4 | 12 | 4 | 12 | 12 | 8 | 8 | 8 | 8 | 8 | 
5 | 5 | 5 | 5 | 5 | 5 | 5 | 5 | 11 | 5 | 
6 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | Yes
7 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | Yes
8 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | Yes
9 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | No
10 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | No
11 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | No
12 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | No

0. starting state
1. in valid identifier or valid keyword
2. in valid number
3. in floating-point number
4. in valid floating-point number
5. in comment
6. found an identifier or keyword
7. found a whole number
8. found a floating-point number
9. found a separator
10. found an operator
11. end of comment
12. unknown token encountered

Now, to build the non-deterministic finite-state machine, I first had to create the regular expressions for the tokens. I didn't actually quite make regular expressions for all of the tokens (i.e. I forgot to write the regular expression for comments), but here are the ones I did make:
* Keyword/identifier: `letter·(letter|digit|_ or $)*`
* Number: `digit⁺·(ε|(.·digit⁺))`
* Separator: ` (|)|{|}|[|]|, |.| : |;`
* Operator: `+|−|*|/|%|=|>|<`

To explain in further detail, valid keywords/identifiers must begin with a single letter. Then, that letter may be followed by any combination of any number of letters, digits, _, or $. Numbers must begin with at least one digit. Then that digit may or may not be followed by a decimal point and at least one more digit. And finally, both separators and operators consist of exactly one character from their respective list.
