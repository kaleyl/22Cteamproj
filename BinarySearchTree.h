/**~*~*
 Binary Search Tree template
 *~**/


#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"

template<class ItemType>
class BinarySearchTree : public BinaryTree<ItemType>
{
public:
    // insert a node at the correct location
    bool insert(const ItemType & newEntry, int compare(const ItemType&, const ItemType&));
    // remove a node if found
    bool remove(const ItemType & anEntry);
    // find a target node
    bool getEntry(const ItemType & target, int compare(const ItemType&, const ItemType&), void visit(ItemType&)) const;
    

private:
    // internal insert node: insert newNode in nodePtr subtree
    BinaryNode<ItemType>* _insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode, int compare(const ItemType&, const ItemType&));
    
    // internal remove node: locate and delete target node under nodePtr subtree
    BinaryNode<ItemType>* _remove(BinaryNode<ItemType>* nodePtr, const ItemType target, bool & success);
    
    // delete target node from tree, called by internal remove node
    BinaryNode<ItemType>* deleteNode(BinaryNode<ItemType>* targetNodePtr);
    
    // remove the leftmost node in the left subtree of nodePtr
    BinaryNode<ItemType>* removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType & successor);

    
};

///////////////////////// public function definitions ///////////////////////////

/********************************************************
 This function will insert the targeted the item in
 the tree and return a bool value indicating whether
 the item has been successfully inserted
 ********************************************************/
template<class ItemType>
bool BinarySearchTree<ItemType>::insert(const ItemType & newEntry, int compare(const ItemType&, const ItemType&) )
{
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
    this->rootPtr = _insert(this->rootPtr, newNodePtr, compare);
    return true;
}


/********************************************************
 This function will remove the targeted the item in
 the tree and return a bool value indicating whether
 the item has been successfully removed
 ********************************************************/
template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType & target)
{
    bool isSuccessful = false;
    this->rootPtr = _remove(this->rootPtr, target, isSuccessful);
    return isSuccessful;
}



/********************************************************
 This function will get and return the reference targeted
 item and return a bool value indicating whether
 the item has been successfully removed
 This function is used to search item in the tree
 ********************************************************/
template<class ItemType>
bool BinarySearchTree<ItemType>::getEntry(const ItemType & target, int compare(const ItemType&, const ItemType&), void visit(ItemType&)) const
{
    
    bool found = false;
    BinaryNode<ItemType>* nodePtr = this->rootPtr;
    ItemType anItem;
    int compareValue;
    
    while (nodePtr != nullptr) {
        
        anItem = nodePtr->getItem();
        //visit(anItem);
        compareValue = compare(anItem, target);
        
        if (compareValue == 1) {
            nodePtr = nodePtr->getLeftPtr();
        }
        
        else {
            if (compareValue == 0) {
                if (!found) {
                    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
                    cout << "ID      Title                   Artist                 Album                          Genre    Duration  Year\n";
                    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
                }
                found = true;
                visit(anItem);
            }
            nodePtr = nodePtr->getRightPtr();
        }
        
    }
    if (found) cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
    return found;
    
}



//////////////////////////// private functions ////////////////////////////////////////////

//function COMPARE : 0 = equal, 1 = left > right, -1 = left < right
//the insert function will insert item into the node of the tree
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr,
                                                          BinaryNode<ItemType>* newNodePtr, int compare(const ItemType&, const ItemType&))
{
    //function COMPARE : 0 = equal, 1 = left > right, -1 = left < right
    if( nodePtr == nullptr)
    {
        nodePtr = newNodePtr;
        // return nodePtr;
    }
    else
    {
        ItemType rootItem = nodePtr->getItem();
        ItemType newItem = newNodePtr->getItem();
        
        if( compare(newItem, rootItem ) == -1 )     // if new node is smaller than root
        {
            nodePtr->setLeftPtr(_insert (nodePtr->getLeftPtr(), newNodePtr, compare));
        }
        else
        {
            nodePtr->setRightPtr(_insert (nodePtr->getRightPtr(), newNodePtr, compare ));
        }
    }
    return nodePtr;
}


//the _remove function will remove item node from the tree
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr,
                                                          const ItemType target,
                                                          bool & success)

{
    if (nodePtr == 0)
    {
        success = false;
        return 0;
    }
    if (nodePtr->getItem() > target)
        nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success));
    else if (nodePtr->getItem() < target)
        nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success));
    else
    {
        nodePtr = deleteNode(nodePtr);
        success = true;
    }
    return nodePtr;
}


//the _delete function will delete the node from the tree
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::deleteNode(BinaryNode<ItemType>* nodePtr)
{
    if (nodePtr->isLeaf())
    {
        delete nodePtr;
        nodePtr = 0;
        return nodePtr;
    }
    else if (nodePtr->getLeftPtr() == 0)
    {
        BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getRightPtr();
        delete nodePtr;
        nodePtr = 0;
        return nodeToConnectPtr;
    }
    else if (nodePtr->getRightPtr() == 0)
    {
        BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftPtr();
        delete nodePtr;
        nodePtr = 0;
        return nodeToConnectPtr;
    }
    else
    {
        ItemType newNodeValue;
        nodePtr->setRightPtr(removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
        nodePtr->setItem(newNodeValue);
        return nodePtr;
    }
}


//the _removeLeftmostNode function will remove the leftmost node in the tree
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType>* nodePtr,
                                                                     ItemType & successor)
{
    if (nodePtr->getLeftPtr() == 0)
    {
        successor = nodePtr->getItem();
        return deleteNode(nodePtr);
    }
    else
    {
        nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
        return nodePtr;
    }
}





#endif
