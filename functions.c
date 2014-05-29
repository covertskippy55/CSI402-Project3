/* Name: Dilan Samarasinghe
   ID: DS697678
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "struct_def.h"
#include "prototypes.h"
/* This function takes in the schema name, the data name and a pointer to the first attribute in the linked list
   and produces a pointer to the first attribute
*/
ATTRIBUTES* addRelation(char *schemaName,char *dataName,ATTRIBUTES *head){
  FILE *schemafp; // File pointer for schema file
  char *token;   // holds string to be tokenized
  char line[MAX_QUERY_LENGTH]; //holds the original non tokenized line
  char line2[MAX_QUERY_LENGTH];// holds the string to be modified using strtok
  int size; // holds the size of each attribute
  ATTRIBUTES *temp; // pointer to temp node
  ATTRIBUTES *start; // pointer to the first node, this is whats returned at the end
  
  // open schema file and check for errors
  if((schemafp = fopen(schemaName,"r")) == NULL){
    fprintf(stderr,"Error occured opening schema file: %s \n",schemaName);
    exit(1);
  }
  // read the first line to get the number of attributes
  fgets(line,MAX_QUERY_LENGTH,schemafp);

  // read through the the rest of schema file
  while(fgets(line,MAX_QUERY_LENGTH,schemafp) != NULL){
    //allocate space for the temp node and check for errors
    if((temp = malloc(sizeof(ATTRIBUTES))) == NULL){
      fprintf(stdout,"Error occured with malloc \n");
      exit(1);
    }
    // copy the original line to line2 in order to be tokenized
    strcpy(line2,line);
    // set token to be the line2 and start tokenize it
    token = line2;
    // this is our attribute name
    token = strtok(token," \t\n");
    //printf("%s\n",token);
    // copy that over to the name value of temp
    strcpy(temp->name,token);
    //  this is our type (I or S)
    token = strtok(NULL," ");
    // copy that over to type
    strcpy(temp->type,token);
    // printf("%s\n",token);
    // this is our attribute size
    token = strtok(NULL," ");
    //printf("%s\n",token);
    //convert the string to in int and set size of temp to be that
    size = atoi(token);
    temp->length = size;
    // set both pointer to next attribute and pointer to first tuple value to be null
    temp->aNext = NULL;
    temp->vNext = NULL;
    //printf("%d\n",size);

    // here we add the node to the list, if the list is empty set node to be the start and set the start to be this node
    if(head == NULL){
      head = temp;
      start = head;
    }
    // otherwise move the pointer to the next node and set the pointer to be temp;
    else{
      head->aNext = temp;
      head = temp;
    }
  }
  return start; // return the first node
}

/* This function takes in the first attribute pointer and the name of the data file, and will read in each value to their own node
   it will then link these with the corret attribute and with each other. It returns nothing
*/
void addValues(ATTRIBUTES *head,char *dataName){
  char *record; // holds the fread values
  int intvalue; // holds the freat value when its an int
  VALUES *temp; // temp node pointer to a value node to be propegated with data
  FILE *datafp; // data file pointer
  // ATTRIBUTES *pointer; an attribute pointer used for testing IGNORE THIS  
  VALUES *vPointer; // a pointer to a value node
  ATTRIBUTES *start; // pointer to the first node of the attribute linked list
  
  // malloc record and check for errors
  if((record = malloc(MAX_QUERY_LENGTH)) == NULL){
    fprintf(stderr,"Error occured with malloc\n");
    exit(1);
  }

  // open the data file and check for errors
  if((datafp = fopen(dataName,"r")) == NULL){
    fprintf(stderr,"Error occured opening data file: %s \n",dataName);
    exit(1);
  }

  // set start to be the first attribute node
  start = head;

  // we start reading the data file until EOF, using feof condition
  while(!feof(datafp)){

    // traverse through each attribute
    for(head = start;head!=NULL;head = head->aNext){

      // malloc space for the value node and check for errors								    
      if((temp = malloc(sizeof(VALUES))) == NULL){
	fprintf(stderr,"Error occured with malloc\n");
	exit(1);
      }
      // malloc space for the array that holds the string value
      temp->sValue = malloc(MAX_QUERY_LENGTH);
      
      //set the pointer to next value node to be null
      temp->vNext = NULL;
      // check if the current attribute is of type string
      if(strcmp(head->type,"S") == 0){
	// and if so read bytes of the length of that attribute into record, and make sure the return value is approriate
	if(fread((void*)record,head->length,1,datafp)==1){
	  // check if the record value isnt junk
	  if(strcmp(record,"") == 0){
	  }
	  else{
	    // otherwise copy that value to the string value of the node
	    strcpy(temp->sValue,record);
	  }
	}
      }
      // check if the type is an int, if it is read the int and set the int value of temp to be that value
      else if(strcmp(head->type,"I") == 0){
	if(fread((void*)&intvalue,head->length,1,datafp)==1){
	  temp->iValue = intvalue;
	  //printf("%d\n",temp->iValue);
	}
      }

      // here we add the node to the list, check if the first node of the list exist, if it doesnt make temp it
      if(head->vNext == NULL){
	head->vNext = temp;
	
      }
      // otherwise traverse the linked list of values until you reach the last node then set the next of that node to be the temp node we created
      else{
	vPointer = head->vNext;
	while(vPointer->vNext !=NULL){
	  vPointer = vPointer->vNext;
	}
	vPointer->vNext = temp;
      }
    }

  }
  /*  IGNORE THIS PART, IT WAS USED FOR TESTING
  for(pointer = start;pointer!=NULL;pointer = pointer->aNext){
    for(vPointer = pointer->vNext;vPointer!=NULL;vPointer = vPointer->vNext){
      if((vPointer->sValue[0]) != "/0"){
	printf("The string is: %s\n",vPointer->sValue);
      }
    }

  }
  */
  
}

/* This function takes in a pointer to the start of the attribute linked list and prints how many attributes there are
   It returns nothing.
*/
void nattr(ATTRIBUTES *head){
  int count = 0; // counting value
  ATTRIBUTES *pointer; // attribute pointer to the start of the list
  // traverse through the linked list till you reach the end and increment count  
  for(pointer = head;pointer!=NULL; pointer = pointer->aNext){
    count++;
  }
  printf("The number of attributes belonging to this relation is: %d\n",count);
  fflush(stdout);
}

/* This function takes in a pointer to the start of the attribute linked list and prints the tupl esize of the specified relation.
   It returns nothing
*/
void tuplelen(ATTRIBUTES *head){
  ATTRIBUTES *pointer; // pointer to traverse the list
  int tupleSize = 0;  // tuple size counter

  // traverse the list and add up the length values for each attribute
  for(pointer = head;pointer!=NULL;pointer = pointer->aNext){
    tupleSize+=pointer->length;
  }
  // then print that value
  printf("The tuple length for this relation is: %d\n",tupleSize);
  fflush(stdout);
}

/* This function takes in a name of an attribute and a pointer to the attribute list, and prints the type of attribute and the legnth of the attribute.
   It returns nothing.
*/
void infattr(char *name,ATTRIBUTES *head){
  ATTRIBUTES *pointer; // pointer to traverse the list

  // traverse the list and see if the name matches with the name provided
  for(pointer = head; pointer!=NULL; pointer = pointer->aNext){
    if(strcmp(pointer->name,name) == 0){
      // if it does print what type of attribute it is and how long it is
      printf("The attribute type is: %s , the attribute length is: %d \n",pointer->type,pointer->length);
      fflush(stdout);
      return;
    }



  }
  // If control reaches this point, the attribute name is not valid so print an error message
  printf("The attribute, %s, does not exist in the specified relation\n",name);
  fflush(stdout);

}

/* This function takes in a pointer to the first value node of the value linked list and returns how many tuples there are in that list. */
int count(VALUES *head){
  int count = 0; // counter

  // traverse the list till you reach the node before last, we have to stop here because 
  // we used feof to read the data file. Because feof isnt set until after we actually reach end of file, there will be an extra node at the end of each list.

  while(head->vNext !=NULL){
    count++; // 
    head = head->vNext;
  }
  // print the count and also return it
  printf("Number of tuples in this relation is: %d \n",count);
  fflush(stdout);
  return count;
}

/* This function takes in a pointer to attribute list and the name of an attribute. It will search the attribute list and if a match is found it will print the values in that attributes.
   Currently i have been unable to remove duplicates.
*/
void project(ATTRIBUTES *head,char *attribute){
  VALUES *vPointer; // pointer to value linked list
  //  int i = 0;
  // int size = count(head->vNext);
  //char print[size];
  //int iprint[size];
  // traverse through the attribute linked list until we reach end
  for(;head!=NULL;head = head->aNext){

    // if the attribute name matches check if the attribute is a string or an int
    if(strcmp(head->name,attribute)== 0){
      if(strcmp(head->type,"S")==0){
	// if its a string traverse the value linked list and pring the string values
	for(vPointer = head->vNext;vPointer->vNext !=NULL;vPointer = vPointer->vNext){
	  printf("%s\n",vPointer->sValue);
	  fflush(stdout);
	}
      }
      // if its an int traverse the value list and print the int values in each node
      if(strcmp(head->type,"I")== 0){
	for(vPointer = head->vNext;vPointer->vNext !=NULL;vPointer = vPointer->vNext){
          printf("%d\n",vPointer->iValue);
        }
      }
      // stop the loop here because we found matching attribute
      break;
    }
  }

  // if control reaches this point it means we didnt find the correct attribute, so print an error message to reflect that
  if(head == NULL){
    printf("The attribute, %s, is invalid\n",attribute);
    fflush(stdout);
  }

}
