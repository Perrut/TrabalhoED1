#include <stdio.h>
#include <stdlib.h>
#include "TAN.h"
#include <sys/stat.h>

void exibe_opcoes(){
	printf("1 - Exibe informacoes\t2 - Inserir arquivo\n3 - Inserir diretorio\t4 - Buscar arquivo\n5 - Buscar diretorio\t6 - Excluir\n7 - Renomear\t8 - Mover\n9 - Transformar\t10 - Impressao completa\n");
}

void exibe_selecionados(TAN *dir, TAN *arq){
	if(!dir && !arq){
		printf("Nenhum diretorio selecionado \tNenhum arquivo selecionado\n");
	} else if(dir && arq){
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
    TAN *busca = NULL;
    int n = 1;
    char *nome = NULL, tipo;
    char choice;

    TAN *dir_atual = RAIZ;
    TAN *arq_atual = NULL;

    printf("\n");
    imprime(RAIZ);
    printf("\n");
    exibe_opcoes();
    printf("\n");
    exibe_selecionados(dir_atual, arq_atual);
    printf("Opcao desejada  (11 para exibir opcoes, 0 para sair): ");
    scanf("%d", &n);

    while(1){
    	switch(n){
			case 1:
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &tipo);
				while(tipo != 'A' && tipo != 'D'){
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &tipo);
				}
				if(tipo == 'A')
					if(!arq_atual) printf("Nenhum arquivo selecionado!\n");
					else exibe_informacoes(arq_atual);
				else
					if(!dir_atual) printf("Nenhum diretorio selecionado!\n");
					else exibe_informacoes(dir_atual);
				break;

			case 2:
				printf("Nome do arquivo: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				printf("Tipo (B - binario, T - texto): ");
				scanf(" %c", &tipo);
				while(tipo != 'B' && tipo != 'T'){
					printf("Entrada invalida!\n");
					printf("Tipo (B - binario, T - texto): ");
					scanf(" %c", &tipo);
				}
				busca = buscaArquivo(RAIZ, nome);
				if(busca){
					printf("Ja existe um arquivo com o mesmo nome!\n");
					break;
				}
				TAN *arq = aloca_arq(nome, tipo);
				insere(RAIZ, dir_atual, arq);
				arq_atual = arq;
				exibe_informacoes(arq);
				break;

			case 3:
				printf("Nome do diretorio: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				busca = buscaDiretorio(RAIZ, nome);
				if(busca){
					printf("Ja existe um diretorio com o mesmo nome!\n");
					break;
				}
				TAN *dir = aloca_dir(nome);
				insere(RAIZ, dir_atual, dir);
				dir_atual = dir;
				exibe_informacoes(dir);
				break;

			case 4:
				printf("Arquivo para a busca: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				TAN *busca = buscaArquivo(RAIZ, nome);
				if(busca){
					arq_atual = busca;
					exibe_informacoes(arq_atual);
				} else printf("Arquivo nao encontrado!\n");
				break;

			case 5:
				printf("Diretorio para a busca: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				TAN *buscaD = buscaDiretorio(RAIZ, nome);
				if(buscaD){
					dir_atual = buscaD;
					exibe_informacoes(dir_atual);
				} else printf("Diretorio nao encontrado!\n");
				break;

			case 6:
				printf("Arquivo/Pasta a ser deletado: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				printf("Tipo (D - diretorio, B - binario, T - texto): ");
				scanf(" %c", &tipo);
				while(tipo != 'D' && tipo != 'B' && tipo != 'T'){
					printf("Entrada invalida!\n");
					printf("Tipo (D - diretorio, B - binario, T - texto): ");
					scanf(" %c", &tipo);
				}
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
						while(resp != 's' && resp != 'n'){
							printf("Entrada invalida!\n");
							printf("Ao excluir um diretorio voce perdera todos os arquivos\nnele contidos, tem certeza? (s/n) ");
							scanf(" %c", &resp);
						}
						if(resp == 's'){
							if(e_filho(rmv, dir_atual) || rmv == dir_atual) dir_atual = NULL;
							if(arq_atual){
								ARQ *f = arq_atual -> info;
								if(buscaArquivo(rmv, f -> nome)) arq_atual = NULL;
							}
							deleta(RAIZ, rmv);
							printf("Diretorio removido!\n");
						}
					} else printf("Diretorio nao encontrado!\n");
				} else{
					rmv = buscaArquivoTipo(RAIZ, nome, tipo);
					if(rmv){
						if(rmv == arq_atual) arq_atual = NULL;
						deleta(RAIZ, rmv);
						printf("Arquivo removido!\n");
					}
					else printf("Arquivo nao encontrado!\n");
				}
				break;

			case 7:
				printf("Arquivo atual ou diretorio atual? (A/D) ");
				scanf(" %c", &choice);
				while(choice != 'A' && choice != 'D'){
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &choice);
				}
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
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				if(choice == 'A'){
					if(arq_atual){
						busca = buscaArquivo(RAIZ, nome);
						if(busca){
							printf("Ja existe um arquivo com o mesmo nome!\n");
							break;
						}
						renomear(RAIZ, arq_atual, nome);
						exibe_informacoes(arq_atual);
					}
				} else{
					busca = buscaDiretorio(RAIZ, nome);
					if(busca){
						printf("Ja existe um diretorio com o mesmo nome!\n");
						break;
					}
					renomear(RAIZ, dir_atual, nome);
					exibe_informacoes(dir_atual);
				}
				break;

			case 8:
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &choice);
				while(choice != 'A' && choice != 'D'){
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &choice);
				}
				if(choice == 'A'){
					if(!arq_atual){
						printf("Nenhum arquivo selecionado!\n");
					} else{
						printf("Diretorio destino: ");
						nome = (char*) malloc(100*sizeof(char));
						scanf(" %[^\n]s", nome);
						TAN *dest = buscaDiretorio(RAIZ, nome);
						if(!dest) printf("Diretorio nao encontrado!\n");
						else{
							//pai do movido
							ARQ *confere_dir = arq_atual -> info;
							TAN *confere_pai = buscaDiretorio(RAIZ, confere_dir -> nomePai);
							if(confere_pai){
								confere_dir = dest -> info;
								DIR *pai = confere_pai -> info;
								if(strcmp(confere_dir -> nome, pai -> nome) == 0){
									printf("O diretorio ja se encontra no local de destino!\n");
									break;
								}
							}
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
						nome = (char*) malloc(100*sizeof(char));
						scanf(" %[^\n]s", nome);
						TAN *dest = buscaDiretorio(RAIZ, nome);
						if(!dest) printf("Diretorio nao encontrado!\n");
						else{
							//pai do movido
							DIR *confere_dir = dir_atual -> info;
							TAN *confere_pai = buscaDiretorio(RAIZ, confere_dir -> nomePai);
							if(confere_pai){
								confere_dir = dest -> info;
								DIR *pai = confere_pai -> info;
								if(strcmp(confere_dir -> nome, pai -> nome) == 0){
									printf("O diretorio ja se encontra no local de destino!\n");
									break;
								}
								if(dest == dir_atual){
									printf("Diretorio destino e o mesmo que o diretorio atual!\n");
									break;
								}
								if(e_filho(dir_atual, dest)){
									printf("Impossivel mover para dentro do proprio diretorio!\n");
									break;
								}
							}
							mover(RAIZ, dest, dir_atual);
							imprime(dest);
							printf("Movido com sucesso!\n");
						}
					}
				}
				break;

			case 9:
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &choice);
				while(choice != 'A' && choice != 'D'){
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &choice);
				}
				if(choice == 'A'){
					if(!arq_atual){
						printf("Nenhum arquivo selecionado!\n");
					} else{
						ARQ *ar = arq_atual -> info;
						busca = buscaDiretorio(RAIZ, ar -> nome);
						if(busca){
							printf("Ja existe um diretorio com o mesmo nome!\n");
							break;
						}
						transformar(RAIZ, arq_atual, 'D');
						exibe_informacoes(arq_atual);
						dir_atual = arq_atual;
						arq_atual = NULL;
						printf("Transformado com sucesso!\n");
					}
				} else{
					if(!dir_atual){
						printf("Nenhum diretorio selecionado!\n");
					} else if(dir_atual == RAIZ){
						printf("Impossivel fazer alteracoes no diretorio raiz!\n");
						break;
					} else{
						printf("Voce perdera todos os arquivos e diretorios contidos,\ntem certeza? (s/n) ");
						scanf(" %c", &choice);
						while(choice != 's' && choice != 'n'){
							printf("Entrada invalida!\n");
							printf("Voce perdera todos os arquivos e diretorios contidos,\ntem certeza? (s/n) ");
							scanf(" %c", &choice);
						}
						if(choice == 's'){
							printf("Tipo (B - binario, T - texto): ");
							scanf(" %c", &tipo);
							DIR *dr = dir_atual -> info;
							busca = buscaArquivo(RAIZ, dr -> nome);
							if(busca){
								printf("Ja existe um arquivo com o mesmo nome!\n");
								break;
							}
							transformar(RAIZ, dir_atual, tipo);
							exibe_informacoes(dir_atual);
							arq_atual = dir_atual;
							dir_atual = NULL;
							printf("Transformado com sucesso!\n");
						}
					}
				}
				break;

			case 10:
				if(dir_atual) imprime(dir_atual);
				else imprime(RAIZ);
				break;

			case 11:
				exibe_opcoes();
				break;
    	}

    	if(!n) break;
    	printf("\n");
    	exibe_selecionados(dir_atual, arq_atual);
    	printf("Opcao desejada (11 para exibir opcoes, 0 para sair): ");
    	scanf("%d", &n);
    }

    TAN *del = RAIZ -> filho;
    DIR *exc = RAIZ -> info;
    while(del){
    	deleta(RAIZ, del);
    	del = del -> prox_irmao;
    }
	free(RAIZ);
	free(nome);

    return 0;
}
