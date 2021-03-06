/*
 * principal.c
 * Implementação de editor de texto gráfico.
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "texto.h"
#include "memo.h"

 int main(int argc, char **argv) {
 	
 	if(argc == 1){
 		printf("Enter with file name!\n");
 		return 0;
 	}else if(argc > 2){
 		printf("Enter with just one file name!\n");
 		return 0;
 	}

 	FILE *file = fopen(argv[1], "r+");
 	if(file == NULL){
 		file = fopen(argv[1], "w+");
 	}

 	texto_t* texto;
 	texto = texto_inicia(argv[1]);

 	texto_le_arquivo(texto, argv[1], file);
 	fclose(file);

 	while(texto_processa_comandos(texto)) {
 		texto_atualiza_tela(texto);
 	}
 	
 	texto_destroi(texto);
 	memo_relatorio();

 	return 0;
 }

