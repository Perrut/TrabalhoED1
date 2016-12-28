#include <stdio.h>
#include <stdlib.h>
#include "TAN.h"
#include <sys/stat.h>

//FALTA TRANSFORMAR

int main(void){
    TAN *RAIZ = aloca_dir("RAIZ");
    
    TAN *arq1 = aloca_arq("teste1",'T');
    TAN *arq2 = aloca_arq("teste2",'B');
    
    TAN * pastapai=aloca_dir("pastapai");
    TAN * pastafilho=aloca_dir("pastafilho");
    
    TAN * arquivotxtfilho=aloca_arq("arquivotxtfilho",'T');
    TAN * arquivobinfilho=aloca_arq("arquivobinfilho",'B');
    TAN * arquivobinfilho2=aloca_arq("arquivobinfilho2",'B');
    
    insere(RAIZ,arq1);
    insere(RAIZ,arq2);
    insere(RAIZ,pastapai);
    insere(pastapai, pastafilho);
    insere(pastafilho, arquivobinfilho2);
    insere(pastafilho, arquivotxtfilho);
    insere(pastafilho, arquivobinfilho);
    imprime(RAIZ);
    
    renomear(arquivobinfilho2, "ptinha");
    imprime(RAIZ);
    
    return 0;
}
