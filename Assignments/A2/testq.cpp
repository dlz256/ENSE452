/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
   Description: test code
*/

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>

#include "queue.h"

/**
   Compare the given queue to the given array of data elements.  Return
   true if they are all equal.
 */
bool testQueueEquality( Queue & q, std::vector<Data> const & array)
{
    if (q.size() != array.size())
	return false;

    for (unsigned i = 0; i < q.size(); ++i)
    {
        Data d(0,0);
	q.remove(&d);
	if ( !d.equals(array[i]) )
	    return false;
	q.insert(d);
    }
    return true;
}

/*
    try adding an element to the head of the queue
    return true if the first element is not changed
*/
bool testInsertHead()
{       

     Queue q1;

    q1.insert(Data(1,1));
    q1.insert(Data(-2, -3));
    q1.insert(Data(3,3));
    q1.insert(Data(-4,-5));
    q1.insert(Data(5,5));

    std::vector<Data> dataVec;
    dataVec.push_back(Data(0,0));
    dataVec.push_back(Data(1,1));
    dataVec.push_back(Data(-2, -3));
    dataVec.push_back(Data(3,3));
    dataVec.push_back(Data(-4,-5));
    dataVec.push_back(Data(5,5));
    Data data(0,0);
    q1.insert(data, 0);
    q1.print();
    return testQueueEquality(q1, dataVec);;

}

/*
    try adding an element to the head of the queue
    return true if the last element is not changed
*/
bool testInsertTail()
{
    Queue q1;

    q1.insert(Data(1,1));
    q1.insert(Data(-2, -3));
    q1.insert(Data(3,3));
    q1.insert(Data(-4,-5));
    q1.insert(Data(5,5));

    std::vector<Data> dataVec;
    dataVec.push_back(Data(1,1));
    dataVec.push_back(Data(-2, -3));
    dataVec.push_back(Data(3,3));
    dataVec.push_back(Data(-4,-5));
    dataVec.push_back(Data(5,5));
    dataVec.push_back(Data(0,0));

    Data data(0,0);
    q1.insert(data, q1.size());
    q1.print();
    bool equal = testQueueEquality(q1, dataVec);
    return equal;
}


int main()
{
    using namespace std;

    cout << "Testing queue.\n";
    Queue q1;

    q1.insert(Data(1,1));
    q1.insert(Data(3,3));
    q1.insert(Data(5,5));
    q1.insert(Data(-2,-3), 1);  //added
    q1.insert(Data(-4,-5), 3);  //added

    q1.print();

    vector<Data> dataVec;
    dataVec.push_back(Data(1,1));
    dataVec.push_back(Data(3,3));
    dataVec.push_back(Data(5,5));

    dataVec.insert(dataVec.begin() + 1, Data(-2,-3));   //added
    dataVec.insert(dataVec.begin() + 3, Data(-4,-5));   //added

    assert(testQueueEquality(q1, dataVec));
    assert(testInsertHead());  //test position 0 (head)
    assert(testInsertTail());  //test position 5 (same as size so goes to tail)
    // q1.print();

    Data d44(4, 4);
    bool found = q1.search(d44);
    assert(found == false);


    q1.insert(d44);  // now is (1,1),(3,3),(5,5),(4,4)
    found = q1.search(d44);
    assert(found == true);

    // now queue is(1,1),(3,3),(5,5),(4,4) and 
    // dataVec has (1,1),(3,3),(5,5).  Not equal
    assert(testQueueEquality(q1, dataVec) == false);

    Data temp;
    q1.remove(&temp);  // now q1 is (3,3),(5,5),(4,4)

    Data temp2(1,1);
    assert(temp.equals(temp2));  // (1,1) == (1,1)

    Data temp3(6,6);
    found = q1.search(temp3);
    assert(found == false);
}
