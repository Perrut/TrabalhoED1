#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//MODIFICAÇÕES NA STRUCT DIRETORIO
//MODIFICAÇÃO NA LEITURA DA ENTRADA PADRÃO PARA INSERÇÃO NO ARQUIVO
//MODIFICAÇÃO EM INSERIR
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
	char *horaCriacao;
	char *ultimaModD;
	char *ultimaModH;
	struct arvore *filho;
	struct arvore *prox_irmao;
} TAN;

int calcularTamanhoArquivo(FILE *arquivo) {
    // guarda o estado antes de chamar a função fseek
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

char* ler_texto() {
    //permite que o usuario insira uma entrada de tamanho qualquer
    unsigned int tam_max = 128;
    unsigned int tam_atual = 0;
    char *texto = malloc(tam_max);
    tam_atual = tam_max;
    if(texto != NULL) {
        int c = 0;
        unsigned int i = 0;
        //aceitar a entrada do usuario ate que aperte enter ou fim de arquivo
        while ((c = getchar()) != '\n') {
            texto[i++] = (char) c;
            //se eu atingir o tamanho maximizar e depois realloc
            if(i == tam_atual) {
                tam_atual = i+tam_max;
                texto = realloc(texto, tam_atual);
            }
        }
        texto[i] = '\0';
    }
    return texto;
}

FILE * criarArquivo(char*nome, char tipo){
	FILE *arq;
	char conteudo[1000];
	if(tipo=='T'){
		arq=fopen(nome,"wt");
		printf("Insira o conteudo do arquivo: \n");
		scanf("%s", conteudo);
        //char*conteudo=ler_texto();
		fprintf(arq, "%s", conteudo);
		return arq;
	}
	else{
		arq=fopen(nome,"wb");
		printf("Insira o conteudo do arquivo: \n");
		scanf("%s", conteudo);
        //char*conteudo=ler_texto();
		fprintf(arq, "%s", conteudo);
		return arq;
	}
}

TAN * buscaDiretorio(TAN *a, char *nome){
     DIR *elem = a->info;
     if(strcmp(elem -> nome, nome) == 0) return a;
     TAN *p;
     for(p = a -> filho; p; p = p->prox_irmao){
        TAN *resp = buscaDiretorio(p,nome);
        if(resp) return resp;
     }
     return NULL;
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
				char *hc = (char*) malloc(30*sizeof(char));
				strcpy(hc, ctime(&mytime));
				aux2 -> ultimaModH = hc;
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
				char *hc = (char*) malloc(30*sizeof(char));
				strcpy(hc, ctime(&mytime));
				aux2 -> ultimaModH = hc;
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
        //int aux=dirpai->diretorioscontidos;
        //dirpai->diretorioscontidos=aux+1;
    }
    else{
        ARQ *arqfilho = filho -> info;
        arqfilho -> nomePai = dirpai -> nome;
        //int aux=dirpai->arquivoscontidos;
        //dirpai->arquivoscontidos=aux+1;
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
	novo -> horaCriacao = (char*) malloc(30*sizeof(char));
	strcpy(novo -> horaCriacao, ctime(&mytime));
	novo -> dataCriacao = (char*) malloc(30*sizeof(char));
	strcpy(novo -> dataCriacao, ctime(&mytime));
	novo -> ultimaModD = (char*) malloc(30*sizeof(char));
	strcpy(novo -> ultimaModD, ctime(&mytime));
	novo -> ultimaModH = (char*) malloc(30*sizeof(char));
	strcpy(novo -> ultimaModH, ctime(&mytime));
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
	novo -> horaCriacao = (char*) malloc(30*sizeof(char));
	strcpy(novo -> horaCriacao, ctime(&mytime));
	novo -> dataCriacao = (char*) malloc(30*sizeof(char));
	strcpy(novo -> dataCriacao, ctime(&mytime));
	novo -> ultimaModD = (char*) malloc(30*sizeof(char));
	strcpy(novo -> ultimaModD, ctime(&mytime));
	novo -> ultimaModH = (char*) malloc(30*sizeof(char));
	strcpy(novo -> ultimaModH, ctime(&mytime));
	novo -> tipo = 'D';
	novo -> filho = novo -> prox_irmao = NULL;
	return novo;
}

TAN * buscaArquivo(TAN *a, char *nome){
     ARQ * elem=a->info;
     if(strcmp(elem->nome, nome) == 0) return a;
     TAN *p;
     for(p = a -> filho; p; p = p -> prox_irmao){
        TAN *resp = buscaArquivo(p,nome);
        if(resp) return resp;
     }
     return NULL;
}

void renomear(TAN *raiz, TAN *a, char *novonome){
    //AO RENOMEAR O NÓ, DEVE-SE PASSAR O NOVO NOME AOS SEUS FILHOS
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
	 char *hc = (char*) malloc(30*sizeof(char));
	 strcpy(hc, ctime(&mytime));
	 a -> ultimaModH = hc;
	 char *dc = (char*) malloc(30*sizeof(char));
	 strcpy(dc, ctime(&mytime));
	 a -> ultimaModD = dc;
     atualizaDataHora(raiz, a);
}

void mover(TAN *raiz, TAN *destino, TAN *movido){
    //este metodo coloca o nó movido como filho do nó destino
    //o nó de destino não pode ser um arquivo, uma vez que um arquivo nao pode ter filhos
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
	free(a -> horaCriacao);
	free(a -> ultimaModD);
	free(a -> ultimaModH);
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
		arq -> nome = dir -> nome;
		arq -> nomePai = dir -> nomePai;
		arq -> arqv = criarArquivo(arq -> nome, tipo);
		arq -> tamanho = calcularTamanhoArquivo(arq -> arqv);
		obj -> info = arq;
		obj -> tipo = tipo;
		free(dir);
	} else{
		ARQ *arq = obj -> info;
		DIR *dir = (DIR*) malloc(sizeof(DIR));
		dir -> nome = arq -> nome;
		dir -> nomePai = arq -> nomePai;
		remove(arq -> nome);
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
		printf("%c/%s/%s/%s/%s\n",a->tipo, dir -> nome, dir->nomePai, a -> dataCriacao, a -> horaCriacao);
	}
	else{
		ARQ *arq = a -> info;
		printf("%c/%s/%s/%ld/%s/%s\n",a->tipo, arq -> nome,arq->nomePai,arq -> tamanho, a -> dataCriacao, a -> horaCriacao);
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
					free(a -> horaCriacao);
					free(a -> ultimaModD);
					free(a -> ultimaModH);
					free(a);
				} else{
					while(pivot -> prox_irmao != a)
						pivot = pivot -> prox_irmao;
					pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
					free(d -> nome);
					free(a -> dataCriacao);
					free(a -> horaCriacao);
					free(a -> ultimaModD);
					free(a -> ultimaModH);
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
					free(a -> horaCriacao);
					free(a -> ultimaModD);
					free(a -> ultimaModH);
					free(a);
				} else{
					while(pivot -> prox_irmao != a)
						pivot = pivot -> prox_irmao;
					pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
					free(arq -> nome);
					free(a -> dataCriacao);
					free(a -> horaCriacao);
					free(a -> ultimaModD);
					free(a -> ultimaModH);
					free(a);
				}
			}
		}
	}
}

void imprime_mesmo_nivel(TAN *a){
	if(a -> tipo == 'D'){
		DIR *dir = a -> info;
		if(strcmp(dir -> nomePai, "NULL") == 0){
			printf("%c/%s/%s/%s/%s\n",a->tipo, dir -> nome, dir->nomePai, a -> dataCriacao, a -> horaCriacao);
		}
	}
}

void teste(TAN *a){
	TAN *aux = a -> filho;
	while(aux){
		ARQ *ar = aux -> info;
		printf("%s\n", ar -> nome);
		aux = aux -> prox_irmao;
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
	printf("Criacao: %s %s\n", a -> dataCriacao, a -> horaCriacao);
	if(a -> tipo == 'D') printf("Arquivos: %ld   Diretorios: %ld\n", calculaArquivos(a), calculaPastas(a));
	printf("Ultima modificacao: %s %s\n", a -> ultimaModD, a -> ultimaModH);
}
