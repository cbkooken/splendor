/*
 *  deck.cpp
 *  CS11 Splendor
 *  Edited by: Cam Kooken (@cbkooken)
 */

#include "deck.h"

//constructor
Vector::Vector() {
  capacity = 0;
  vSize = 0;
  buffer = nullptr;
}

//destructor
Vector::~Vector() {
  if (buffer != nullptr) {
    delete [] buffer;
  }
}

/* Return the number of elements in the vector (which is not necessariy equal to
 * the vector's capacity). */
int Vector::size() {
  return vSize;
}

/* Returns true if the vector contains no elements, false otherwise. */
bool Vector::empty() {
  return vSize == 0;
}

/* Add the given element to the back of the vector. If there isn't enough space
 * for this additional element, make the vector twice as large (this makes the
 * push_back() function have O(1) amortized complexity). 
 */
void Vector::push_back(Data value) {
  if (vSize == capacity) {
    //Need to reserve more space for this new element.
    reserve_more();
  }
  //Store our element at the end of the vector and update the size to respect
  //our vector rules.
  buffer[vSize] = value;
  vSize++;
}

/* Doubles the vector's capacity (or increase it to 1 if the vector was just
 * constructed) by allocating a new array twice the size of the
 * current one, copying over any elements, and pointing to the new array.
 */
void Vector::reserve_more() {
  if (capacity == 0) {
    capacity = 1;
  }
  else {
    capacity *= 2;
  }
  Data *new_buffer = new Data[capacity]; 
  for (int i = 0; i < vSize; i++) {
    new_buffer[i] = buffer[i];
  }

  if (buffer != nullptr) {
    delete [] buffer;
  }

  buffer = new_buffer;
}

/* "Remove" the last element of the vector by decrementing the size.
 * The element is still in memory, but the vector class has no knowledge
 * of it due to how its methods are implemented. 
 *
 * ERRORS: Calling this function on an empty vector causes undefined behavior.
 */ 
void Vector::pop_back() {
  vSize--; 
}

/* Return the first element of the vector. 
 *
 * ERRORS: Calling this function on an empty vector causes undefined behavior.
 * */
Data Vector::front() {
  return buffer[0];
}

/* Return the last element of the vector. 
 *
 * ERRORS: Calling this function on an empty vector causes undefined behavior.
 * */
Data Vector::back() {
  return buffer[vSize - 1];
}

/* Returns the value at the given index in our vector. 
 *
 * ERRORS: Calling this function with an index outside the vector's range causes
 * the program to terminate with an error.
 */
Data Vector::at(int index) {
  if (index < 0 || index >= vSize) {
    cerr << "ERROR: a vector of size " << vSize;
    cerr << " was accessed at index " << index << endl;
    exit(EXIT_FAILURE);
  }
  return buffer[index];
}

/* Insert a new value before the element at the given index, shifting all
 * elements at that index and above to the right by 1. 
 *
 * ERRORS: Calling this function with an index outside the bounds (0, size)
 * causes the program to terminate with an error. We allow an insertion at index
 * size because that's equivalent to a call to push_back().
 */
void Vector::insert(int index, Data value) {
  if (index < 0 || index > vSize) {
    cerr << "ERROR: trying to insert an element out of bounds" << endl;
    exit(EXIT_FAILURE);
  }
  
  //Make more space in the vector if needed to hold this new element
  if (vSize == capacity) {
    reserve_more();
  }
  Data new_element = value, old_element;
  //Insert element and shift over all following elements
  for (int i = index; i < vSize + 1; i++) {
    old_element = buffer[i];
    buffer[i] = new_element;
    new_element = old_element;
  }
  //Update our size
  vSize++;
}

/* Erase the value at the given index, shifting all
 * elements above that index to the left by 1. 
 *
 * ERRORS: Calling this function with an index outside the bounds (0, size)
 * causes the program to terminate with an error. 
 */
void Vector::erase(int index) {
  if (index < 0 || index >= vSize) {
    cerr << "ERROR: trying to erase an element out of bounds" << endl;
    exit(EXIT_FAILURE);
  }
  for (int i = index; i < vSize - 1; i++) {
    buffer[i] = buffer[i + 1];
  }
  vSize--;
}
