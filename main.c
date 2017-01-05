#include <stdio.h>
#include <stdlib.h>
#include "TAN.h"
#include <sys/stat.h>

void exibe_opcoes(){
    //painel de opções que serão desviadas para o fluxo do switch
	printf("1 - Exibe informacoes\t2 - Inserir arquivo\n3 - Inserir diretorio\t4 - Buscar arquivo\n5 - Buscar diretorio\t6 - Excluir\n7 - Renomear\t8 - Mover\n9 - Transformar\t10 - Impressao completa\n");
}

void exibe_selecionados(TAN *dir, TAN *arq){
    //este metodo printa na tela o arquivo e o diretorio selecionados pelo usuario atualmente
    //o usuario escolhe um arquivo e um diretorio para seleção
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

int main(int argv, char **argc){
    if(argv < 2){
		printf("Arquivo nao informado!\n");
		return 0;
	}
	FILE *a = fopen(argc[1], "r");
	
	TAN *RAIZ = NULL;
	
	char tipo;
	char campo1[100];//nome
	char campo2[100];//nomePai
	char campo3[100];//tamanho(B ou T) OU data(D)
	char campo4[100];//data(B ou T) OU hora(D)
	char campo5[100];//hora(B ou T)
	
	char c = ' ';
	int atual = 0;
	int cursor = 0;
	while(fscanf(a, "%c", &c) != EOF){
		if(c == '\n'){
			if(!atual)break;
			if(tipo == 'D'){
				campo4[cursor] = '\0';
				if(strcmp(campo2, "NULL") == 0){
					RAIZ = aloca_dir_texto(tipo, campo1, campo2, campo3, campo4);	
				}else{
					TAN *aux = buscaDiretorio(RAIZ, campo2);
					if(aux){
						TAN *aux2 = aloca_dir_texto(tipo, campo1, campo2, campo3, campo4);
						insere(RAIZ, aux, aux2);
						aux2 = NULL;
					}
					aux = NULL;
				}
			} else{
				TAN *aux = buscaDiretorio(RAIZ, campo2);
				if(aux){
					TAN *aux2 = aloca_arq_texto(tipo, campo1, campo2, campo3, campo4, campo5);
					insere(RAIZ, aux, aux2);
					aux2 = NULL;
				}
				aux = NULL;
			}
			campo5[cursor] = '\0';
			atual = 0;
			cursor = 0;
		}
		else if(c == '/'){
			atual++;
			if(atual == 2){
				campo1[cursor] = '\0';
			}
			else if(atual == 3){
				campo2[cursor] = '\0';
			}
			else if(atual == 4){
				campo3[cursor] = '\0';
			}
			else if(atual == 5){
				campo4[cursor] = '\0';
			}
			else if(atual == 6){
				campo5[cursor] = '\0';
			}
			cursor = 0;
		}
		else if(atual == 0){
			tipo = c;
		}
		else if(atual == 1){
			campo1[cursor] = c;
			cursor++;
		}
		else if(atual == 2){
			campo2[cursor] = c;
			cursor++;
		}
		else if(atual == 3){
			campo3[cursor] = c;
			cursor++;
		}
		else if(atual == 4){
			campo4[cursor] = c;
			cursor++;
		}
		else if(atual == 5){
			campo5[cursor] = c;
			cursor++;
		}
		c = ' ';
	}
	
	fclose(a);
    
    //o diretorio atual começa como RAIZ, que nunca pode ser modificada
    //o arquivo atual começa como nulo, uma vez que nao foi criado ainda
    TAN *busca = NULL;
    int n = 1;
    char *nome = NULL;
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
        //executa um loop de switch e 10 casos enquanto o usuario nao selecionar 0 como opção
    	switch(n){
			case 1:
			    //exibir informação do elemento  selecionado
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &tipo);
                //perguntar para o usuario qual dos elementos atuais, arquivo ou diretorio
                //terá seu suas informações exibidas
				while(tipo != 'A' && tipo != 'D'){
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &tipo);
				}
				if(tipo == 'A')
                    //selecionando arquivo
					if(!arq_atual) printf("Nenhum arquivo selecionado!\n");
					else exibe_informacoes(arq_atual);
				else
				//selecionando diretorio
					if(!dir_atual) printf("Nenhum diretorio selecionado!\n");
					else exibe_informacoes(dir_atual);
				break;

			case 2:
			    //inserir arquivo
				printf("Nome do arquivo: ");
				//definindo e capturando o nome do arquivo
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				printf("Tipo (B - binario, T - texto): ");
				//captura o tipo de arquivo selecionado pelo usuario
				scanf(" %c", &tipo);
				while(tipo != 'B' && tipo != 'T'){
                    //se nao selecionar corretamente entra em um loop
                    //até selecionar uma opção válida
					printf("Entrada invalida!\n");
					printf("Tipo (B - binario, T - texto): ");
					scanf(" %c", &tipo);
				}
				busca = buscaArquivo(RAIZ, nome);
				if(busca){
                    //verificação de arquivos com mesmo nome
					printf("Ja existe um arquivo com o mesmo nome!\n");
					break;
				}
				//fazendo a inserção e atualizado o arquivo atual para o arquivo recém adicionado
				TAN *arq = aloca_arq(nome, tipo);
				if(dir_atual) insere(RAIZ, dir_atual, arq);
				else insere(RAIZ, RAIZ, arq);
				arq_atual = arq;
				exibe_informacoes(arq);
				break;

			case 3:
			    //inserir diretorio
				printf("Nome do diretorio: ");
				//definindo e capturando o nome do diretorio
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				busca = buscaDiretorio(RAIZ, nome);
				if(busca){
                    //verificação de diretorio com mesmo nome
					printf("Ja existe um diretorio com o mesmo nome!\n");
					break;
				}
				//fazendo a inserção e atualizado o diretorio atual para o diretorio recém adicionado
				TAN *dir = aloca_dir(nome);
				if(dir_atual) insere(RAIZ, dir_atual, dir);
				else insere(RAIZ, RAIZ, dir);
				dir_atual = dir;
				exibe_informacoes(dir);
				break;

			case 4:
			    //buscar arquivo
				printf("Arquivo para a busca: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				//seleciona e captura o nome de arquivo para busca
				TAN *busca = buscaArquivo(RAIZ, nome);
				if(busca){
					arq_atual = busca;
					//ao encontrar o arquivo a ser buscado, exibe as informações dele
					exibe_informacoes(arq_atual);
				} else printf("Arquivo nao encontrado!\n");
				break;

			case 5:
			    //buscar diretorio
				printf("Diretorio para a busca: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				//seleciona e captura o nome de diretorio para busca
				TAN *buscaD = buscaDiretorio(RAIZ, nome);
				if(buscaD){
					dir_atual = buscaD;
					//ao encontrar o diretorio a ser buscado, exibe as informações dele
					exibe_informacoes(dir_atual);
				} else printf("Diretorio nao encontrado!\n");
				break;

			case 6:
			    //excluir
				printf("Arquivo/Pasta a ser deletado: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				//seleciona e captura o nome do arquivo ou diretorio a ser deletado
				printf("Tipo (D - diretorio, B - binario, T - texto): ");
				scanf(" %c", &tipo);
				//seleciona um diretorio, arquivo binario ou arquvo texto
				while(tipo != 'D' && tipo != 'B' && tipo != 'T'){
                    //fica em loop até receber uma opção válida
					printf("Entrada invalida!\n");
					printf("Tipo (D - diretorio, B - binario, T - texto): ");
					scanf(" %c", &tipo);
				}
				TAN *rmv = NULL;
				if(tipo == 'D'){
                    //diretorio foi selecionado para remoção
					rmv = buscaDiretorio(RAIZ, nome);
                    //busca o diretorio a ser removido
					if(rmv){
					    //se encotrar o diretorio
						if(rmv == RAIZ){
							printf("Impossivel remover o diretorio RAIZ!\n");
							break;
						}
						printf("Ao excluir um diretorio voce perdera todos os arquivos\nnele contidos, tem certeza? (s/n) ");
						char resp;
						scanf(" %c", &resp);
						//captura uma resposta do usuario
						while(resp != 's' && resp != 'n'){
                            //mantem no loop enquanto nao recebe uma entrada válida
							printf("Entrada invalida!\n");
							printf("Ao excluir um diretorio voce perdera todos os arquivos\nnele contidos, tem certeza? (s/n) ");
							scanf(" %c", &resp);
						}
						if(resp == 's'){
							if(e_filho(rmv, dir_atual) || rmv == dir_atual) dir_atual = NULL;
                            //se o diretorio a ser removido é o atual, a seleção de diretorio será NULL
							if(arq_atual){
								ARQ *f = arq_atual -> info;
								if(buscaArquivo(rmv, f -> nome)) arq_atual = NULL;
								//limpando a seleção de arquivo atual
							}
							deleta(RAIZ, rmv);
							printf("Diretorio removido!\n");
						}//nao encontrado o diretorio printa esta mensagem
					} else printf("Diretorio nao encontrado!\n");
				} else{
				    //removendo um arquivo
					rmv = buscaArquivoTipo(RAIZ, nome, tipo);
					if(rmv){
						if(rmv == arq_atual) arq_atual = NULL;
						//se o arquivo a ser removido é o atual, o arquivo selecionado será NULL
						deleta(RAIZ, rmv);
						printf("Arquivo removido!\n");
					}
					else printf("Arquivo nao encontrado!\n");
				}
				break;

			case 7:
			    //renomear
				printf("Arquivo atual ou diretorio atual? (A/D) ");
				scanf(" %c", &choice);
				while(choice != 'A' && choice != 'D'){
                    //permanece no loop enquanto nao recebe uma opção valida
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &choice);
				}
				if(!arq_atual && choice != 'D'){
				    //se o arquivo selecionado for nulo
					printf("Nenhum arquivo selecionado!\n");
					break;
				}
				if(dir_atual == RAIZ && choice == 'D'){
				    //tentando renomear raiz
					printf("Impossivel fazer alteracoes no diretorio RAIZ!!!\n");
					break;
				}
				if(!dir_atual && choice == 'D'){
				    //diretorio selecionado é nulo
					printf("Nenhum diretorio selecionado!\n");
					break;
				}
				printf("Novo nome: ");
				nome = (char*) malloc(100*sizeof(char));
				scanf(" %[^\n]s", nome);
				//selecionando e capturando um novo nome
				if(choice == 'A'){
					if(arq_atual){
						busca = buscaArquivo(RAIZ, nome);
						if(busca){
						    //se ja houver um arquivo com mesmo nome, nao faz a alteração
							printf("Ja existe um arquivo com o mesmo nome!\n");
							break;
						}
						//caso busca retorne nulo (nao há um arquivo com mesmo nome)
						//a operação de renomear é realizada
						renomear(RAIZ, arq_atual, nome);
						exibe_informacoes(arq_atual);
					}
				} else{
				    //as mesmas operações sao realizadas para o caso de Diretorio
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
			    //mover
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &choice);
				while(choice != 'A' && choice != 'D'){
                    //permanece no loop enquanto a opção nao for válida
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &choice);
				}
				if(choice == 'A'){
				    //escolhendo um arquivo
					if(!arq_atual){
					    //se o arquivo selecionado for nulo
						printf("Nenhum arquivo selecionado!\n");
					} else{
						printf("Diretorio destino: ");
						nome = (char*) malloc(100*sizeof(char));
						scanf(" %[^\n]s", nome);
                        //selecionando e capturando o nome do diretorio de destino do arquivo
						TAN *dest = buscaDiretorio(RAIZ, nome);
						//procura o diretorio de destino, se nao achar, retorna a mensagem abaixo
						if(!dest) printf("Diretorio nao encontrado!\n");
						else{
							//pai do movido
							ARQ *confere_dir = arq_atual -> info;
							//buscando o pai do arquivo atual(movido)
							TAN *confere_pai = buscaDiretorio(RAIZ, confere_dir -> nomePai);
							if(confere_pai){
                                //se o pai do arquivo atual é valido
								confere_dir = dest -> info;
                                //confere_dir recebe as informações do nó de destino
								DIR *pai = confere_pai -> info;
								//o diretorio "pai" recebe as informações do pai do arquivo selecionado
								if(strcmp(confere_dir -> nome, pai -> nome) == 0){
                                    //se o pai e o destino tiverem o mesmo nome, significa que o arquivo ja se encontra
                                    //no local de destino
									printf("O arquivo ja se encontra no local de destino!\n");
									break;
								}
							}
							//caso contrario, realiza a operação de mover
							mover(RAIZ, dest, arq_atual);
							imprime(dest);
							printf("Movido com sucesso!\n");
						}
					}
				} else{
				    //escolhendo diretorio
					if(!dir_atual){
                        //se o diretorio atual é nulo
						printf("Nenhum diretorio selecionado!\n");
					} else if(dir_atual == RAIZ){
					    //nao permite fazer operações com a raiz
						printf("Impossivel fazer operacoes sobre a raiz!\n");
					} else{
						printf("Diretorio destino: ");
						nome = (char*) malloc(100*sizeof(char));
						scanf(" %[^\n]s", nome);
                        //selecionado e capturando nome do diretorio destino
						TAN *dest = buscaDiretorio(RAIZ, nome);
						//fazendo a busca e retornado-a para o nó dest
						if(!dest) printf("Diretorio nao encontrado!\n");
						//se for nulo, o diretorio é invalido
						else{
							//pai do movido
							DIR *confere_dir = dir_atual -> info;
							//confere_dir recebe informações do diretorio selecionado atualmente
							TAN *confere_pai = buscaDiretorio(RAIZ, confere_dir -> nomePai);
							//confere_pai recebe o resultado da busca pelo pai do diretorio selecionado atualmente
							if(confere_pai){
								confere_dir = dest -> info;
								 //confere_dir recebe as informações do nó de destino
								DIR *pai = confere_pai -> info;
								//o diretorio "pai" recebe as informações do pai do diretorio selecionado
								if(strcmp(confere_dir -> nome, pai -> nome) == 0){
                                    //se o pai e o destino tiverem o mesmo nome, significa que o diretorio ja se encontra
                                    //no local de destino
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
			    //transformar
				printf("Arquivo ou diretorio selecionado? (A/D) ");
				scanf(" %c", &choice);
				while(choice != 'A' && choice != 'D'){
                    //enquanto nao recebe uma opção válida permanece no loop
					printf("Opcao invalida!\n");
					printf("Arquivo ou diretorio selecionado? (A/D) ");
					scanf(" %c", &choice);
				}
				if(choice == 'A'){
				    //escolhendo um arquivo
					if(!arq_atual){
					    //arquivo selecionado é nulo
						printf("Nenhum arquivo selecionado!\n");
					} else{
						ARQ *ar = arq_atual -> info;
						busca = buscaDiretorio(RAIZ, ar -> nome);
						if(busca){
						    //faz a busca por um diretorio de mesmo nome que o arquivo a ser transformado
							printf("Ja existe um diretorio com o mesmo nome!\n");
							break;
						}
						transformar(RAIZ, arq_atual, 'D');
						exibe_informacoes(arq_atual);
                        //transformando e exibindo informações
						dir_atual = arq_atual;
						arq_atual = NULL;
						//atualizando arquivo e diretorio selecionado
						printf("Transformado com sucesso!\n");
					}
				} else{
				    //escolhendo um diretorio
					if(!dir_atual){
                        //diretorio selecionado é nulo
						printf("Nenhum diretorio selecionado!\n");
					} else if(dir_atual == RAIZ){
					    //nao permite alteração na raiz
						printf("Impossivel fazer alteracoes no diretorio raiz!\n");
						break;
					} else{
					    //ao transformar um arquivo perdem-se todos os arquivos e diretorios filhos
						printf("Voce perdera todos os arquivos e diretorios contidos,\ntem certeza? (s/n) ");
						scanf(" %c", &choice);
						while(choice != 's' && choice != 'n'){
						    //permanece no loop até receber uma opção válida
							printf("Entrada invalida!\n");
							printf("Voce perdera todos os arquivos e diretorios contidos,\ntem certeza? (s/n) ");
							scanf(" %c", &choice);
						}
						if(choice == 's'){
						    //caso a resposta seja sim
							printf("Tipo (B - binario, T - texto): ");
                            //selecionando o tipo de arquivo para transformação
							scanf(" %c", &tipo);
							DIR *dr = dir_atual -> info;
							busca = buscaArquivo(RAIZ, dr -> nome);
							if(busca){
                                //verifica se já nao existe um arquivo com o mesmo nome que o diretorio a ser transformado
								printf("Ja existe um arquivo com o mesmo nome!\n");
								break;
							}
							//caso nao haja, realiza a transformação
							transformar(RAIZ, dir_atual, tipo);
							exibe_informacoes(dir_atual);
							arq_atual = dir_atual;
							dir_atual = NULL;
							//atualiza as informações de diretorio e arquivo selecionado
							printf("Transformado com sucesso!\n");
						}
					}
				}
				break;

			case 10:
			    //imprime o diretorio selecionado
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
    //saindo do loop, limpa a memoria
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
