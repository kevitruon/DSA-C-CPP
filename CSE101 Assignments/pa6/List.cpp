/************************************************************
 *   Name:   Kevin Truong (#1834510)
 *   Email:  ktruon13@ucsc.edu
 *   Class:  CSE101-02 (Tantalo) Winter 2023
 *   File:   List.cpp
 ************************************************************/

#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

// Private Constructor --------------------------------------------------------

// Node constructor
List::Node::Node(ListElement x)
{
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Class Constructors & Destructors -------------------------------------------

// Creates a new List in the empty state.
List::List()
{
    frontDummy = new Node((ListElement)NULL);
    backDummy = new Node((ListElement)NULL);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

// Copy Constructor.
List::List(const List &L)
{
    frontDummy = new Node((ListElement)NULL);
    backDummy = new Node((ListElement)NULL);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;

    Node *N = L.frontDummy->next;
    while (N != L.backDummy)
    {
        insertBefore(N->data);
        N = N->next;
    }
}

// Destructor
List::~List()
{
    clear();
    delete frontDummy;
    delete backDummy;
}

// Access functions
int List::length() const
{
    return (num_elements);
}

ListElement List::front() const
{
    if (num_elements == 0)
    {
        std::cout << "List: front(): empty list";
        throw std::length_error("List: front(): empty list");
    }
    return (frontDummy->next->data);
}

ListElement List::back() const
{
    if (num_elements == 0)
    {
        std::cout << "List: back(): empty list";
        throw std::length_error("List: back(): empty list");
    }
    return (backDummy->prev->data);
}

int List::position() const
{
    return (pos_cursor);
}

ListElement List::peekNext() const
{
    if (num_elements == 0)
    {
        std::cout << "List: peekNext(): empty list";
        throw std::length_error("List: peekNext(): empty list");
    }
    if (pos_cursor == num_elements)
    {
        std::cout << "List: peekNext(): cursor position at back of List";
        throw std::length_error("List: peekNext(): cursor position at back of List");
    }
    return (afterCursor->data);
}

ListElement List::peekPrev() const
{
    if (num_elements == 0)
    {
        std::cout << "List: peekPrev(): empty list";
        throw std::length_error("List: peekPrev(): empty list");
    }
    if (pos_cursor == 0)
    {
        std::cout << "List: peekPrev(): cursor position at front of List";
        throw std::length_error("List: peekPrev(): cursor position at front of List");
    }
    return (beforeCursor->data);
}

// Manipulation procedures

void List::clear()
{
    moveFront();
    while (num_elements > 0)
    {
        eraseAfter();
    }
    pos_cursor = 0;
}

void List::moveFront()
{
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
}

void List::moveBack()
{
    pos_cursor = num_elements;
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
}

ListElement List::moveNext()
{
    if (num_elements == 0)
    {
        std::cout << "List: moveNext(): empty list";
        throw std::length_error("List: moveNext(): empty list");
    }
    if (pos_cursor == num_elements)
    {
        std::cout << "List: moveNext(): cursor position at back of List";
        throw std::length_error("List: moveNext(): cursor position at back of List");
    }
    pos_cursor++;
    beforeCursor = afterCursor;
    afterCursor = afterCursor->next;
    return beforeCursor->data;
}

ListElement List::movePrev()
{
    if (num_elements == 0)
    {
        std::cout << "List: movePrev(): empty list";
        throw std::length_error("List: movePrev(): empty list");
    }
    if (pos_cursor <= 0)
    {
        std::cout << "List: movePrev(): cursor position at front of List";
        throw std::length_error("List: movePrev(): cursor position at front of List");
    }
    pos_cursor--;
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;
    return afterCursor->data;
}

void List::insertAfter(ListElement x)
{
    Node *N = new Node(x);

    if (num_elements == 0) // empty
    {
        N->prev = frontDummy;
        N->next = backDummy;
        frontDummy->next = N;
        backDummy->prev = N;
    }
    else if (pos_cursor == 0 && num_elements > 0) // front
    {
        N->prev = frontDummy;
        N->next = afterCursor;
        frontDummy->next = N;
        afterCursor->prev = N;
    }
    else if (pos_cursor == num_elements) // back
    {
        N->prev = beforeCursor;
        N->next = backDummy;
        beforeCursor->next = N;
        backDummy->prev = N;
    }
    else // middle
    {
        N->next = afterCursor;
        N->prev = beforeCursor;
        afterCursor->prev = N;
        beforeCursor->next = N;
    }

    // N->next = afterCursor;
    // N->prev = beforeCursor;
    // afterCursor->prev = N;
    // beforeCursor->next = N;

    afterCursor = N;
    num_elements++;
}

void List::insertBefore(ListElement x)
{
    Node *N = new Node((ListElement)x);

    if (num_elements == 0)
    {
        N->prev = frontDummy;
        N->next = backDummy;
        frontDummy->next = N;
        backDummy->prev = N;
    }
    else if (pos_cursor == 0 && num_elements > 0)
    {
        N->prev = frontDummy;
        N->next = afterCursor;
        frontDummy->next = N;
        afterCursor->prev = N;
    }
    else if (pos_cursor == num_elements)
    {
        N->prev = beforeCursor;
        N->next = backDummy;
        beforeCursor->next = N;
        backDummy->prev = N;
    }
    else
    {
        N->next = afterCursor;
        N->prev = beforeCursor;
        afterCursor->prev = N;
        beforeCursor->next = N;
    }

    // N->next = afterCursor;
    // N->prev = beforeCursor;
    // afterCursor->prev = N;
    // beforeCursor->next = N;

    beforeCursor = N;
    num_elements++;
    pos_cursor++;
}

void List::setAfter(ListElement x)
{
    if (num_elements == 0)
    {
        std::cout << "List: setAfter(): empty list";
        throw std::length_error("List: setAfter(): empty list");
    }
    if (pos_cursor == num_elements)
    {
        std::cout << "List: setAfter(): cursor position at back of List";
        throw std::length_error("List: setAfter(): cursor position at back of List");
    }
    afterCursor->data = x;
}

void List::setBefore(ListElement x)
{
    if (num_elements == 0)
    {
        std::cout << "List: setBefore(): empty list";
        throw std::length_error("List: setBefore(): empty list");
    }
    if (pos_cursor == 0)
    {
        std::cout << "List: setBefore(): cursor position at front of List";
        throw std::length_error("List: setBefore(): cursor position at front of List");
    }
    beforeCursor->data = x;
}

void List::eraseAfter()
{
    if (num_elements == 0)
    {
        std::cout << "List: eraseAfter(): empty list";
        throw std::length_error("List: eraseAfter(): empty list");
    }
    if (pos_cursor == num_elements)
    {
        std::cout << "List: eraseAfter(): cursor position at back of List";
        throw std::length_error("List: eraseAfter(): cursor position at back of List");
    }
    Node *N = afterCursor;
    afterCursor = afterCursor->next;
    if (pos_cursor == 0)
    {
        afterCursor->prev = frontDummy;
        frontDummy->next = afterCursor;
    }
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    num_elements--;
    delete N;
}

void List::eraseBefore()
{
    if (num_elements == 0)
    {
        std::cout << "List: eraseBefore(): empty list";
        throw std::length_error("List: eraseBefore(): empty list");
    }
    if (pos_cursor == 0)
    {
        std::cout << "List: eraseBefore(): cursor position at front of List";
        throw std::length_error("List: eraseBefore(): cursor position at front of List");
    }
    Node *N = beforeCursor;
    beforeCursor = beforeCursor->prev;
    if (pos_cursor == num_elements)
    {
        backDummy->prev = beforeCursor;
        beforeCursor->next = backDummy;
    }
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    pos_cursor--;
    num_elements--;
    delete N;
}

// Other Functions
int List::findNext(ListElement x)
{
    Node *curr = afterCursor;
    while (curr != backDummy)
    {
        if (curr->data == x)
        {
            afterCursor = curr->next;
            beforeCursor = curr;
            pos_cursor++;
            return pos_cursor;
        }
        curr = curr->next;
        pos_cursor++;
    }
    pos_cursor = num_elements;
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
    return -1;
}

int List::findPrev(ListElement x)
{
    Node *curr = beforeCursor;
    int i = pos_cursor - 1;
    while (curr != frontDummy)
    {
        if (curr->data == x)
        {
            beforeCursor = curr->prev;
            afterCursor = curr;
            pos_cursor = i;
            return i;
        }
        curr = curr->prev;
        i--;
    }
    if (pos_cursor == 0)
    {
        beforeCursor = frontDummy;
        afterCursor = frontDummy->next;
        return -1;
    }
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
    return -1;
}

void List::cleanup()
{
    if (num_elements <= 1)
    {
        return;
    }
    Node *prev = frontDummy;
    Node *curr = frontDummy->next;
    Node *next = curr->next;
    Node *retainer = afterCursor;

    while (curr != backDummy)
    {
        bool delete_curr = false;
        next = curr->next;
        Node *check = frontDummy;
        while (check != curr)
        {
            if (check->data == curr->data)
            {
                delete_curr = true;
                break;
            }
            check = check->next;
        }

        if (delete_curr)
        {
            if (retainer == curr)
            {
                retainer = retainer->next;
            }
            if (curr == afterCursor)
            {
                afterCursor = prev;
            }
            prev->next = next;
            next->prev = prev;
            num_elements--;
            delete curr;
        }
        else
        {
            prev = curr;
        }
        curr = next;
    }
    for (moveFront(); afterCursor != backDummy && peekNext() != retainer->data; moveNext())
        ;
}

List List::concat(const List &L) const
{
    List nL;
    for (Node *N = frontDummy->next; N != backDummy; N = N->next)
    {
        nL.insertBefore(N->data);
    }
    for (Node *N = L.frontDummy->next; N != L.backDummy; N = N->next)
    {
        nL.insertBefore(N->data);
    }
    nL.moveFront();
    return nL;
}

std::string List::to_string() const
{
    std::string s = "(";
    Node *N = frontDummy->next;
    while (N->next != backDummy)
    {
        s += std::to_string(N->data) + ", ";
        N = N->next;
    }
    s += std::to_string(N->data) + ")";
    return s;
}

bool List::equals(const List &R) const
{
    if (length() != R.length())
        return false;

    Node *curr1 = frontDummy->next;
    Node *curr2 = R.frontDummy->next;

    while (curr1 != backDummy && curr2 != R.backDummy)
    {
        if (curr1->data != curr2->data)
            return false;

        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return true;
}

// Overriden Operators

std::ostream &operator<<(std::ostream &stream, const List &L)
{
    return stream << L.List::to_string();
}

bool operator==(const List &A, const List &B)
{
    return A.equals(B);
}

List &List::operator=(const List &L)
{
    if (this != &L)
    { // not self assignment
        // make a copy of L
        List temp = List(L);

        // then swap the copy's fields with fields of this
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }

    // return this with the new data installed
    return *this;

    // the copy, if there is one, is deleted upon return
}
