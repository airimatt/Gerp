/*
 *  project 4: gerp
 *  
 *  FSTreeTraversal.cpp
 *  Airi Matsushita & Darya Clark
 *  29/11/22
 *
 *  This file contains the implementation of the FSTreeTraversal function, 
 *  which recursively traverses down the directory/file tree and lists the 
 *  paths to get to each file, as well as a main function to create a 
 *  directory/file tree and call the FSTreeTraversal function
 */

#include "FSTree.h"
#include "DirNode.h"

#include <fstream>
#include <iostream>

using namespace std; 


void FSTreeTraversal(DirNode *currDir, string path);

// Purpose: creates tree directory, calls FSTreeTraversal function
// Parameters: integer storing argument count, pointer to argument value
// Returns: nothing
int main(int argc, char *argv[]) {
    
    (void) argc;
    
    // creates instance of FSTree
    FSTree fileTree(argv[1]);
    DirNode *root = fileTree.getRoot();
    
    if (!root->isEmpty()) {
        FSTreeTraversal(root, "");
    }

    return 0;
}

// Purpose: recursively traverses through directories, listing each 
//          one and its contents
// Parameters: pointer to current directory, string containing path to file
// Returns: nothing
void FSTreeTraversal(DirNode *currDir, string path) {
   
   // update path to hold current directory's name     
   path += currDir->getName() + "/";
    // if the directory has files, print each 
    if (currDir->hasFiles()) {
        for (int i = 0; i < currDir->numFiles(); i++) {
            cout << path + currDir->getFile(i) << endl;
        }
    }
    // if the directory has subdirectories, iterate through each and call func
    if (currDir->hasSubDir()) {
        for (int i = 0; i < currDir->numSubDirs(); i++) {
            FSTreeTraversal(currDir->getSubDir(i), path);
        }
    }
}