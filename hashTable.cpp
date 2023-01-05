/*
 *  project 4: gerp
 *  
 *  hashTable.cpp
 *  Airi Matsushita & Darya Clark
 *  1/12/22
 *
 *  Contains implementations of the hashTable class, which contain member
 *  functions that allow for efficient use of the hash data structure
 *  in the gerp program 
 */

#include "hashTable.h"

#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

// Purpose: generates random (huge) integer in memory
// Parameters: key element
// Returns: random integer in memory where key will be placed in hash
static size_t hashFunction(KeyType key)
{
    return std::hash<KeyType>{}(key);
}

// Purpose: constructor, sets up size of hash table
// Parameters: nothing
// Returns: nothing
hashTable::hashTable() 
{
    tableSize = 1000;
    numElements = 0;
    
    wordTable.resize(tableSize);
}

// Purpose: destructor, frees all memory used in program
// Parameters: nothing
// Returns: nothing
hashTable::~hashTable() 
{
    eraseTable();
}

// Purpose: frees all memory used in hash table
// Parameters: nothing
// Returns: nothing
void hashTable::eraseTable()
{
    for (int i = 0; i < tableSize; i++) {
        vector<HashElement> curr = wordTable.at(i);
        int length = curr.size();
        for (int j = 0; j < length; j++) {
            curr.at(j).value.clear();
        }
        curr.clear();
    }
    wordTable.clear();
}

// Purpose: prints key value pairs in hash table (used for testing/debugging)
// Parameters: nothibng
// Returns: nothing
void hashTable::printHashTable() 
{
    for (int i = 0; i < tableSize; i++) {
        cout << i << "\t"; 
        vector <HashElement> *curr = &wordTable.at(i);
        int size = curr->size();
        for (int j = 0; j < size; j++) {
            vector <wordInstance> *inner = &curr->at(j).value;
            int length = inner->size();
            for (int k = 0; k < length; k++) {
                cout << "{ " << inner->at(k).w << ", " 
                    << inner->at(k).lineNum << ", " 
                     << inner->at(k).fileIndex << " } ";
            }
            cout << " | ";
        }
        cout << endl;
    }
}

// Purpose: returns the table size
// Parameters: nothing
// Returns: the number of buckets, or the hash table size
int hashTable::numBuckets()
{
    return tableSize;
}

// Purpose: returns the size of a bucket at given index in hash table
// Parameters: index of hash table
// Returns: size of bucket at given index
int hashTable::bucketSize(int WTindex)
{
    return wordTable.at(WTindex).size();
}

// Purpose: returns the hash value at a given index of a bucket at another 
//          index in the hash table 
// Parameters: index of bucket in hash table, index of specific HashElement
//             inside the bucket
// Returns: vector of wordInstances (value)
ValueType hashTable::getElement(int bucket, int indexInBucket)
{
    return wordTable.at(bucket).at(indexInBucket).value;
}

// Purpose: returns the key at a given index of a bucket at another 
//          index in the hash table 
// Parameters: index of bucket in hash table, index of specific HashElement
//             inside the bucket
// Returns: word (key) of hash element
KeyType hashTable::getKey(int bucket, int indexInBucket)
{
    return wordTable.at(bucket).at(indexInBucket).key;
}

// Purpose: inserts new wordInstance into the hash table
// Parameters: wordInstance struct containing the word, line number, and
//             file index, and "standardized" (lowercased) word to add as key
// Returns: nothing
void hashTable::insert(wordInstance toAdd, string standardized)
{
    int WTindex = hashFunction(standardized) % tableSize;

    int loadFactor = numElements / tableSize;
    if (loadFactor > 0.7) {
        expand();
    }
    
    vector<HashElement> *curr = &(wordTable.at(WTindex));
    int length = curr->size();
    bool inserted = false;
    
    for (int i = 0; i < length; i++) {
        HashElement *check = &(curr->at(i));
        wordInstance *collision = &(check->value.back());

        if (check->key == standardized) {
        // if word does not exist in the same line and file as 
        // an element already in the hash table, add the instance
            if (!checkCollision(toAdd, *collision)) {
                check->value.push_back(toAdd);
                    inserted = true;
                    numElements++;
                    return;
            } else {          // do not insert toAdd when duplicate (same word
                return;       // occurs more than once on the same line)
            }
        }
    }
    // if the word has not been inserted (does not exist in hash table yet),
    // call firstInsert function to insert it as a key-value pair
    if (!inserted) {
        firstInsert(WTindex, toAdd, standardized);
        numElements++;
    }
}

// Purpose: if word does not exist in the hash table, create a wordInstance and
//          a hashElement of the word and add to hash table
// Parameters: index of hash table (where the word maps to), a wordInstance,
//             and the "standardized" version of the word to add as key
// Returns: nothing
void hashTable::firstInsert(int WTindex, wordInstance first, 
                            string standardized)
{
    HashElement toAdd;

    toAdd.key = standardized;
    toAdd.value.push_back(first);

    wordTable.at(WTindex).push_back(toAdd);
}

// Purpose: checks for collision of a word already existing in the
//          same line and file as the word to be potentially added
// Parameters: wordInstance to be potentially added, wordInstance to check
//             against for possibly collision in chain
// Returns: true if same word exists on the same line in the same file, false
//          otherwise
bool hashTable::checkCollision(wordInstance toAdd,
                               wordInstance toCheck)
{
    if ((toAdd.w == toCheck.w) and
        (toAdd.lineNum == toCheck.lineNum) and
        (toAdd.fileIndex == toCheck.fileIndex)) {
        return true;
    }
    return false;
}

// Purpose: performs mod operator to obtain index of word in hash table
// Parameters: key in hash table
// Returns: index of where word goes in hash table
size_t hashTable::getHashVal(KeyType key) 
{
    size_t WTindex = hashFunction(key) % tableSize;
    return WTindex;
}

// Purpose: rehashes table when necessary
// Parameters: nothing
// Returns: nothing
void hashTable::expand() 
{   
    int oldTableSize = tableSize;

    vector<vector<HashElement>> oldWordTable(wordTable);

    eraseTable();

    tableSize *= 2;
    wordTable.resize(tableSize);

    for (int i = 0; i < oldTableSize; i++) {
        vector<HashElement> tempVector(oldWordTable.at(i));
        int index = tempVector.size();
        for (int j = 0; j < index; j++) {
            string standard = (tempVector.at(j).key);
            vector<wordInstance> inner(tempVector.at(j).value);
            int length = inner.size();
            for (int k = 0; k < length; k++) {
                wordInstance curr(inner.at(k));
                insert(curr, standard);
            }
        }
    }
}