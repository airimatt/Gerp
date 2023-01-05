/*
 *  project 4: gerp
 *  
 *  gerp.h
 *  Airi Matsushita & Darya Clark
 *  1/12/22
 *
 *  Contains interface of the member functions of the gerp class, 
 *  which contains the main logic of the gerp program
 * 
 */

#ifndef GERP_H_
#define GERP_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "hashTable.h"
#include "DirNode.h"
#include "FSTree.h"

using namespace std;

class gerp {

    public:

        gerp();
        ~gerp();
        
        void runGerp(string inputDir, string outputFile);

    private:

        vector<vector<string>> files;
        hashTable table;

        template<typename streamtype>
        void openFile(string file_name, streamtype &stream);
        
        void generateFileTree(string inputDir);

        // goes down to leaf of the tree
        void filePathTraversal(DirNode *currDir, string parentDir);
        
        // once at leaf, store the path from directory to target file
        void storePath(string pathToFile);
        
        int getFileIndex();

        // once at leaf, calls this function on file at leaf and gets info
        void copyFileLines(string pathToOpen, int index);        

        string validWord(string inputWord);

        void hashWord(string word, int lineCounter, int index);

        string wordLowerCase(string word);

        void wordSearch(string word, ofstream &output, bool sensitive);

        void printGerp(int index, vector<wordInstance> toPrint, 
                       ofstream &output);

        // runs query loop asking for word to search
        void queryLoop(string outputFile);

};

#endif