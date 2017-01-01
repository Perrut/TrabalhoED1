#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct arquivo{
	char *nome;
	char *nomePai;
	FILE *arqv;
	long tamanho;
} ARQ;

typedef struct diretorio{
	char *nome;
	char *nomePai;
} DIR;

typedef struct arvore{
	void *info;
	char tipo;
	char *dataCriacao;
	char *ultimaModD;
	struct arvore *filho;
	struct arvore *prox_irmao;
} TAN;

int calcularTamanhoArquivo(FILE *arquivo) {
    // guarda o estado antes de chamar a fun��o fseek
    long posicaoAtual = ftell(arquivo);
    // guarda tamanho do arquivo
    long tamanho;
    // calcula o tamanho
    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    // recupera o estado antigo do arquivo
    fseek(arquivo, posicaoAtual, SEEK_SET);
    return tamanho;
}

FILE * criarArquivo(char*nome, char tipo){
	FILE *arq;
	char conteudo[5000];
	if(tipo=='T'){
		arq=fopen(nome,"wt");
		printf("Insira o conteudo do arquivo: \n");
		scanf(" %[^\n]s", conteudo);
        //char*conteudo=ler_texto();
		fprintf(arq, "%s", conteudo);
		return arq;
	}
	else{
		arq=fopen(nome,"wb");
		printf("Insira o conteudo do arquivo: \n");
		scanf(" %[^\n]s", conteudo);
        //char*conteudo=ler_texto();
		fprintf(arq, "%s", conteudo);
		return arq;
	}
}

TAN * buscaDiretorio(TAN *a, char *nome){
     DIR *elem = a->info;
     if(strcmp(elem -> nome, nome) == 0 && a -> tipo == 'D') return a;
     TAN *p;
     for(p = a -> filho; p; p = p->prox_irmao){
        TAN *resp = buscaDiretorio(p,nome);
        if(resp) return resp;
     }
     return NULL;
}

int e_filho(TAN *a1, TAN *a2){
	if(a1 == a2)
		return 1;
	TAN *p;
	for(p = a1 -> filho; p; p = p->prox_irmao){
        int r = e_filho(p,a2);
        if(r) return r;
    }
    return 0;
}

void atualizaDataHora(TAN *raiz, TAN *a){
	TAN *aux = a;
	TAN *aux2 = NULL;
	if(aux -> tipo == 'D'){
		DIR *d = aux -> info;
		while(strcmp(d -> nomePai, "NULL") != 0){
			aux2 = buscaDiretorio(raiz, d -> nomePai);
			if(aux2){
				time_t mytime;
				mytime = time(NULL);
				char *dc = (char*) malloc(30*sizeof(char));
				strcpy(dc, ctime(&mytime));
				aux2 -> ultimaModD = dc;
				aux = aux2;
				d = aux -> info;
			} else break;
		}
	} else{
		ARQ *d = aux -> info;
		char *c = d -> nomePai;
		while(strcmp(c, "NULL") != 0){
			aux2 = buscaDiretorio(raiz, c);
			if(aux2){
				time_t mytime;
				mytime = time(NULL);
				char *dc = (char*) malloc(30*sizeof(char));
				strcpy(dc, ctime(&mytime));
				aux2 -> ultimaModD = dc;
				aux = aux2;
				DIR *dir = aux -> info;
				c = dir -> nomePai;
			} else break;
		}
	}
}

void insere(TAN *raiz, TAN *pai, TAN *filho){
    //arquivo nao pode ter filhos
    if(pai -> tipo != 'D'){
    	printf("ERRO: arquivo nao pode ter filhos!!!");
    	return;
    }
    DIR *dirpai = pai -> info;
    if(filho -> tipo == 'D'){
        DIR *dirfilho = filho -> info;
        dirfilho -> nomePai = dirpai -> nome;
    }
    else{
        ARQ *arqfilho = filho -> info;
        arqfilho -> nomePai = dirpai -> nome;
    }
	filho -> prox_irmao = pai -> filho;
	pai -> filho = filho;
	atualizaDataHora(raiz, filho);
}

TAN * aloca_arq(char *nome, char tipo){
	TAN *novo = (TAN*) malloc(sizeof(TAN));
	ARQ *aux = (ARQ*) malloc(sizeof(ARQ));
	FILE *arq = criarArquivo(nome,tipo);
	aux -> tamanho = calcularTamanhoArquivo(arq);
	aux -> nome = nome;
	aux -> nomePai="NULL";
	aux -> arqv = arq;
	time_t mytime;
	mytime = time(NULL);
	novo -> dataCriacao = (char*) malloc(30*sizeof(char));
	strcpy(novo -> dataCriacao, ctime(&mytime));
	novo -> ultimaModD = (char*) malloc(30*sizeof(char));
	strcpy(novo -> ultimaModD, ctime(&mytime));
	novo -> info = aux;
	novo -> tipo = tipo;
	novo -> filho = novo -> prox_irmao = NULL;
	return novo;
}

TAN * aloca_dir(char *nome){
	TAN *novo = (TAN*) malloc(sizeof(TAN));
	DIR *aux = (DIR*) malloc(sizeof(DIR));
	aux -> nome = nome;
	aux -> nomePai="NULL";
	novo -> info = aux;
	time_t mytime;
	mytime = time(NULL);
	novo -> dataCriacao = (char*) malloc(30*sizeof(char));
	strcpy(novo -> dataCriacao, ctime(&mytime));
	novo -> ultimaModD = (char*) malloc(30*sizeof(char));
	strcpy(novo -> ultimaModD, ctime(&mytime));
	novo -> tipo = 'D';
	novo -> filho = novo -> prox_irmao = NULL;
	return novo;
}

TAN * buscaArquivoTipo(TAN *a, char *nome, char tipo){
     ARQ * elem=a->info;
     if(strcmp(elem->nome, nome) == 0 && a -> tipo == tipo) return a;
     TAN *p;
     for(p = a -> filho; p; p = p -> prox_irmao){
        TAN *resp = buscaArquivoTipo(p,nome,tipo);
        if(resp) return resp;
     }
     return NULL;
}

TAN * buscaArquivo(TAN *a, char *nome){
     ARQ * elem=a->info;
     if(strcmp(elem->nome, nome) == 0 && a -> tipo != 'D') return a;
     TAN *p;
     for(p = a -> filho; p; p = p -> prox_irmao){
        TAN *resp = buscaArquivo(p,nome);
        if(resp) return resp;
     }
     return NULL;
}

void renomear(TAN *raiz, TAN *a, char *novonome){
    //AO RENOMEAR O N�, DEVE-SE PASSAR O NOVO NOME AOS SEUS FILHOS
     if(a -> tipo == 'D'){
		 DIR *dir = a -> info;
		 dir -> nome = novonome;
		 TAN *pivot = a -> filho;
		 while(pivot){
		 	if(pivot -> tipo == 'D'){
		 		DIR *d = pivot -> info;
		 		d -> nomePai = novonome;
		 	}
		 	else{
		 		ARQ *a = pivot -> info;
		 		a -> nomePai = novonome;
		 	}
		 	pivot = pivot -> prox_irmao;
		 }
     }
     else{
		 ARQ *arq = a -> info;
		 rename(arq -> nome, novonome);
		 arq -> nome = novonome;
     }
     time_t mytime;
	 mytime = time(NULL);
	 char *dc = (char*) malloc(30*sizeof(char));
	 strcpy(dc, ctime(&mytime));
	 a -> ultimaModD = dc;
     atualizaDataHora(raiz, a);
}

void mover(TAN *raiz, TAN *destino, TAN *movido){
    //este metodo coloca o n� movido como filho do n� destino
    //o n� de destino n�o pode ser um arquivo, uma vez que um arquivo nao pode ter filhos
    if(destino -> tipo != 'D'){
    	printf("Impossivel mover para dentro de arquivo!!!\n");
    	return;
    }
    if(movido -> tipo == 'D'){
    	DIR *d = movido -> info;

    	//pai do movido
    	TAN *aux = buscaDiretorio(raiz, d -> nomePai);

    	if(aux){
    		atualizaDataHora(raiz, movido);
    		TAN *pivot = aux -> filho;
    		if(pivot == movido) aux -> filho = pivot -> prox_irmao;
    		else{
    			while(pivot -> prox_irmao != movido){
    				pivot = pivot -> prox_irmao;
    			}
    			pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
    		}

    		movido -> prox_irmao = destino -> filho;
    		destino -> filho = movido;
    		DIR *dest = destino -> info;
    		d -> nomePai = dest -> nome;
    		atualizaDataHora(raiz, movido);
    	}
    } else{
    	ARQ *a = movido -> info;

    	//pai do movido
    	TAN *aux = buscaDiretorio(raiz, a -> nomePai);

    	if(aux){
    		atualizaDataHora(raiz, movido);
    		TAN *pivot = aux -> filho;
    		if(pivot == movido) aux -> filho = pivot -> prox_irmao;
    		else{
    			while(pivot -> prox_irmao != movido){
    				pivot = pivot -> prox_irmao;
    			}
    			pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
    		}
    		movido -> prox_irmao = destino -> filho;
    		destino -> filho = movido;
    		DIR *dest = destino -> info;
    		a -> nomePai = dest -> nome;
    		atualizaDataHora(raiz, movido);
    	}
    }
}

void libera(TAN *a){
	if(a -> prox_irmao)
		libera(a -> prox_irmao);
	if(a -> filho)
		libera(a -> filho);
	if(a -> tipo != 'D'){
		ARQ *aux = a -> info;
		remove(aux -> nome);
		free(aux -> nome);
	} else{
		DIR *d = a -> info;
		free(d -> nome);
	}
	free(a -> dataCriacao);
	free(a -> ultimaModD);
	free(a);
}

void deleta_filhos(TAN *a){
	if(a -> filho) libera(a -> filho);
	a -> filho = NULL;
}

void transformar(TAN *raiz, TAN *obj, char tipo){
	if(obj -> tipo == 'D'){
		deleta_filhos(obj);
		DIR *dir = obj -> info;
		ARQ *arq = (ARQ*) malloc(sizeof(ARQ));
		arq -> nome = (char*) malloc(100*sizeof(char));
		strcpy(arq -> nome, dir -> nome);
		arq -> nomePai = dir -> nomePai;
		free(dir -> nome);
		free(obj -> dataCriacao);
		free(obj -> ultimaModD);
		time_t mytime;
		mytime = time(NULL);
		obj -> dataCriacao = (char*) malloc(30*sizeof(char));
		strcpy(obj -> dataCriacao, ctime(&mytime));
		obj -> ultimaModD = (char*) malloc(30*sizeof(char));
		strcpy(obj -> ultimaModD, ctime(&mytime));
		arq -> arqv = criarArquivo(arq -> nome, tipo);
		arq -> tamanho = calcularTamanhoArquivo(arq -> arqv);
		obj -> info = arq;
		obj -> tipo = tipo;
		free(dir);
	} else{
		ARQ *arq = obj -> info;
		DIR *dir = (DIR*) malloc(sizeof(DIR));
		dir -> nome = (char*) malloc(100*sizeof(char));
		strcpy(dir -> nome, arq -> nome);
		dir -> nomePai = arq -> nomePai;
		remove(arq -> nome);
		free(arq -> nome);
		free(obj -> dataCriacao);
		free(obj -> ultimaModD);
		time_t mytime;
		mytime = time(NULL);
		obj -> dataCriacao = (char*) malloc(30*sizeof(char));
		strcpy(obj -> dataCriacao, ctime(&mytime));
		obj -> ultimaModD = (char*) malloc(30*sizeof(char));
		strcpy(obj -> ultimaModD, ctime(&mytime));
		obj -> info = dir;
		obj -> tipo = tipo;
		free(arq);
	}
	atualizaDataHora(raiz, obj);
}

long calculaPastas(TAN *a){
	TAN *aux = a -> filho;
	long num = 0;
	while(aux){
		if(aux -> tipo == 'D') num++;
		aux = aux -> prox_irmao;
	}
	return num;
}

long calculaArquivos(TAN *a){
	TAN *aux = a -> filho;
	long num = 0;
	while(aux){
		if(aux -> tipo != 'D') num++;
		aux = aux -> prox_irmao;
	}
	return num;
}

void imprime(TAN *a){
	if(a -> tipo == 'D'){
		DIR *dir = a -> info;
		printf("%c/%s/%s/%s",a->tipo, dir -> nome, dir->nomePai, a -> dataCriacao);
	}
	else{
		ARQ *arq = a -> info;
		printf("%c/%s/%s/%ld/%s",a->tipo, arq -> nome,arq->nomePai,arq -> tamanho, a -> dataCriacao);
	}
	TAN *p = a -> filho;
	for(; p; p = p -> prox_irmao)
		imprime(p);
}

void deleta(TAN *raiz, TAN *a){
	if(a){
		atualizaDataHora(raiz, a);
		if(a -> tipo == 'D'){
			deleta_filhos(a);
			DIR *d = a -> info;
			TAN *aux = buscaDiretorio(raiz, d -> nomePai);
			if(aux){
				TAN *pivot = aux -> filho;
				if(pivot == a){
					aux -> filho = pivot -> prox_irmao;
					free(d -> nome);
					free(a -> dataCriacao);
					free(a -> ultimaModD);
					d -> nomePai = NULL;
					free(a);
				} else{
					while(pivot -> prox_irmao != a)
						pivot = pivot -> prox_irmao;
					pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
					free(d -> nome);
					free(a -> dataCriacao);
					free(a -> ultimaModD);
					d -> nomePai = NULL;
					free(a);
				}
			}
		}
		else{
			ARQ *arq = a -> info;
			remove(arq -> nome);

			TAN *aux = buscaDiretorio(raiz, arq -> nomePai);

			if(aux){
				TAN *pivot = aux -> filho;

				if(pivot == a){
					aux -> filho = pivot -> prox_irmao;
					free(arq -> nome);
					free(a -> dataCriacao);
					free(a -> ultimaModD);
					arq -> nomePai = NULL;
					free(a);
				} else{
					while(pivot -> prox_irmao != a)
						pivot = pivot -> prox_irmao;
					pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
					free(arq -> nome);
					free(a -> dataCriacao);
					free(a -> ultimaModD);
					arq -> nomePai = NULL;
					free(a);
				}
			}
		}
	}
}

void exibe_informacoes(TAN *a){
	if(a -> tipo == 'D'){
		DIR *d = a -> info;
		printf("Nome: %s\n", d -> nome);
	} else{
		ARQ *arq = a -> info;
		printf("Nome: %s\n", arq -> nome);
	}
	if(a -> tipo == 'B')
		printf("Tipo: Arquivo Binario\n");
	else if(a -> tipo == 'T')
		printf("Tipo: Arquivo Texto\n");
	else
		printf("Tipo: Diretorio\n");
	printf("Criacao: %s", a -> dataCriacao);
	if(a -> tipo == 'D') printf("Arquivos: %ld   Diretorios: %ld\n", calculaArquivos(a), calculaPastas(a));
	printf("Ultima modificacao: %s", a -> ultimaModD);
}
