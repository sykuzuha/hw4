#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    int balanceFactor(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* n, int diff);
    AVLNode<Key, Value>* internalFind(const Key& key) const;

    


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
int AVLTree<Key, Value>::balanceFactor(AVLNode<Key, Value>* node)
{
    return this->height(node->getRight()) - this->height(node->getLeft()); 
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // DONE
    if(this->root_ == NULL)
    {
        Node<Key, Value>* root = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        this->root_ = root;
        return;
    }
    AVLNode<Key, Value>* prev = NULL;
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);

    while(current != NULL)
    {
        prev = current;
        if(new_item.first < current->getKey())
        {
            current = current->getLeft();
        }
        else if(new_item.first > current->getKey())
        {
            current = current->getRight();
        }
        else{
            current->setValue(new_item.second);
            return;
        }
    }
    AVLNode<Key, Value>* n = new AVLNode<Key, Value>(new_item.first, new_item.second, prev);
    if(new_item.first < prev->getKey())
    {
        prev->setLeft(n);
    }
    else{
        prev->setRight(n);
    }

    AVLNode<Key, Value>* p = n->getParent();
    n->setBalance(0);
    if(p->getBalance() == 1 || p->getBalance() == -1)
    {
        p->setBalance(0);
    }
    else if(p->getBalance() == 0)
    {
        int balanceAdjustment;
        if (p->getLeft() == n) {
            balanceAdjustment = -1; // The new node is a left child.
        } else {
            balanceAdjustment = 1; // The new node is a right child.
        }
        p->updateBalance(balanceAdjustment);
        insertFix(p, n);
    }
   
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
	AVLNode<Key, Value>* n = internalFind(key);
  if (!n) {
      return;
  }

	if (n->getLeft() && n->getRight()) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(n));
        nodeSwap(n, pred);
  }

	AVLNode<Key, Value>* p = n->getParent();
	int diff = 0;
    if(p != NULL)
    {
        if(p->getLeft() == n)
        {
            diff = 1;
        }
        else if(p->getRight() == n)
        {
            diff = -1;
        }
    }


	AVLNode<Key, Value>* child;
    if (n->getLeft() != NULL) {
        child = n->getLeft();
    } else {
        child = n->getRight();
    }

	if (n->getParent()) {
        if (n == p->getLeft()) {
            p->setLeft(child);
        } 
        else if (n == p->getRight()){ 
            p->setRight(child);
        }
    } 
    else 
    {    
        this->root_ = child;
    }

    if (child) {
        child->setParent(n->getParent());
    }


	delete n;
	removeFix(p, diff); 
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* y) {
    AVLNode<Key, Value>* x = y->getLeft();
    AVLNode<Key, Value>* T2 = x->getRight();

    // Perform rotation
    x->setRight(y);
    y->setLeft(T2);

    // Update parents
    if (T2) T2->setParent(y);
    x->setParent(y->getParent());
    y->setParent(x);

    // If y was the root, update the root
    if (!x->getParent()) this->root_ = x;
    else if (x->getParent()->getLeft() == y) x->getParent()->setLeft(x);
    else x->getParent()->setRight(x);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x) {
    AVLNode<Key, Value>* y = x->getRight(); 
    AVLNode<Key, Value>* T2 = y->getLeft();

    // Perform rotation
    y->setLeft(x);
    x->setRight(T2);

    // Update parents
    if (T2) T2->setParent(x);
    y->setParent(x->getParent());
    x->setParent(y);

    // If x was the root, update the root
    if (!y->getParent()) this->root_ = y;
    else if (y->getParent()->getLeft() == x) y->getParent()->setLeft(y);
    else y->getParent()->setRight(y);
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    if(p == NULL)
    {
        return;
    }
    AVLNode<Key, Value>* g = p->getParent();
    if(g == NULL)
    {
      return;
    }
    if(g->getLeft() == p) {
        g->updateBalance(-1);
        if(g->getBalance() == 0) //Case 1
        {
            return;
        }
        else if(g->getBalance() == -1) //Case 2
        {
            insertFix(g,p);
        }
        else if(g->getBalance() == -2)
        {
            if(p->getBalance() == -1) //Case 3
            {
                rotateRight(g);
                g->setBalance(0);
                p->setBalance(0);
            }
            else if(p->getBalance() == 1) //Case 3
            {
                rotateLeft(p);
                rotateRight(g);
                if(n->getBalance() == -1)
                {
                    g->setBalance(1);
                    p->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 0)
                {
                    g->setBalance(0);
                    p->setBalance(0);
                    n->setBalance(0);
                }
                else if(n->getBalance() == 1)
                {
                    g->setBalance(0);
                    p->setBalance(-1);
                    n->setBalance(0);
                }
            }
            
        }
      } 
      if(g->getRight() == p) 
      {
            g->updateBalance(1);
            if(g->getBalance() == 0) //Case 1
            {
                return;
            }
            else if(g->getBalance() == 1) //Case 2
            {
                insertFix(g,p);
            }
            else if(g->getBalance() == 2)
            {
                if(p->getBalance() == 1) //Case 3: Zig-Zig (Left-Left).
                {
                    rotateLeft(g);
                    g->setBalance(0);
                    p->setBalance(0);
                }
                else if(p->getBalance() == -1) //Case 3: Zig-Zag (Left-Right).
                {
                    rotateRight(p);
                    rotateLeft(g);
                    if(n->getBalance() == 1)
                    {
                        g->setBalance(-1);
                        p->setBalance(0);
                        n->setBalance(0);
                    }
                    else if(n->getBalance() == 0)
                    {
                        g->setBalance(0);
                        p->setBalance(0);
                        n->setBalance(0);
                    }
                    else if (n->getBalance() == -1)
                    {
                        g->setBalance(0);
                        p->setBalance(1);
                        n->setBalance(0);
                    }
                }
                
            } 
        
        }
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
    if(n == NULL)
    {
        return;
    }
    AVLNode<Key,Value>* p = n->getParent();
    int ndiff;
    if(p != NULL)
    {
        if(p->getLeft() == n)
        {
            ndiff = 1;
        }
        else if (p->getRight() == n)
        {
            ndiff = -1;
        }
        
    }

    if(diff == -1)
    {
        if(n->getBalance() + diff == -2) //Case 1
        {
            AVLNode<Key, Value>* c = n->getLeft();
            if(c->getBalance() == -1) //Case 1a
            {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            else if(c->getBalance() == 0) //Case 1b
            {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
            else if (c->getBalance() == 1)//Case 1c
            {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if(g->getBalance() == 1)
                {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance() == -1)
                {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == -1) //Case 2
        {
            n->setBalance(-1);
            return;
        }
        else if(n->getBalance() + diff == 0) //Case 3
        {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
    else if(diff == 1)
    {
        if(n->getBalance() + diff == 2) //Case 1
        {
            AVLNode<Key, Value>* c = n->getRight();
            if(c->getBalance() == 1) //Case 1a
            {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            else if(c->getBalance() == 0) //Case 1b
            {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            }
            else if (c->getBalance() == -1)
            {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if(g->getBalance() == -1)
                {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 0)
                {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if(g->getBalance() == 1)
                {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == 1) //Case 2
        {
            n->setBalance(1);
            return;
        }
        else if(n->getBalance() + diff == 0) //Case 3
        {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& key) const
{
	AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
	return node;
}

#endif
