#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <fstream>

#include "ListNode.h"

template <class T>
class LinkedList
{
private:
    ListNode<T> *head;      // List head pointer
    int count;           // To keep track of the number of nodes in the list


public:
    LinkedList();  // Constructor
    ~LinkedList() {this->clearList();}   // Destructor

    // Linked list operations
    int getCount() const {return count;}
    ListNode<T>* getHead() {return head;}
    bool insertNode(T);
    bool deleteNode(T);
    void displayList() ;
    bool searchList(T&) ;
    void writeToFile(ofstream&) const;
    void clearList();
};

//**************************************************
// Constructor
// This function allocates and initializes a sentinel node
//      A sentinel (or dummy) node is an extra node added before the first data record.
//      This convention simplifies and accelerates some list-manipulation algorithms,
//      by making sure that all links can be safely dereferenced and that every list
//      (even one that contains no data elements) always has a "first" node.
//**************************************************
template <class T>
LinkedList<T>::LinkedList()
{
    head = new ListNode<T>; // head points to the sentinel node
    head->next = NULL;
    count = 0;
}

//**************************************************
// displayList shows the value
// stored in each node of the linked list
// pointed to by head.
//**************************************************
template <class T>
void LinkedList<T>::displayList()
{
    ListNode<T> *pCur;     // To move through the list
    
    const string LINE = "======= ======================= ======================= =============================== ======= ========= =====\n";
    
    /*cout << setprecision(2) << fixed;
     cout << LINE;
     cout << setw(8) << left << "ID"
     << setw(24) << left << "Title"
     << setw(23) << left << "Artist"
     << setw(31) << left << "Album"
     << setw(7) << left << "Genre"
     << setw(10) << left << "Duration"
     << setw(5) << left << "Year" << endl;
     cout << LINE;*/
    // While pCur points to a node, traverse the list.
    pCur = head->next;  // Skip the head of the list.
    while (pCur)        // Traverse the list.
    {
        // Display the value in the current node node.
        /*cout << setw(8) << left << pCur->item.getKey()
         << setw(24) << left << pCur->item.getItem().getTitle()
         << setw(23) << left << pCur->item.getItem().getArtist()
         << setw(31) << left << pCur->item.getItem().getAlbum()
         << setw(7) << left << pCur->item.getItem().getGenre()
         << setw(10) << left << pCur->item.getItem().getDuration()
         << setw(5) << left << pCur->item.getItem().getYear() << endl;
         */
        cout << pCur->item << endl;
        
        // Move to the next node.
        pCur = pCur->next;
    }
}

//**************************************************
// The insertNode function inserts a node with
// dataIn copied to its value member.
//**************************************************
template <class T>
bool LinkedList<T>::insertNode(T dataIn)
{
    ListNode<T> *newNode;  // A new node
    ListNode<T> *pCur;     // To traverse the list
    ListNode<T> *pPre;     // The previous node
    
    // Allocate a new node and store dataIn there.
    newNode = new ListNode<T>;
    newNode->item = dataIn;
    
    // Initialize pointers
    pPre = head;
    pCur = head->next;
    
    // Find location: skip all nodes whose id is less than dataIn's id
    while (pCur != NULL && pCur->item< dataIn)
    {
        pPre = pCur;
        pCur = pCur->next;
    }
    
    // Return false if key is duplicated (Key is unique)
    if (pCur != NULL && pCur->item == dataIn)
        return false;

    // Insert the new node between pPre and pCur
    pPre->next = newNode;
    newNode->next = pCur;
    
    count++;
    
    return true;
}

//**************************************************
// The deleteNode function searches for a node
// with targetId as its value. The node, if found, is
// deleted from the list and from memory.
//**************************************************
template <class T>
bool LinkedList<T>::deleteNode(T deleteItem)
{
    ListNode<T> *pCur;       // To traverse the list
    ListNode<T> *pPre;        // To point to the previous node
    
    // Initialize pointers
    pPre = head;
    pCur = head->next;
    
    // Find node containing the target: Skip all nodes whose id is less than the target
    while (pCur != NULL && pCur->item < deleteItem)
    {
        pPre = pCur;
        pCur = pCur->next;
    }
    
    // If found, delete the node
    if (pCur != NULL && pCur->item == deleteItem)
    {
        pPre->next = pCur->next;
        delete pCur;
        return true;
    }
    else
        return false;
    
}

//**************************************************
// The searchList function searches for a node
// with the id entered by the user
//**************************************************
template <class T>
bool LinkedList<T>::searchList(T& ReturnedItem)
{
    
    ListNode<T> *pCur;       // To traverse the list
    
    // Initialize pointers
    pCur = head;
    
    // Find node containing the target: Skip all nodes whose id is less than the target
    while (pCur != NULL && pCur->item < ReturnedItem)
    {
        pCur = pCur->next;
    }
    
    // If found, delete the node
    if (pCur != NULL && pCur->item == ReturnedItem)
    {
        ReturnedItem = pCur->item;
        return true;
    }
    else
        return false;
    
}

//**************************************************
// writeToFile writes data
// stored in each node of the linked list
// into an output file
//**************************************************
template <class T>
void LinkedList<T>::writeToFile(ofstream &outputFile) const
{
    ListNode<T> *pCur;     // To move through the list
    
    // While pCur points to a node, traverse the list.
    pCur = head->next;  // Skip the head of the list.
    while (pCur != NULL)        // Traverse the list.
    {
        // Write the value in the current node to the output file.
        outputFile << pCur->item.getId() << ";"
        << pCur->item.getTitle() << ";"
        << pCur->item.getArtist() << ";"
        << pCur->item.getAlbum() << ";"
        << pCur->item.getGenre() << ";"
        << pCur->item.getDuration() << ";"
        << pCur->item.getYear() << endl;
        
        // Move to the next node.
        pCur = pCur->next;
    }
    
}

//**************************************************
// This function deletes every node in the list.   *
//**************************************************
template <class T>
void LinkedList<T>::clearList()
{
    ListNode<T> *pCur;   // To traverse the list
    ListNode<T> *pNext;  // To point to the next node
    
    // Position nodePtr at the head of the list.
    pCur = head->next;
    
    // While pCur is not at the end of the list...
    while (pCur != NULL)
    {
        // Save a pointer to the next node.
        pNext = pCur->next;
        
        // Delete the current node.
        //cout << "DEBUG - Destructor: Now deleting " << pCur->item.getName() << endl;
        delete pCur;
        
        // Position pCur at the next node.
        pCur = pNext;
    }
    //cout << "DEBUG - Destructor: Now deleting the sentinel node toy id = " << head->toy.getId() << endl;
    delete head; // delete the sentinel node
}


#endif
