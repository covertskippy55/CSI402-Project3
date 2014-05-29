/* Name: Dilan Samarasinghe
   ID: DS697678
*/

ATTRIBUTES* addRelation(char*,char*,ATTRIBUTES*);
void nattr(ATTRIBUTES*);
void tuplelen(ATTRIBUTES*);
void infattr(char*,ATTRIBUTES*);
void addValues(ATTRIBUTES*,char*);
int count(VALUES*);
void project(ATTRIBUTES*,char*);
void select(char*,char*,char*,ATTRIBUTES*);
int inStringArray(char[][],int,char*);
