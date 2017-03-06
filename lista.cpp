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

#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//########################################################################################################################################
//----------------//
// LISTA DINAMICA //
//----------------//

NoLista::NoLista(unsigned int tamanhoInformacao) {
	info = malloc(tamanhoInformacao);
	anterior = NULL;
	proximo  = NULL;
}
NoLista::~NoLista() {
	free(info);
}


Lista::Lista(unsigned int tamanhoInformacao) {
	tamInfo = tamanhoInformacao;
	tamanho = 0;
	inicio  = NULL;
	fim     = NULL;
}


int Lista::vazia() {
	if (inicio == NULL)
		return SIM;
	return NAO;
}


int Lista::empilha(void *novo) {
	return insere(novo, FIM);
}


int Lista::desempilha(void *novo) {
	return remove(novo, FIM);
}


int Lista::insere(void *novo, int pos) {
	int i = 0, pos_inv;
	NoLista *aux, *x;
	aux = new NoLista(tamInfo);  /* alocacao do novo elemento */
	if (aux == NULL)
		return NAO;
	if (aux->info == NULL) {
		delete aux;
		return NAO;
	}
	memcpy(aux->info, novo, tamInfo);
	if (vazia()) {  /* insercao do primeiro elemento */
		inicio = aux;
		fim    = aux;
		tamanho++;
		return SIM;
	}
	if (pos == FIM  ||  (pos>0 && pos>=tamanho)) {  /* insercao no fim */
		fim->proximo  = aux;
		aux->anterior = fim;
		fim = aux;
		tamanho++;
		return SIM;
	}
	if (pos == INI  ||  (pos<0 && (-1*pos)>tamanho)) {  /* insercao no inicio */
		aux->proximo = inicio;
		inicio->anterior = aux;
		inicio = aux;
		tamanho++;
		return SIM;
	}    
	if (pos < 0)
		pos = tamanho + pos + 1;
	if (pos <= tamanho/2) {  /* se posicao mais proxima do inicio */
		x = inicio;
		while (i < (pos-1)) {
			x = x->proximo;
			i++;
		}
	}    
	else {  /* se posicao mais proxima do fim */
		pos_inv = tamanho - pos;
		x = fim;
		while (i < pos_inv) {
			x = x->anterior;
			i++;
		}
	}
	/* insercao no meio */
	aux->proximo = x->proximo;
	x->proximo->anterior = aux;
	x->proximo = aux;
	aux->anterior = x;
	tamanho++;
	return SIM;
}


int Lista::inserePrioridade(void *novo, int (*compara)(void *e1, void *e2)) {
	NoLista *aux, *x;
	aux = new NoLista(tamInfo);  /* alocacao do novo elemento */
	if (aux == NULL)
		return NAO;
	if (aux->info == NULL) {
		delete aux;
		return NAO;
	}
	memcpy(aux->info, novo, tamInfo);
	if (vazia()) {  /* insercao do primeiro elemento */
		inicio = aux;
		fim    = aux;
		tamanho++;
		return SIM;
	}
	x = inicio;
	while ( x  &&  compara(novo,x->info)!=MAIOR )  /* enquanto novo elemento tiver prioridade menor ou igual */
		x = x->proximo;
	if (x == NULL) {  /* insersao no fim */
		fim->proximo  = aux;
		aux->anterior = fim;
		fim = aux;
		tamanho++;
		return SIM;
	}
	else if (x == inicio) {  /* insersao no inicio */
		aux->proximo = inicio;
		inicio->anterior = aux;
		inicio = aux;
		tamanho++;
		return SIM;
	}
	else {  /* insercao no meio */
		aux->anterior = x->anterior;
		x->anterior->proximo = aux;
		x->anterior = aux;
		aux->proximo = x;
		tamanho++;
		return SIM;
	}
}


int Lista::remove(void *destino, int pos) {
	int pos_inv, i = 0;
	NoLista *aux, *x;
	if (vazia())
		return NAO;
	if (pos == FIM  ||  (pos>0 && pos>=tamanho-1)) {  /* remocao no fim */
		aux = fim;
		fim = aux->anterior;
		memcpy(destino, aux->info, tamInfo);
		delete aux;
		tamanho--;
		if (fim == NULL) {
			inicio = NULL;
			return SIM;
		}
		fim->proximo = NULL;
		return SIM;
	}
	if (pos == INI  ||  (pos<0 && (-1*pos)>tamanho)) {  /* remocao do inicio */
		aux = inicio;
		inicio = aux->proximo;
		memcpy(destino, aux->info, tamInfo);
		delete aux;
		tamanho--;
		if (inicio == NULL) {
			fim = NULL;
			return SIM;
		}
		inicio->anterior = NULL;
		return SIM;
	}
	if (pos < 0)
		pos = tamanho + pos;
	if (pos <= tamanho/2) {  /* se posicao mais proxima do inicio */
		x = inicio;
		while (i < (pos-1)) {
			x = x->proximo;
			i++;
		}
	}    
	else {  /* se posicao mais proxima do fim */
		pos_inv = tamanho - pos;
		x = fim;
		while (i < pos_inv) {
			x = x->anterior;
			i++;
		}
	}
	/* remocao do meio */
	aux = x->proximo;
	x->proximo = aux->proximo;
	x->proximo->anterior = x;
	memcpy(destino, aux->info, tamInfo);
	delete aux;
	tamanho--;
	return SIM;
}


int Lista::acessa(void *destino, int pos) {
	int pos_inv, i = 0;
	NoLista *x;
	if (vazia())
		return NAO;
	if (pos == INI  ||  (pos<0 && (-1*pos)>tamanho)) {  /* acessa no inicio */
		memcpy(destino, inicio->info, tamInfo);
		return SIM;
	}
	if (pos == FIM  ||  (pos>0 && pos>=tamanho)) {  /* acessa no fim */
		memcpy(destino, fim->info, tamInfo);
		return SIM;
	}    
	if (pos < 0)
		pos = tamanho + pos;
	if (pos <= tamanho/2) {  /* se posicao mais proxima do inicio */
		x = inicio;
		while (i < pos) {
			x = x->proximo;
			i++;
		}
	}    
	else {  /* se posicao mais proxima do fim */
		pos_inv = tamanho - pos - 1;
		x = fim;
		while (i < pos_inv) {
			x = x->anterior;
			i++;
		}
	}
	/* acesso no meio */
	memcpy(destino, x->info, tamInfo);
	return SIM;
}
	

int Lista::troca(void *novo, int pos) {
	int pos_inv, i = 0;
	NoLista *x;
	if (vazia())
		return NAO;
	if (pos == INI  ||  (pos<0 && (-1*pos)>tamanho)) {  /* acessa no inicio */
		memcpy(inicio->info, novo, tamInfo);
		return SIM;
	}
	if (pos == FIM  ||  (pos>0 && pos>=tamanho)) {  /* acessa no fim */
		memcpy(fim->info, novo, tamInfo);
		return SIM;
	}    
	if (pos < 0)
		pos = tamanho + pos;
	if (pos <= tamanho/2) {  /* se posicao mais proxima do inicio */
		x = inicio;
		while (i < pos) {
			x = x->proximo;
			i++;
		}
	}    
	else {  /* se posicao mais proxima do fim */
		pos_inv = tamanho - pos - 1;
		x = fim;
		while (i < pos_inv) {
			x = x->anterior;
			i++;
		}
	}
	/* acesso no meio */
	memcpy(x->info, novo, tamInfo);
	return SIM;
}


void Lista::concatena(Lista *lista) {
	if (lista->tamanho != 0) {
		lista->inicio->anterior = fim;
		if (tamanho != 0)
			fim->proximo = lista->inicio;
		else
			inicio = lista->inicio;
		fim = lista->fim;
		tamanho += lista->tamanho;
		lista->inicio  = NULL;
		lista->fim     = NULL;
		lista->tamanho = 0;
	}
}


void Lista::inverte() {
	NoLista *no = inicio, *x;
	if (no) {
		no = no->proximo; //inicia pelo segundo elemento
		if (no) {
			while (no->proximo != NULL) {
				x = no;
				no = no->proximo;
				x->anterior->proximo = x->proximo;
				x->proximo->anterior = x->anterior;
				x->proximo       = inicio;
				//x->anterior      = NULL;
				inicio->anterior = x;
				inicio = x;
			}
			no->anterior->proximo = NULL;
			no->proximo  = inicio;
			fim          = no->anterior;
			no->anterior = NULL;
			inicio       = no;
		}
	}
}


void Lista::ordena(int (*compara)(void *e1, void *e2), int decrescente) {
	Lista   *aux = new Lista(tamInfo);
	NoLista *no  = inicio, *x;
	while (no) {  //REVER... evitar o uso de fila auxiliar de prioridade !!!!!
		aux->inserePrioridade(no->info, compara);
		x  = no;
		no = no->proximo;
		delete x;
	}
	inicio  = aux->inicio;
	fim     = aux->fim;
	tamanho = aux->tamanho;
	aux->inicio  = NULL;
	aux->fim     = NULL;
	aux->tamanho = 0;
	if ( !decrescente )
		inverte();
	delete aux;
}


void Lista::limpa() {
	NoLista *x = NULL;
	while (vazia() == NAO) {
		x = inicio;
		inicio = inicio->proximo;
		delete x;
	}
	inicio  = NULL;
	fim     = NULL;
	tamanho = 0;
}


	
Lista::~Lista() {
	NoLista *x = NULL;
	while (vazia() == NAO) {
		x = inicio;
		inicio = inicio->proximo;
		delete x;
	}
}



//########################################################################################################################################
//----------------//
// LISTA ESTATICA //
//----------------//

NoListaE::NoListaE(unsigned int tamanhoInformacao) {
	info = malloc(tamanhoInformacao);
	anterior = -1;
	proximo  = -1;
}
NoListaE::~NoListaE() {
	free(info);
}

int ListaE::aloca() {
	int pos = livre;
	if (pos != -1) {
		livre = vet[pos]->proximo;
		vet[livre]->anterior = -1;
		
		vet[pos]->proximo  = -1;
		vet[pos]->anterior = -1;
	}
	return pos;
}

void ListaE::desaloca(int posicao) {
	vet[posicao]->proximo  = livre;
	vet[posicao]->anterior = -1;
	vet[livre]->anterior   = posicao;
	livre = posicao;
}

ListaE::ListaE(unsigned int tamanhoInformacao, unsigned int tamanhoVetor) {
	tamInfo = tamanhoInformacao;
	tamVet  = tamanhoVetor;
	tamanho =  0;
	livre   =  0;
	inicio  = -1;
	fim     = -1;
	vet     = new NoListaE*[tamVet];

	for (unsigned int i=0; i < tamVet; i++) {
		vet[i] = new NoListaE(tamInfo);
		vet[i]->proximo  = i+1;
		vet[i]->anterior = i-1;
	}
	vet[0]->anterior       = -1;
	vet[tamVet-1]->proximo = -1;
}


int ListaE::vazia() {
	if (inicio == -1)
		return SIM;
	return NAO;
}


int ListaE::empilha(void *novo) {
	return insere(novo, FIM);
}


int ListaE::desempilha(void *novo) {
	return remove(novo, FIM);
}


int ListaE::insere(void *novo, int pos) {
	int i = 0, pos_inv;
	int aux, x;
	aux = aloca();  /* alocacao do novo elemento */
	if (aux == -1)
		return NAO;
	memcpy(vet[aux]->info, novo, tamInfo);
	if (vazia()) {  /* insercao do primeiro elemento */
		inicio = aux;
		fim    = aux;
		tamanho++;
		return SIM;
	}
	if (pos == FIM  ||  (pos>0 && pos>=tamanho)) {  /* insercao no fim */
		vet[fim]->proximo  = aux;
		vet[aux]->anterior = fim;
		fim = aux;
		tamanho++;
		return SIM;
	}
	if (pos == INI  ||  (pos<0 && (-1*pos)>tamanho)) {  /* insercao no inicio */
		vet[aux]->proximo = inicio;
		vet[inicio]->anterior = aux;
		inicio = aux;
		tamanho++;
		return SIM;
	}    
	if (pos < 0)
		pos = tamanho + pos + 1;
	if (pos <= tamanho/2) {  /* se posicao mais proxima do inicio */
		x = inicio;
		while (i < (pos-1)) {
			x = vet[x]->proximo;
			i++;
		}
	}    
	else {  /* se posicao mais proxima do fim */
		pos_inv = tamanho - pos;
		x = fim;
		while (i < pos_inv) {
			x = vet[x]->anterior;
			i++;
		}
	}
	/* insercao no meio */
	vet[aux]->proximo = vet[x]->proximo;
	vet[ vet[x]->proximo ]->anterior = aux;
	vet[x]->proximo = aux;
	vet[aux]->anterior = x;
	tamanho++;
	return SIM;
}


int ListaE::inserePrioridade(void *novo, int (*compara)(void *e1, void *e2)) {
	int aux, x;
	aux = aloca();  /* alocacao do novo elemento */
	if (aux == -1)
		return NAO;
	memcpy(vet[aux]->info, novo, tamInfo);
	if (vazia()) {  /* insercao do primeiro elemento */
		inicio = aux;
		fim    = aux;
		tamanho++;
		return SIM;
	}
	x = inicio;
	while ( x  &&  compara(novo,vet[x]->info)!=MAIOR )  /* enquanto novo elemento tiver prioridade menor ou igual */
		x = vet[x]->proximo;
	if (x == -1) {  /* insersao no fim */
		vet[fim]->proximo  = aux;
		vet[aux]->anterior = fim;
		fim = aux;
		tamanho++;
		return SIM;
	}
	else if (x == inicio) {  /* insersao no inicio */
		vet[aux]->proximo = inicio;
		vet[inicio]->anterior = aux;
		inicio = aux;
		tamanho++;
		return SIM;
	}
	else {  /* insercao no meio */
		vet[aux]->anterior = vet[x]->anterior;
		vet[ vet[x]->anterior ]->proximo = aux;
		vet[x]->anterior = aux;
		vet[aux]->proximo = x;
		tamanho++;
		return SIM;
	}
}


int ListaE::remove(void *destino, int pos) {
	int pos_inv, i = 0;
	int aux, x;
	if (vazia())
		return NAO;
	if (pos == FIM  ||  (pos>0 && pos>=tamanho-1)) {  /* remocao no fim */
		aux = fim;
		fim = vet[aux]->anterior;
		memcpy(destino, vet[aux]->info, tamInfo);
		desaloca(aux);
		tamanho--;
		if (fim == -1) {
			inicio = -1;
			return SIM;
		}
		vet[fim]->proximo = -1;
		return SIM;
	}
	if (pos == INI  ||  (pos<0 && (-1*pos)>tamanho)) {  /* remocao do inicio */
		aux = inicio;
		inicio = vet[aux]->proximo;
		memcpy(destino, vet[aux]->info, tamInfo);
		desaloca(aux);
		tamanho--;
		if (inicio == -1) {
			fim = -1;
			return SIM;
		}
		vet[inicio]->anterior = -1;
		return SIM;
	}
	if (pos < 0)
		pos = tamanho + pos;
	if (pos <= tamanho/2) {  /* se posicao mais proxima do inicio */
		x = inicio;
		while (i < (pos-1)) {
			x = vet[x]->proximo;
			i++;
		}
	}    
	else {  /* se posicao mais proxima do fim */
		pos_inv = tamanho - pos;
		x = fim;
		while (i < pos_inv) {
			x = vet[x]->anterior;
			i++;
		}
	}
	/* remocao do meio */
	aux = vet[x]->proximo;
	vet[x]->proximo = vet[aux]->proximo;
	vet[ vet[x]->proximo ]->anterior = x;
	memcpy(destino, vet[aux]->info, tamInfo);
	desaloca(aux);
	tamanho--;
	return SIM;
}


int ListaE::acessa(void *destino, int pos) {
	int pos_inv, i = 0;
	int x;
	if (vazia())
		return NAO;
	if (pos == INI  ||  (pos<0 && (-1*pos)>tamanho)) {  /* acessa no inicio */
		memcpy(destino, vet[inicio]->info, tamInfo);
		return SIM;
	}
	if (pos == FIM  ||  (pos>0 && pos>=tamanho)) {  /* acessa no fim */
		memcpy(destino, vet[fim]->info, tamInfo);
		return SIM;
	}    
	if (pos < 0)
		pos = tamanho + pos;
	if (pos <= tamanho/2) {  /* se posicao mais proxima do inicio */
		x = inicio;
		while (i < pos) {
			x = vet[x]->proximo;
			i++;
		}
	}    
	else {  /* se posicao mais proxima do fim */
		pos_inv = tamanho - pos - 1;
		x = fim;
		while (i < pos_inv) {
			x = vet[x]->anterior;
			i++;
		}
	}
	/* acesso no meio */
	memcpy(destino, vet[x]->info, tamInfo);
	return SIM;
}
	



void ListaE::limpa() {
	tamanho =  0;
	livre   =  0;
	inicio  = -1;
	fim     = -1;
	for (unsigned int i=0; i < tamVet; i++) {
		vet[i]->proximo  = i+1;
		vet[i]->anterior = i-1;
	}
	vet[0]->anterior       = -1;
	vet[tamVet-1]->proximo = -1;
}


	
ListaE::~ListaE() {
	for (unsigned int i=0; i < tamVet; i++)
		delete vet[i];
	delete vet;
	vet = NULL;
}




//########################################################################################################################################
//------------------------//
// FILA ESTATICA CIRCULAR //
//------------------------//

Fila::Fila(unsigned int tamanhoInformacao, unsigned int tamanhoVetor) {
	tamInfo = tamanhoInformacao;
	tamVet  = tamanhoVetor;
	vet = (void **) malloc(tamVet * sizeof(void *));
	for (unsigned int i=0; i<tamVet; i++)
		vet[i] = malloc(tamInfo);
	inicio  =  0;
	fim     = -1;
	tamanho =  0;
}
int Fila::vazia() {
	if (tamanho == 0)
		return SIM;
	return NAO;
}
int Fila::insere(void *novo) {
	if (tamanho == tamVet)
		return NAO;
	if (fim == tamVet-1)
		fim = 0;
	else
		fim++;
	memcpy(vet[fim], novo, tamInfo);
	tamanho++;
	return SIM;
}
int Fila::remove(void *novo) {
	if (tamanho == 0)
		return NAO;
	memcpy(novo, vet[inicio], tamInfo);
	if (inicio == tamVet-1)
		inicio = 0;
	else
		inicio++;
	tamanho--;
	return SIM;
}
int Fila::remove() {
	if (tamanho == 0)
		return NAO;
	if (inicio == tamVet-1)
		inicio = 0;
	else
		inicio++;
	tamanho--;
	return SIM;
}
int Fila::acessa(void *novo) {
	if (tamanho == 0)
		return NAO;
	memcpy(novo, vet[inicio], tamInfo);
	return SIM;
}
void Fila::limpa() {
	inicio  =  0;
	fim     = -1;
	tamanho =  0;
}
Fila::~Fila() {
	for (unsigned int i=0; i<tamVet; i++)
		free(vet[i]);
	free(vet);
}






//########################################################################################################################################
//----------------//
// PILHA ESTATICA //
//----------------//

Pilha::Pilha(unsigned int tamanhoInformacao, unsigned int tamanhoVetor) {
	tamInfo = tamanhoInformacao;
	tamVet  = tamanhoVetor;
	vet = (void **) malloc(tamVet * sizeof(void *));
	for (unsigned int i=0; i<tamVet; i++)
		vet[i] = malloc(tamInfo);
	topo = -1;
	tamanho = 0;
}
int Pilha::vazia() {
	if (topo == -1)
		return SIM;
	return NAO;
}
int Pilha::empilha(void *novo) {
	if (topo == tamVet-1)
		return NAO;
	topo++;
	memcpy(vet[topo], novo, tamInfo);
	tamanho++;
	return SIM;
}
int Pilha::desempilha(void *novo) {
	if (topo == -1)
		return NAO;
	memcpy(novo, vet[topo], tamInfo);
	topo--;
	tamanho--;
	return SIM;
}
int Pilha::desempilha() {
	if (topo == -1)
		return NAO;
	topo--;
	tamanho--;
	return SIM;
}
int Pilha::acessa(void *novo) {
	if (topo == -1)
		return NAO;
	memcpy(novo, vet[topo], tamInfo);
	return SIM;
}
void Pilha::limpa() {
	topo = -1;
	tamanho = 0;
}
Pilha::~Pilha() {
	for (unsigned int i=0; i<tamVet; i++)
		free(vet[i]);
	free(vet);
}












//########################################################################################################################################
//----------------------------------------------------//
// FILA ESTATICA CIRCULAR - especifica para o tipo XY //
//----------------------------------------------------//

FilaXY::FilaXY(unsigned int tamanhoVetor) {
	tamVet  = tamanhoVetor;
	vet = (XY *) malloc(tamVet * sizeof(XY));
	inicio  =  0;
	fim     = -1;
	tamanho =  0;
}
int FilaXY::vazia() {
	if (tamanho == 0)
		return SIM;
	return NAO;
}
int FilaXY::insere(XY *novo) {
	if (tamanho == tamVet)
		return NAO;
	if (fim == tamVet-1)
		fim = 0;
	else
		fim++;
	///memcpy(&vet[fim], novo, sizeof(XY));
	vet[fim] = *novo;
	tamanho++;
	return SIM;
}
int FilaXY::remove(XY *novo) {
	if (tamanho == 0)
		return NAO;
	///memcpy(novo, &vet[inicio], sizeof(XY));
	*novo = vet[inicio];
	if (inicio == tamVet-1)
		inicio = 0;
	else
		inicio++;
	tamanho--;
	return SIM;
}
int FilaXY::remove() {
	if (tamanho == 0)
		return NAO;
	if (inicio == tamVet-1)
		inicio = 0;
	else
		inicio++;
	tamanho--;
	return SIM;
}
int FilaXY::acessa(XY *novo) {
	if (tamanho == 0)
		return NAO;
	///memcpy(novo, &vet[inicio], sizeof(XY));
	*novo = vet[inicio];
	return SIM;
}
void FilaXY::limpa() {
	inicio  =  0;
	fim     = -1;
	tamanho =  0;
}
FilaXY::~FilaXY() {
	free(vet);
}
//########################################################################################################################################
//--------------------------------------------//
// PILHA ESTATICA - especifica para o tipo XY //
//--------------------------------------------//
PilhaXY::PilhaXY(unsigned int tamanhoVetor) {
	tamVet  = tamanhoVetor;
	vet = (XY *) malloc(tamVet * sizeof(XY));
	topo = -1;
	tamanho = 0;
}
int PilhaXY::vazia() {
	if (topo == -1)
		return SIM;
	return NAO;
}
int PilhaXY::empilha(XY *novo) {
	if (topo == tamVet-1)
		return NAO;
	topo++;
	///memcpy(&vet[topo], novo, sizeof(XY));
	vet[topo] = *novo;
	tamanho++;
	return SIM;
}
int PilhaXY::desempilha(XY *novo) {
	if (topo == -1)
		return NAO;
	///memcpy(novo, &vet[topo], sizeof(XY));
	*novo = vet[topo];
	topo--;
	tamanho--;
	return SIM;
}
int PilhaXY::desempilha() {
	if (topo == -1)
		return NAO;
	topo--;
	tamanho--;
	return SIM;
}
int PilhaXY::acessa(XY *novo) {
	if (topo == -1)
		return NAO;
	///memcpy(novo, &vet[topo], sizeof(XY));
	*novo = vet[topo];
	return SIM;
}
void PilhaXY::limpa() {
	topo = -1;
	tamanho = 0;
}
PilhaXY::~PilhaXY() {
	free(vet);
}
//########################################################################################################################################
//--------------------------------------------------------//
// FILA ESTATICA CIRCULAR - especifica para o tipo Grafo* //
//--------------------------------------------------------//

FilaGrafo::FilaGrafo(unsigned int tamanhoVetor) {
	tamVet  = tamanhoVetor;
	vet = (Grafo **) malloc(tamVet * sizeof(Grafo *));
	inicio  =  0;
	fim     = -1;
	tamanho =  0;
}
int FilaGrafo::vazia() {
	if (tamanho == 0)
		return SIM;
	return NAO;
}
int FilaGrafo::insere(Grafo **novo) {
	if (tamanho == tamVet)
		return NAO;
	if (fim == tamVet-1)
		fim = 0;
	else
		fim++;
	///memcpy(&vet[fim], novo, sizeof(Grafo *));
	vet[fim] = *novo;
	tamanho++;
	return SIM;
}
int FilaGrafo::remove(Grafo **novo) {
	if (tamanho == 0)
		return NAO;
	///memcpy(novo, &vet[inicio], sizeof(Grafo *));
	*novo = vet[inicio];
	if (inicio == tamVet-1)
		inicio = 0;
	else
		inicio++;
	tamanho--;
	return SIM;
}
int FilaGrafo::remove() {
	if (tamanho == 0)
		return NAO;
	if (inicio == tamVet-1)
		inicio = 0;
	else
		inicio++;
	tamanho--;
	return SIM;
}
int FilaGrafo::acessa(Grafo **novo) {
	if (tamanho == 0)
		return NAO;
	///memcpy(novo, &vet[inicio], sizeof(Grafo *));
	*novo = vet[inicio];
	return SIM;
}
void FilaGrafo::limpa() {
	inicio  =  0;
	fim     = -1;
	tamanho =  0;
}
FilaGrafo::~FilaGrafo() {
	free(vet);
}
