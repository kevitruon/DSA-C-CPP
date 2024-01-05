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

// Node Constructor
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

// Class Constructors & Destructors
Dictionary::Dictionary()
{
    nil = new Node("", 0);
    current = root = nil;
    num_pairs = 0;
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
    if (P == nil)
    {
        Node *N = new Node(k, v);
        N->left = nil;
        N->right = nil;
        N->parent = nil;
        root = N;
    }
    else if (k < P->key)
    {
        Node *N = new Node(k, v);
        P->left = N;
        N->parent = P;
        N->left = N->right = nil;
    }
    else
    {
        Node *N = new Node(k, v);
        P->right = N;
        N->parent = P;
        N->left = N->right = nil;
    }
    num_pairs++;
}

void Dictionary::remove(keyType k)
{
    Node *N = search(root, k);
    if (N == nil)
    {
        throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }
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
        Node *y = findMin(N->right);
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
