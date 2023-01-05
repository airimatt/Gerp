/*
 *  project 4: gerp
 *  
 *  main.cpp
 *  Airi Matsushita & Darya Clark
 *  1/12/22
 *
 *  Includes main function that creates an instance of gerp, takes in user
 *  input, and runs the gerp program
 * 
 */

#include "gerp.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    gerp gerping; 
    if (argc != 3) {
        // error message if wrong number of arguments
        cerr << "Usage: ./gerp inputDirectory outputFile\n";
    }
    gerping.runGerp(argv[1], argv[2]);
}