/** -*- c++ -*-
   Project: Implementation of a Queue in C++.
   Course: enel452
   Programmer: Karim Naqvi

   Description: Implements a queue based on a singly-linked list.
   Element data is assumed to be a value type, and must support the equals method.
*/

#ifndef QUEUE_H
#define QUEUE_H

/** Data stores an (x,y) int pair.  This might represent a screen
    coordinate, for example.
 */
struct Data {
    int x;
    int y;
    bool equals(Data other) const { return x==other.x && y==other.y; }
    Data() : x(0), y(0) {}
    Data(int xx, int yy) : x(xx), y(yy) {}
};

/** A holding class for the data and a pointer to the next element.
 */
class QElement {
    friend class Queue;
private:
    Data data;
    QElement* next;

    QElement(Data d) : data(d), next(0) {}
}; 

/**
   Stores a sequence of elements, accessible via the "normal" queue
   discipline (i.e. insertions at the tail, removals from the head.)

   This queue implementation stores elements of type Data.  It is
   required that clients of this class pass a Data element to the
   insert() method.  The data is copied into the queue, i.e. it's
   treated as a value type.
 */
class Queue {
    
private:
    QElement* head;
    QElement* tail;
    bool verbose;               // if true, emit debugging information
    int nelements;              // num elements in queue 
public:
    /** Construct an empty queue. */
    Queue();

    /**
       Destruct a queue.
       \post All elements will be deleted.
     */
    ~Queue();
    
    /** Insert a new element at the tail of the queue. */
    void insert(Data d);

    /** Insert a new element at the position stated in the queue. */
    void insert(Data d, unsigned position);


    /**
       Remove the head element of the queue.

       \pre Queue must be non-empty.

       \post The head element is removed from the queue.  The
       data is copied into the data buffer provided.
     */
    void remove(Data* data);

    /**
       Search the queue for an element equal to 'd'.

       \post Returns a true if found.
     */
    bool search(Data d) const;

    /** Print the elements of the queue, from head to tail. */
    void print() const;

    /** Return the number of elements currently in the queue. */
    unsigned size() const;

    /** Enable extra informative messages for debugging purposes. */
    void setVerbose(bool flag)  { verbose = flag; }
};
        
#endif // QUEUE_H
