/**~*~*
 ListNode template
 *~**/


#ifndef ListNode_h
#define ListNode_h

// Declare a class for ListNode
template <class T>
struct ListNode
{
    T item;     // The value in this node
    ListNode<T>
    *next;  // To point to the next node

    
};


#endif /* ListNode_h */
