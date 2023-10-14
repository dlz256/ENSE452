/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
*/

#include "queue.h"
#include <iostream>
#include <cstdlib>              // for exit

using namespace std;

Queue::Queue()
{
    head = 0;
    tail = 0;
    nelements = 0;
    verbose = false;
}

Queue::~Queue()
{
    for (QElement* qe = head; qe != 0;)
    {
	QElement* temp = qe;
	qe = qe->next;
	delete(temp);
    }
}

void Queue::remove(Data* d)
{
    if (size() > 0)
    {
        QElement* qe = head;
        head = head->next;
        nelements--;
        *d = qe->data;
	delete qe;
    }
    else
    {
        cerr << "Error: Queue is empty.\n";
        exit(1);
    }
}

void Queue::insert(Data d)
{
    if (verbose) std::cout << "insert(d)\n";
    QElement* el = new QElement(d);
    if (size() > 0)
    {
        tail->next = el;
    }
    else
    {
        head = el;
    }
    tail = el;
    nelements++;
}

bool Queue::search(Data otherData) const
{
    QElement* insideEl = head;
    for (int i = 0; i < nelements; i++)
    {
        if (insideEl->data.equals(otherData))
            return true;
        insideEl = insideEl->next;
    }
    return false;
}

void Queue::print() const
{
    QElement* qe = head;
    if (size() > 0)
    {
        for (unsigned i = 0; i < size(); i++)
        {
            cout << i << ":(" << qe->data.x << "," << qe->data.y << ") ";
            qe = qe->next;
        }
    }
    cout << "\n";
}

unsigned Queue::size() const
{
    return nelements;
}


/*
    The position is a number from 0 to size(), and the data element d will be inserted in front of that queue position. 
    That is, if position is 0, it will insert at the front of the queue (the head), 
    if you set position to size()-1 it will insert just in front of the tail,
    if you set position to size() the element will be appended at the tail (just as a normal insert function does).
*/
void Queue:: insert(Data d, unsigned position)
{
    if (verbose) std::cout << "insert(d) at position:" << position << " \n";

    if (position > size()) {
        std::cerr << "insert: range error.\n";
        exit(3);
    }
    QElement* el = new QElement(d);

    if (size() == 0) //if queue is empty
    {
        head = el;
        tail = el;
    } 
    else if (position == 0)  //if position is 0, insert at front(head)
    {
        el->next = head;    //push back the head
        head = el;          //the head is now the new element
    } 
    else 
    {
        QElement* current = head;

        for (unsigned int i=0; i< position-1; i++)
        {
            current = current->next; //traverse the queue to the desired position, pushing back all elements
        }
        // Adjust pointers to insert at the desired position
        el->next = current->next;   //connect the element by pointing to the next element
        current->next = el;         //and pointing the previous to the element
        if (position == size())    //if position is the same as the size, then add to tail
        {
            tail = el;
        }
      
    }
    nelements++;
}

