/*
 * principal.c Esse programa testa as TADs implementadas para um jogo
 * solitário.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014, 2015 João V. Lima, UFSM 2005       Benhur Stein, UFSM
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Créditos do código acima.
 * Com alterações realizadas pelo aluno Lucas Lima
 * de Oliveira para realização do trabalho T4, Solitaire, da matéria de 
 * Laboratório de Programação II
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "memo.h"
#include "jogo.h"
#include "vetor.h"
#include "fila.h"
#include "carta.h"
#include "pilha.h"
#include "tela.h"

#define QTD 52
#define PILHAS 28

void inicia_jogo(jogo solit){

	vetor_t* cartas = vetor_cria();
	vetor_t*  fora_ordem = vetor_cria();
	int i, j;

	srand(time(NULL));

	for(i=0; i<4; i++)
		for(j=1; j<14; j++)
			vetor_insere_carta(cartas, (i*13)+j-1, carta_cria(j, i));

	for(i=0; i<QTD; i++){
		j = ((int)rand())%vetor_numelem(cartas);
		carta c = vetor_remove_carta(cartas, j);
		vetor_insere_carta(fora_ordem, i, c);
	}

	for(i=0; i<7; i++){
		carta c;

		for(j=0; j<=i; j++){
			c = vetor_remove_carta(fora_ordem, 0);
			pilha_insere_carta(jogo_pilha(solit, i), c);
		}
		carta_abre(c);
	}

	for(i=0; i<QTD-PILHAS; i++){
		carta c = vetor_remove_carta(fora_ordem, 0);
		pilha_insere_carta(jogo_monte(solit), c);
	}

	vetor_destroi(cartas);
	vetor_destroi(fora_ordem);
}

void abre_carta_monte(jogo solit){
	carta c;

	if(pilha_vazia(jogo_monte(solit))){
		while(!pilha_vazia(jogo_descartes(solit))){
			c = pilha_remove_carta(jogo_descartes(solit));
			carta_fecha(c);
			pilha_insere_carta(jogo_monte(solit), c);
		}
	}else{
		c = pilha_remove_carta(jogo_monte(solit));
		carta_abre(c);
		pilha_insere_carta(jogo_descartes(solit), c);

	}
	jogo_desenha(solit);
}

void passa_carta_ases(jogo solit, int indice){

	carta comp, c = pilha_acessa_carta(jogo_descartes(solit));

	if(pilha_vazia(jogo_ases(solit, indice))){
		
		if(carta_valor(c) == 1){

			c = pilha_remove_carta(jogo_descartes(solit));
			pilha_insere_carta(jogo_ases(solit, indice), c);
			jogo_desenha(solit);

		}else tela_escreve_centralizado(jogo_tela(solit), "Comando inválido!\n", 18);

	}else{
		comp = pilha_acessa_carta(jogo_ases(solit, indice));

		if(carta_naipe(c) == carta_naipe(comp) && carta_valor(c) == carta_valor(comp) +1){

			c = pilha_remove_carta(jogo_descartes(solit));
			pilha_insere_carta(jogo_ases(solit, indice), c);
			jogo_desenha(solit);

		}else tela_escreve_centralizado(jogo_tela(solit), "Comando inválido!\n", 18);
	}
}

void passa_carta_pilhas(jogo solit, int indice){
	carta comp, c = pilha_acessa_carta(jogo_descartes(solit));

	if(pilha_vazia(jogo_pilha(solit, indice))){
		
		if(carta_valor(c) == 13){

			c = pilha_remove_carta(jogo_descartes(solit));
			pilha_insere_carta(jogo_pilha(solit, indice), c);
			jogo_desenha(solit);

		}else tela_escreve_centralizado(jogo_tela(solit), "Comando inválido!\n", 18);

	}else{

		comp = pilha_acessa_carta(jogo_pilha(solit, indice));

		if(carta_valor(c) +1 == carta_valor(comp)){

			c = pilha_remove_carta(jogo_descartes(solit));
			pilha_insere_carta(jogo_pilha(solit, indice), c);
			jogo_desenha(solit);
			
		}else tela_escreve_centralizado(jogo_tela(solit), "Comando inválido!\n", 18);
	}
}

bool fim_jogo(jogo solit){
	int i, qtde = 0;

	for(i=0; i<4; i++)
		if(pilha_topo(jogo_ases(solit, i)) == 13)
			qtde++;

	return (qtde == 4);
}



int main(int argc, char **argv){
	jogo solit;
	solit = jogo_cria();	

	inicia_jogo(solit);
	jogo_desenha(solit);

	do{
		char cmd[3];
		int i;

		cmd[0] = tela_le(jogo_tela(solit));
		if(cmd[0] == 27) break;		//ESC -> Sai do jogo;

		switch(cmd[0]){
			case ' ':				// ESPAÇO -> Abre carta do monte no descarte;
				abre_carta_monte(solit);
				break;
			
			case 'M': case 'm': 	// M -> Mover uma carta do descarte para uma pilha;
				cmd[0] = tela_le(jogo_tela(solit));

				switch(cmd[0]){
					case 'A': case 'a':
						passa_carta_ases(solit, 0);
						break;
					case 'S': case 's':
						passa_carta_ases(solit, 1);
						break;
					case 'D': case 'd':
						passa_carta_ases(solit, 2);
						break;
					case 'F': case 'f':
						passa_carta_ases(solit, 3);
						break;
					case '1': case '2': case '3': case '4': case '5': case '6': case '7':
						i = atoi(&cmd[0])-1;
						passa_carta_pilhas(solit, i);
				}
				break;

			default:
				tela_escreve_centralizado(jogo_tela(solit), "Comando inválido!\n", 18);
				break;
		}


	}while(!fim_jogo(solit));

	jogo_destroi(solit);

	/* relatório de memória */
	memo_relatorio();
	return 0;
}
