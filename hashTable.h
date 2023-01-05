/*
 *  project 4: gerp
 *  
 *  hashTable.h
 *  Airi Matsushita & Darya Clark
 *  1/12/22
 *
 *  Contains interface of the hashTable class, which contain member
 *  functions that allow for efficient use of the hash data structure
 *  in the gerp program 
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

// #include <bits/stdc++.h>
#include <vector>
#include <string>
#include <functional>

using namespace std;

struct wordInstance {

    string w;
    int lineNum;
    int fileIndex;
    
};

typedef std::string   KeyType;
typedef vector<wordInstance>  ValueType;

class hashTable {
    
    struct HashElement {
            KeyType    key;
            ValueType  value;
	};

    // pointer to array to contain vector of elements
    vector<vector<HashElement>> wordTable;
    
    public:

        hashTable();
        ~hashTable();

        void insert(wordInstance toAdd, string standardized);
        size_t getHashVal(KeyType key);
        ValueType getElement(int bucket, int indexInBucket);
        KeyType getKey(int bucket, int indexInBucket);
        int bucketSize(int WTindex);
        void printHashTable();
        
    private:
    
        int tableSize;
        int numElements;

        enum HashFunction {GOOD_HASH_FUNCTION};
        int numBuckets();
        bool checkCollision(wordInstance toAdd, wordInstance toCheck);
        int hashSize();
        void expand();
        void checkForExpand(KeyType key, size_t WTindex);
        // void printHashTable();
        void eraseTable();
        void firstInsert(int WTindex, wordInstance first, string standardized);

};

#endif