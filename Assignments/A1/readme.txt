Assignment 1 - Filter
ENSE 452
Sept 12, 2023
Dillan Zurowski

This program allows two complex numbers to be inputted into the system then
perform a desired operation on those numbers in the form a + bj.

Build: make
Run: ./A1
Filter on Powershell: Get-Content mycommands.txt | .\A1 1>output.txt 2>junk.txt
Filter on other: ./A1 < mycommands.txt 1> output.txt 2> junk.txt

Using the built in filter commands, the program will read from mycommands.txt,
output only the results to output.txt, then any other text to junk.txt

Additional error code:5 failed to convert to double
This occurs when coverting from the input string to a double.
