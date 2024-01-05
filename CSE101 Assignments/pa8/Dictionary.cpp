/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   Dictionary.cpp
 ************************************************************/

#include <cstring>
#include <limits>
#include <sstream>
#include <stdexcept>
#include "Dictionary.h"

#define RED 0
#define BLACK 1

Dictionary::Node::Node(keyType k, valType v)
{
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

// Helper Functions
void Dictionary::inOrderString(std::string &s, Node *R) const
{
    if (R != nil)
    {
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}

void Dictionary::preOrderString(std::string &s, Node *R) const
{
    if (R != nil)
    {
        s += R->key + "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::preOrderCopy(Node *R, Node *N)
{
    if (R != N)
    {
        setValue(N->key, N->val); // WORK ON THIS
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::postOrderDelete(Node *R)
{
    if (R != nil)
    {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}

Dictionary::Node *Dictionary::search(Node *R, keyType k) const
{
    while (R != nil)
    {
        if (root == nil || k == R->key)
        {
            return R;
        }
        else if (k < R->key)
        {
            R = R->left;
        }
        else
        {
            R = R->right;
        }
    }
    return nil;
}

Dictionary::Node *Dictionary::findMin(Node *R)
{
    Node *min = R;
    if (min != nil)
    {
        while (min->left != nil)
        {
            min = min->left;
        }
    }
    return min;
}

Dictionary::Node *Dictionary::findMax(Node *R)
{
    Node *max = R;
    if (max != nil)
    {
        while (max->right != nil)
        {
            max = max->right;
        }
    }
    return max;
}

Dictionary::Node *Dictionary::findNext(Node *N)
{
    if (N->right != nil)
    {
        return findMin(N->right);
    }
    Node *y = N->parent;
    while (y != nil && N == y->right)
    {
        N = y;
        y = y->parent;
    }
    return y;
}

Dictionary::Node *Dictionary::findPrev(Node *N)
{
    if (N->left != nil)
    {
        return findMax(N->left);
    }
    Node *y = N->parent;
    while (y != nil && N == y->left)
    {
        N = y;
        y = y->parent;
    }
    return y;
}
// added a helper function to help with remove()
void Dictionary::transplant(Dictionary::Node *u, Dictionary::Node *v)
{
    // copied straight from the handout
    if (u->parent == nil)
    {
        root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != nil)
    {
        v->parent = u->parent;
    }
}

// RBT Helper Functions
void Dictionary::LeftRotate(Node *N)
{
    Node *y = N->right;
    // turn y's left subtree into x's right subtree
    N->right = y->left;
    if (y->left != nil) // not necessary if using sentinal nil node
    {
        y->left->parent = N;
    }
    // link y's parent to x
    y->parent = N->parent;
    if (N->parent == nil)
    {
        root = y;
    }
    else if (N == N->parent->left)
    {
        N->parent->left = y;
    }
    else
    {
        N->parent->right = y;
    }
    // put x on y's left
    y->left = N;
    N->parent = y;
}

void Dictionary::RightRotate(Node *N)
{
    Node *y = N->left;

    N->left = y->right;
    if (y->right == nil)
    {
        y->right->parent = N;
    }
    y->parent = N->parent;
    if (N->parent == nil)
    {
        root = y;
    }
    else if (N == N->parent->right)
    {
        N->parent->right = y;
    }
    else
    {
        N->parent->left = y;
    }

    y->right = N;
    N->parent = y;
}

void Dictionary::RB_Insert(Node *N)
{
    Node *y = nil;
    Node *x = root;
    while (x != nil)
    {
        y = x;
        if (N->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    N->parent = y;
    if (y == nil)
    {
        root = N;
    }
    else if (N->key < y->key)
    {
        y->left = N;
    }
    else
    {
        y->right = N;
    }
    N->left = nil;
    N->right = nil;
    N->color = RED;
    RB_InsertFixUp(N);
}
void Dictionary::RB_InsertFixUp(Node *N)
{
    Node *y = nil;
    while (N->parent->color == RED)
    {
        if (N->parent == N->parent->parent->left)
        {
            y = N->parent->parent->right;
            if (y->color == RED)
            {
                N->parent->color = BLACK;
                y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            }
            else
            {
                if (N == N->parent->right)
                {
                    N = N->parent;
                    LeftRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            }
        }
        else
        {
            y = N->parent->parent->left;
            if (y->color == RED)
            {
                N->parent->color = BLACK;
                y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            }
            else
            {
                if (N == N->parent->left)
                {
                    N = N->parent;
                    RightRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void Dictionary::RB_Transplant(Node *u, Node *v)
{
    if (u->parent == nil)
    {
        root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void Dictionary::RB_DeleteFixUp(Node *N)
{
    while (N != root && N->color == BLACK)
    {
        if (N == N->parent->left)
        {
            Node *w = N->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                N->parent->color = RED;
                LeftRotate(N->parent);
            }
            if (w->left->color == BLACK && w->right->color)
            {
                w->color = RED;
                N = N->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                }
                w->color = N->parent->color;
                N->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(N->parent);
                N = root;
            }
        }
        else
        {
            Node *w = N->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                N->parent->color = RED;
                RightRotate(N->parent);
                w = N->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                N = N->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = N->parent->left;
                }
                w->color = N->parent->color;
                N->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    N->color = BLACK;
}

void Dictionary::RB_Delete(Node *N)
{
    Node *y = N;
    int yocolor = y->color;
    Node *x;
    if (N->left == nil)
    {
        Node *x = N->right;
        RB_Transplant(N, N->right);
    }
    else if (N->right == nil)
    {
        Node *x = N->left;
        RB_Transplant(N, N->left);
    }
    else
    {
        y = findMin(N->right);
        yocolor = y->color;
        Node *x = y->right;
        if (y->parent == N)
        {
            x->parent = y;
        }
        else
        {
            RB_Transplant(y, y->right);
            y->right = N->right;
        }
        RB_Transplant(N, y);
        y->left = N->left;
        y->left->parent = y;
        y->color = N->color;
    }
    if (yocolor == BLACK)
    {
        RB_DeleteFixUp(x);
    }
    num_pairs--;
}

// Class Constructors & Destructors
Dictionary::Dictionary()
{
    nil = new Node("", 0);
    current = root = nil;
    num_pairs = 0;
    nil->color = BLACK;
}

Dictionary::Dictionary(const Dictionary &D)
{
    nil = new Node("sadfsjadkhslakdjfhsdfsadfsdafsadfsdfdsfnhdfghfgdhdfcvbcgjtsdfgstgsdfg", 999);
    current = root = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary()
{
    postOrderDelete(root);
    delete nil;
}

// Access Function

int Dictionary::size() const
{
    return num_pairs;
}

bool Dictionary::contains(keyType k) const
{
    Node *N = root;
    N = search(root, k);
    return N != nil;
}

valType &Dictionary::getValue(keyType k) const
{
    Node *N = root;
    N = search(root, k);
    if (N == nil)
    {
        throw std::logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
    }
    return N->val;
}

bool Dictionary::hasCurrent() const
{
    // std::cout <<"Current Key: "<< current->key << " Current Value: " << current->val << "\n";
    return current != nil;
}

keyType Dictionary::currentKey() const
{
    if (hasCurrent() == false)
    {
        throw std::logic_error("Dictionary: currentKey(): current undefined");
    }
    return current->key;
}

valType &Dictionary::currentVal() const
{
    if (hasCurrent() == false)
    {
        throw std::logic_error("Dictionary: currentVal(): current undefined");
    }
    return current->val;
}

// Manipulation procedures
void Dictionary::clear()
{
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::setValue(keyType k, valType v)
{
    Node *P = nil;
    Node *R = root;
    while (R != nil)
    {
        P = R;
        if (k < R->key)
        {
            R = R->left;
        }
        else if (k > R->key)
        {
            R = R->right;
        }
        else
        {
            R->val = v;
            return;
        }
    }
    Node *N = new Node(k, v);
    if (P == nil)
    {

        N->left = nil;
        N->right = nil;
        N->parent = nil;
        root = N;
    }
    else if (k < P->key)
    {

        P->left = N;
        N->parent = P;
        N->left = N->right = nil;
    }
    else
    {
        P->right = N;
        N->parent = P;
        N->left = N->right = nil;
    }
    N->color = RED;
    RB_InsertFixUp(N);
    num_pairs++;
}

void Dictionary::remove(keyType k)
{
    Node *N = search(root, k);
    if (N == nil)
    {
        throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }
    int og = N->color;
    Node *y;
    if (N->left == nil)
    {
        transplant(N, N->right);
        num_pairs--;
    }
    else if (N->right == nil)
    {
        transplant(N, N->left);
        num_pairs--;
    }
    else
    {
       y = findMin(N->right);
        og = y->color;
        if (y->parent != N)
        {
            transplant(y, y->right);
            y->right = N->right;
            y->right->parent = y;
            num_pairs--;
        }
        transplant(N, y);
        y->left = N->left;
        y->left->parent = y;
        num_pairs--;
    }
    if (current->key == k)
    {
        current = nil;
    }
    if(og == BLACK){
        RB_DeleteFixUp(y);
    }
}

void Dictionary::begin()
{
    current = findMin(root);
}

void Dictionary::end()
{
    current = findMax(root);
}

void Dictionary::next()
{
    if (hasCurrent() == false)
    {
        throw std::logic_error("Dictionary: next(): current not defined");
    }
    current = findNext(current);
}

void Dictionary::prev()
{
    if (hasCurrent() == false)
    {
        throw std::logic_error("Dictionary: prev(): current not defined");
    }
    current = findPrev(current);
}

// Other Functions

std::string Dictionary::to_string() const
{
    std::string s;
    inOrderString(s, root);
    return s;
}

std::string Dictionary::pre_string() const
{
    std::string s;
    preOrderString(s, root);
    return s;
}

bool Dictionary::equals(const Dictionary &D) const
{
    return (D.num_pairs == num_pairs && to_string() == D.to_string());
}

// Overloaded Operators
std::ostream &operator<<(std::ostream &stream, Dictionary &D)
{
    return stream << D.Dictionary::to_string();
}

bool operator==(const Dictionary &A, const Dictionary &B)
{
    return (A.equals(B));
}

Dictionary &Dictionary::operator=(const Dictionary &D)
{
    if (equals(D) == false)
    {
        Dictionary temp = Dictionary(D);
        std::swap(nil, temp.nil);
        std::swap(current, temp.current);
        std::swap(root, temp.root);
        std::swap(num_pairs, temp.num_pairs);
    }
    return *this;
}
