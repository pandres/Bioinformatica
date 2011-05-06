#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.c"

						/* LIBRERIA CON FUNCIONES AUXILIARES  */


// DECLARACION DE FUNCIONES
int s_i(char*,int);
int cmpString(int, int, int, char*, char*);
void printString(NODO*);
void printTotalString(NODO*);

int esRaiz(NODO*);
int tiene_Slink(NODO*);
NODO *buscarHijo(NODO*,int,char*);
NODO *crearNodo(int,NODO*,NODO*,int,int*,char*);
int longPath(NODO*);
void printNodo(NODO*);
void printSTree(NODO*,int);
void freeSTree(NODO*);
char* find_longest_common_string();
void aux_find_longest_common_string(NODO*);


//---------------------------------------------------

		// LIBRERIA PARA STRINGS

//Devuelve S(i) en Ascii
int s_i(char *S, int indice)
{
	
	  /*
	  En ASCII:
	  	'a' = 96
	  	' ' = 32
	  	'$' = 36
	  */	
    
    int letra, aux;
    aux=S[indice-1];

    if (aux==32) letra = 26; 
    else{
    	if (aux==36) letra = 27;
      else letra = aux - 97;
    }
    return letra;
}

int cmpString(int j1, int j2, int i2, char* S1, char* S2){
    int pos=0;
    int lenS2;
    lenS2=i2-j2+1;
    while(pos<lenS2 && s_i(S1,j1+pos)==s_i(S2, j2+pos)){
    	pos++;
    }
    return pos;
}


char *catPeso(char *in) {

     char* S = (char*)malloc(sizeof(char)*(strlen(in)+1));
     S = strcat(S, in);
     S = strcat(S, "$");
     return S;
}



void printString(NODO* n){
   int i;
   char* S;
   //printf ("S: %s S1: %s\n", S, S1);
   printf("[");
   if(n!=NULL){
   	if(esRaiz(n)==0){
      if(n->posS1 != 0)	S=S1;
			else S=S2;
			for(i=n->from; i<=*(n->to); i++){
				printf("%c", S[i-1]);
			}
    }  				
	 }
	 printf("]");  
}


void printTotalString(NODO* n){
   int length,pos,i;
   char* S;
   //printf ("S: %s S1: %s\n", S, S1);
   printf("{");
   if(n!=NULL){
   	if(esRaiz(n)==0){
      if(n->posS1 != 0){
				length=*(n->to)-(n->posS1)+1;
				pos=n->posS1;
				S=S1;
			}	
      else{
				length=*(n->to)-(n->posS2)+1;
				pos=n->posS2;
				S=S2;
			}	
			for(i=0; i<length; i++){
				printf("%c", S[pos+i-1]);
			}
    }  				
	 }
	 printf("}");  
}



//ver print total string!!!!
int printTotalString1(NODO* n){
   char* S;
   int camino[strlen(S1)+strlen(S2)];
   int i=0;
			    if(n->posS2 != 0) S=S2;
   				else S=S1;	   
   //printf ("S: %s S1: %s\n", S, S1);
   printf(" {");
   if(n!=NULL){
   	 if(n->padre != NULL){
		   while(n->padre != NULL){
		      camino[i] = n->from;         
		      n = n -> padre;
		      i++;
		   }
		   i--;
		   while(i>=0){
	   	
		      n = buscarHijo(n, camino[i], S);
		      printString(n);
		      i--;	
		   }
		 }
	 }
	 printf("} ");  
   return 0;
}



//-------------------------------------------


		// LIBRERIA PARA ARBOLES

int esRaiz(NODO* n){
        if(n==NULL) return 0;
	if(n->padre == NULL) return 1;
	else return 0; 
}

int tiene_Slink(NODO* n){
        if(n==NULL) return 0;
	if(n->slink != NULL) return 1;
	else return 0;	
}

NODO *buscarHijo(NODO * nodo, int from, char *string)
{
    NODO *hijo;
    int letra;
    letra = s_i(string, from);
    hijo = nodo->hijos[letra];
    return hijo;
}




NODO *crearNodo(int posicion, NODO * padre, NODO * slink, int from,	int *to, char *S){
    int letra;
    NODO *nodo = (NODO *) malloc(sizeof(NODO));

    if (nodo == NULL) {
	perror("\nNo hay memoria para un nodo\n");
	exit(0);
    }
    memset(nodo->hijos, 0, CANTALFA);
    nodo->creado = flagString;
    nodo->posS1 = 0;
    nodo->posS2 = 0;
    nodo->padre = padre;
    nodo->slink = slink;
    nodo->from = from;
    nodo->to = to;
    nodo->esHoja = 1;

    if (padre != NULL) {
	letra = s_i(S, from);
	padre->hijos[letra] = nodo;
	padre->esHoja = 0;	
  }

    return nodo;
}


int longPath(NODO * nodo)
{
    int aux;
       //si nodo no es la raiz...
    if (nodo->padre!=NULL) aux = *(nodo->to) - (nodo->from) + 1;
    else aux = 0;   
    return aux;
}


void printNodo(NODO * n)
{
    int pr;
  	printString(n);
    printTotalString(n);
    printf("Nodo: %d Pos: (%d,%d) Padre: %d ",(int) n, n->posS1, n->posS2, (int) n->padre);
	  if(esRaiz(n)==0){
			 printString(n->padre);
	  }
	     printf(" - slink: %d",(int) n->slink);
	  if(tiene_Slink(n->slink)){
			 printString(n->slink);
	     printTotalString(n->slink);
	  }
	  printf("\n");
    if (n->esHoja == 0) {
	    printf("Hijos:");
	    for (pr = 0; pr < CANTALFA; pr++){
	       if (n->hijos[pr] != NULL){
 	         printString(n->hijos[pr]);
		       printf("(%d)", (int) n->hijos[pr]);
		     }  
		  }
    }
    printf("\n");
}

void printSTree(NODO * nodo, int nivel){
    int i;
    printf
	  ("------------------------------------------------------------------------\n");
    printf("niv: %d ", nivel-1);
/*    if(nodo->posS2!=0) printNodo(nodo, S2);
    else printNodo(nodo, S1);*/
    printNodo(nodo);
    if (nodo->esHoja == 0) {
	    nivel++;
	    for (i = 0; i < CANTALFA; i++){
	       if (nodo->hijos[i] != NULL) printSTree(nodo->hijos[i], nivel);
      }
    }  
}


//Libera la memoria usada por el arbol
void freeSTree(NODO * nodo)
{
    int i;

    if (nodo != NULL) {
	for (i = 0; i < CANTALFA; i++)
	    if (nodo->hijos[i] != 0)
		freeSTree(nodo->hijos[i]);
	// printf("Liberando--> %d\n", (int) nodo);
	free(nodo);
    }
}

char* find_longest_common_string(){
	int i;
	char* S;
  posicion=0;
  length=0;
	aux_find_longest_common_string(ST);
//printf("\nl: %d  -  pos: %d\n", length, posicion);
	S = (char*)malloc(length);
	for(i=0; i<length;i++){
		S[i]=S1[posicion+i-1];
	}
	return(S);
}

void aux_find_longest_common_string(NODO* nodo){
	int i;
	int aux;
	if(nodo->posS1!=0&&nodo->posS2!=0){
		aux=*(nodo->to)-(nodo->posS1)+1;
		if(aux>length){
			length=aux;
			posicion=nodo->posS1;
		}
	}
  if (nodo->esHoja == 0) {
    for (i = 0; i < CANTALFA; i++){
       if (nodo->hijos[i] != NULL) aux_find_longest_common_string(nodo->hijos[i]);
    }
  }  
}
	