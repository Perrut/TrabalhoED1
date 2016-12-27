#include <stdio.h>
#include <stdlib.h>

long calcularTamanhoArquivo(FILE *arquivo) {
 
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

int main(void){
	FILE *arquivo;
	
	arquivo = fopen("exemplo.txt", "r");	
	
	if(arquivo){
		printf("%ld \n", calcularTamanhoArquivo(arquivo));
	}
	
	printf("DATA : %s HORA: %s\n",__DATE__,__TIME__);
	
	FILE *pont_arq; // cria variável ponteiro para o arquivo
  	char palavra[20]; // variável do tipo string
 
	//abrindo o arquivo com tipo de abertura w
	pont_arq = fopen("arquivo_palavra.bin", "w");
 
	//testando se o arquivo foi realmente criado
	if(pont_arq == NULL)
	{
		printf("Erro na abertura do arquivo!");
		return 1;
	}
 
    printf("Escreva uma palavra para testar gravacao de arquivo: ");
    scanf("%s", palavra);
 
    //usando fprintf para armazenar a string no arquivo
    fprintf(pont_arq, "%s", palavra);
 
    //usando fclose para fechar o arquivo
    fclose(pont_arq);
    fclose(arquivo);
 
    printf("Dados gravados com sucesso!");
 
    return(0);
}
