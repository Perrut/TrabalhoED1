#include <stdio.h>
#include <stdlib.h>

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

FILE * criarArquivo(char*nome, char tipo){
	FILE *arq;
	if(tipo=='T'){
		arq=fopen(nome,"wt");
		//AUMENTAR O TAMANHO DO ARRAY DE CONTEUDO
		char conteudo[500];
		printf("Insira o conteudo do arquivo: \n");
		//le a string do usuario e limpa o buffer para poder receber string com espaço entre as palavras
		scanf("%[^\n]s", conteudo);
		setbuf(stdin, NULL);
		fprintf(arq, conteudo);
		return arq;
	}
	else{
		arq=fopen(nome,"wb");
		char conteudo[500];
		printf("Insira o conteudo do arquivo: \n");
		//le a string do usuario e limpa o buffer para poder receber string com espaço entre as palavras
		scanf("%[^\n]s", conteudo);
		setbuf(stdin, NULL);
		//FAZER A ESCRITA DO ARQUIVO BINARIO USANDO FPRINT MESMO?
		fprintf(arq, conteudo);
		return arq;
	}
}

void insere(TAN*pai, TAN*filho){
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
}

TAN * aloca_arq(char *nome, char tipo){
	TAN *novo = (TAN*) malloc(sizeof(TAN));
	ARQ *aux = (ARQ*) malloc(sizeof(ARQ));
	FILE *arq = criarArquivo(nome,tipo);
	aux -> tamanho = calcularTamanhoArquivo(arq);
	aux -> nome = nome;
	aux -> nomePai="NULL";
	aux -> arqv = arq;
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
	novo -> tipo = 'D';
	novo -> filho = novo -> prox_irmao = NULL;
	return novo;
}

TAN * buscaArquivo(TAN *a, char *nome){
     ARQ * elem=a->info;
     
     if(elem->nome==nome) return a;
     
     TAN *p;
     for(p = a -> filho; p; p = p -> prox_irmao){
        TAN *resp = buscaArquivo(p,nome);
        if(resp) return resp;
     }
     return NULL;
}

TAN * buscaDiretorio(TAN *a, char *nome){
     DIR *elem = a->info;
     if(elem -> nome == nome) return a;
     TAN *p;
     for(p = a -> filho; p; p = p->prox_irmao){
        TAN *resp = buscaDiretorio(p,nome);
        if(resp) return resp;
     }
     return NULL;
}

// TAN * buscaElemento

void renomear(TAN *a, char *novonome){
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
    	}
    } else{
    	ARQ *a = movido -> info;
    	
    	//pai do movido
    	TAN *aux = buscaDiretorio(raiz, a -> nomePai);
    	
    	if(aux){
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
	}
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
}

void imprime(TAN *a){
	if(a -> tipo == 'D'){
		DIR *dir = a -> info;
		printf("%c/%s/%s/ ",a->tipo, dir -> nome, dir->nomePai);
		printf("%s/%s\n",__DATE__,__TIME__);
	}
	else{
		ARQ *arq = a -> info;
		printf("%c/%s/%s/%ld/",a->tipo, arq -> nome,arq->nomePai,arq -> tamanho);
		printf("%s/%s\n",__DATE__,__TIME__);
	}
	TAN *p = a -> filho;
	for(; p; p = p -> prox_irmao)
		imprime(p);
}

void deleta(TAN *raiz, TAN *a){
	if(a){
		if(a -> tipo == 'D'){
			
			deleta_filhos(a);
			
			DIR *d = a -> info;
			TAN *aux = buscaDiretorio(raiz, d -> nomePai);
		
			if(aux){
				TAN *pivot = aux -> filho;
			
				if(pivot == a){
					aux -> filho = pivot -> prox_irmao;
					free(a);
				} else{
					while(pivot -> prox_irmao != a)
						pivot = pivot -> prox_irmao;
					pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
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
					free(a);
				} else{
					while(pivot -> prox_irmao != a)
						pivot = pivot -> prox_irmao;
					pivot -> prox_irmao = pivot -> prox_irmao -> prox_irmao;
					free(a);
				}
			}
		}
	}
}
