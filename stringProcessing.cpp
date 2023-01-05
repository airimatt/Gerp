/*
 *  project 4: gerp
 *  
 *  stringProcessing.cpp
 *  Airi Matsushita & Darya Clark
 *  29/11/22
 *
 *  This file contains the implementation of the stripNonAlphaNum() function,
 *  which strips off non alphanumeric characters at the beginning and end of
 *  the input string.
 */

#include "stringProcessing.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <ctype.h>

using namespace std;

// Purpose: strips off non alphanumeric characters from the front
//          and back of the input string
// Parameters: input string
// Returns: a string that begins and ends with alphanumeric characters
std::string stripNonAlphaNum(std::string input) {

    // start from beginning of the word and iterate through until the first
    // instance of an alpha numerical character is found
    // if (input.size() == 1 and !isalnum(input.front())) {
    //     return "";
    // }
    
    unsigned int track = 0;

    for (unsigned int i = 0; i < input.length(); i++) {
        if (!isalnum(input[i])) {
            track++;
        }
    }
    if (track == input.length()) {
        return "";
    }
    else {
        while (!isalnum(input.front())) {        
            input.erase(0, 1);
        }
        // do the same from the back
        while (!isalnum(input.back())) {
            input.erase(input.length() - 1, 1);        
        }
    }

    return input;
}