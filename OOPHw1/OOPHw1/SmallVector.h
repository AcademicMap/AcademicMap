#pragma once

// Rutkay Bozkurt
// 150120114
// Written in Visual Studio 2015 Community
// Tested in ITU's Linux Server using ssh

#include <iostream>
#include <cstring>

using namespace std;

class SmallVector
{
public:
	SmallVector(); // Default Constructor
	SmallVector( const int * , int ); // Constructing with an array
	SmallVector(const SmallVector &);	// Copy constructor
	~SmallVector();		// Destructor

	void push_back(int);	// Push one integer
	void push_back( const int*, int);	// Push array of integers
	int pop_back();	// Pop one integer
	SmallVector reverse() const;	// Getting reverse of a SmallVector

	const int& operator[](int) const;	// Overloading Subscript operator for rvalue 
	int& operator[](int);	//Overloading Subscript operator for lvalue
	SmallVector operator+(const SmallVector) const;	//Overload addition operator
	SmallVector operator*(int) const ;	//Overload multiplication operator
	SmallVector& operator=(const SmallVector);	//Overload assignment operator

	const int getSize() const; //Getter for size

private:
	int StaticBuffer[32];	
	int * DynamicBuffer;
	int size, capacity;
};

//Default Constructor
SmallVector::SmallVector()	
{
	size = 0;	// Set size 0 for empty smallvector 
	capacity = 32;	// Capacity is set to 32 for static buffer
}

//rvalue subscript overload
const int& SmallVector::operator[](int index) const {	
	// If there is only static buffer
	if (size <= 32) {	
		// If index is negative
		if (index < 0)	
			return StaticBuffer[size + index];	// Return value indexing from end to start of smallvector
		//If positive
		else if (index < size)	
			return StaticBuffer[index];	//Return normally

		// Out of Boundaries
		else if (index >= size)	// Too high
			return StaticBuffer[size - 1];	//Return last element
		else if (index < -size) // Too low
			return StaticBuffer[0];	// Return first element


	}
	//If dynamic buffer is used
	else {	
		// Positive index
		if (index >= 32)	// Index is at Dynamic Buffer
			return DynamicBuffer[index - 32];	// Substract size of static buffer to get index of dynamic buffer
		else if (index >= 0)	// Index is at Static Buffer
			return StaticBuffer[index];	// Return value at index

		// Negative index
		else if (index >= (32 - size))	// Index is at dynamic buffer
			return DynamicBuffer[size - 32 + index];	// Add index(negative) value to end of dynamic buffer to get element
		else if (index >= (-size))	// Index at Static Buffer
			return StaticBuffer[size + index];	// Index(negative) added to size to find element

		// Out of Boundaries
		else if (index >= size)	// Too high
			return DynamicBuffer[size - 33];	//Return last element
		else	// Too low
			return StaticBuffer[0]; // Return first element
	}
}

//lvalue subscript overload
//same as rvalue but without const to make it modifiable
int& SmallVector::operator[](int index){ 

	if (size <= 32) {
		if (index < 0)
			return StaticBuffer[size + index];
		else if (index < size)
			return StaticBuffer[index];
		else if (index >= size)
			return StaticBuffer[size-1];
		else if (index < -size)
			return StaticBuffer[0];


	}
	else {
		if (index >= 32)
			return DynamicBuffer[index - 32];
		else if (index >= 0)
			return StaticBuffer[index];
		else if (index >= (32 - size))
			return DynamicBuffer[size - 32 + index];
		else if (index >= (-size))
			return StaticBuffer[size + index];
		else if (index >= size)
			return DynamicBuffer[size - 33];
		else
			return StaticBuffer[0];
	}
}

//Addition operator overload
SmallVector SmallVector::operator+(const SmallVector opvector) const  {
	SmallVector resultvector(*this);	//Result vector is created same as first operand
	for (int i = 0; i < opvector.size; i++) {	
		resultvector.push_back(opvector[i]);	// Second Operand is concatenated one by one at the end of result vector
	}
	return resultvector; //return result vector
}

//Multiplication operator overload
SmallVector SmallVector::operator*(int k) const {
	SmallVector resultvector; // Result vector is created empty
	for (int i = 0; i < k; i++) {
		resultvector = resultvector + *this; // Get result by adding k times of wanted smallvector
	}
	return resultvector;	//Return result
}

//Assigment operator overload
SmallVector& SmallVector::operator=(const SmallVector invector) {

	if (size > 32) delete[] DynamicBuffer; // If leftvalue vector has DynamicBuffer beforehand delete it

	size = invector.size; // Getting new size
	capacity = invector.capacity; // Getting new capacity

	//Smallvector we are assigning has DynamicBuffer
	if (invector.size > 32) {
		//Get static buffer one by one
		for (int i = 0; i < 32; i++) { 
			StaticBuffer[i] = invector.StaticBuffer[i];
		}
		//Create a new DynamicBuffer according to smallvector we are assining
		DynamicBuffer = new int[invector.capacity - 32];	//Allocate Buffer
		memcpy(DynamicBuffer, invector.DynamicBuffer, (size - 32) * sizeof(int));	//Copy values
	}
	// Smallvector we are assigning doesn't have DynamicBuffer
	else{
		//Just copy StaticBuffer one by one
		for (int i = 0; i < invector.size; i++) {
			StaticBuffer[i] = invector.StaticBuffer[i];
		}
	}
		
	return *this; // Return result
}

//Constructor with array input
SmallVector::SmallVector( const int * arr, int arrsize ) {

	capacity = 32;	//Set capacity to default

	//Fill in static buffer according array size
	for (int i = 0; i < arrsize; i++) {
		if (i == 32) break; //If size is greater than 32 break
		StaticBuffer[i] = arr[i];
	}
	
	//Size greater than StaticBuffer
	if( arrsize > 32 ) {

		// Multiply capacity by 2 until smallvector is bigger than input array
		while (capacity < arrsize) {
			capacity = capacity * 2;
		}
		// Allocate DynamicBuffer
		DynamicBuffer = new int[capacity - 32]; //Substract static buffer to get exact size

		//Fill dynamic buffer until input array is finished
		for (int i = 32; i < arrsize; i++) {
			DynamicBuffer[i - 32] = arr[i];
		}

	}

	size = arrsize; //Size is now input array's size
}

//Copy constructor
SmallVector::SmallVector(const SmallVector &invector) {
	// Copy size and capacity
	size = invector.size;
	capacity = invector.capacity;

	//Copy StaticBuffer
	for (int i = 0; i < size; i++) {
		if (i == 32) break;
		StaticBuffer[i] = invector.StaticBuffer[i];
	}
	//Copy when there is DynamicBuffer
	if (size > 32) {
		DynamicBuffer = new int[capacity - 32]; //Allocate space
		memcpy(DynamicBuffer, invector.DynamicBuffer, (size - 32) * sizeof(int)); // Copy values
	}
}

//Push a single integer to the end of vector
void SmallVector::push_back(int input) {
	// Small vector is full
	if (size == capacity) {		
		capacity = capacity * 2;	//Double the capacity
		
		// If dynamic buffer is used, resize it
		if (size > 32) {		

			int * tmp = new int[size - 32];		 // Create a temporary buffer to keep data
			memcpy(tmp, DynamicBuffer, (size - 32) * sizeof(int));		//Copy data
			delete[] DynamicBuffer;				// Delete old dynamic buffer

			DynamicBuffer = new int[capacity - 32]; // Allocate new and larger dynamic buffer
			memcpy(DynamicBuffer, tmp, (size - 32) * sizeof(int));	//Copy content back from temp buffer
			delete[] tmp;		// Delete temporary buffer
		}

		// If there is no dynamic buffer used before, create it
		else {	
			DynamicBuffer = new int[capacity - 32]; //Creating dynamic buffer
		}
		DynamicBuffer[size - 32] = input;	//Put input at end of smallvector
		
	}
	// Small vector is not full
	else { 
		//No DynamicBuffer
		if (size < 32) 
			StaticBuffer[size] = input;	// Put input at the end of static buffer
		//There is Dynamic Buffer
		else 
			DynamicBuffer[size - 32] = input; // Put input at end of dynamic buffer
	}
	size++;	//Increment size by one
}

// Push an array of integer to the end of vector
void SmallVector::push_back(const int * arr, int arrsize) {
	// Push array elements one by one until given size is reached
	for (int i = 0; i < arrsize; i++) {
		push_back(arr[i]);
	}
}

//Get last element from vector
int SmallVector::pop_back() {
	int result; // Result value
	//Dynamic Buffer is used
	if (size > 32)
		result = DynamicBuffer[size - 33]; // Get last element of dynamic buffer
	//No Dynamic buffer
	else
		result = StaticBuffer[size - 1];	// Get last element of static buffer
	size--; //Size is decremented by one
	return result;	//Return result
}

//Get reverse of a small vector without changing it
SmallVector SmallVector::reverse() const {
	SmallVector resultvector; // Empty result vector
	//Push values from end to beginning one by one into empty vector
	for (int i = 1; i <= size; i++) {
		resultvector.push_back((*this)[-i]);
	}
	
	return resultvector;	//Return result vector
}

//Getter function for size
const int SmallVector::getSize() const {
	return size;	//Returns size
}

//Destructor
SmallVector::~SmallVector()
{	//If there is a Dynamic Buffer delete it
	if (size > 32) { 
		delete[] DynamicBuffer;
	}
}