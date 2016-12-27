#include <stdio.h>
#include <stdlib.h>

typedef struct arquivo{
	char *nome;
	int tamanho;
} ARQ;

typedef struct diretorio{
	char *nome;
	int pastas;
} DIR;

typedef struct arvore{
	void *info;
	int tipo;
	struct arvore *filho;
	struct arvore *prox_irmao;
} TAN;

void insere(TAN *pai, TAN *filho){
	filho -> prox_irmao = pai -> filho;
	pai -> filho = filho;
}

TAN * aloca_arq(char *nome, int tamanho){
	TAN *novo = (TAN*) malloc(sizeof(TAN));
	ARQ *aux = (ARQ*) malloc(sizeof(ARQ));
	aux -> tamanho = tamanho;
	aux -> nome = nome;
	novo -> info = aux;
	novo -> tipo = 1;
	novo -> filho = novo -> prox_irmao = NULL;
	return novo;
}

TAN * aloca_dir(char *nome, int pastas){
	TAN *novo = (TAN*) malloc(sizeof(TAN));
	DIR *aux = (DIR*) malloc(sizeof(DIR));
	aux -> pastas = pastas;
	aux -> nome = nome;
	novo -> info = aux;
	novo -> tipo = 0;
	novo -> filho = novo -> prox_irmao = NULL;
	return novo;
}

//TAN * busca(TAN *a, int x){
//	if(a == NULL)
//		return NULL;
//	if(a -> info == x)
//		return a;
//	TAN *aux = busca(a -> filho, x);
//	if(aux)
//		return aux;
//	return busca(a -> prox_irmao, x);
//}

void imprime(TAN *a){
	if(a -> tipo == 0){
		DIR *dir = a -> info;
		printf("%s ", dir -> nome);
	}
	else{
		ARQ *arq = a -> info;
		printf("%s %d", arq -> nome, arq -> tamanho);
	}
	TAN *p = a -> filho;
	for(; p; p = p -> prox_irmao)
		imprime(p);
}

void libera(TAN *a){
	if(a -> prox_irmao)
		libera(a -> prox_irmao);
	if(a -> filho)
		libera(a -> filho);
	free(a);
}

void deleta(TAN *a){
	if(a){
		if(a -> filho)
			libera(a -> filho);
		a -> filho = NULL;
	}
}

int main(void){
	TAN *arvore = NULL;
	int n;
	
	arvore = aloca_arq("ovo", 90);
	
	imprime(arvore);
	printf("\n");

	return 0;
}
