- Changes made by: Dillan Zurowski - 200431334


- describe the addition you made (which should be clearly marked in the
source code)
    The addition that I made was adding an insert function that allowed for a Data type to be inserted at any position within the linked list.
- describe any unit tests you added
    1. testInsertHead
        - this function attempts to add an (0,0) with position 0, and compares it to a hardcoded vector with that position filled with (0,0). returns true if the queue matches the vector after insertion.
    2. testInsertTail
        - This function adds (0,0) to the position that matches the size of the queue. This will add to the tail of the queue.
- I also added to the main test to include the new insert function. 