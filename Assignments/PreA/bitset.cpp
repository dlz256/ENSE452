/*

Questionaire Q3
ENSE 452
Sept 9, 2023
Dillan Zurowski
Write a C function called bitSet
which takes two arguments: (1) the address of the memory-mapped register,
and (2) a bit number in the range [0, 31]. The function sets the designated
bit to 1, without modifying any other bits in the register.

*/

#include <iostream>
#include <cstdint> // Include the cstdint header
#include <bitset> // Include the bitset header

using namespace std;

void bitSet(uint32_t *address, int);

int main()
{
    bitSet( reinterpret_cast<uint32_t*>(0x1100DEEC), 14);
    return 0;
}

/*
    set a specific bit of an address to 1
*/
void bitSet( uint32_t *address, int location)
{
    uintptr_t addressValue = reinterpret_cast<uintptr_t>(address);  //cast the address to an integer pointer
    bitset<32> addressArray = bitset<32>(addressValue);             //can usre the bitset library to set and reset individual bits
    cout << "Binary address: " << addressArray << endl;
    cout << "Decimal address: " << addressValue << endl;
    cout << "Bit at location " << location << ": " << addressArray[location-1] << endl;

    addressArray.set(location-1);
    cout << "Bit at location " << location << ": " << addressArray[location-1] << endl;
    cout << "Binary address: " << addressArray << endl;
}