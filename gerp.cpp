/*
 *  project 4: gerp
 *  
 *  gerp.cpp
 *  Airi Matsushita & Darya Clark
 *  1/12/22
 *
 *  Contains implementations of the member functions of the the gerp class, 
 *  which contains the main logic of the gerp program
 */

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>

#include "gerp.h"
#include "stringProcessing.h"

using namespace std;

// Purpose: constructor
// Parameters: nothing
// Returns:
gerp::gerp() 
{
    // constructor :D
}

// Purpose: deconstructor
// Parameters: nothing
// Returns: 
gerp::~gerp() 
{
    // deconstructor D:
}

// Purpose: opens file, or throws runtime error if file cannot be opened
// Parameters: name of file to be opened, streamtype
// Returns: nothing
template<typename streamtype>
void gerp::openFile(string file_name, streamtype &stream) 
{
    stream.open(file_name);
    if (not stream.is_open()) {
        throw runtime_error("Unable to open file: " + file_name);
    }
}

// Purpose: takes in an input directory name and generates a 
//          directory-to-file tree
// Parameters: parent directory (input)
// Returns: nothing
void gerp::generateFileTree(string inputDir)
{
    FSTree fileTree(inputDir);
    DirNode *root = fileTree.getRoot();
    
    if (!root->isEmpty()) {
        filePathTraversal(root, inputDir);
    } else {
        throw runtime_error("Could not build index, exiting.");
        // cerr << "Could not build index, exiting." << endl;
        // exit(1);
    }
}

// Purpose: recursively traverses through directories, listing each 
//          one and its contents
// Parameters: pointer to current directory, parent directory name
// Returns: nothing
void gerp::filePathTraversal(DirNode *currDir, string parentDir) 
{        
    if (currDir->hasFiles()) {
        for (int i = 0; i < currDir->numFiles(); i++) {
            // get path to file
            string newPath = parentDir + "/" + currDir->getFile(i);
            storePath(newPath);
        }
    }
    if (currDir->hasSubDir()) {
        for (int i = 0; i < currDir->numSubDirs(); i++) {
            // recursively iterate through
            string subPath = parentDir + "/" 
                                       + currDir->getSubDir(i)->getName();
            filePathTraversal(currDir->getSubDir(i), subPath);
        }
    }
}

// Purpose: stores the path to a file (string) in the 0th index of the vector
//          that will contain the contents of that file
// Parameters: path from parent directory to file
// Returns: nothing
void gerp::storePath(string pathToFile) 
{
    // create new node at back, store file path at index 0
    vector<string> oneFile;
    oneFile.push_back(pathToFile);
    files.push_back(oneFile);
}

// Purpose: gets the index of the file (stored in vector) in the files vector
// Parameters: nothing
// Returns: index of where desired file is stored inside the file vector
int gerp::getFileIndex() 
{
    int index = files.size() - 1;
    return index;
}

// Purpose: copies every line of the input file into a file vector and for
//          each line, read in each word and hash each one in the hash table
// Parameters: path of file to open, index of where the file is located
// Returns: nothing
void gerp::copyFileLines(string pathToOpen, int index)
{
    string line, word;
    ifstream input;

    openFile(pathToOpen, input);

    int lineCounter = 1;

    while (getline(input, line)) {

        // line 1 will be stored in index 1, line 2 in index 2, etc...
        files.at(index).push_back(line);

        // declare string stream for each line read in
        stringstream lineStream(line);
        while (lineStream >> word) {
            hashWord(word, lineCounter, index); 
        }
        lineCounter++;
    }
    input.close();
}

// Purpose: strips off non alphanumeric characters from the front
//          and back of the input string
// Parameters: input string
// Returns: a string that begins and ends with alphanumeric characters
string gerp::validWord(string inputWord)
{
    // call string processing on input word and return stripped word
    return stripNonAlphaNum(inputWord);
}

// Purpose: adds word into the hash table
// Parameters: word to be hashed, line number where word is located, index of
//             file the word is located in
// Returns: nothing
void gerp::hashWord(string word, int lineCounter, int index)
{
    // strip off non alphanumeric characters from input word
    string stripped = (validWord(word));
    // "standardize" word into lowercase to store in hash as key
    string lowercase = (wordLowerCase(stripped));

    // create and insert element into hash table
    wordInstance wordToHash = {stripped, lineCounter, index};
    table.insert(wordToHash, lowercase);
}

// Purpose: iterates through each character in the string and turns them into
//          lowercase (if they are alphanumeric characters)
// Parameters: word to be turned into all lowercase
// Returns: all lowercase version of word
string gerp::wordLowerCase(string word)
{
    string lowercase = "";

    int length = word.size();
    for (int i = 0; i < length; i++) {
        if (isalnum(word[i])) {
            // only add letters if they are lowercase and alphnumerical
            lowercase += tolower(word[i]);
        }
    }
    return lowercase;   
}

// Purpose: when user passes in a word, send output stream for every instance 
//          of that word. (in other words, check the bucket; if it is full, 
//          print the bucket -- path, line number, and line content)
// Parameters: word to search in hash, output stream to print out the path,
//             line number, and line content, and boolean that determines if
//             the search is case-sensitive or not
// Returns: nothing
void gerp::wordSearch(string word, ofstream &output, bool sensitive)
{
    string stripped = (validWord(word));
    string lowercase = (wordLowerCase(stripped));

    // in vector of HashElements
    int index = table.getHashVal(lowercase);
    int size = table.bucketSize(index);

    bool printed = false;
    for (int i = 0; i < size; i++) {
        // key word to check against
        string keyCheck = table.getKey(index, i);
        if (keyCheck == lowercase) {
            // get vector of particular word
            vector<wordInstance> toPrint = table.getElement(index, i);
            int length = toPrint.size();
            for (int j = 0; j < length; j++) {
                if (sensitive) {
                    // must match word exactly
                    if (stripped == toPrint.at(j).w) {
                        printGerp(j, toPrint, output);
                        printed = true;
                    }
                } else {
                    // in insensitive case, print entire list
                    printGerp(j, toPrint, output);
                    printed = true;
                }
            }
        }
    }
    // specific messages depending on case
    if (!printed and sensitive) {
        output << word
               << " Not Found. Try with @insensitive or @i.\n";
    } else if (!printed and !sensitive) {
        output << word << " Not Found.\n";
    }
}

// Purpose: helper function to print out the path, line numeber, and content
// Parameters: index of word location in hash table, vector of wordInstances
//             which contain the line number and file index of the word's
//             location, and outputstream
// Returns: nothing
void gerp::printGerp(int index, vector<wordInstance> toPrint, ofstream &output)
{
    int indexOfFile = toPrint.at(index).fileIndex;
    int lineNumber = toPrint.at(index).lineNum;
    output << files.at(indexOfFile).at(0)
           << ":" << lineNumber
           << ": "
           << files.at(indexOfFile).at(lineNumber) << endl;   
}

// Purpose: runs query loop for gerp program
// Parameters: output file that we will redirect output to
// Returns: nothing
void gerp::queryLoop(string outputFile)
{
    ofstream outfile;
    openFile(outputFile, outfile);

    string command, word;
    bool keepGoing = true;

    cout << "Query? ";
    cin >> command;
    while (keepGoing) {
        if (command == "@q" or command == "@quit") {
            keepGoing = false;
        } else if (command == "@f") {
            cin >> word;
            outfile.close();
            openFile(word, outfile);
        } else if (command == "@i" or command == "@insensitive") {
            cin >> word;
            wordSearch(word, outfile, false);
        } else {
            wordSearch(command, outfile, true);
        }

        if (keepGoing) {
            cout << "Query? ";
            cin >> command;
        }
    }
    cout << "Goodbye! Thank you and have a nice day.\n";
    
    outfile.close();
    return;
}

// Purpose: runs gerp program by calling helper functions :D
// Parameters: input directory and output file (from cml)
// Returns: nothing
void gerp::runGerp(string inputDir, string outputFile)
{
    generateFileTree(inputDir);

    int length = files.size();
    // iterate through every file stored
    for (int i = 0; i < length; i++) {
        copyFileLines(files.at(i).at(0), i);
    }

    queryLoop(outputFile);
}