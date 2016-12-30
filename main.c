#include <stdio.h>
#include <stdlib.h>
#include "TAN.h"
#include <sys/stat.h>

void exibe_opcoes(){
	printf("1 - Exibe informacoes\t2 - Inserir arquivo");
	printf("\n3 - Inserir diretorio\t4 - Buscar arquivo");
	printf("\n5 - Buscar diretorio\t6 - Excluir");
	printf("\n7 - Renomear\t8 - Mover");
	printf("\n9 - Transformar\t10 - Impressao completa\n");
}

void exibe_selecionados(TAN *dir, TAN *arq){
	if(dir && arq){
		ARQ *a = arq -> info;
		DIR *d = dir -> info;
		printf("Diretorio selecionado: %s\tArquivo selecionado: %s %c\n", d -> nome, a -> nome, arq -> tipo);
	} else if(dir){
		DIR *d = dir -> info;
		printf("Diretorio selecionado: %s\tNenhum arquivo selecionado!\n", d -> nome);
	}
	else{
		ARQ *a = arq -> info;
		printf("Nenhum diretorio selecionado!\tArquivo selecionado: %s %c\n", a -> nome, arq -> tipo);
	}
}

int main(void){
    TAN *RAIZ = aloca_dir("RAIZ");
    int n = 1;
    char *nome = NULL, tipo;
    char choice;
    char cleaner;
    
    TAN *dir_atual = RAIZ;
    TAN *arq_atual = NULL;
    
    printf("\n");
    imprime_mesmo_nivel(RAIZ);
    printf("\n");
    exibe_opcoes();
    exibe_selecionados(dir_atual, arq_atual);
    
    printf("\nOpcao desejada: ");
    scanf("%d", &n);
    
    while(1){
    	switch(n){
			case 1:
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &tipo);
				if(tipo == 'A')
					if(!arq_atual) printf("Nenhum arquivo selecionado!\n");
					else exibe_informacoes(arq_atual);
				else 
					if(!dir_atual) printf("Nenhum diretorio selecionado!\n");
					else exibe_informacoes(dir_atual); 
				break;
			
			case 2:
				printf("Nome do arquivo: ");
				nome = (char*) malloc(sizeof(char));
				scanf(" %s", nome);
				printf("Tipo (B - binario, T - texto): ");
				scanf(" %c", &tipo);
				TAN *arq = aloca_arq(nome, tipo);
				insere(RAIZ, dir_atual, arq);
				break;
				
			case 3:
				printf("Nome do diretorio: ");
				nome = (char*) malloc(sizeof(char));
				scanf(" %s", nome);
				TAN *dir = aloca_dir(nome);
				insere(RAIZ, dir_atual, dir);
				break;
				
			case 4:
				printf("Arquivo para a busca: ");
				nome = (char*) malloc(sizeof(char));
				scanf(" %s", nome);
				TAN *busca = buscaArquivo(RAIZ, nome);
				if(busca){ 
					arq_atual = busca;
					exibe_informacoes(arq_atual);
				} else printf("Arquivo nao encontrado!\n");
				break;
				
			case 5:
				printf("Diretorio para a busca: ");
				nome = (char*) malloc(sizeof(char));
				scanf(" %s", nome);
				TAN *buscaD = buscaDiretorio(RAIZ, nome);
				if(buscaD){ 
					dir_atual = buscaD;
					exibe_informacoes(dir_atual);
				} else printf("Diretorio nao encontrado!\n");
				break;
				
			case 6:
				printf("Arquivo/Pasta a ser deletado: ");
				nome = (char*) malloc(sizeof(char));
				scanf(" %s", nome);
				printf("Tipo (D - diretorio, B - binario, T - texto): ");
				scanf(" %c", &tipo);
				TAN *rmv = NULL;
				if(tipo == 'D'){
					rmv = buscaDiretorio(RAIZ, nome);
					if(rmv){
						if(rmv == RAIZ){
							printf("Impossivel remover o diretorio RAIZ!\n");
							break;
						}
						printf("Ao excluir um diretorio voce perdera todos os arquivos\nnele contidos, tem certeza? (s/n) ");
						char resp;
						scanf(" %c", &resp);
						if(resp == 's'){
							if(rmv == dir_atual) dir_atual = NULL;
							deleta(RAIZ, rmv);
						}
					} else printf("Diretorio nao encontrado!\n");
				} else{
					rmv = buscaArquivo(RAIZ, nome);
					if(rmv){
						if(rmv == arq_atual) arq_atual = NULL;
						deleta(RAIZ, rmv);
					}
					else printf("Arquivo nao encontrado!\n");
				}
				break;
			
			case 7:
				printf("Arquivo atual ou diretorio atual? (A/D) ");
				scanf(" %c", &choice);
				if(!arq_atual && choice != 'D'){
					printf("Nenhum arquivo selecionado!\n");
					break;
				}
				if(dir_atual == RAIZ && choice == 'D'){
					printf("Impossivel fazer alteracoes no diretorio RAIZ!!!\n");
					break;
				}
				if(!dir_atual && choice == 'D'){
					printf("Nenhum diretorio selecionado!\n");
					break;
				}
				printf("Novo nome: ");
				nome = (char*) malloc(sizeof(char));
				scanf(" %s", nome);
				if(choice == 'A'){
					if(arq_atual){
						renomear(RAIZ, arq_atual, nome);
						exibe_informacoes(arq_atual);
					}
				} else{
					renomear(RAIZ, dir_atual, nome);
					exibe_informacoes(dir_atual);
				}
				break;

			case 8:
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &choice);
				if(choice == 'A'){
					if(!arq_atual){
						printf("Nenhum arquivo selecionado!\n");
					} else{
						printf("Diretorio destino: ");
						nome = (char*) malloc(sizeof(char));
						scanf(" %s", nome);
						TAN *dest = buscaDiretorio(RAIZ, nome);
						if(!dest) printf("Diretorio nao encontrado!\n");
						else{
							mover(RAIZ, dest, arq_atual);
							imprime(dest);
							printf("Movido com sucesso!\n");
						}
					}
				} else{
					if(!dir_atual){
						printf("Nenhum diretorio selecionado!\n");
					} else if(dir_atual == RAIZ){
						printf("Impossivel fazer operacoes sobre a raiz!\n");
					} else{
						printf("Diretorio destino: ");
						nome = (char*) malloc(sizeof(char));
						scanf(" %s", nome);
						TAN *dest = buscaDiretorio(RAIZ, nome);
						if(!dest) printf("Diretorio nao encontrado!\n");
						else{
							mover(RAIZ, dest, arq_atual);
							imprime(dest);
							printf("Movido com sucesso!\n");
						}
					}
				}
				break;
				
			case 9:
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &choice);
				if(choice == 'A'){
					if(!arq_atual){
						printf("Nenhum arquivo selecionado!\n");
					} else{
						transformar(RAIZ, arq_atual, 'D');
						exibe_informacoes(arq_atual);
						arq_atual = NULL;
					}
				} else{
					if(!dir_atual){
						printf("Nenhum diretorio selecionado!\n");
					} else if(dir_atual == RAIZ){
						printf("Impossivel fazer alteracoes no diretorio raiz!\n");
						break;
					} else{
						printf("Você perdera todos os arquivos e diretorios contidos,\ntem certeza? (s/n) ");
						scanf(" %c", &choice);
						if(choice == 's'){
							printf("Tipo (B - binario, T - texto): ");
							scanf(" %c", &tipo);
							transformar(RAIZ, dir_atual, tipo);
							exibe_informacoes(dir_atual);
							dir_atual = NULL;
						}
					}
				}
				break;
			
			case 10:
				imprime(RAIZ);
				break;
				
			case 13:
				exibe_opcoes();
				break;
    	}
    	
    	if(!n) break;
    	
    	exibe_selecionados(dir_atual, arq_atual);
    	printf("\nOpcao desejada (13 para exibir opcoes): ");
    	scanf("%d", &n);
    }
    
    TAN *del = RAIZ -> filho;
    while(del){
    	deleta(RAIZ, del);
    	del = del -> prox_irmao;
    }
    //dar free nas outras coisas
    //terminar de rever o mover
    
    return 0;
}
