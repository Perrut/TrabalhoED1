#include <stdio.h>
#include <stdlib.h>

typedef struct arvore{
	int info;
	struct arvore *filho;
	struct arvore *prox_irmao;
} TAN;

void insere(TAN *pai, TAN *filho){
	filho -> prox_irmao = pai -> filho;
	pai -> filho = filho;
}

TAN * aloca(int info){
	TAN *novo = (TAN*) malloc(sizeof(TAN));
	novo -> info = info;
	novo -> filho = novo -> prox_irmao = NULL;
	return novo;
}

TAN * busca(TAN *a, int x){
	if(a == NULL)
		return NULL;
	if(a -> info == x)
		return a;
	TAN *aux = busca(a -> filho, x);
	if(aux)
		return aux;
	return busca(a -> prox_irmao, x);
}

void imprime(TAN *a){
	printf("%d ", a -> info);
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
	
	arvore = aloca(0);
	
	printf("Entre com um numero para a insercao: ");
	scanf("%d", &n);
	while(n > 0){
		TAN *aux = NULL;
		aux = aloca(n);
		insere(arvore, aux);
		printf("Entre com um numero para a insercao: ");
		scanf("%d", &n);
	}
	
	printf("Entre com um numero para a busca: ");
	scanf("%d", &n);
	while(n > 0){
		TAN *aux = NULL;
		aux = busca(arvore, n);
		if(aux){
			int i;
			printf("Entre com um numero para a insercao: ");
			scanf("%d", &i);
			while(i > 0){
				TAN *novo = NULL;
				novo = aloca(i);
				insere(aux, novo);
				printf("Entre com um numero para a insercao: ");
				scanf("%d", &i);
			}
		} else printf("Não encontrado!\n");
		printf("Entre com um numero para a busca: ");
		scanf("%d", &n);
	}
	
	printf("Entre com um numero para remocao: ");
	scanf("%d", &n);
	while(n > 0){
		TAN *aux = busca(arvore, n);
		deleta(aux);
		printf("Entre com um numero para remocao: ");
		scanf("%d", &n);
	}
	
	imprime(arvore);
	printf("\n");

	return 0;
}
