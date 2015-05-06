//
// Assignment 5
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

enum TOKEN { ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, NUMBER };
using namespace std;

//Constants
const int defaultSize = 1; //default size is 1
const int minSize = 0; //min size of array is 0
const int maxSize = 100; //max size of array is 100
const int kDefaultValue = 0; //default value is set to 0

//Reference: http://www.cplusplus.com/forum/windows/88843/
//String Sample Context
static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

//Length of the string sample context
int stringLength = sizeof(alphanum) - 1;

// Random Character Generator Function
// returns a random character
char genRandomChar(){
    return alphanum[rand() % stringLength]; //character generator and returns a random charater
}

//Class Template of Element for each item
template<class Element>
class SmcArray{
    
public:
    SmcArray(); // Default Constructor
    SmcArray(int s); //  Constructor with params
    ~SmcArray(); // Deconstructor
    
    int getSize() const; // Get size of array
    void pushItem(Element value); //inserts item on end of the array
    void setItem(Element value, int index); //sets an item  (overwrites) at given index
    Element getItem(int index); //retrieves an item at a given index - This item can be of Type Element, which can be several different types
    
    Element getItem(int index) const; //retrieves an item at a given index w/const
    void insertItem(Element value, int index); //inserts an item at a given index
    void removeItem(int index); // Delete array item
    void printArray(bool linear) const; // Print Array
    void setDefault(Element value); //Sets a default value for out of bounds index expansions via changeSize
    Element getDefault(); //Returns the default value
    void changeSize(unsigned int newSize); //Changes the array size
    
protected:
    int size; // Size of array (elements)
    Element * items; //Array of items of type element
    Element defaultValue;// Default value
    void allocateArray();//Allocates an array of given size
    void copyArrayIncreasedSize(int s); //Copies an array with increased size
    bool checkIndexBounds(int index) const;//Checks the array is within bounds
};

//Default Constructor
template <class Element>
SmcArray<Element>::SmcArray(){
    //Set the array using the defaults
    this->size = defaultSize;
    //Allocate an array of this->size
    this->defaultValue = kDefaultValue;
    this->allocateArray();
    this->setItem(0,0);
}

//Constructor with params
template <class Element>
SmcArray<Element>::SmcArray(int s){
    this->size = s;
    //Allocate an array of this->size
    this->defaultValue = kDefaultValue;
    this->allocateArray();
}

//Deletes the items in array
template <class Element>
SmcArray<Element>::~SmcArray(){
}

//Allocates an array of given size
template <class Element>
void SmcArray<Element>::allocateArray(){
    //Allocate an array of this->size
    this->items = new Element[this->size];
}

//Gets size of the array
template <class Element>
int SmcArray<Element>::getSize() const{
    return this->size; //returns the size fo the array
}

//Add an item to the end of the array
template <class Element>
void SmcArray<Element>::pushItem(Element value){
    //Increase the size of array
    this->copyArrayIncreasedSize(this->size + 1);
    //Add our new value into the last slot in the array
    this->items[this->size - 1] = value;
}

//sets an item in the array (overwrite)
template <class Element>
void SmcArray<Element>::setItem(Element value, int index){
    //Check to see if the index can be written to/if the index is a valid index
    if(this->checkIndexBounds(index)){
        //Write to the array
        this->items[index] = value; //writes the value into the item array
    }else if(index > this->size-1){
        //Two Routes can be taken
        //1st -  Use helper function pushItem() to add to end of array
        //this->pushItem(value);  //For the record, this method was a better option.
        //2nd - Adjust the size, then set the item
        this->changeSize(this->size+1); //Increase the size by 1
        this->setItem(value,this->size-1); //Recursive Call, use "this->size-1" in the event that index is n+2 greater than the size of the original size of the array
    }else{
        //Item is not within the bounds of the array
        cerr << "Inserted item is not within the bounds of the array\n";//error statement when item is out of bounds
    }
}

//Gets an item in the array and checks to see if the index is within bounds
template <class Element>
Element SmcArray<Element>::getItem(int index){
    //Check to see if the index can be retrieved
    if(this->checkIndexBounds(index)){
        return this->items[index]; //writes the value into the item array
    }else{
        //Even though this will return the default value the user is prompted with a message indicating that a default value has been returned
        return this->defaultValue; //returns the default value
    }
}

//Gets an item in the array and checks to see if the index is within bounds
template <class Element>
Element SmcArray<Element>::getItem(int index) const{
    //Check to see if the index can be retrieved
    if(this->checkIndexBounds(index)){
        //Write to the array
        return this->items[index]; //writes the value into the item array
    }else{
        //Even though this will return the default value the user is prompted with a message indicating that a default value has been returned
        return this->defaultValue; //returns the default value
    }
}

//insert an item to the at the given index of the array
template <class Element>
void SmcArray<Element>::insertItem(Element value, int index){
    //extend the array by 1 - update the size by adding 1 and inserts the value into the array
    //Checks that within bounds
    if (minSize <= index &&  index <= this->getSize() ){
        Element * newArray = new Element[this->getSize()+1];
        //Copy array that is increased by 1 at given index from the original array into the new array
        if (index < 0 || index >= this->getSize()+1)
            cout << "The index of the item to be removed is out of range." << endl;
        else{
            //sets the values in array before the inserted value
            for (int i = 0; i < index; i++) newArray[i] = items[i];
            //inserts the value after the given index
            newArray[index] = value;
            //sets the values in the array after the inserted value
            for (int i = index+1; i < this->size+1; i++) newArray[i] = items[i-1];
        }
        //Deallocate old memory
        delete [] this->items;
        //Copy to new pointer
        this->items = newArray;
        this->size++;
    }else{
        cerr << "Error: Program is crashing\n"; //Error statement
        throw "Array out of bounds exception";
    }
}

//Remove an item at a given index
template <class Element>
void SmcArray<Element>::removeItem(int index){
    //Remove 1 item from the array Decrease the size of the array
    if (index < 0 || index >=size) cout << "The index of the item to be removed is out of range." << endl; //Error statement
    else{
        for (int i = index; i < this->size-1; i++) //starts the loop where the item is removed and goes to one less than original array size
            items[i] = items[i+1]; //sets the values after the item is deleted
        this->size = size-1; //reduces the size of the array by 1
    }
}

//Prints the array
template <class Element>
void SmcArray<Element>::printArray(bool linear) const{
    if(linear){
        for (int i=0; i < this->size; i++){
            cout << this->items[i] << " ";
        }
    }else{
        //Prints what is in my array - loop through an print each item
        for (int i=0; i < this->size; i++){
            cout << "Index is: " << i << " Array value is: " << this->items[i] << endl;
        }
    }
    cout << endl;
}

//Copies an array in an increased size array
template <class Element>
void SmcArray<Element>::copyArrayIncreasedSize(int s){
    //check to see if new size is within the minSize And MaxSize - check if the new size is greater than the existing size
    if ((s > this->size) && s < maxSize){
        Element * newArray = new Element[s];
        //Copy everything from the original array into the new array
        for (int i = 0; i < this->size; i++){
            newArray[i] = this->items[i];
        }
        //Deallocate old memory
        delete [] this->items;
        //Copy to new pointer
        this->items = newArray;
        this->size = s;
    }else{
        cerr << "Error: program is crashing\n"; //Error statement
        throw "Array out of bounds exception";
    }
}

//Checks if the index is within the bounds of the array
template <class Element>
bool SmcArray<Element>::checkIndexBounds(int index) const{
    if(index >= minSize && index < this->size){ //checks if the index is greater than or equal to the minimum size and less than size of the array
        return true; //if yes then return true
    }else{
        return false; //if no then return false
    }
}

//Sets a default value for out of bounds index expansions via changeSize
template <class Element>
void SmcArray<Element>::setDefault(Element value){
    this->defaultValue = value;
}

//Returns the default value
template <class Element>
Element SmcArray<Element>::getDefault(){
    return this->defaultValue;
}

//Changes the size of the array
template <class Element>
void SmcArray<Element>::changeSize(unsigned int newSize){
    //Check to make sure there is a default value
    if (/* DISABLES CODE */ (false)) {
        cerr << __PRETTY_FUNCTION__ << " a default value has not been set\n";//_PRETTY_FUNCTION helps with debugging by identifying the name of where error is located
    }
    //First check to make sure the new size is within the bounds of the system
    if(newSize > minSize && newSize < maxSize){
        Element * newArray = new Element[newSize];
        //Copy array that is decreased by 1 at given index from the original array into the new array
        for (int i = 0; i < newSize; i++){
            if(i >= this->size){
                newArray[i] = defaultValue;
            }else{
                newArray[i] = this->items[i];
            }
        }
        //Deallocate old memory
        delete[] this->items;
        //Copy to new pointer
        this->items = newArray;
        this->size = newSize;
    }else{
        cerr << __PRETTY_FUNCTION__ << " Size changed to a illegal size\n"; //_PRETTY_FUNCTION helps with debugging by identifying the name of where error is located
    }
}

class BigInt{
protected:
    SmcArray<int> digits;
    bool negative; //Indicates a negative number
    bool negativeHandled;
    bool negativeHandledDivideOverride;
public:
    BigInt();                        // initialise to zero.
    ~BigInt(); //Deconstructor
    void assign(const BigInt & a);
    void assign(int a);              // various ways to initialise
    void assign(long a);
    void assign(string a);
    void print() const;
    void add(const BigInt & a);
    void subtract(const BigInt & a);
    void multiply(const BigInt & a);
    void divide(const BigInt & a);
    int compare(const BigInt & a) const;
    int compareAbsoluteValue(const BigInt & a) const;
    int getSize() const;
    int convertCharToInt(char c);
    bool handleNegativesForAddition(const BigInt & a);
    bool handleNegativesForSubtraction(const BigInt & a);
    bool checkDigitsZero(BigInt& bigInt);
    void setDigits(SmcArray<int> a);
    void removeLeadingZeros();
    void setNegative(bool negative){
        this->negative = negative;
    }
    void setPositive(bool positive){
        this->negative = !positive;
    }
    bool getNegative() const{
        return this->negative;
    }
    bool getPositive() const{
        return !this->getNegative();
    }
    void setNegativeHandledDivideOverride(bool negOverride){
        this->negativeHandledDivideOverride = negOverride;
    }
};

//Constructor
BigInt::BigInt(){
    //Initialize an array object
    this->digits.setItem(0, 0); //intialize with 0
    this->negative = false;
    this->negativeHandled = false;
    this->negativeHandledDivideOverride = false;
}
//Constructor
BigInt::~BigInt(){
}

//Assigning a big int
void BigInt::assign(const BigInt & a){
    //Change the existing size to the new size
    this->digits.changeSize(a.getSize());
    this->negative = a.getNegative();
    //Iterate through the array and copy everything over
    for (int i = 0; i < a.getSize(); i++){
        this->digits.setItem(a.digits.getItem(i), i);
    }
}

//Take an integer and split into digits
void BigInt::assign(int a){
    //Modulus  -- Loop until until no remainder
    if(a < 0){
        this->negative = true;
    }
    a = abs(a);
    int b = a;
    int length = 1;
    while (b /= 10)
        length++;
    int digit;
    this->digits.changeSize(length);
    for(int i = 0; i < this->digits.getSize(); i++){
        digit = a % 10;
        a = a / 10;
        this->digits.setItem(digit,length-1-i);
    }
}

//Take an integer and split into digits
void BigInt::assign(long a){
    //Modulus  -- Loop until until no remainder
    if(a < 0){
        this->negative = true;
    }
    a = abs(a);
    long b = a;
    int length = 1;
    while (b /= 10)
        length++;
    int digit;
    this->digits.changeSize(length);
    for(int i = 0; i < this->digits.getSize(); i++){
        digit = (int) (a % 10);
        a = a / 10;
        this->digits.setItem(digit,length-1-i);
    }
}

//Take a string and split into digits
void BigInt::assign(string a){
    //1) Loop through each character in string
    //2) covert that character to an integer
    //3) add that integer into the "this->digits" array by using setItem
    if(a[0] == '-'){
        this->negative = true;
        a.erase(0,1);
    }
    this->digits.changeSize((int) a.size());
    for(int i = 0; i < a.size(); i++){
        char myChar = a.at(i);
        int myTempInt = this->convertCharToInt(myChar);
        this->digits.setItem(myTempInt, i);
    }
}

//Print bigInt
void BigInt::print() const{
    if(this->negative){
        cout << "-" ;
    }
    for (int i  = 0; i < this->digits.getSize() ; i++){
        cout << this->digits.getItem(i);
    }
    cout << endl;
}

bool BigInt::handleNegativesForAddition(const BigInt & a){
    this->negativeHandled = true;
    if(this->getPositive() && a.getPositive()){
        return false;
    }else if(this->getPositive() && a.getNegative()){
        this->subtract(a);
        return true;
    }else if(this->getNegative() && a.getNegative()){
        return false;
    }else if(this->getNegative() && a.getPositive()){
        this->subtract(a);
        return true;
    }else{
        //satisfy compiler
        cout << "warning: should not have reached this" << endl;
        return false;
    }
}

//Adding two large integers that have been split into an array of large digits
void BigInt::add(const BigInt & a){
    if(!this->negativeHandledDivideOverride){
        if(!this->negativeHandled){
            if(this->handleNegativesForAddition(a)){
                return;
            }
        }
    }
    int carry = 0;
    int sumNum = 0;
    //Check if equal to 0
    if(this->digits.getSize() == 1 || a.digits.getSize() == 1){
        if(this->digits.getItem(0) == 0){
            this->assign(a);
            return;
        }else if(a.digits.getItem(0) == 0){
            return;
        }
    }
    int indexDifference = abs(this->digits.getSize() - a.digits.getSize()); //Indexdifference is the difference between the two nubers 5 and 2 --- difference is 3
    int indexSpread = this->digits.getSize() - a.digits.getSize(); //
    //Determines the size of loop if a.digits is less than digits then index is one less than a.digits
    if(this->digits.getSize() < a.digits.getSize()){
        //Insert leading 0's to change the size of this->digits
        for (int k = 0; k < indexDifference; k++){
            this->digits.insertItem(0, 0);
        }
        indexDifference = 0;
    }
    int startingIndex = this->digits.getSize()-1;
    for(int i = startingIndex; i >= indexDifference; i--){
        if(this->digits.getSize() >= a.digits.getSize()){
            sumNum = this->digits.getItem(i) + a.digits.getItem(i-indexDifference) + carry;
        }else{
            sumNum = this->digits.getItem(i) + a.digits.getItem(i) + carry;
        }
        
        if(sumNum >= 10){
            carry = (sumNum/10) % 10;
            sumNum= sumNum-10;
        }else{
            carry = 0;
        }
        this->digits.setItem(sumNum, i);
    }
    //if A was the larger, copy the rest of the items down into the this->digits
    if(indexSpread < 0){
        for (int k = indexDifference -1; k >= 0; k--){
            this->digits.setItem(a.digits.getItem(k), k);
        }
    }
    //If there is a carry
    if(carry == 1){
        //Check if the carry needs to be inserted at the beginning of the array at in 0
        if(indexDifference - 1 < 0){
            this->digits.insertItem(carry, 0);
        }else{
            if(indexDifference - 1 >= 1){
                while (indexDifference - 1 >= 0 && carry ==1){
                    int firstDigit = this->digits.getItem(indexDifference-1);
                    if((firstDigit + carry) > 9){
                        //Move it up one. If the carry in addition to the existing digit is larger
                        //than 9, we need to carry over to the next number
                        this->digits.setItem(firstDigit + carry - 10, indexDifference-1);
                        if(indexDifference - 1 == 0 && carry ==1){
                            this->digits.insertItem(carry, 0);
                            break;
                        }
                    }else{
                        //Add the carry to the first digit in the array
                        this->digits.setItem(this->digits.getItem(indexDifference-1)+1, indexDifference-1);
                        break;
                    }
                    indexDifference --;
                }
            }else{
                int firstDigit = this->digits.getItem(indexDifference-1);
                if((firstDigit + carry) > 9){
                    //Move it up one - If the carry in addition to the existing digit is larger than 9, we need to carry over to the next number
                    this->digits.setItem(firstDigit + carry - 10, indexDifference-1);
                    this->digits.insertItem(carry, 0);
                }else{
                    //Add the carry to the first digit in the array
                    this->digits.setItem(this->digits.getItem(indexDifference-1)+1, indexDifference-1);
                }
            }
        }
    }
    this->negativeHandled = false;
}

bool BigInt::handleNegativesForSubtraction(const BigInt & a){
    this->negativeHandled = true;
    if(this->getPositive() && a.getPositive()){
        return false;
    }else if(this->getPositive() && a.getNegative()){
        this->add(a);
        return true;
    }else if(this->getNegative() && a.getNegative()){
        this->negativeHandled = false;
        return false;
    }else if(this->getNegative() && a.getPositive()){
        this->add(a);
        return true;
    }else{
        //satisfy compiler
        cout << "warning: should not have reached this" << endl;
        return false;
    }
}

//Subtracting two integers, results must be positive.
void BigInt::subtract(const BigInt & a){
    if(!this->negativeHandledDivideOverride){
        if(!this->negativeHandled){
            if(this->handleNegativesForSubtraction(a)){
                return;
            }
        }
    }
    int maxindex = 0;  //maximum index
    int index = 0;
    int diffNum = 0;
    index = this->digits.getSize() - 1;
    maxindex = this->digits.getSize() - a.digits.getSize();
    //Compares two numbers to determine if the the result will be a negative number. If negative result, program ends.
    if(this->compareAbsoluteValue(a) < 0){
        BigInt tempInt;
        tempInt.assign(a);
        if(this->getPositive() && tempInt.getPositive()){
            tempInt.setNegative(true);
        }
        index = tempInt.digits.getSize() - 1;
        maxindex = tempInt.digits.getSize() - this->digits.getSize();
        for(int i = index; i >= 0; i--){
            if(this->digits.getItem(i-maxindex) > tempInt.digits.getItem(i)){
                tempInt.digits.setItem(tempInt.digits.getItem(i) + 10, i);
                tempInt.digits.setItem(tempInt.digits.getItem(i-1)-1, i-1);
            }
            diffNum = tempInt.digits.getItem(i) -  this->digits.getItem(i-maxindex);
            tempInt.digits.setItem(diffNum, i);
        }
        this->assign(tempInt);
        this->removeLeadingZeros();
        if(!this->negativeHandled){
            if(this->getNegative() && a.getNegative()){
                this->setNegative(false);
            }
        }
    }else{
        index = this->digits.getSize() - 1;
        maxindex = this->digits.getSize() - a.digits.getSize();
        for(int i = index; i >= 0; i--){
            if(this->digits.getItem(i) < a.digits.getItem(i-maxindex)){
                this->digits.setItem(this->digits.getItem(i) + 10, i);
                this->digits.setItem(this->digits.getItem(i-1)-1, i-1);
            }
            diffNum = this->digits.getItem(i) -  a.digits.getItem(i-maxindex);
            this->digits.setItem(diffNum, i);
        }
        this->removeLeadingZeros();
    }
    this->negativeHandled = false;
}

//Multiplying two numbers
void BigInt::multiply(const BigInt & a){
    this->negativeHandled = true;
    if(this->getNegative() && a.getNegative()){
        this->setPositive(true);
    }else if(this->getNegative() && a.getPositive()){
        this->setNegative(true);
    }else if(this->getPositive() && a.getNegative()){
        this->setNegative(true);
    }
    int multNum = 0;
    int carry = 0;
    int zeroIndex = 0;
    vector <BigInt> middleStepNumbers;
    for( int j = a.digits.getSize() - 1 ; j >= 0; j--){
        //Middle Line
        SmcArray<int> middleLine(0);
        for(int z = 0; z < zeroIndex; z++){
            middleLine.pushItem(0);
        }
        carry = 0;
        for(int i = this->digits.getSize() - 1; i >= 0; i--){
            multNum = this->digits.getItem(i) * a.digits.getItem(j) + carry;
            if(multNum >= 10){
                carry = multNum / 10;
                multNum = multNum % 10;
            }else{
                carry = 0;
            }
            middleLine.insertItem(multNum, 0);
        }
        if(carry!= 0){ //If there is a carry
            middleLine.insertItem(carry, 0);
        }
        zeroIndex++;
        BigInt myMiddleLine;
        myMiddleLine.setDigits(middleLine);
        middleStepNumbers.push_back(myMiddleLine);
    }
    BigInt result;
    result.assign(0);
    for(int i = 0; i< middleStepNumbers.size(); i++ ){
        BigInt myInt = middleStepNumbers[i];
        result.add(myInt);
    }
    result.setNegative(this->getNegative());
    this->assign(result);
    this->removeLeadingZeros();
    BigInt zero;
    zero.assign(0);
    if(this->compareAbsoluteValue(zero) == 0){
        this->setPositive(true);
    }
    this->negativeHandled = false;
}

//Dividing two numbers
void BigInt::divide(const BigInt & a){
    //Assumption 2: this->digits will always be bigger than 1. A is divisable by B
    this->negativeHandled = true;
    this->negativeHandledDivideOverride = true;
    if(this->getNegative() && a.getNegative()){
        this->setPositive(true);
    }else if(this->getNegative() && a.getPositive()){
        this->setNegative(true);
    }else if(this->getPositive() && a.getNegative()){
        this->setNegative(true);
    }
    
    BigInt one;
    one.assign(1);
    if(a.compareAbsoluteValue(one) == 0){
        return;
    }
    
    BigInt frontPart;
    frontPart.setNegativeHandledDivideOverride(true);
    SmcArray<int> frontDigits;
    for(int i = 0; i < a.getSize(); i++){ //Looping to get digits
        frontDigits.setItem(this->digits.getItem(i), i);
    }
    
    if(a.compareAbsoluteValue(frontPart) == -1){ //Checking to see if numerator is less than the denominator
        return;
    }
    SmcArray<int> result;
    result.changeSize(this->digits.getSize());
    int numTimesSubtracted = 0; //Initializing variable that counts the number of times subtracting
    int numeratorIndex = a.getSize(); //Getting size of numberator
    frontPart.digits = frontDigits; //Copying front digits into part digits
    
    //When the number of digits in numerator and denominator are the same and making sure the numbers are different
    while (numeratorIndex == this->digits.getSize() && (a.compareAbsoluteValue(frontPart) != 1)){
        frontPart.subtract(a);
        numTimesSubtracted = numTimesSubtracted + 1;
        result.setItem(numTimesSubtracted,numeratorIndex);
    }
    if(numTimesSubtracted > 0 ){
        this->assign(numTimesSubtracted);
        return;
    }
    //If a it is the length of 1
    if(a.getSize() == 1){
        SmcArray<int> result2;
        int remainder = 0;
        int divisor = a.digits.getItem(0);
        for(int i = 0; i < this->getSize(); i++){
            //If the demoninator is less than numerator
            int numerator = this->digits.getItem(i);
            if(remainder != 0) numerator = (remainder * 10) + numerator;
            int timesSubtracted = 0;
            while (numerator >= divisor){
                numerator -= divisor;
                timesSubtracted++;
            }
            result2.pushItem(timesSubtracted);
            remainder = numerator;
        }
        this->digits = result2;
        this->removeLeadingZeros();
        this->negativeHandled = false;
        this->negativeHandledDivideOverride = false;
        return;
    }
    
    //If a it is the length of 2
    if(a.getSize() == 2 && a.digits.getItem(1) == 0){
        SmcArray<int> result3;
        int remainder = 0;
        int divisor2 = a.digits.getItem(0);
        for(int i = 0; i < this->getSize()-1; i++){
            int numerator = this->digits.getItem(i);
            if(remainder != 0) numerator = (remainder * 10) + numerator;
            int timesSubtracted = 0;
            while (numerator >= divisor2){
                numerator -= divisor2;
                timesSubtracted++;
            }
            result3.pushItem(timesSubtracted);
            remainder = numerator;
        }
        this->digits = result3;
        this->removeLeadingZeros();
        this->negativeHandled = false;
        this->negativeHandledDivideOverride = false;
        return;
    }
    
    //When the number of digits in numerator is greater than the number of digits in the denominator
    while(numeratorIndex < this->digits.getSize()){
        //If denominator is larger than the front part of numerator then add a digit to numerator number
        numTimesSubtracted = 0; //Setting counter equal to zero
        while(a.compareAbsoluteValue(frontPart) == 1 ){
            if(frontPart.digits.getSize() > this->digits.getSize()){ //Checking to make sure length of numbers is okay
                cout << "Problem with length " << endl; //Error message
            }
            
            frontPart.digits.setItem(this->digits.getItem(numeratorIndex),frontPart.digits.getSize()); //Setting the digit in the answer array
            numeratorIndex ++; //Increment the numeratorIndex if we add another digits
        }
        
        //Check if the rest of the digits are zero
        if(this->checkDigitsZero(frontPart)){
            while (a.compareAbsoluteValue(frontPart) != 1 && numeratorIndex <= this->digits.getSize()){ //Comparing the numerator and denominator front part are not equal and the numerator index is less than or equal to the digit size
                frontPart.subtract(a); //Subtract the front part from the numerator
                numTimesSubtracted ++; //Incrementing the counter that is counting the number of times the denominator is subtracted from the denominator
            }
        }
        //Storing the result
        if(numeratorIndex < 0) numeratorIndex = numeratorIndex + 1; // Incrementing the numerator index if index becomes negative
        result.setItem(numTimesSubtracted, numeratorIndex); //Setting the digit in the answer array
    }
    this->digits = result; //Putting the result into the digits array
    this->removeLeadingZeros(); //Callling function that removes the leading zeroes
    this->negativeHandled = false;
    this->negativeHandledDivideOverride = false;
}

bool BigInt::checkDigitsZero(BigInt& bigInt){
    for (int i = 0; i < bigInt.getSize(); i++){
        if (bigInt.digits.getItem(i) != 0){
            return true;
        }
    }
    return false;
}

//Comparing two numbers to check if they are >, <, =
// -1 if a is greater than this->digits
// 0 if they are equal
// 1 if this-> digits is greater than 'a'
int BigInt::compare(const BigInt & a) const{
    if(this->getNegative() && a.getPositive()){
        return -1;
    }else if(this->getPositive() && a.getNegative()){
        return 1;
    }
    //Compares the size of the arrays
    if(this->digits.getSize() > a.getSize()){
        return 1;
    }else if(this->digits.getSize() < a.getSize()){
        return -1;
    }else{
        //The numbers of digits must equal - Loop through each digit and determine which BigInt is larger
        for(int i = 0; i < this->digits.getSize(); i++){
            if(this->digits.getItem(i) > a.digits.getItem(i)){
                //If these are equal to each other continue
                return 1;
            }else if(this->digits.getItem(i) < a.digits.getItem(i) ) {
                return -1;
            }
        }
        return 0;  //Did not find any numbers that were < >, the numbers are equal
    }
}

//Comparing two numbers to check if they are >, <, =
// -1 if a is greater than this->digits
// 0 if they are equal
// 1 if this-> digits is greater than 'a'
int BigInt::compareAbsoluteValue(const BigInt & a) const{
    //Compares the size of the arrays
    if(this->digits.getSize() > a.getSize()){
        return 1;
    }else if(this->digits.getSize() < a.getSize()){
        return -1;
    }else{
        //The numbers of digits must equal
        //Loop through each digit and determine which BigInt is larger
        for(int i = 0; i < this->digits.getSize(); i++){
            if(this->digits.getItem(i) > a.digits.getItem(i)){
                //If these are equal to each other continue
                return 1;
            }else if(this->digits.getItem(i) < a.digits.getItem(i) ) {
                return -1;
            }
        }
        return 0;  //Did not find any numbers that were < >, the numbers are equal
    }
}

//Gets the size of an array
int BigInt::getSize() const{
    return this->digits.getSize();
}

void BigInt::setDigits(SmcArray<int> a){
    this->digits = a;
}

//Simple char to int converter via ASCII indexs
int BigInt::convertCharToInt(char c){
    int myTempInt = (int)c;
    if(myTempInt < 48 || myTempInt > 57) {
        std::cout << "Error: Not an integer!\n";
        return -1; //Error checking
    }
    return myTempInt - 48;
}

void BigInt::removeLeadingZeros(){
    for(int i = 0; i <= this->digits.getSize()-1;){ //Remove Leading zeros
        //if the number is not equal to 0 or there is only 1 element left in the array, stop removing zeros
        if(digits.getItem(i) != 0 || this->digits.getSize() <= 1){
            return;
        }else{
            if(digits.getItem(i) == 0)
                this->digits.removeItem(i);
        }
        i = i;
    }
}

void testAdditionCase(int numA, int numB){
    BigInt myBigInt;
    BigInt secondBigInt;
    myBigInt.assign(numA);
    secondBigInt.assign(numB);
    myBigInt.add(secondBigInt);
    BigInt result;
    result.assign(numA + numB);
    if(myBigInt.compare(result) == 0){
        cout << "Test Passed - Add: " << numA << "+" << numB << "=" << numA + numB;
        cout << " *****TEST PASSED******" << endl;
    }else{
        cout << "Test Failed - Add: " << numA << "+" << numB << "=" << numA + numB;
        cout << " **===TEST FAILED===***" << endl;
        cout << "The expected result when adding a + b : " << numA + numB << endl;
        cout << "The actual result when adding a + b   : ";
        myBigInt.print();
    }
}

void testSubtractionCase(int numA, int numB){
    BigInt myBigInt;
    BigInt secondBigInt;
    myBigInt.assign(numA);
    secondBigInt.assign(numB);
    myBigInt.subtract(secondBigInt);
    BigInt result;
    result.assign(numA - numB);
    if(myBigInt.compare(result) == 0){
        cout << "Test Passed - Subtract: " << numA << "-" << numB << "=" << numA - numB;
        cout << " *****TEST PASSED******" << endl;
    }else{
        cout << "Test Failed - Subtract: " << numA << "-" << numB << "=" << numA - numB;
        cout << " **===TEST FAILED===***" << endl;
        cout << "The expected result when subtracting a - b : " << numA - numB << endl;
        cout << "The actual result when subtracting a - b   : ";
        myBigInt.print();
    }
}

void testMultiplicationCase(int numA, int numB){
    BigInt myBigInt;
    BigInt secondBigInt;
    myBigInt.assign(numA);
    secondBigInt.assign(numB);
    myBigInt.multiply(secondBigInt);
    BigInt result;
    result.assign(numA * numB);
    if(myBigInt.compare(result) == 0){
        cout << "Test Passed - Multiply: " << numA << "*" << numB << "=" << numA * numB;
        cout << " *****TEST PASSED******" << endl;
    }else{
        cout << "Test Failed - Multiply: " << numA << "*" << numB << "=" << numA * numB;
        cout << " **===TEST FAILED===***" << endl;
        cout << "The expected result when Multiplying a * b : " << numA * numB << endl;
        cout << "The actual result when Multiplying a * b : ";
        myBigInt.print();
    }
}

void testDivideCase(int numA, int numB){
    BigInt myBigInt;
    BigInt secondBigInt;
    myBigInt.assign(numA);
    secondBigInt.assign(numB);
    myBigInt.divide(secondBigInt);
    BigInt result;
    result.assign(numA / numB);
    if(myBigInt.compare(result) == 0){
        cout << "Test Passed - Divide: " << numA << "/" << numB << "=" << numA / numB;
        cout << " *****TEST PASSED******" << endl;
    }else{
        cout << "Test Failed - Divide: " << numA << "/" << numB << "=" << numA / numB;
        cout << " **===TEST FAILED===***" << endl;
        cout << "The expected result when Dividing a / b : " << numA / numB << endl;
        cout << "The actual result when Dividing a / b   : ";
        myBigInt.print();
    }
}

vector<string> parseString(string inputString){
    vector<string> parsedInput;
    string token;
    stringstream stringStream(inputString);
    while(getline(stringStream,token,' ')){
        parsedInput.push_back(token);
    }
    return parsedInput;
}

//if its + * / .... assume it is a operator
TOKEN tokenDetector(string s){
    if(s == "+" || s == "*" || s == "x" || s == "X" || s == "/" || s == "\\"  || s == "-" ){
        if(s == "+" ){
            return ADDITION;
        }else if(s == "*" || s == "x" || s == "X" ){
            return MULTIPLICATION;
        }else if (s == "/" || s == "\\" ){
            return DIVISION;
        }else if (s == "-"){
            return SUBTRACTION;
        }else{
            cout << "Error" << endl;
            return NUMBER;
        }
    }else{
        return NUMBER;
    }
}

void RPNCalcuator(vector<string>parsedInput){
    BigInt* zero = new BigInt();
    zero->assign(0);
    vector<BigInt*> myStack;
    for(int i = 0; i < parsedInput.size(); i++){
        if(tokenDetector(parsedInput[i]) != NUMBER){
            //If it is an operator -- Do some math
            if(myStack.size() < 2){
                cout << "Error: Illegal Reverse Polish Notation Syntax" << endl;
                return;
            }
            //pop last two values
            BigInt* second = new BigInt();
            second->assign(*myStack.back());
            myStack.pop_back();
            BigInt* first = new BigInt();
            first->assign(*myStack.back());
            myStack.pop_back();
            BigInt* result = new BigInt();
            //Determine which operator & do the calcuation
            TOKEN oper = tokenDetector(parsedInput[i]);
            switch (oper){
                case ADDITION:
                    first->add(*second);
                    result->assign(*first);
                    break;
                case DIVISION:
                    if(second->compare(*zero) != 0){
                        first->divide(*second);
                        result->assign(*first);
                        //result = first / second;
                    }else{
                        cout << "Attempted to divide by 0" << endl;
                        delete zero;
                        return;
                    }
                    break;
                case SUBTRACTION:
                    first->subtract(*second);
                    result->assign(*first);
                    //result = first - second;
                    break;
                case MULTIPLICATION:
                    first->multiply(*second);
                    result->assign(*first);
                    break;
                default:
                    cout<< "Error: Operator not found" << endl;
            }
            delete first;
            delete second;
            //Push the result back onto the stack
            myStack.push_back(result);
        }else{
            //If it is a number -- just add it to the stack.
            if(i == parsedInput.size() - 1){
                cout << "Error: Illegal Reverse Polish Notation Syntax" << endl;
                return;
            }
            BigInt* number = new BigInt();
            number->assign(parsedInput[i].c_str());
            myStack.push_back(number);
        }
    }
    if(zero){
        delete zero;
    }
    if(myStack.size() > 1){
        cout << "Error: Illegal Reverse Polish Notation Syntax" << endl;
        return;
    }
    cout << "Output is:  ";
    myStack.back()->print();
}

void testRPNProvidedUseCases(){
    vector<string> useCases;
    useCases.push_back("5 1 2 + 4 * + 3 -");
    useCases.push_back("123 + 321"); //should fail
    useCases.push_back("123 321 +");
    useCases.push_back("123 2 + 50 25 25 + + + + + "); //should fail
    useCases.push_back("123 2 + 50 25 25 + + "); //should fail
    useCases.push_back("123 2 + 50 25 25 + + + + + "); //should fail
    useCases.push_back("123 2 + 50 25 25 + + *"); //should pass
    useCases.push_back("99999999999999999999999999999999999999999999999 2 * 1 -");
    useCases.push_back("99999999999999999999999999999999999999999999999 200 / 1 +");
    useCases.push_back("99999999999999999999999999999999999999999999999 20 / 1 +");
    useCases.push_back("-3 -4 *");
    useCases.push_back("-2002 15 / -2 +");
    useCases.push_back("-1000 15 / 9 *");
    
    for (int i = 0; i < useCases.size(); i++){
        cout << endl << "Input is:   " << useCases[i] << endl;
        vector<string> parsedInput = parseString(useCases[i]);
        RPNCalcuator(parsedInput);
    }
}

void testAddition(){
    cout << "Tests for Addition: " << endl;
    cout << "-------------------" << endl;
    testAdditionCase(0, 1234);
    testAdditionCase(1234, 0);
    testAdditionCase(79492, 794920);
    testAdditionCase(794920, 79492);
    testAdditionCase(7940, 792123122);
    testAdditionCase(792123122, 7940);
    testAdditionCase(1, 2222);
    testAdditionCase(2222,1);
    testAdditionCase(12, 2222);
    testAdditionCase(2222,12);
    testAdditionCase(2222, 2222);
    testAdditionCase(0, 0);
    testAdditionCase(0,25635);
    testAdditionCase(25635, 307620);
    testAdditionCase(333255,1025400);
    testAdditionCase(1358655, 20508000);
    testAdditionCase(500, 500);
    testAdditionCase(0, 0);
    testAdditionCase(0,132);
    testAdditionCase(132, 880);
    testAdditionCase(1012,15400);
    testAdditionCase(16412, 198000);
    testAdditionCase(4123, 6999);
    testAdditionCase(6999, 4123);
    testAdditionCase(8, 9099);
    testAdditionCase(6, 99);
    testAdditionCase(5, 99);
    testAdditionCase(1, 999);
    testAdditionCase(99, 55);
    testAdditionCase(55, 99);
    testAdditionCase(999, 1);
    testAdditionCase(99, 5);
    testAdditionCase(99, 6);
    testAdditionCase(99, 6);
    testAdditionCase(9099, 8);
    testAdditionCase(90900999, 9);
    testAdditionCase(90900999, 1);
    cout <<endl;
}

void testSubtraction(){
    cout << "Tests for Subtraction: " << endl;
    cout << "----------------------" << endl;
    testSubtractionCase(0, 1234);
    testSubtractionCase(1234, 0);
    testSubtractionCase(79492, 794920);
    testSubtractionCase(794920, 79492);
    testSubtractionCase(7940, 792123122);
    testSubtractionCase(792123122, 7940);
    testSubtractionCase(1, 2222);
    testSubtractionCase(2222,1);
    testSubtractionCase(2222,9);
    testSubtractionCase(12, 2222);
    testSubtractionCase(2222,12);
    testSubtractionCase(2222, 2222);
    testSubtractionCase(0, 0);
    testSubtractionCase(0,25635);
    testSubtractionCase(25635, 307620);
    testSubtractionCase(33325500,1025400);
    testSubtractionCase(1358655, 20508000);
    testSubtractionCase(500, 500);
    testSubtractionCase(0, 0);
    testSubtractionCase(0,132);
    testSubtractionCase(132, 880);
    testSubtractionCase(1012,15400);
    testSubtractionCase(16412, 198000);
    testSubtractionCase(4123, 6999);
    testSubtractionCase(1000, 1);
    testSubtractionCase(10000, 2);
    testSubtractionCase(1000, 55);
    testSubtractionCase(1000, 888);
    testSubtractionCase(100000, 777);
    testSubtractionCase(100000, 99999);
    cout << endl;
}

void testAdditionNegative(){
    cout << "Tests for Addition: " << endl;
    cout << "-------------------" << endl;
    testAdditionCase(10, 5);
    testAdditionCase(10, -5);
    testAdditionCase(-10, -5);
    testAdditionCase(-10, 5);
    testAdditionCase(5, 10);
    testAdditionCase(5, -10);
    testAdditionCase(6, -12);
    testAdditionCase(4, -10);
    testAdditionCase(-5, -10);
    testAdditionCase(-5, 10);
    testAdditionCase(10, 5);
    testAdditionCase(10, -5);
    testAdditionCase(-10, -5);
    testAdditionCase(-10, 5);
    testAdditionCase(10, 4);
    testAdditionCase(10, -4);
    testAdditionCase(-10, -4);
    testAdditionCase(-10, 4);
    testAdditionCase(4, 10);
    testAdditionCase(4, -10);
    testAdditionCase(-4, -10);
    testAdditionCase(-4, 10);
    testAdditionCase(-1000, 1);
    testAdditionCase(-10000, 2);
    testAdditionCase(-1000, 55);
    testAdditionCase(-1000, 888);
    testAdditionCase(-100000, 777);
    testAdditionCase(-100000, 99999);
    testAdditionCase(1000, -1);
    testAdditionCase(10000,- 2);
    testAdditionCase(1000, -55);
    testAdditionCase(1000, -888);
    testAdditionCase(100000,- 777);
    testAdditionCase(100000, -99999);
    testAdditionCase(1, -1000);
    testAdditionCase(2, -10000);
    testAdditionCase(55 ,-1000);
    testAdditionCase(888,-1000);
    testAdditionCase(777, -100000);
    testAdditionCase(99999,-100000);
    testAdditionCase(-1, 1000);
    testAdditionCase(-2, 10000);
    testAdditionCase(-55 ,1000);
    testAdditionCase(-888,1000);
    testAdditionCase(-777, 100000);
    testAdditionCase(-99999,100000);
    cout <<endl;
}

void testSubtractionNegative(){
    cout << "Tests for Subtraction: " << endl;
    cout << "----------------------" << endl;
    testSubtractionCase(10, 5);
    testSubtractionCase(10, -5);
    testSubtractionCase(-10, -5);
    testSubtractionCase(-10, 5);
    testSubtractionCase(10, 4);
    testSubtractionCase(10, -4);
    testSubtractionCase(-10, -4);
    testSubtractionCase(-10, 4);
    testSubtractionCase(4, 10);
    testSubtractionCase(4, -10);
    testSubtractionCase(-4, -10);
    testSubtractionCase(-4, 10);
    testSubtractionCase(-1000, 1);
    testSubtractionCase(-10000, 2);
    testSubtractionCase(-1000, 55);
    testSubtractionCase(-1000, 888);
    testSubtractionCase(-100000, 777);
    testSubtractionCase(-100000, 99999);
    testSubtractionCase(1000, -1);
    testSubtractionCase(10000,- 2);
    testSubtractionCase(-1000, -55);
    testSubtractionCase(1000, -888);
    testSubtractionCase(-100000,- 777);
    testSubtractionCase(100000, -99999);
    testSubtractionCase(1, -1000);
    testSubtractionCase(2, -10000);
    testSubtractionCase(55 ,-1000);
    testSubtractionCase(888,-1000);
    testSubtractionCase(777, -100000);
    testSubtractionCase(99999,-100000);
    testSubtractionCase(-1, 1000);
    testSubtractionCase(-2, 10000);
    testSubtractionCase(-55 ,1000);
    testSubtractionCase(-888,1000);
    testSubtractionCase(-777, 100000);
    testSubtractionCase(-99999,100000);
    cout << endl;
}

void testMultiply(){
    cout << "Tests for Multiplication: " << endl;
    cout << "-------------------------" << endl;
    testMultiplicationCase(7, 5678);
    testMultiplicationCase(5678, 7);
    testMultiplicationCase(39746, 22);
    testMultiplicationCase(22, 39746);
    testMultiplicationCase(0, 39746);
    testMultiplicationCase(39746,0);
    testMultiplicationCase(1, 39746);
    testMultiplicationCase(39746, 1);
    testMultiplicationCase(9909, 9);
    testMultiplicationCase(9, 9909);
    testMultiplicationCase(9999, 109);
    testMultiplicationCase(109, 9999);
    testMultiplicationCase(70007, 7007);
    testMultiplicationCase(7007, 70007);
    testMultiplicationCase(5000, 900);
    testMultiplicationCase(900, 5000);
    testMultiplicationCase(120009, 5906);
    testMultiplicationCase(5906, 120009);
    testMultiplicationCase(-7, 5678);
    testMultiplicationCase(-5678, 7);
    testMultiplicationCase(-39746, 22);
    testMultiplicationCase(-22, 39746);
    testMultiplicationCase(0, -39746);
    testMultiplicationCase(39746,0);
    testMultiplicationCase(1, -39746);
    testMultiplicationCase(39746,-1);
    testMultiplicationCase(9909, -9);
    testMultiplicationCase(9, -9909);
    testMultiplicationCase(-9999, -109);
    testMultiplicationCase(-109, -9999);
    testMultiplicationCase(70007, -7007);
    testMultiplicationCase(-7007, -70007);
    testMultiplicationCase(5000, 900);
    testMultiplicationCase(-900, -5000);
    testMultiplicationCase(120009, -5906);
    testMultiplicationCase(-5906, 120009);
    testMultiplicationCase(0, -55);
    cout << endl;
}

void testDivide(){
    cout << "Tests for Division: " << endl;
    cout << "-------------------" << endl;
    testDivideCase(1, 55);
    testDivideCase(5, 5556);
    testDivideCase(1315451, 55);
    testDivideCase(11115451, 222);
    testDivideCase(1545451, 12);
    testDivideCase(1545451, 12);
    testDivideCase(1545451, 2312);
    testDivideCase(1545451, 1234);
    testDivideCase(1545451, 54321);
    testDivideCase(792123122, 7940);
    testDivideCase(9999, 99);
    testDivideCase(9999, 9);
    testDivideCase(2222,12);
    testDivideCase(33325500,1025400);
    testDivideCase(6999, 4123);
    testDivideCase(89999, 11111);
    testDivideCase(2222, 2222);
    testDivideCase(500, 500);
    testDivideCase(800000, 800000);
    testDivideCase(2222,1);
    testDivideCase(1,1);
    testDivideCase(0,1);
    testDivideCase(1545451, 154545);
    testDivideCase(1545451, 15);
    testDivideCase(794920, 79492);
    testDivideCase(70007, 7007);
    testDivideCase(7007, 70007);
    testDivideCase(5000, 900);
    testDivideCase(900, 5000);
    testDivideCase(-1315451, -55);
    testDivideCase(-11115451, -222);
    testDivideCase(1545451, -12);
    testDivideCase(-1545451, 12);
    testDivideCase(1545451, -2312);
    testDivideCase(1545451, -1234);
    testDivideCase(-1545451, -54321);
    testDivideCase(792123122, 7940);
    testDivideCase(9999, 99);
    testDivideCase(-9999, -9);
    testDivideCase(2222,-12);
    testDivideCase(-33325500,1025400);
    testDivideCase(6999, -4123);
    testDivideCase(89999, -11111);
    testDivideCase(-2222, -2222);
    testDivideCase(500, -500);
    testDivideCase(800000, -800000);
    testDivideCase(2222,1);
    testDivideCase(1,1);
    testDivideCase(0,1);
    testDivideCase(1545451, 154545);
    testDivideCase(1545451, 15);
    testDivideCase(794920, 79492);
    testDivideCase(70007, 7007);
    testDivideCase(-7007, -70007);
    testDivideCase(5000, 900);
    testDivideCase(900, 5000);
    testDivideCase(1009, -1);
    testDivideCase(100000009, 1);
    testDivideCase(1000009, -1);
    testDivideCase(1009, -2);
    testDivideCase(100000009, 2);
    testDivideCase(1000009, -2);
    testDivideCase(1009, 5);
    testDivideCase(1000009, -5);
    cout << endl;
}

int main(int argc, const char * argv[]) {
    //The following are test cases for the various function. Uncomment if you want to see the test cases displayed.
    
    testAddition();
    testSubtraction();
    testMultiply();
    testDivide();
    testAdditionNegative();
    testSubtractionNegative();
    
    testRPNProvidedUseCases();
    
    string inputString;
    while (true) {
        cout << endl;
        cout << "*********************************************" << endl;
        cout << "Enter RPN expression OR Enter Q or q to quit: ";
        getline(cin, inputString);
        if (inputString == "q" || inputString == "Q" ) {
            break;
        }
        vector<string> parsedInput = parseString(inputString);
        RPNCalcuator(parsedInput);
    }
    cout << "end" << endl;
    return 0;
}
