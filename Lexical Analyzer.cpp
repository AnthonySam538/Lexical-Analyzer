#include <iostream>
#include <fstream>
#include <set>

std::string remove_whitespace(const std::string & my_string)
{
    std::string output = "";

    //For each character in my_string,
    for (char current_character : my_string)
    {
        //if the current character is not whitespace,
        if (!isspace(current_character))
        {
            //append it to the output string.
            output += current_character;
        }
    }

    return output;
}

int main()
{
    std::string myString = "4345734.25";
    std::cout << (myString.find('.') != std::string::npos) << std::endl;

    //Create necessary variables
    char current_character;
    std::string current_token = "";
    unsigned short current_state = 0;
    bool stay_on_character = false;
    const unsigned short state_table[12][8] = { {1, 3,11, 7, 7, 8, 0, 9},
                                                {1, 1, 1, 2, 2, 2, 2, 2},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {4, 3, 4, 5, 4, 4, 4, 4},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {6, 5, 6, 6, 6, 6, 6, 6},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {9, 9, 9, 9, 9, 9, 9,10},
                                                {0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0} };
    const std::set<std::string> keywords{ "int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not" };
    const std::set<char> separators{ '(', ')', '{', '}', '[', ']', ',', '.', ':', ';' };
    const std::set<char> operators{ '+', '-', '*', '/', '%', '=', '>', '<' };
    std::ifstream my_file("DeclarationAssignmentInput.txt");
    //std::ifstream my_file("SampleInputFile1-2021.txt");

    std::cout << "TOKEN\t\t\tLEXEMES\n\n";

    while (stay_on_character == true || my_file >> std::noskipws >> current_character) // note that if stay_on_character is true, then the next character won't be retrieved
    {
        stay_on_character = false;

        current_token += current_character;

        //Go to the next state as defined by current_character
        if ((current_character >= 'a' && current_character <= 'z') || (current_character >= 'A' && current_character <= 'Z'))
            current_state = state_table[current_state][0];
        else if (current_character >= '0' && current_character <= '9')
            current_state = state_table[current_state][1];
        else if (current_character == '_' || current_character == '$')
            current_state = state_table[current_state][2];
        else if (current_character == '.')
            current_state = state_table[current_state][3];
        else if (separators.find(current_character) != separators.end())
            current_state = state_table[current_state][4];
        else if (operators.find(current_character) != operators.end())
            current_state = state_table[current_state][5];
        else if (current_character == '\t' || current_character == ' ' || current_character == '\n')
            current_state = state_table[current_state][6];
        else if (current_character == '!')
            current_state = state_table[current_state][7];
        else
            current_state = 11;

        //If we have found a token, then display it and reset back to the starting state.
        switch (current_state)
        {
        //Found a keyword or an identifier
        case 2:
            current_token = remove_whitespace(current_token.substr(0, current_token.length() - 1)); // exclude the last character, then remove whitespace
            //Use the list of keywords to determine if we're to display a keyword or an identifier
            if (keywords.find(current_token) != keywords.end())
                std::cout << "KEYWORD\t\t=\t";
            else
                std::cout << "IDENTIFIER\t=\t";
            std::cout << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            stay_on_character = true;
            break;
        //Found an integer
        case 4:
            current_token = remove_whitespace(current_token.substr(0, current_token.length() - 1)); // exclude the last character, then remove whitespace
            std::cout << "INTEGER\t\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            stay_on_character = true;
            break;
        //Found a floating-point number
        case 6:
            current_token = remove_whitespace(current_token.substr(0, current_token.length() - 1)); // exclude the last character, then remove whitespace
            std::cout << "FLOAT\t\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            stay_on_character = true;
            break;
        //Found a separator
        case 7:
            current_token = remove_whitespace(current_token.substr(0, current_token.length())); // remove whitespace
            std::cout << "SEPARATOR\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            break;
        //Found an operator
        case 8:
            current_token = remove_whitespace(current_token.substr(0, current_token.length())); // remove whitespace
            std::cout << "OPERATOR\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            break;
        //Reached the end of a comment
        case 10:
            //No significant action needs to be taken, besides resetting current_token
            current_token = "";
            current_state = 0;
            break;
        //Invalid character
        case 11:
            std::cout << "Lexical error.";
            break;
        }
    }

    return 0;
}
