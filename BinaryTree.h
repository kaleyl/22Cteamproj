//
//  BinaryTree.h
//  Team_Project_Group_9
//

#ifndef BinaryTree_h
#define BinaryTree_h

#include <fstream>

#include "BinaryNode.h"
#include "Queue.h"

template<class ItemType>
class BinaryTree
{
protected:
    BinaryNode<ItemType>* rootPtr;       // ptr to root node
    //BinaryNode<ItemType>* leftMost(BinaryNode<ItemType>* nodePtr); // point to the left most node smallest
    //BinaryNode<ItemType>* rightMost(BinaryNode<ItemType>* nodePtr); //point to the right most largest
    int count;                            // number of nodes in tree
    
public:
    // "admin" functions
    BinaryTree() {rootPtr = 0; count = 0;}
    BinaryTree(const BinaryTree<ItemType> & tree){ }
    virtual ~BinaryTree() { destroyTree(rootPtr);}
    BinaryTree & operator = (const BinaryTree & sourceTree);
    
    // common functions for all binary trees
    bool isEmpty() const    {return count == 0;}
    int size() const        {return count;}
    void clear()            {destroyTree(rootPtr); rootPtr = 0; count = 0;}
    //traversals
    void preOrder(void visit(ItemType &)) const {_preorder(visit, rootPtr);}
    void inOrder(void visit(ItemType &)) const  {_inorder(visit, rootPtr);}
    void postOrder(void visit(ItemType &)) const{_postorder(visit, rootPtr);}
    void RightRootLeft(void visit(ItemType &),int level) const{_RightRootLeft(visit, rootPtr, level);}
    
    void breathTrav(void visit (ItemType&)) const;
    
    // abstract functions to be implemented by derived class
    virtual bool insert(const ItemType & , int compare(const ItemType&, const ItemType&)) = 0;
    virtual bool remove(const ItemType & data) = 0;
    virtual bool getEntry(const ItemType & target, int compare(const ItemType&, const ItemType&), void visit(ItemType&)) const = 0;
    
    
    
private:
    // delete all nodes from the tree
    void destroyTree(BinaryNode<ItemType>* nodePtr);
    
    // copy from the tree rooted at nodePtr and returns a pointer to the copy
    BinaryNode<ItemType>* copyTree(const BinaryNode<ItemType>* nodePtr);
    
    // internal traverse
    void _preorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
    void _inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
    void _postorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
    void _RightRootLeft(void visit(ItemType &), BinaryNode<ItemType>* nodePtr, int level) const;
    
};

//////////////////////////////////////////////////////////////////////////

//the function find and returns the pointer of the rightmost node in the tree
/*
template<class ItemType>
BinaryNode<ItemType>* BinaryTree<ItemType>:: rightMost(BinaryNode<ItemType>* nodePtr)
{
    if(nodePtr->getRightPtr())
    {
        return rightMost(nodePtr->getRightPtr());
    }
    return nodePtr;
}

//the function find and returns the pointer of the leftmost node in the tree
template<class ItemType>
BinaryNode<ItemType>* BinaryTree<ItemType>:: leftMost(BinaryNode<ItemType>* nodePtr)
{
    if(nodePtr->getLeftPtr())
    {
        return leftMost(nodePtr->getLeftPtr());
    }
    return nodePtr;
}
*/

//the function copies the tree
template<class ItemType>
BinaryNode<ItemType>* BinaryTree<ItemType>::copyTree(const BinaryNode<ItemType>* nodePtr)
{
    BinaryNode<ItemType>* newNodePtr = nullptr;
    
    if (nodePtr)
    {
        newNodePtr = new BinaryNode<ItemType>(nodePtr->getItem());
        newNodePtr->setLeftPtr(copyTree(nodePtr->getLeftPtr()));
        newNodePtr->setRightPtr(copyTree(nodePtr->getRightPtr()));
        return newNodePtr;
    }
    return  nullptr;
}


//the functio destroy the tree
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
    //check if it is a leaf
    if(!nodePtr)
    {
        return;
    }
    else if(!nodePtr->getLeftPtr() && !nodePtr->getRightPtr())
    {
        delete nodePtr;
    }
    
    //if not check the right one
    if(nodePtr->getRightPtr())
    {
        destroyTree(nodePtr->getRightPtr());
    }
    else if(nodePtr->getLeftPtr())
    {
        destroyTree(nodePtr->getLeftPtr());
    }
    
}

//This function perform the PreOrder depth-first traversal
template<class ItemType>
void BinaryTree<ItemType>::_preorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr != nullptr)
    {
        ItemType item = nodePtr->getItem();
        visit(item);
        _preorder(visit, nodePtr->getLeftPtr());
        _preorder(visit, nodePtr->getRightPtr());
    }
}

//This function perform the InOrder depth-first traversal
template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr != nullptr)
    {
        ItemType item = nodePtr->getItem();
        _inorder(visit, nodePtr->getLeftPtr());
        visit(item);
        _inorder(visit, nodePtr->getRightPtr());
    }
}

//This function perform the PostOrder depth-first traversal
template<class ItemType>
void BinaryTree<ItemType>::_postorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr != nullptr)
    {
        ItemType item = nodePtr->getItem();
        _postorder(visit, nodePtr->getLeftPtr());
        _postorder(visit, nodePtr->getRightPtr());
        visit(item);
    }
}

//This function perform the breath-first traversal
template <class ItemType>
void BinaryTree<ItemType>::breathTrav(void visit (ItemType&)) const
{
    Queue< BinaryNode<ItemType>* > que;
    
    BinaryNode<ItemType>* temp = rootPtr;
    if(temp)
        que.enqueue(temp);
    while(!que.isEmpty())
    {
        que.dequeue(temp);
        ItemType item = temp->getItem();
        visit(item);
        if(temp->getLeftPtr())
            que.enqueue(temp->getLeftPtr());
        if(temp->getRightPtr())
            que.enqueue(temp->getRightPtr());
    }
}

// traverse the tree in the order of right, root, left
// and each displayed item should be indented based on its level
template<class ItemType>
void BinaryTree<ItemType>::_RightRootLeft(void visit(ItemType &), BinaryNode<ItemType>* nodePtr, int level) const
{
    if (nodePtr != 0)
    {
        _RightRootLeft(visit, nodePtr->getRightPtr(),level+1);
        cout << endl;
        for(int i = 0; i<level; i++) cout << "\t\t";
        cout << "(Level " << level << ")  ";
        ItemType item = nodePtr->getItem();
        visit(item);
        _RightRootLeft(visit, nodePtr->getLeftPtr(),level+1);
    }
}

//This function overload the = operator to assign the one tree to the other
template<class ItemType>
BinaryTree<ItemType> & BinaryTree<ItemType>::operator=(const BinaryTree<ItemType> & sourceTree)
{
    if (this != &sourceTree)
    {
        this->clear();
        rootPtr = this->copyTree(sourceTree.rootPtr);
        count = sourceTree.count;
    }
    return *this;
    
}






#endif 


