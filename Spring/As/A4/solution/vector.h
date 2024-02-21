#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector *Vector;
typedef struct VectorIterator *VectorIterator;


////////////////
//Constructor://
////////////////

//Create default Vector 
Vector init_vector(void);

//Create Vector that can have only 100 nums in a array
Vector init_vector_limited(void);



////////////////
//Destructors://
////////////////

//Free memory that used to Vector v
void remove_vector(Vector v);

//Free memory thar used to VectorIterator iter
void remove_iter(VectorIterator iter);



///////////////////////
//Methods for Vector://
///////////////////////

//Add n to the end of vector v
void push_back(Vector v, int n);

//Print all nums in the vector v in a line(for debugging)
void print_vector(Vector v);



///////////////////////////////
//Methods for VectorIterator://
///////////////////////////////

//Create VectorIterator from begin of vector v
VectorIterator begin(Vector v);

//Create VectorIterator from end of vector v
VectorIterator end(Vector v);

//Get num that VectorIterator point
int get_num(VectorIterator iter);  

//Shift VectorIterator iter to next num
void next(VectorIterator iter);

//Check is VectorIterators i and j same?
int same(VectorIterator i, VectorIterator j);

#endif
