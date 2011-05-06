/*
 * Knuth Morris Pratt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int* zboxes(char *S)
{
	int *Z;
	int n;
	int k, kprima, l, r;
	int longbeta, q;
	int i;

	n = strlen(S);
	Z = calloc(n, sizeof(int));
	if (Z<0) perror("No hay memoria para Z\n");
	
	i = 0;
	while (S[i] == S[1 + i]) i++;
	Z[1] = i;
	if (Z[1] > 0) {
		r = Z[1];
		l = 1;
	} else {
		r = l = 0;
	}

	for (k = 2; k < n; k++) {
		if (k > r) {
			i = 0;
			while (S[i] == S[k + i])
				i++;
			Z[k] = i;
			if (Z[k] > 0) {
				r = k + Z[k] - 1;
				l = k;
			}
		} else {
			kprima = k - l;
			longbeta = r - k + 1;
			if (Z[kprima] < longbeta) {
				Z[k] = Z[kprima];
			} else {
				i = 0;
				while (S[r + 1 + i] == S[longbeta + i]) i++;
				q = r + 1 + i;
				Z[k] = q - k;
				r = q - 1;
				l = k;
			}
		}
	}
	return Z;
}

int* preKMP(char* P, int n) 
{
	int* sp;
	int* Z;
	int i, j;
	/*printf("\nPatron:  %s\n",P);
	printf("Z-boxes: ");

	for ( i=0; i < n; i++) {
		printf("%d", Z[i]);
	}
	printf("\nsp:      ");

*/
	Z = zboxes(P);
	sp = calloc(n,sizeof(int));
	if (sp<0) perror("No hay memoria para SP");
	
	for (i=0; i<n; i++) sp[i]=0; 

	for (j=n-1; j>0; j--) {

		i = j + Z[j] - 1;
		sp[i] = Z[j];
	}	

  free(Z);
	return (sp);
}

void KMP(char *T, char *P, int m, int n) 
{
  int count=0;
	int c=1, p=1;
	int *sp;

	sp = preKMP(P, n);
	
	/*for ( i=0; i < n; i++) {
		printf("%d", sp[i]);
	}
	printf("\n\n");
  */

	while (c + (n - p) <= m) {
	
		while ((T[c-1]==P[p-1]) && (p<=n)) {
		
			p++;
			c++;		
		}
		if (p==n+1){
			 printf("Ocurrencia en la posicion %d\n", c-n);
			 count++;
		}
		if (p==1) c++;
		else p = sp[p-2] + 1; //funcion de falla
  }
  printf("\n Se encontraron %d ocuurencias de P en T\n\n", count);
  free(sp);
}

char* cargaString(char* in, char* S) {

FILE* file;
char* buffer_linea;
char cc;
int lenfile;


	if ((file = fopen( in, "r")) == NULL) {
		printf("No se pudo abrir %s\n", in);
		exit(1);
	}

	fseek(file, 1, SEEK_END);
	lenfile = ftell(file);
	rewind(file);
	//printf("lenfile: %d\n", lenfile);
	
	S = (char*)malloc((sizeof(char) * (lenfile+10)));
	buffer_linea = malloc(lenfile * sizeof(char));
	bzero(S, lenfile );
					
	cc = 'a';
	
	while(cc != EOF) {
					
		fgets(buffer_linea, lenfile, file);
		cc = fgetc(file);
		if (cc == EOF) strncat(S, buffer_linea, strlen(buffer_linea)-1);
		else strcat(S, buffer_linea);
		
		fseek(file, -1, SEEK_CUR);
	}

	S[strlen(S)] = '\0';
	
	free(buffer_linea);
	fclose(file);
	
	return S;
}

int main(int argc, char **argv)
{
	char *P;
	char *T;
	int m, n;

	if (argc < 3) {
		 printf("Faltan argumentos, uso:\n");
		 printf("<programa> <archivo> <archivo_patron>.\n");
		 exit(0);
     	}
 
//	printf("\nCargando strings...\n");
	T = cargaString(argv[1], T);
	P = cargaString(argv[2], P);

	if (strcmp(T, "") == 0) {
		printf("El archivo %s esta vacio.\n", argv[1]);
		exit(1);
	}
	if (strcmp(P, "") == 0) {
		printf("El archivo %s esta vacio.\n", argv[2]);
		exit(1);
	}

	m = strlen(T);
	n = strlen(P);
	KMP(T, P, m, n);
	return 1;
}
