/* Name: Dilan Samarasinghe
   ID: DS697678
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "struct_def.h"
#include "prototypes.h"

/* 
   The main function will open up the config,data,query and schema files. It will create a database out of the information and run the queries from the query file against it.
 */
int main(int argc,char *argv[]){
  int i = 0;// both this and j are temp values used to traverse arrays
  int j;
  // int foundRelation = 0; // this flag tells us if we found a relation matching the query
  int numRelations;//holds the number of relationships
  FILE *configFile;//file pointer to config file
  FILE *queryFile;//file pointer to query file
  char originalRelationName[MAX_STRING_LENGTH]; // holds the original file name thats not concatenated
  char relationName[MAX_STRING_LENGTH];//array to hold relation name from config file
  char schemaName[MAX_STRING_LENGTH+4]; // array to hold the concatenated relation name, we allocate 4 extra char spaces for the trailing file type
  char dataName[MAX_STRING_LENGTH+4]; // array to hold the concatenated data name.
  char *attrname; // holds the name of attributes from query file
  //char *operator;
  //char *value;
  char query[MAX_QUERY_LENGTH];// array to hold each query from the query file
  RELATIONS *relations; // a pointer to a relation node
  char *sch = {".sch"}; // the string to be concatenated to get the schema name
  char *dat = {".dat"}; // the string to be concatenated to get the data name
  char *token; // holds tokenized sting

//check if the number of arguments is correct
  if(argc != NUM_ARG){
     fprintf(stderr,"Invalid number of commads;Use format: p3 configfile queryfile next time \n");
     exit(1);
  }
  // open config file
  if((configFile = fopen(argv[FIRST_ARG],"r")) == NULL){
     fprintf(stderr,"Error occured during the opening of configuration file: %s \n",argv[1]);
     exit(1);
  }
  //open query file
  if((queryFile = fopen(argv[SECOND_ARG],"r")) == NULL){
     fprintf(stderr,"Error occured during the opening of query file: %s \n",argv[2]);
     exit(1);
  }
  // get the first line of the config file
  fgets(relationName,MAX_STRING_LENGTH,configFile);
  
  // this above line is the number of relations but is a string so set numRelations using atoi
  numRelations = atoi(relationName);
  // malloc an array of RELATIONS using numRelations and sizeof RELATIONS
  if(( relations = malloc(sizeof(RELATIONS)*numRelations))== NULL){
    fprintf(stderr,"Error occured with malloc \n");
      exit(1);
  }
  // remove the new line character by looking for it in the string and replacing it with \0
  while(fgets(relationName,MAX_STRING_LENGTH,configFile)!= NULL){
    for(j = 0; j < MAX_STRING_LENGTH;j++){
      if(relationName[j] == '\n'){
	relationName[j] = '\0';
      }
    }
    // copy the name of the relation to the relation node
    strcpy(relations[i].name,relationName);
    // set the pointer to the attribute list to be null
    relations[i].vNext = NULL;
    // printf("%s %d\n",relations[i].name,i);
    
    // copy the relation name to originialRelationName because we're going to concatenate 
    strcpy(originalRelationName,relationName);
    // get the schema name by concatenating and copy that to schemaname
    strcat(relationName,sch);
    strcpy(schemaName,relationName);
    // printf("%s\n",schemaName);
   
    // copy back the original relation name
    strcpy(relationName,originalRelationName);
    // do the concatenating to get the data name and copy that
    strcat(relationName,dat);
    strcpy(dataName,relationName);
    //  printf("%s\n",dataName);
    // call the add relation function which sets up the attribute list and set the attribute list pointer to the return value
    relations[i].vNext = addRelation(schemaName,dataName,relations[i].vNext);
    // call the addValues function which sets up the actual tuple values
    addValues(relations[i].vNext,dataName);
    i++;
  }

  /* WE START QUERY FILE FROM THIS POINT ON */

  // read through the query fiile until we reach the end
   while(fgets(query,MAX_QUERY_LENGTH,queryFile)!= NULL){
     // remove the new line character from each line
    for(j = 0; j < strlen(query);j++){
      if(query[j] == '\n'){
        query[j] = '\0';
      }
    }
    // start tokenizing the each line of the query file
    token = strtok(query," \t\n");

    // see if the command is nattr if it is traverse the array of relations looking for the correct relation name, which we get by tokenizing again
    if(strcmp(token,"nattr") == 0){
      token = strtok(NULL," ");
      for(i = 0;i <numRelations;i++){
	if(strcmp(relations[i].name,token) == 0){
	  // if we find the correct relation call nattr and then break the loop
	  nattr(relations[i].vNext);
	  break;
	}
      }
      // if we traverse the relations array and find no value matching the value asked for we reach this point and we output an error message
      if(i >= numRelations){
	printf("The relation,%s, is not valid\n",token);
	fflush(stdout);
      }
    }
    // same as above function but for tuplelen instead
    if(strcmp(token,"tuplen") == 0){
      token = strtok(NULL," ");
      for(i = 0;i <numRelations;i++){
        if(strcmp(relations[i].name,token) == 0){
          tuplelen(relations[i].vNext);
          break;
        }
      }
      if(i >= numRelations){
        printf("The relation,%s, is not valid\n",token);
        fflush(stdout);
      }
    }
    // same as above function but for infattr
    if(strcmp(token,"infattr") == 0){
      token = strtok(NULL," ");
      for(i = 0;i <numRelations;i++){
        if(strcmp(relations[i].name,token) == 0){
	  attrname = strtok(NULL," "); // this holds the attribute name
          infattr(attrname,relations[i].vNext);
          break;
        }
      }
      if(i >= numRelations){
        printf("The relation,%s, is not valid\n",token);
        fflush(stdout);
      }
    }
    // same as above function but for count
    if(strcmp(token,"count") == 0){
      token = strtok(NULL," ");
      for(i = 0;i <numRelations;i++){
        if(strcmp(relations[i].name,token) == 0){
          count(relations[i].vNext->vNext);
          break;
        }
      }
      if(i >= numRelations){
        printf("The relation,%s, is not valid\n",token);
        fflush(stdout);
      }
    }
    // same as above function but for project
    if(strcmp(token,"project") == 0){
      token = strtok(NULL," ");
      for(i = 0;i <numRelations;i++){
        if(strcmp(relations[i].name,token) == 0){
	  token = strtok(NULL," ");
          project(relations[i].vNext,token);
          break;
        }
      }
      if(i >= numRelations){
        printf("The relation,%s, is not valid\n",token);
        fflush(stdout);
      }
    }
    /*    if(strcmp(token,"select") == 0){
      token = strtok(NULL," ");
      for(i = 0;i <numRelations;i++){
        if(strcmp(relations[i].name,token) == 0){
          attrname = strtok(NULL," ");
	  operator = strtok(NULL," ");
	  value = strtok(NULL," ");
	  select(attrname,operator,value,relations[i].vNext);
        }
      }
      if(i >= numRelations){
        printf("The relation,%s, is not valid\n",token);
        fflush(stdout);
      }
    }
    */
    // because select hasnt been programmed yet this error is outputted
    if(strcmp(token,"select") == 0){
      token = strtok(NULL," ");
      for(i = 0;i <numRelations;i++){
        if(strcmp(relations[i].name,token) == 0){
         printf("This function has not been programmed yet\n");
	 fflush(stdout);
	 break;
        }
      }
      if(i >= numRelations){
        printf("The relation,%s, is not valid\n",token);
        fflush(stdout);
      }
    }

    // if the command is quit then end main
    if(strcmp(token,"quit") == 0){
      return 0;
    }

				  

   }
  
  
  return 0;
}//END OF MAIN
