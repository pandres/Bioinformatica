#include <stdio.h>

#define CANTALFA 28

//Este es el alfabeto con el trabaja el arbol, '$' es el caracter unico
const char alfa[CANTALFA] =
    { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '$',
};




typedef struct STNODO {
    struct STNODO *hijos[CANTALFA];
    int creado;
    int posS1;  // Lo utilizo para los arboles generalizados (string S1)
    int posS2;  // Lo utilizo para los arboles generalizados (string S2)
    struct STNODO *padre;
    struct STNODO *slink;
    int from;
    int *to;
    int esHoja;
} NODO;

int flagString;
int flagSpecial;
NODO* ST;
char* S1;
char* S2;
int e1;
int e2;
// Estas variables se utilizan en la busqueda del string comun mas largo en un arbol
int length; 
int posicion;
