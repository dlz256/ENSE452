/*

Assignment 1 - Filter
ENSE 452
Sept 12, 2023
Dillan Zurowski

This program allows two complex numbers to be inputted into the system then
perform a desired operation on those numbers in the form a + bj.

Build: make
Run: ./A1
Filter on Powershell: Get-Content mycommands.txt | .\A1 1>output.txt 2>junk.txt

Using the built in filter commands, the program will read from mycommands.txt,
output only the results to output.txt, then any other text to junk.txt

*/
#include <string>   //allow strings
#include <iostream> //allow basic input/output

using namespace std;

struct Complex // complex class imitates a complex number
{
    double real;      // real part of the number
    double imaginary; // imaginary part of the number
};

void add(Complex z1, Complex z);      // add two complex numbers
void subtract(Complex z1, Complex z); // subtract two complex numbers
void multiply(Complex z1, Complex z); // multiply two complex numbers
void divide(Complex z1, Complex z);   // divide two complex numbers

int main()
{

    cerr << "Complex calculator \n";
    cerr << "Type a letter to specify the arithmetic operator (A, S, M, D)\n";
    cerr << "followed by two complex numbers expressed as pairs of doubles.\n";
    cerr << "Type Q to quit.";
    cerr << "\nEnter Exp: ";
    string input; // stores the user input
    // This is the loop in the reads input from cin
    while (getline(cin, input)) // read a line of input
    {
        if (input.empty() || input.find_first_not_of(' ') == string::npos)
        {
            cerr << "Enter Exp: ";
            continue; // if input is empty, ignore the entry and restart while loop
        }
        //storge for the 5 input values +1// if extra values are added
        //ex. [a][1][2][3][4]
        string array[6]; 

        //temporary storage in case the number is a double 
        //with a decimal and more then one char
        string piece = "";  
        int index = 0;  //determines which element of the 5 values it is.
        for (int i = 0; i < (int)input.length(); i++)      //loop through the input
        {
            if (isspace(input[i]))
                continue;              // skip spaces
            piece.push_back(input[i]); // add a character to an array, could be a double    
            if (isspace(input[i + 1]) || i == (int)input.length() - 1)
            {  //if the next value is a space or the end of the input
                array[index] = piece;   //save the current value to the next index of the array
                piece = "";             //reset the temp string
                index++;                //increas the index
            }
        }

        if (!isalpha(array[0][0]))  //if the first letter is not a letter
        {
            cout << "error code: 1: illegal command";
            cerr << "\nEnter Exp: ";
            continue;
        }
        if (array[0].compare("q") == 0 || array[0].compare("Q") == 0)  //if quit command
        {
            return 1;   //end program
        }
        if (array[4].empty())   //of the last element is missing
        {
            cout << "error code: 2: missing arguments";
            cerr << "\nEnter Exp: ";
            continue;
        }
        if (!array[5].empty())  //if there is too many elements
        {
            cout << "error code: 3: extra arguments";
            cerr << "\nEnter Exp: ";
            continue;
        }

        Complex firstNumber;    //stores the values for the first inputted number
        Complex secondNumber;   //stores the values for the second number
        try
        {
            firstNumber.real = stod(array[1]);      //real number, stod converts to double
            firstNumber.imaginary = stod(array[2]); //imaginary
            secondNumber.real = stod(array[3]);
            secondNumber.imaginary = stod(array[4]);
        }
        catch (const exception &e)  //exception saving numbers as double
        {
            cout << "error code 5: failed to convert to double";
            continue;
        }

        //cases for different operations
        if (array[0].compare("a") == 0 || array[0].compare("A") == 0)
        {
            add(firstNumber, secondNumber);
        }
        else if (array[0].compare("s") == 0 || array[0].compare("S") == 0)
        {
            subtract(firstNumber, secondNumber);
        }
        else if (array[0].compare("m") == 0 || array[0].compare("M") == 0)
        {
            multiply(firstNumber, secondNumber);
        }
        else if (array[0].compare("d") == 0 || array[0].compare("D") == 0)
        {
            divide(firstNumber, secondNumber);
        }
        else    //if not one of the 4 operations or q
        {
            cout << "error code: 1: illegal command";
            cerr << "\nEnter Exp: ";
        }
        cerr << "Enter Exp: ";
    }
    return 0;
}

/*
    Add the two complex numbers
    a jb + c jd
    result = (a + c) + j(b + d)

*/
void add(Complex first, Complex second)
{
    Complex result;
    result.real = first.real + second.real;
    result.imaginary = first.imaginary + second.imaginary;
    string sign;
    if (result.imaginary < 0)   //check the sign as it has a leading j
        sign = '-';
    else
        sign = '+';
    cout << result.real << " " << sign << " j " << abs(result.imaginary) << endl;
}
/*
    Subtract the two complex numbers
    a jb - c jd
    result = (a + c) + j(b + d)

*/
void subtract(Complex first, Complex second)
{
    Complex result;
    result.real = first.real - second.real;
    result.imaginary = first.imaginary - second.imaginary;
    string sign;
    if (result.imaginary < 0)
        sign = '-';
    else
        sign = '+';
    cout << result.real << " " << sign << " j " << abs(result.imaginary) << endl;
}
/*
    Multiply the two complex numbers
    a jb * c jd
    result = (a * c - b * d) + j(a * d + b * c)


*/
void multiply(Complex first, Complex second)
{
    Complex result;
    result.real = (first.real * second.real) - (first.imaginary * second.imaginary);
    result.imaginary = (first.real * second.imaginary) + (first.imaginary * second.real);
    string sign;
    if (result.imaginary < 0)
        sign = '-';
    else
        sign = '+';
    cout << result.real << " " << sign << " j " << abs(result.imaginary) << endl;
}
/*
    Multiply the two complex numbers
    a jb * c jd
    result = [(a * c + b * d) / (c^2 + d^2)] + j[(b * c - a * d) / (c^2 + d^2)]
*/
void divide(Complex first, Complex second)
{
    Complex result;
    double denominator =
        (second.real * second.real) + (second.imaginary * second.imaginary);
    if (denominator == 0)
    {
        cout << "error code: 4: divide by zero";
        return;
    }
    result.real =
        ((first.real * second.real) + (first.imaginary * second.imaginary)) / denominator;
    result.imaginary =
        ((first.imaginary * second.real) - (first.real * second.imaginary)) / denominator;
    string sign;
    if (result.imaginary < 0)
        sign = '-';
    else
        sign = '+';
    cout << result.real << " " << sign << " j " << abs(result.imaginary) << endl;
}
