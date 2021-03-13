#include <iostream>
#include <fstream>
#include <string>
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
    //Create necessary variables
    char current_character; // the character we're looking at
    std::string current_token = ""; // what we have so far
    unsigned short current_state = 0; // the state we're in
    bool stay_on_character = false;
    const unsigned short state_table[6][9] = { { 1,  2, 12,  9,  9, 10,  0,  5, 12},
                                               { 1,  1,  1,  6,  6,  6,  6,  6,  6},
                                               {12,  2, 12,  3,  7,  7,  7,  7,  7},
                                               {12,  4, 12, 12, 12, 12, 12, 12, 12},
                                               {12,  4, 12, 12,  8,  8,  8,  8,  8},
                                               { 5,  5,  5,  5,  5,  5,  5, 11,  5} };
    const std::set<std::string> keywords{ "int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not" };
    const std::set<char> separators{ '(', ')', '{', '}', '[', ']', ',', '.', ':', ';' };
    const std::set<char> operators{ '+', '-', '*', '/', '%', '=', '>', '<' };
    std::string filename;

    //Get a file
    std::cout << "Enter the name of your file: ";
    std::getline(std::cin, filename);
    std::ifstream my_file(filename);

    std::cout << "TOKEN\t\t\tLEXEMES\n\n";

    while (stay_on_character == true || my_file >> std::noskipws >> current_character) // Note that if stay_on_character is true, then the next character won't be retrieved. Otherwise, the next character will be retrieved.
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
        else if (isspace(current_character))
            current_state = state_table[current_state][6];
        else if (current_character == '!')
            current_state = state_table[current_state][7];
        else
            current_state = state_table[current_state][8];

        //If we have found a token, then display it and reset back to the starting state.
        switch (current_state)
        {
        //Found a keyword or an identifier
        case 6:
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
        case 7:
            current_token = remove_whitespace(current_token.substr(0, current_token.length() - 1)); // exclude the last character, then remove whitespace
            std::cout << "INTEGER\t\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            stay_on_character = true;
            break;
        //Found a floating-point number
        case 8:
            current_token = remove_whitespace(current_token.substr(0, current_token.length() - 1)); // exclude the last character, then remove whitespace
            std::cout << "FLOAT\t\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            stay_on_character = true;
            break;
        //Found a separator
        case 9:
            current_token = remove_whitespace(current_token.substr(0, current_token.length())); // remove whitespace
            std::cout << "SEPARATOR\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            break;
        //Found an operator
        case 10:
            current_token = remove_whitespace(current_token.substr(0, current_token.length())); // remove whitespace
            std::cout << "OPERATOR\t=\t" << current_token << std::endl; // display the token
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            break;
        //Reached the end of a comment
        case 11:
            //No significant action needs to be taken, besides resetting current_token
            current_token = "";
            current_state = 0;
            break;
        //Unknown token
        case 12:
            std::cout << "Unknown token found: " << current_token << std::endl;
            //Reset back to the starting state
            current_token = "";
            current_state = 0;
            break;
        }
    }

    switch (current_state)
    {
    //Ended in a valid identifier or valid keyword
    case 1:
        current_token = remove_whitespace(current_token.substr(0, current_token.length())); // remove whitespace
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
    //Ended in valid integer
    case 2:
        current_token = remove_whitespace(current_token.substr(0, current_token.length())); // remove whitespace
        std::cout << "INTEGER\t\t=\t" << current_token << std::endl; // display the token
        //Reset back to the starting state
        current_token = "";
        current_state = 0;
        stay_on_character = true;
        break;
    //Ended in valid floating-point number
    case 4:
        current_token = remove_whitespace(current_token.substr(0, current_token.length())); // remove whitespace
        std::cout << "FLOAT\t\t=\t" << current_token << std::endl; // display the token
        //Reset back to the starting state
        current_token = "";
        current_state = 0;
        stay_on_character = true;
        break;
    }

    return 0;
}
