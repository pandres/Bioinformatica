/* 
 * Ukkonen Algorithm
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libs.c"


//--Funcion utilizada para calcular la regla que corresponde aplicar---//


int calculaRegla(NODO * nodoActual, int g, int j, int i, char *S)
{
    int regla;
    char* Saux;
    if(esRaiz(nodoActual)) Saux = S;
    else{
       if(nodoActual->posS1!=0 && nodoActual->posS2!=0) Saux=S1;
       else{
       	  if(nodoActual->posS1!=0) Saux=S1;
       	  else Saux=S2;
       }
    }	//para saber de quien es el from

    if ((longPath(nodoActual) ==  g) || esRaiz(nodoActual) || (g == 0)) {//#ver de reducir guarda
      	if (nodoActual->hijos[s_i(S, i + 1)] != NULL) {
  		regla = 3;
      		if(flagString == 2 && longPath(nodoActual->hijos[s_i(S, i + 1)])==1 && 
      		(nodoActual->hijos[s_i(S, i + 1)]->posS2 == 0)){
      		   nodoActual->hijos[s_i(S, i + 1)]->posS2=j;
      		}
      	}
        else regla = 20;
    }
    else{
         if (s_i(S, i + 1) == s_i(Saux, nodoActual->from + g)){
         	regla = 3;
         	if((flagString == 2) && (longPath(nodoActual) == (g+1)) && 
         	(nodoActual->posS2==0)) nodoActual->posS2=j;	
         }
         else regla = 21;
    }
    return regla;
}

//--------------------FIND`S-----------------------//

//Estan funciones buscan "alfa" en el arbol


NODO *special_find(NODO * arbol, int *g, int* matcheado)
{
    NODO *temp;
    int j=1;
    int gprima;
    int auxletra;
    auxletra = s_i(S2, j);
    //printf("1 %d\n", auxletra);
    if (arbol->hijos[auxletra]!=NULL) temp = buscarHijo(arbol, j, S2);
    else return arbol;
    //printf("2\n");
    gprima = longPath(temp);
    while(*matcheado+gprima<=strlen(S2)){
       if(cmpString(temp->from, j, j+gprima-1, S1, S2)==gprima){
    //printf("a\n");
          temp->posS2 = 1;
          *matcheado=*matcheado+gprima;
          j=j+gprima;
          auxletra = s_i(S2, j);
          if (temp->hijos[auxletra]!=NULL){
    //printf("a2\n");
              temp = buscarHijo(temp, j, S2);
              gprima=longPath(temp);
          }
          else{
    //printf("a3\n");
             *g=gprima;
              return temp;
          }
       }
       else{
    //printf("b\n");
          *g=cmpString(temp->from, j, j+gprima-1, S1, S2);
          *matcheado=*matcheado+*g;
          return temp;
       }
    }
    //printf("3\n");
    *g=cmpString(temp->from, j, strlen(S2), S1, S2);
    *matcheado=*matcheado+*g;
    return temp;
}


//Este es el find "tonto": busca en el arbol el final de la palabra S(j..i)

NODO *find_j_i_naive(NODO * nodoActual, int *g, int j, char *S, int actualizoPosS2)
{
    NODO *temp;
    int gprima,h=0;
    if (*g == 0) return nodoActual;	// Si longBeta = 0...
    temp = buscarHijo(nodoActual, j, S);
    gprima = longPath(temp);
    if(flagString==2 && (temp->posS2 == 0) && (*g>=gprima) && actualizoPosS2 == 1){
       temp->posS2 = j;
    }
    while (*g > gprima) {
			h = h + gprima;
			*g = *g - gprima;
			temp = buscarHijo(temp, j + h, S);
			gprima = longPath(temp);
                        if(flagString==2 && temp->posS2 == 0 && *g>=gprima && actualizoPosS2 == 1){
                             temp->posS2 = j;
                        }
    }
    return temp;
}

//Este es el find que utilizamos cuando atravesamoa algun suffix link: 
//  busca en el arbol el final de la palabra S(j..i) pero partiendo de s(w)

NODO *find_j_i(NODO * nodoActual, int *g, int j, int i, char *S)
{
    NODO *temp;
    int gprima, h;
    int posicion_absoluta_gamma;	// posicion donde comienza gamma en el string S

    if (*g == 0){ 
    	 return nodoActual;//Si longgamma = 0 
    }

    posicion_absoluta_gamma = i - *g;
    h = 1;
    temp = buscarHijo(nodoActual, posicion_absoluta_gamma + h, S);
    gprima = longPath(temp);
    if(flagString==2 && temp->posS2 == 0 && (*g>=gprima)){
       temp->posS2 = j;
    }

    while (*g > gprima) {
			h = h + gprima;
			*g = *g - gprima;
			temp = buscarHijo(temp, posicion_absoluta_gamma + h, S);
			gprima = longPath(temp);
                        if(flagString==2 && temp->posS2 == 0 && *g>=gprima){
                             temp->posS2 = j;
                        }
    }

    return temp;
}

//--------------------------REGLAS---------------------------//

void regla1(int *e)
{
    *e = *e + 1;
}

//----

void extension_regla2a(NODO * n, int j){
    if (flagString == 1){
	n->posS1=j;
    }
    else{
	n->posS2=j;
    }
}

NODO *regla2a(NODO * nodoActual, int j, int i, int *e, char *S)
{
    NODO *nuevo;
    nuevo = crearNodo(0, nodoActual, NULL, i + 1, e, S);
    extension_regla2a(nuevo, j); // para los arboles generalizados
    return nuevo;
}

//----

int buscarPos(NODO *n){
    int pos;
    NODO* aux;
    aux = n;
    while(!esRaiz(aux->padre)){
	aux=aux->padre;
    }
    pos = aux->from;
    return pos;
}

void extension_regla2b(NODO* nuevo1, NODO* nuevo2, int j, int pos1, int pos2){

    if (flagString == 1){
    	nuevo1->posS1=pos1;
	    nuevo2->posS1=j;
    }
    else{
        nuevo1->posS1=pos1;
        if(pos2!=0) nuevo1->posS2=pos2;
        else{
            nuevo1->posS2=j;
        }
	  nuevo2->posS2=j;
    }
}

NODO *regla2b(NODO * nodoActual, int g, int j, int i, int *e, char *S){

    int *nuevo_to;
    NODO *nuevo1;		// Es el que parte la arista
    NODO *nuevo2;		// Es la nueva hoja
    char* Saux;
    
    nuevo_to = (int *) malloc(sizeof(int));
    if(nodoActual->posS1!=0)  Saux=S1;
    else Saux = S2;

    //Nodo que parte la arista      
    *nuevo_to = nodoActual->from + g - 1;
    nuevo1 = crearNodo(0, nodoActual->padre, NULL, nodoActual->from, nuevo_to, Saux);
    //Actualizamos nodoActual
    nodoActual->padre = nuevo1;
    nodoActual->from = *nuevo_to + 1;
    nuevo1->hijos[s_i(Saux, nodoActual->from)] = nodoActual; // le asigno el hijo al nuevo1
    //Nodo que contiene a S[i+1]
    nuevo2 = crearNodo(0, nuevo1, NULL, i + 1, e, S);

    
//cuando digo que nuevo1 lo tiene a nuevo2 de hijo??????
    extension_regla2b(nuevo1, nuevo2, j, nodoActual->posS1, nodoActual->posS2);
    return nuevo2;
}


//----------------------------------------------------//

// Esta es la impplementacion del Algoritmo SEA del libro del libro de Dan Gusfield (ver pag 100)
NODO *SEA(NODO * I, NODO * nodoAnterior, int j, int i, int *regla3, int* reglaAnterior, int *e, char *S){
    int regla;
    int g=-1;			// variables utilizadas por skip/count trick
    int k, p1, p2; // a estas variables las uso para hacer las reglas tres en el arbol generalizado (con S2)
    NODO *nodoActual;
    NODO *nuevo;
    NODO *auxiliar;
    nuevo = NULL;
//printf("\n\n\ningreso a SEA  FASE : %d  --   j: %d  raglaAnt: %d -  NodoExtAnt: ", i + 1, j, *reglaAnterior);
//printTotalString(nodoAnterior);
//printf("1\n");
//-----------------PASO 1 -----------------//

    //si el nodo de la extension anterior S[j-1..i+1] no tiene slink
    if (!(tiene_Slink(nodoAnterior))) {
       if(tiene_Slink(nodoAnterior->padre)){ //si no vengo de regla 2b-caso "sin slink"
	         nodoActual = nodoAnterior->padre;
           g = longPath(nodoAnterior);
       }
       else{ 
      	   nodoActual = I;
       }
    } 
    else {
	      nodoActual = nodoAnterior;
	      g = 0;
    }
//printf("2\n");
//    printf("\nFin paso 1: g: %d - NodoActual: ", g);
//printf("3\n");    
//    printTotalString(nodoActual);
//printf("4\n");


//-----------------PASO 2 -----------------//

    if(!esRaiz(nodoActual)){
       if(tiene_Slink(nodoActual)&&!esRaiz(nodoActual->slink)){
//printf("a\n");
  	      if ((*reglaAnterior != 3 && flagString == 1) || ((flagString==2) && (((*reglaAnterior == 20 || *reglaAnterior == 21) && flagSpecial == 1) || (*reglaAnterior != 3 && flagSpecial == 0)))) g--;
	           nodoActual = nodoActual->slink;
	           if(flagString==2&&nodoActual->posS2==0) nodoActual->posS2=j;
	              nodoActual = find_j_i(nodoActual, &g, j, i, S);
             }
       else{
	        g = i - j + 1;    //find naive (no usa slink)
	        nodoActual = find_j_i_naive(I, &g, j, S, 1);
       }
    }
    else{
	     g = i - j + 1;		//find naive (no usa slink)
	     nodoActual = find_j_i_naive(I, &g, j, S, 1);
    }

//printf("5\n");
//    printf("\nFin paso 2:  g: %d - nodoActual:", g);
//printf("6\n");    
//    printTotalString(nodoActual);
//printf("7\n");
//-----------------PASO 3 -----------------//

    regla = calculaRegla(nodoActual, g, j, i, S);
//printf("8\n");
 //   printf("\nEn Paso 3: regla a usar: %d\n", regla);
    nuevo = nodoActual;
    if (regla == 20) {
	     	nuevo = regla2a(nodoActual, j, i, e, S);
    }
    if (regla == 21) {
		nuevo = regla2b(nodoActual, g, j, i, e, S);


		if(tiene_Slink(nodoActual)){
		   if(longPath(nodoActual) == longPath(nodoActual->slink)){
		      (nuevo->padre)->slink = (nodoActual->slink)->padre;
		   }
		}
		else{
		    g = i - j;
		    nodoActual = find_j_i_naive(I, &g, j+1, S, 0);
		    if (longPath(nodoActual)==g){
		       (nuevo->padre)->slink = nodoActual;
		    }
		}
    }   
    if (regla == 3) {
				*regla3 = 1;
				if(flagString==2){
					p2=i-j;
//printf("hola\n");
					for(k=j+1;k<=i+1;k++){
						 p1=p2;
	           auxiliar=find_j_i_naive(ST, &p1, k, S2, 1);
	           calculaRegla(auxiliar, p1, k, i, S2);
					   p2--;
					}   
				}
    }     
//printf("chau\n");    
//    printf(" -- Fin paso3:  regla: %d  -  regla3: %d  - nuevo: \n", regla, *regla3);
//    printTotalString(nuevo);

//-----------------PASO 4 -----------------//

    if (*reglaAnterior == 21 && regla == 21){
      	(nodoAnterior->padre)->slink = nuevo->padre;
      }	
    *reglaAnterior = regla;
    if ((regla == 20) || (regla == 21)){
//      printf("\nFin paso4:   nodoAnterior: %d\n", (int) nuevo);
      return nuevo;
    }
    else{
//      printf("\nFin paso4:   nodoAnterior: %d\n", (int) nodoAnterior);
      return nodoAnterior;
    }
}


//-------------------UKKNOEN------------------//


NODO *ukkonen(NODO* I, char *S, int m, int* e){
    int root_to;
    int matcheado=0;
    int i, j_i, j_estrella;	// i = fases j = extensiones
    int regla3 = 0;
    int reglaAnterior = 0;
    int faseInicial;
    int auxg=0;
//  estas variables se utilizan para hacer reglas 3 en el caso especial
    int k, p1, p2;  
    NODO* auxiliar; 
//    int auxj_i=0;
    NODO* nodoAnterior;
    if(flagString == 1){
        faseInicial = 1;
        *e = 0;
        root_to = 0;
        j_i = 1;
    
    /*------------Genero I(1)------------*/

        // Crear el root
        I = crearNodo(0, NULL, NULL, 0, &root_to, S);
        //Creamos el hijo
        *e = *e + 1;
        nodoAnterior = crearNodo(0, I, NULL, 1, e, S);
        nodoAnterior->posS1=1;
    }
    else{
         special_find(ST, &auxg, &matcheado);
//         printf("auxg: %d n->S1: %d n->S2: %d e: %d\n", auxg, nodoAnterior->posS1, nodoAnterior->posS2, matcheado);
//         printTotalString(nodoAnterior);
         //printSTree(ST,1);
         if(matcheado>=2){
					  p2=matcheado-1;
					 	for(k=2;k<=matcheado;k++){
							 p1=p2;
		           auxiliar=find_j_i_naive(ST, &p1, k, S2, 1);
		           calculaRegla(auxiliar, p1, k, i, S2);
						   p2--;
						}            
				 }		
         //printSTree(ST,1);				 
         *e = matcheado;
         faseInicial = matcheado;
         nodoAnterior=ST; // inicialmente empieza desde la raiz
         j_i = 0;
   
    }
    /*-----------Calculo I(i+1)-----------*/
    for (i = faseInicial; i <= (m - 1); i++) {	//Comienza Fase i+1
           /*----SPA---- */   //ver pag 106 Dan Gusfuield
     	  // paso 1
		regla1(e);
		reglaAnterior = 1;

 	 // paso 2
		while ((i + 1 > j_i) && (!regla3)){//mientras(no regla3 o hicimos todas las extnsiones
		    nodoAnterior =
                    SEA(I, nodoAnterior, j_i + 1, i, &regla3, &reglaAnterior, e, S);
		    						j_i++;
/*		    						if(reglaAnterior==3 && flagString==2 && auxj_i==0){
		    							 auxj_i=j_i;
		    						}*/
                    //printSTree(I, 1);
  		}
/*			if(auxj_i!=0 && flagString == 2){ //restauro el valor de j_i pues hice muchas veces regla 3
					j_i=auxj_i;
					auxj_i=0;
   		}
*/
	   	 if (regla3) {
		    j_estrella = j_i;
		    regla3 = 0;
		 }
		 else j_estrella = i + 2;
         //paso 3
	 j_i = j_estrella - 1;
	 if(i==faseInicial) flagSpecial = 0;
    }
    return (I);
}

int main(int argc, char **argv){
    int lenS1;
    int lenS2;

    if (argc < 3) {
			printf("Faltan argumentos, ingrese un texto alfabetico\n");
			exit(0);
    }
    S1 = catPeso(argv[1]);
    S2 = catPeso(argv[2]);
    lenS1 = strlen(S1);
    lenS2 = strlen(S2);
    flagString=1;
    ST = ukkonen(ST, S1, lenS1, &e1);
    //printSTree(ST, 1);
    flagString = 2;
    flagSpecial = 1;
    ST = ukkonen(ST, S2, lenS2, &e2);
    //printSTree(ST, 1);
    find_longest_common_string();
    printf("\nRespuesta: %s\n", find_longest_common_string());
    free(S1);
    free(S2);
    freeSTree(ST);
    return 1;
}
// como condicion S1 y S2 deben ser distrintos de vacio
