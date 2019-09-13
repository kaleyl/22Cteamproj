/**~*~*
 HashTable template
 *~**/

#ifndef _HASH_TABLE
#define _HASH_TABLE

#include "LinkedList.h"
#include <iomanip>
#include <string>

using namespace std;

template <class ItemType>
class HashTable {

private:

	LinkedList<ItemType>* table;
	int size;
	int collisionCount;
	int count;  // Count number of buckets that have been occupied
	void _rehash();

public:

	// constructor
	HashTable() {
		size = 10;      // default size of hash table
		table = new LinkedList<ItemType>[size];
		collisionCount=0;
		count=0;
	}

    HashTable (int n){
        size = n;
        table = new LinkedList<ItemType>[size];
        collisionCount=0;
        count=0;
    }

	~HashTable() {
        destroyHash();
	}

	int hash(string key) {
		int k;
		k = stoi(key);
		return (k % size);
	}
    
    // bad hash function (sum of digits)
    // because
    // max sum is 9 + 9 + 9 + 9 = 36
    // but there are 53 buckets
    /*
    int hash (string key){
        int sum=0;
        
        for (int i=0; i<key.length(); ++i)
            sum += (int)key[i] - 48;
        
        return sum;
    }
     */

    bool isPrime (int);
	bool insertHash(string key, ItemType & newData);
	bool deleteHash(string key, ItemType & returnedItem);
	bool searchHash(string key, ItemType & returnedItem);
	void destroyHash();
	void printHash() {
         for (int i = 0; i < size; i++)
         {
             table[i].displayList();
         }
	}

	void printStat(){
        
	    string Line = "========================================\n";
	    cout << Line << "Statistic of Hash" << endl << Line;
	    cout << "Load Factor:         " << getLoadFactor()*100 << "%" << endl;
	    cout << "Number of Collision: " << getCollisionCount() << endl;
        cout << Line << endl;
	}

    // setters and getters
    
	int getSize() { return size; }
	int getCollisionCount () {return collisionCount;}
    int getCount () {return count;}
    double getLoadFactor () { return (double)count / (double)size; }
    
    void writeToFile(string);
};


//This function will check if the parameter is a prime number
template <class ItemType>
bool HashTable<ItemType>::isPrime(int n)
{
    for (int i=3; i<n; ++i)
    {
        if (n%i==0)
            return false;
    }
    return true;
}


/********************************************************
/This function insert item into the hash table          *
/it will return a bool value to indicate if the item is *
/inserted successfully.                                 *
 ********************************************************/
template <class ItemType>
bool HashTable<ItemType>::insertHash(string key, ItemType & newData)
{
    bool inserted = false;
    int index = hash(key);

    if (getLoadFactor()>=0.75)
        _rehash();

    inserted = table[index].insertNode(newData);

    if (inserted)
    {
        if (table[index].getCount()==1)
            count++; //update count in the has

        if (table[index].getCount()>1)
            collisionCount++; //update collision count
    }
    return inserted;
}


/********************************************************
 /This function deletes item into the hash table         *
 /it will return a bool value to indicate if the item is *
 /deleted successfully.                                  *
 ********************************************************/

template <class ItemType>
bool HashTable<ItemType>::deleteHash(string key, ItemType & returnedItem)
{
    int index = hash(key);
    bool found=false;
    bool deleted = false;

    found = table[index].searchList(returnedItem);

    if (found)
        deleted = table[index].deleteNode(returnedItem);

    if (deleted && table[index].getCount()==0)
        count--;

    return deleted;
}


/********************************************************
This function seraches and return the targeted item
 in the hash
 ********************************************************/

template <class ItemType>
bool HashTable<ItemType>::searchHash(string key, ItemType & returnedItem)
{
    int index;
    index = hash(key);
    
    returnedItem.setId(key);

    return table[index].searchList(returnedItem);
}


/********************************************************
 This function destorys the hashTable
 ********************************************************/
template <class ItemType>
void HashTable<ItemType>::destroyHash()
{
    for (int i = 0; i < size; ++i)
    {
        if (table[i].getCount()!=0)
        {
            table[i].clearList();
            count--;
        }
    }
}


/********************************************************
 This function will rehash when it is needed, which is
 decided upon the load factor
 ********************************************************/
template <class ItemType>
void HashTable<ItemType>::_rehash()
{
    
    LinkedList<ItemType>* temp = table;
    
    int oldSize = size;
    size *= 2;
    
    while (!isPrime(size))
    {
        size++;
    }
    
    cout << ">>>>>>>>>>>>>>>";
    cout << "rehashing...from size " << oldSize << " to " << size;
    cout << "<<<<<<<<<<<<<<<\n\n";

    table = new LinkedList<ItemType>[size];
    collisionCount = 0;
    count = 0;
    
    //rehash
    for (int i=0; i<oldSize; i++)
    {
        ListNode<ItemType> *nodePtr = temp[i].getHead()->next;

        while (nodePtr)
        {
            ItemType anItem = nodePtr->item;
            insertHash(anItem.getId(), anItem);
            nodePtr = nodePtr->next;
        }
    }
    
}


/********************************************************
 This function print the hashTable to a file
 ********************************************************/
template <class ItemType>
void HashTable<ItemType>::writeToFile(string filename) {
    ofstream outputFile;
    outputFile.open(filename);
    
    for (int i = 0; i < size; i++) {
        table[i].writeToFile(outputFile);
    }
    
    outputFile.close();
    
}

#endif // _HASH_TABLE
