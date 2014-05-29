/* Name: Dilan Samarasinghe
   ID: DS697678
*/
#include "constants.h"

// This structre holds the name of the relation and a pointer to the first attribute 
typedef struct relations{
  char name[MAX_STRING_LENGTH]; // space for the name of the relation
  struct attributes *vNext; // pointer to the first attribute
}RELATIONS;

//This structre holds the name of an attribute, its type( string or int), the length of that attribute type and two pointers, one for the first value of that attribute type and one for the next attribute
typedef struct attributes{
  char name[MAX_STRING_LENGTH]; // space for attribute name
  char type[MAX_STRING_LENGTH]; // space for the type of attribute
  int length; // length of the attribute ( its assumed if its an int this will be 4
  struct attributes *aNext; // pointer to next attribute
  struct values *vNext; // pointer to first value of this type
}ATTRIBUTES;

// This structre holds the value of an attribute type and a pointer to next value
typedef struct values{
  int iValue; // if the type is an int this value will be changed
  char *sValue; // if the type is a string this value will be changed
  struct values *vNext;//pointer to next value of this attribute type
}VALUES;
