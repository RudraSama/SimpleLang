
# Simple Lang Compiler

SimpleLang is a minimalistic high-level language designed to run on an 8-bit CPU. It includes basic constructs such as variable declarations, assignments, arithmetic operations, and conditional statements, but it does not include loops. This language aims to be easy to understand and implement for educational purposes. 

## How to use?
Build the compiler 

    git clone url
    cd SimpleLang
    make
Running compiler

    ./simplelang input_file output_file
   example - 
   

    ./simplelang addition.simpl addition.asm

## Language Syntax
#### Variable Declaration

    int a; 			//declaring vairable without initialization.
    int b = 0;  	//intiailization vairable

#### Variable intialization
   
    int a;
    a = 5; 	//initilization vairable after declaring it.

#### Arithmetic Operations

    //Addition
    int a = 9;
    int b = 10;
    a = a + b; 	//Adding content of a and b and saving it in a.
    //Subtraction
    int c = 10;
    int d = 5;
    c = c - d;	//Subtracting d from c and storing it in c.

It also supports complex arithmetic expressions

    int a = 3;
    int b = (5 + 6) - a;
    int c = a + ((9 + a) - 1 ) + b;

#### If statements
Equals

    int a = 0;
    int b = 10;
    if(a == b){   //if a is equals to b, store 10 in a.
	    a = 10;
    }
    
Not Equals

    int a = 0;
    int b = 10;
    if(a != b){ //if a is not equals to b, store 0 in b
	    b = 0;
    }
   
   Check test folder for more examples.

## Language Constraints

 - Multiplication and Divisions are not allowed (right now).
 - Size of variable is 1 Byte (8bit) because of 8 bit CPU arch.
 - Negative numbers are not allowed (right now). Because of CPU arch. limitations.
 - <, <=, >, >=  are not allowed because there are no CPU instructions. 

## Documentation
Build docks
First install [Doxygen](https://www.doxygen.nl/download.html)

    make doxygen
This will build documentation in `docs` folder. Just open `index.html` file in browser.

 ## TODO
 

 - Adding (software) support for Negative numbers.
 - Adding support for more comparison operators

## Reference 
[8 Bit CPU Simulator](https://github.com/lightcode/8bit-computer)
