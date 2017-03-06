/***************************************************************************
 *   Copyright (C) 2006 by Alexandre Gonçalves Silva                       *
 *   alexandre@joinville.udesc.br                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

 
#include "maxtree.h"


void extinction_height(MaxTree *mt) {
	int extincao;
	Imagem g = imZeros(mt->altura, mt->largura, 1, CINZA);  // imagem com zeros
	Lista *folhas = mt->retornaFolhas();
	Grafo *folha, *pai;
	while ( folhas->remove(&folha) ) {
		extincao = mt->grafo->filho->altura;
		pai = mt->retornaPai(folha);
		while (pai  &&  pai->altura <= (folha->nivel-pai->nivel)) {
			if (pai->visitado  &&  pai->grau > 1  &&  pai->altura == (folha->nivel-pai->nivel)) {  //EMPATE Grimaud,92
				break;
			}
			pai->visitado = true;
			pai = mt->retornaPai(pai);
		}
		if (pai)
			extincao = folha->nivel - pai->nivel;
		folha->setParticao(extincao);
	}
	pai = mt->grafo;
	while (pai) {
		pai->visitado = false;
		pai = pai->proximo;
	}
}

void extinction_area(MaxTree *mt) {
	int extincao, CONTINUA = SIM;
	Imagem g = imZeros(mt->altura, mt->largura, 1, INTEIRA);  // imagem com zeros
	Lista *folhas = mt->retornaFolhas(), *filhos;
	Grafo *folha, *aux, *pai, *filho;
	while ( folhas->remove(&folha) ) {
		extincao = mt->grafo->filho->area;
		aux = folha;
		pai = mt->retornaPai(aux);
		while (CONTINUA  &&  pai) {
			if (pai->grau > 1) {
				filhos = mt->retornaFilhos(pai);
				while ( CONTINUA  &&  filhos->remove(&filho) ) {   // verifica se possui irmao com area maior
					if (filho->visitado  &&  filho != aux  &&  filho->area == aux->area) { //EMPATE Grimaud,92
						filhos->limpa();
						CONTINUA = NAO;
					}
					else if (filho != aux  &&  filho->area > aux->area) {
						filhos->limpa();
						CONTINUA = NAO;
					}
					filho->visitado = true;
				}
			}
			if (CONTINUA) {
				aux = pai;
				pai = mt->retornaPai(aux);
			}
		}
		CONTINUA = SIM;
		if (pai)
			extincao = aux->area;
		folha->setParticao(extincao);
	}
	aux = mt->grafo;
	while (aux) {
		aux->visitado = false;
		aux = aux->proximo;
	}
}

void extinction_volume(MaxTree *mt) { //REVISAR (there is some problem here)
	int extincao, vF, vA, CONTINUA = SIM;
	Imagem g = imZeros(mt->altura, mt->largura, 1, INTEIRA);  // imagem com zeros
	Lista *folhas = mt->retornaFolhas(), *filhos;
	Grafo *folha, *aux, *pai, *filho;
	while ( folhas->remove(&folha) ) {
		extincao = mt->grafo->filho->volume + mt->grafo->filho->area * mt->grafo->filho->nivel;
		aux = folha;
		pai = mt->retornaPai(aux);
		while (CONTINUA  &&  pai) {
			if (pai->grau > 1) {
				filhos = mt->retornaFilhos(pai);
				while ( CONTINUA  &&  filhos->remove(&filho) ) {   // verifica se possui irmao com area maior
					vF = filho->volume + filho->area * (filho->nivel - pai->nivel);
					vA =   aux->volume +   aux->area * (  aux->nivel - pai->nivel);
					if (filho->visitado  &&  filho != aux  &&  vF == vA) {
						filhos->limpa();
						CONTINUA = NAO;
					}
					else if (filho != aux  &&  vF > vA) {
						filhos->limpa();
						CONTINUA = NAO;
					}
					filho->visitado = true;
				}
			}
			if (CONTINUA) {
				aux = pai;
				pai = mt->retornaPai(aux);
			}
		}
		CONTINUA = SIM;
		if (pai) {
			extincao = aux->volume + aux->area * (aux->nivel - pai->nivel);
		}
		folha->setParticao(extincao);
	}
	aux = mt->grafo;
	while (aux) {
		aux->visitado = false;
		aux = aux->proximo;
	}
}

//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//                        NOVOS VALORES DE EXTINCAO                            //
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void extinction_desc(MaxTree *mt) { //COM EMPATE
	int extincao, CONTINUA = SIM;
	Imagem g = imZeros(mt->altura, mt->largura, 1, INTEIRA);  // imagem com zeros
	Lista *folhas = mt->retornaFolhas();
	Grafo *folha, *aux, *pai, *filho;
	if (mt->tamanho == mt->tamanho_original) { //mais rápido para buscar filhos ... 24abr2008
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->descendentes;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filho = pai->filho;  //24abr2008 ... para sib08
					while ( CONTINUA  &&  filho ) {   // verifica se possui irmao com descendentes maior  //24abr2008 ... para sib08
						if (filho->visitado  &&  filho != aux  &&  filho->descendentes == aux->descendentes) { //EMPATE Grimaud,92
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  filho->descendentes > aux->descendentes) {
							CONTINUA = NAO;
						}
						filho->visitado = true;
						filho = filho->irmao; //24abr2008 ... para sib08 (supoe-se todos os nos ativos)
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->descendentes;
			folha->setParticao(extincao+1);
		}

	}
	else {
		Lista *filhos;
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->descendentes;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filhos = mt->retornaFilhos(pai);
					while ( CONTINUA  &&  filhos->remove(&filho) ) {   // verifica se possui irmao com descendentes maior
						if (filho->visitado  &&  filho != aux  &&  filho->descendentes == aux->descendentes) { //EMPATE Grimaud,92
							filhos->limpa();
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  filho->descendentes > aux->descendentes) {
							filhos->limpa();
							CONTINUA = NAO;
						}
						filho->visitado = true;
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->descendentes;
			folha->setParticao(extincao+1);
		}
	}
	aux = mt->grafo;
	while (aux) {
		aux->visitado = false;
		aux = aux->proximo;
	}
}

void extinction_htop(MaxTree *mt) { //COM EMPATE
	int extincao, CONTINUA = SIM;
	Imagem g = imZeros(mt->altura, mt->largura, 1, INTEIRA);  // imagem com zeros
	Lista *folhas = mt->retornaFolhas();
	Grafo *folha, *aux, *pai, *filho;
	if (mt->tamanho == mt->tamanho_original) { //mais rápido para buscar filhos ... 24abr2008
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->excentricidade;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filho = pai->filho;
					while ( CONTINUA  &&  filho ) {   // verifica se possui irmao com excentricidade maior
						if (filho->visitado  &&  filho != aux  &&  filho->excentricidade == aux->excentricidade) { //EMPATE Grimaud,92
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  filho->excentricidade > aux->excentricidade) {
							CONTINUA = NAO;
						}
						filho->visitado = true;
						filho = filho->irmao;
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->excentricidade;
			folha->setParticao(extincao+1);
		}
	}
	else {
		Lista *filhos;
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->excentricidade;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filhos = mt->retornaFilhos(pai);
					while ( CONTINUA  &&  filhos->remove(&filho) ) {   // verifica se possui irmao com excentricidade maior
						if (filho->visitado  &&  filho != aux  &&  filho->excentricidade == aux->excentricidade) { //EMPATE Grimaud,92
							filhos->limpa();
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  filho->excentricidade > aux->excentricidade) {
							filhos->limpa();
							CONTINUA = NAO;
						}
						filho->visitado = true;
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->excentricidade;
			folha->setParticao(extincao+1);
		}
	}
	aux = mt->grafo;
	while (aux) {
		aux->visitado = false;
		aux = aux->proximo;
	}
}

void extinction_hbbox(MaxTree *mt) { //COM EMPATE
	int extincao, CONTINUA = SIM;
	Imagem g = imZeros(mt->altura, mt->largura, 1, INTEIRA);  // imagem com zeros
	Lista *folhas = mt->retornaFolhas();
	Grafo *folha, *aux, *pai, *filho;
	if (mt->tamanho == mt->tamanho_original) { //mais rápido para buscar filhos ... 24abr2008
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->xmax - mt->grafo->filho->xmin;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filho = pai->filho;
					while ( CONTINUA  &&  filho ) {   // verifica se possui irmao com altura maior
						if (filho->visitado  &&  filho != aux  &&  (filho->xmax-filho->xmin) == (aux->xmax-aux->xmin)) { //EMPATE Grimaud,92
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  (filho->xmax-filho->xmin) > (aux->xmax-aux->xmin)) {
							CONTINUA = NAO;
						}
						filho->visitado = true;
						filho = filho->irmao;
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->xmax - aux->xmin;
			folha->setParticao(extincao+1);
		}

	}
	else {
		Lista *filhos;
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->xmax - mt->grafo->filho->xmin;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filhos = mt->retornaFilhos(pai);
					while ( CONTINUA  &&  filhos->remove(&filho) ) {   // verifica se possui irmao com altura maior
						if (filho->visitado  &&  filho != aux  &&  (filho->xmax-filho->xmin) == (aux->xmax-aux->xmin)) { //EMPATE Grimaud,92
							filhos->limpa();
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  (filho->xmax-filho->xmin) > (aux->xmax-aux->xmin)) {
							filhos->limpa();
							CONTINUA = NAO;
						}
						filho->visitado = true;
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->xmax - aux->xmin;
			folha->setParticao(extincao+1);
		}
	}
	aux = mt->grafo;
	while (aux) {
		aux->visitado = false;
		aux = aux->proximo;
	}
}

void extinction_wbbox(MaxTree *mt) { //COM EMPATE
	int extincao, CONTINUA = SIM;
	Imagem g = imZeros(mt->altura, mt->largura, 1, INTEIRA);  // imagem com zeros
	Lista *folhas = mt->retornaFolhas();
	Grafo *folha, *aux, *pai, *filho;
	if (mt->tamanho == mt->tamanho_original) { //mais rápido para buscar filhos ... 24abr2008
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->ymax - mt->grafo->filho->ymin;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filho = pai->filho;
					while ( CONTINUA  &&  filho ) {   // verifica se possui irmao com largura maior
						if (filho->visitado  &&  filho != aux  &&  (filho->ymax-filho->ymin) == (aux->ymax-aux->ymin)) { //EMPATE Grimaud,92
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  (filho->ymax-filho->ymin) > (aux->ymax-aux->ymin)) {
							CONTINUA = NAO;
						}
						filho->visitado = true;
						filho = filho->irmao;
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->ymax - aux->ymin;
			folha->setParticao(extincao+1);
		}
	}
	else {
		Lista *filhos;
		while ( folhas->remove(&folha) ) {
			extincao = mt->grafo->filho->ymax - mt->grafo->filho->ymin;
			aux = folha;
			pai = mt->retornaPai(aux);
			while (CONTINUA  &&  pai) {
				if (pai->grau > 1) {
					filhos = mt->retornaFilhos(pai);
					while ( CONTINUA  &&  filhos->remove(&filho) ) {   // verifica se possui irmao com largura maior
						if (filho->visitado  &&  filho != aux  &&  (filho->ymax-filho->ymin) == (aux->ymax-aux->ymin)) { //EMPATE Grimaud,92
							filhos->limpa();
							CONTINUA = NAO;
						}
						else if (filho != aux  &&  (filho->ymax-filho->ymin) > (aux->ymax-aux->ymin)) {
							filhos->limpa();
							CONTINUA = NAO;
						}
						filho->visitado = true;
					}
				}
				if (CONTINUA) {
					aux = pai;
					pai = mt->retornaPai(aux);
				}
			}
			CONTINUA = SIM;
			if (pai)
				extincao = aux->ymax - aux->ymin;
			folha->setParticao(extincao+1);
		}
	}
	aux = mt->grafo;
	while (aux) {
		aux->visitado = false;
		aux = aux->proximo;
	}
}
