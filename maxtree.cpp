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




MaxTree *img2tree(const Imagem& f, const Estruturante& se) {
	MaxTree *mt = new MaxTree(se.img->matriz, se.img->altura, se.img->largura, f);
	return mt;
}


//----------------------classe arvore-------------------
bool Grafo::isOn() {
  return ativo;
}
void Grafo::setOn() {
  ativo = true;
}
void Grafo::setOff() {
  ativo = false;
}
unsigned int Grafo::getParticao() {
  return particao;
}
void Grafo::setParticao(unsigned int valor) {
  particao = valor;
}
Grafo::Grafo() { //construtor
  //	nivel    =  0;
  //	rotulo   =  1;
  //	ordem    =  0;
  //	particao =  0;
  //	ativo    =  true;
  //	filho = pai = irmao = proximo = NULL;
}

//--------------fim------classe arvore------------------


//----------------------classe fila hierarquica-------------------
FilaHierarquica::FilaHierarquica(unsigned int *histograma, Grafo **hash[257]) {
	unsigned int i, j;
	hash[0] = new Grafo *[1]; hash[0][0] = NULL; //hash!!  posicao da raiz (-1,...)
	for (i=0; i<256; i++) {
		fila[i]   = new FilaXY(histograma[i]);
		hash[i+1] = new Grafo *[histograma[i]]; //hash!!
		for (j=0; j < histograma[i]; j++) //hash!!
			hash[i+1][j] = NULL; //hash!!
	}
}
int FilaHierarquica::vazia(int alvo) {
	return fila[alvo]->vazia();
}
void FilaHierarquica::insere(int alvo, XY *e) {
	fila[alvo]->insere(e);
}
int FilaHierarquica::remove(int alvo, XY *e) {
	return fila[alvo]->remove(e);
}
FilaHierarquica::~FilaHierarquica() { //2008mar14
    for (int i=0; i<256; i++) {
        delete fila[i];
    }
}
//--------------fim------classe fila hierarquica------------------



//----------------------classe Maxtree principal-------------------

short int MaxTree::flood(short int h) {
	XY p, e;
	//- short int m, pn, pm;
	short int m;
	unsigned int vz; //2008jan30
	unsigned int area = 0;
	//_ unsigned int perimetro = 0; //2008jan29
	XY2 xycen; xycen.x=0;      xycen.y=0; //2008jan28
	XY  xymin; xymin.x=altura; xymin.y=largura; //2008jan28_
	XY  xymax; xymax.x=0;      xymax.y=0;       //2008jan28_

	while ( filas->remove(h,&p) ) {

		if (p.x < xymin.x) xymin.x = p.x;  if (p.y < xymin.y) xymin.y = p.y; //2008jan28_
		if (p.x > xymax.x) xymax.x = p.x;  if (p.y > xymax.y) xymax.y = p.y; //2008jan28_

		xycen.x += p.x; //2008jan28
		xycen.y += p.y; //2008jan28

		area++;
		status[p.x][p.y] = rotuloNo[h] + 1;

		for (vz=0; vz < vizinhos_quantidade; vz++) {

			e.x = p.x + vizinhos[vz].x;
			e.y = p.y + vizinhos[vz].y;

			//if (e.x >=0 && e.x < altura && e.y >=0 && e.y < largura) {  //no dominio... ponto a melhorar !!!!!
			if (e.x < altura && e.y < largura) {  //no dominio
				if (status[e.x][e.y] == 0) {  //nao visitado
					//- COMPLEX++;
					m = img[e.x][e.y];

					filas->insere(m,&e);
					status[e.x][e.y] = -1;
					nivelNo[m] = true;
					xyNo[m].x = e.x; xyNo[m].y = e.y;

					while (m > h)
						m = flood(m);
				}
			}
		}
	}

	m = h - 1;
	while ((m >= 0) && ( ! nivelNo[m] ))
		m = m - 1;

	if (m >= 0) {
		grafo = inserir(m, rotuloNo[m]+1, xyNo[m], h, rotuloNo[h]+1, xyNo[h], xymin, xymax, xycen, area, &maiorGrau);
	}
	else {
		grafo = inserir(-1, 1, xyNo[menorValor], menorValor, 1, xyNo[menorValor], xymin, xymax, xycen, 0, &maiorGrau);
	}

	nivelNo[h]  = false;
	rotuloNo[h] = rotuloNo[h] + 1;
	xyNo[h].x = p.x; xyNo[h].y = p.y;

	return m;
}


double getTime();

MaxTree::MaxTree(int **vz, int vzAltura, int vzLargura, const Imagem& imag) {
	XY aux;
	int i, j;
	short int minPx, minPy;
	tamvet = 1000; nvet = 0; vet = new Grafo[tamvet]; nvet=0; //2008fev12

	histograma  = new unsigned int[256];
	rotuloNo    = new unsigned int[256];  //vetor de 256 posicoes
	nivelNo     = new bool[256]; //vetor de 256 posicoes
	xyNo        = new XY[256];   //vetor de 256 posicoes

	altura       = imag.altura;
	largura      = imag.largura;

	for (i=0; i < 256; i++) {
		rotuloNo[i]    = 0;
		nivelNo[i]     = false;
		xyNo[i].x = 0; xyNo[i].y = 0;
		histograma[i] = 0;
	}

	maiorGrau   = 0;
	menorValor   = imag.matriz[0][0];
	minPx        = 0;
	minPy        = 0;

	img    = new unsigned char*[altura];
	status = new int*[altura];
	for (i=0; i < altura; i++) {
		img[i]    = new unsigned char[largura];
		status[i] = new int[largura];
		for (j=0; j < largura; j++) {
			img[i][j]    = imag.matriz[i][j];
			//volume      += img[i][j];
			status[i][j] = 0;
			histograma[ imag.matriz[i][j] ]++;
			if (imag.matriz[i][j] < menorValor) {
				menorValor = imag.matriz[i][j];
				minPx = i;
				minPy = j;
			}
		}
	}

	aux.x = minPx;
	aux.y = minPy;

	filas = new FilaHierarquica(histograma, hash);

	filas->insere(menorValor,&aux);

	grafo = NULL;

	alturaVizinho  = vzAltura;
	larguraVizinho = vzLargura;

	int center_x = alturaVizinho  / 2;
	int center_y = larguraVizinho / 2;
	vizinhos = new XY[alturaVizinho*larguraVizinho];
	vizinhos_quantidade = 0;
	for (i=0; i < alturaVizinho; i++)
		for (j=0; j < larguraVizinho; j++) {
			if ((vz[i][j] > 0) && ((i != center_x) || (j != center_y))) {
				vizinhos[vizinhos_quantidade].x = i - center_x;
				vizinhos[vizinhos_quantidade].y = j - center_y;
				vizinhos_quantidade++;
			}
		}


	tamanho = 0;

	nivelNo[menorValor] = true; //2008fev11
	flood(menorValor);

	grafo->filho->area   = altura*largura; //VERIFICAR (area   nao eh calculada corretamente para a raiz) !!!!!

	tamanho_original = tamanho;

	delete rotuloNo;
	delete nivelNo;
	delete xyNo;

	pilha = new PilhaXY(altura*largura);   //pilha estatica usada no retornaRegiao para melhor desempenho
	fila  = new FilaGrafo(tamanho);     //fila  estatica usada no retornaFilhos para melhor desempenho  (tamanho maximo = numero de nos)
}


Lista *MaxTree::retornaFolhas() {
  Grafo *grafoaux = grafo;
  Lista *folhas = new Lista(sizeof(Grafo *));
  while (grafoaux) { //inserção das folhas...
    if (grafoaux->isOn() && retornaFilho(grafoaux)==NULL) //MUDAR DE retornaFilho(grafoaux)==NULL PARA grafoaux->getOrdem()==0 (a ordem nao esta funcionando) !!!!!
      folhas->insere(&grafoaux);
    grafoaux = grafoaux->proximo;
  }
  return folhas;
}
	
int MaxTree::retornaPixel(Pontos pt, int pos) {
  return img[pt.getX(pos)][pt.getY(pos)];
}

int MaxTree::retornaStatus(Pontos pt, int pos) {
  return status[pt.getX(pos)][pt.getY(pos)];
}

Grafo *MaxTree::retornaPai(Grafo *filho) {
  Grafo *pai = NULL;
  if (filho) {
    pai = filho->pai;
    while (pai && !(pai->isOn()))
      pai = pai->pai;
    if (pai && pai->nivel == -1)
      pai = NULL;
  }
  return pai;
}
Grafo *MaxTree::retornaPai(int nivel, unsigned int rotulo) {
  Grafo *filho = buscar(nivel, rotulo);
  return retornaPai(filho);
}

Grafo *MaxTree::retornaFilho(Grafo *pai) {
  Grafo *filho;
  fila->limpa();  //fila e' agora variavel "global" `a classe
  fila->insere(&pai);
  while ( fila->remove(&filho) ) {
    filho = filho->filho;
    while (filho) { // insere filhos na fila
      if (filho->isOn()) 
	return filho;
      fila->insere(&filho);
      filho = filho->irmao;
    }
  }
  return NULL;
}
Grafo *MaxTree::retornaFilho(int nivel, unsigned int rotulo) {
  Grafo *pai = buscar(nivel, rotulo);
  return retornaFilho(pai);
}

Lista *MaxTree::retornaFilhos(Grafo *pai) {  // um noh eh filho se houver um caminho de "pai" ate ele sem passar por nenhum no ativo
	Grafo *filho, *aux;
	Lista *filhos = new Lista(sizeof(Grafo *));
	//Fila  *fila   = new Fila( sizeof(Grafo *), maiorGrau);
	fila->limpa();  //fila e' agora variavel "global" `a classe
	fila->insere(&pai);

	while ( fila->remove(&filho) ) {
		filho = filho->filho;

		while (filho) {

			if (filho->isOn()) {
				aux = filho->pai;
				while (aux && !aux->isOn())
					aux = aux->pai;
				if (aux == pai)
					filhos->insere(&filho);
			}
			else //incluido em 01dez2007
				fila->insere(&filho);
			filho = filho->irmao;
		}
	}
	return filhos;
}

void MaxTree::atualizaAtributos() {
	// INICIALIZACAO DAS ALTURAS E VOLUMES
	Lista *folhas = retornaFolhas();
	Grafo *no = grafo, *filho;
	short int altura_topo, altura = 0;
	unsigned int volume, descendentes;
	unsigned char profundidade = 0, excentricidade;
	maiorGrau = 0;
	
	if ( ! grafo->isOn() ) {
		grafo->setOn();tamanho++;
	}
	
	fila->limpa();
	if (no->nivel == -1) {
		no->visitado = false; no->grau = 0; no->descendentes = 0; no->excentricidade = 0;
		no = no->filho;
	}
	fila->insere(&no);
	while (fila->remove(&no)) {
		no->visitado = false; no->grau = 0; no->descendentes = 0; no->excentricidade = 0;
		no = no->filho;
		while (no) {
			fila->insere(&no);
			no = no->irmao;
		}
	}
	
	while ( folhas->remove(&no) ) {
		no->descendentes   = 0;
		no->excentricidade = 0; //2008mar13
		no->altura = 0;
		no->volume = 0;
		altura_topo = no->nivel;
		filho = no;
		no    = retornaPai(no);
		//no    = no->pai;
		if ( no  &&  ! filho->visitado) {
			no->grau++;
			if (no->grau > maiorGrau)
				maiorGrau = no->grau;
		}
		volume = 0; descendentes = 0, excentricidade = 0;
		while (no) {
			if ( ! filho->visitado ) {
				volume         += filho->area * (filho->nivel-no->nivel);
				descendentes   += 1;
				filho->visitado = true;
			}
			excentricidade   += 1; //2008mar13
			no->volume       += volume;
			no->descendentes += descendentes;
			if (no->excentricidade <= excentricidade) {
				no->excentricidade = excentricidade;
			}
			if (no->altura < altura) {
				altura = altura_topo - no->nivel;
				no->altura = (unsigned char) altura;
			}
			filho = no;
			no    = retornaPai(no);
			//no    =  no->pai;
			if ( no  &&  ! filho->visitado) {
				no->grau++;
				if (no->grau > maiorGrau)
					maiorGrau = no->grau;
			}
		}
	}

	/* inserido em 04out07 (para o matchTree) */
	no = grafo;
	while (no) {
		no->visitado = false;
		no = no->proximo;
	}
}


void MaxTree::destroi() {
	int k;
	for (k=0; k < 257; k++)
		delete hash[k];
	for (k=0; k < altura; k++) {
		delete img[k];
		delete status[k];
	}
	delete img; //2008mar14
	delete status; //2008mar14
	delete histograma; //2008mar14
	delete vizinhos; //2008mar14
	delete pilha; //2008mar14
	delete fila; //2008mar14
	delete filas; //2008mar14
	img = NULL; status = NULL; histograma = NULL; vizinhos = NULL; //2008mar14
	pilha = NULL; fila = NULL; filas = NULL; //2008mar14
}



Imagem MaxTree::retornaImagem(int particao) {
	Imagem g = imZeros(altura,largura);
	Grafo *aux;
	unsigned short int i, j;
	if (particao == 0 && tamanho == tamanho_original) {
		for (i=0; i < altura; i++)
			for (j=0; j < largura; j++)
				g.matriz[i][j] = img[i][j];
	}
	else if (particao == 0) {
		for (i=0; i < altura; i++) {
			for (j=0; j < largura; j++) {
				aux = buscar(img[i][j], status[i][j]);
				if (aux) {
					if ( aux->isOn() ) {
						g.matriz[i][j] = img[i][j];
					}
					else {
						aux = retornaPai(aux);
						if (aux)
							g.matriz[i][j] = aux->nivel;
					}
				}
			}
		}
	}
	else {
		for (i=0; i < altura; i++) {
			for (j=0; j < largura; j++) {
				aux = buscar(img[i][j], status[i][j]);
				if (aux) {
					if ( aux->isOn()  &&  aux->getParticao()==particao ) {
						g.matriz[i][j] = img[i][j];
					}
					else {
						aux = retornaPai(aux);
						while (aux  &&  aux->getParticao()!=particao)
							aux = retornaPai(aux);
						if (aux)
							g.matriz[i][j] = aux->nivel;
					}
				}
			}
		}
	}
	return g;
}


Imagem MaxTree::ext2img() {
	Imagem g = imZeros(altura,largura,1,INTEIRA);
	Grafo *aux;
	int i, j;
	for (i=0; i < altura; i++) {
		for (j=0; j < largura; j++) {
			aux = buscar(img[i][j], status[i][j]);
			if (aux) {
				if (aux->isOn())
					g.matriz[i][j] = aux->getParticao();
				else {
					aux = retornaPai(aux);
					if (aux)
						g.matriz[i][j] = aux->getParticao();
				}
			}
		}
	}
	return g;
}


void MaxTree::zeraParticoes() {
	Grafo *aux = grafo;
	while (aux) {
		aux->setParticao(0);
		aux = aux->proximo;
	}
}


bool MaxTree::inserir(Grafo *p) {
	hash[p->nivel+1][p->rotulo-1] = p;
	return true;
}

Grafo *MaxTree::buscar(int nivel, unsigned int rotulo) {
	return hash[nivel+1][rotulo-1];
}


Grafo *MaxTree::inserir(short int nPai, unsigned short int rPai, XY xyPai, short int nFilho, unsigned short int rFilho, XY xyFilho, XY xymin, XY xymax, XY2 xycen, unsigned int area, unsigned short int *maiorGrau) {  //retorna sempre o primeiro da lista
	Grafo *grafoPai   = grafo;
	Grafo *grafoFilho = grafo;
	Grafo *aux        = grafo;
	unsigned char altura = 0;

	grafoPai   = buscar(nPai,   rPai);
	grafoFilho = buscar(nFilho, rFilho);
	
	if (grafoPai) {

		if (grafoFilho) {

			grafoPai->descendentes += grafoFilho->descendentes + 1;

			if (grafoPai->excentricidade <= grafoFilho->excentricidade) //2008mar13
				grafoPai->excentricidade = grafoFilho->excentricidade + 1;
			
			grafoFilho->irmao = grafoPai->filho;
			grafoFilho->pai   = grafoPai;
			grafoPai->filho   = grafoFilho;

			if (xymin.x < grafoFilho->xmin) grafoFilho->xmin = xymin.x; if (xymin.y < grafoFilho->ymin) grafoFilho->ymin = xymin.y;
			if (xymax.x > grafoFilho->xmax) grafoFilho->xmax = xymax.x; if (xymax.y > grafoFilho->ymax) grafoFilho->ymax = xymax.y;
			if (grafoFilho->xmin < grafoPai->xmin) grafoPai->xmin = grafoFilho->xmin; if (grafoFilho->ymin < grafoPai->ymin) grafoPai->ymin = grafoFilho->ymin;
			if (grafoFilho->xmax > grafoPai->xmin) grafoPai->xmax = grafoFilho->xmax; if (grafoFilho->ymax > grafoPai->ymax) grafoPai->ymax = grafoFilho->ymax;

			grafoFilho->xcen += xycen.x;          grafoFilho->ycen += xycen.y;          //2008jan28
			grafoPai->xcen   += grafoFilho->xcen; grafoPai->ycen   += grafoFilho->ycen; //2008jan28
			grafoFilho->area += area;
			grafoPai->area   += grafoFilho->area;
			grafoPai->volume += grafoFilho->volume + grafoFilho->area * (nFilho-nPai);
			altura = grafoFilho->altura + (nFilho-nPai);
			if (grafoPai->altura < altura)
				grafoPai->altura  = altura;

			grafoPai->grau++;
			if (grafoPai->grau > *maiorGrau)
				*maiorGrau = grafoPai->grau;

			return grafo;
		}
		
		grafoPai->descendentes++;
		aux = grafoPai->pai;
		while (aux) {
			aux->descendentes++;
			aux = aux->pai;
		}

		grafoFilho = inserirNovo(nFilho,rFilho,xyFilho,xymin,xymax,xycen,area);
		grafoFilho->irmao = grafoPai->filho;
		grafoFilho->pai   = grafoPai;
		grafoPai->filho   = grafoFilho;

		if (xymin.x < grafoPai->xmin) grafoPai->xmin = xymin.x; if (xymin.y < grafoPai->ymin) grafoPai->ymin = xymin.y;
		if (xymax.x > grafoPai->xmax) grafoPai->xmax = xymax.x; if (xymax.y > grafoPai->ymax) grafoPai->ymax = xymax.y;

		grafoPai->xcen   += xycen.x; grafoPai->ycen += xycen.y;
		grafoPai->area   += area;
		grafoPai->volume += area * (nFilho-nPai);
		altura = (nFilho-nPai);
		if (grafoPai->altura < altura)
			grafoPai->altura  = altura;

		grafoPai->grau++;
		if (grafoPai->grau > *maiorGrau)
			*maiorGrau = grafoPai->grau;

		return grafoFilho; //ultimo inserido!!
	}

	grafoPai = inserirNovo(nPai,rPai,xyPai,xymin,xymax,xycen,area);  //Verificar 20NOV06 !!!

	if (grafoFilho) {

		grafoPai->descendentes += grafoFilho->descendentes + 1;

		grafoPai->excentricidade = grafoFilho->excentricidade + 1; //2008mar13

		grafoFilho->irmao = grafoPai->filho; //nao precisa!!!
		grafoFilho->pai   = grafoPai;
		grafoPai->filho   = grafoFilho;

		if (xymin.x < grafoFilho->xmin) grafoFilho->xmin = xymin.x; if (xymin.y < grafoFilho->ymin) grafoFilho->ymin = xymin.y;
		if (xymax.x > grafoFilho->xmax) grafoFilho->xmax = xymax.x; if (xymax.y > grafoFilho->ymax) grafoFilho->ymax = xymax.y;
		if (grafoFilho->xmin < grafoPai->xmin) grafoPai->xmin = grafoFilho->xmin; if (grafoFilho->ymin < grafoPai->ymin) grafoPai->ymin = grafoFilho->ymin;
		if (grafoFilho->xmax > grafoPai->xmin) grafoPai->xmax = grafoFilho->xmax; if (grafoFilho->ymax > grafoPai->ymax) grafoPai->ymax = grafoFilho->ymax;

		grafoFilho->xcen += xycen.x;          grafoFilho->ycen += xycen.y;          //2008jan28
		grafoPai->xcen    = grafoFilho->xcen; grafoPai->ycen    = grafoFilho->ycen; //2008jan28
		grafoFilho->area += area;
		grafoPai->area    = grafoFilho->area;
		grafoPai->volume += grafoFilho->volume + grafoFilho->area * (nFilho-nPai);
		grafoPai->altura  = grafoFilho->altura + (nFilho-nPai);

		grafoPai->grau++;
		if (grafoPai->grau > *maiorGrau)
			*maiorGrau = grafoPai->grau;

		return grafoPai;
	}
	//se nao existir nenhum dos dois
	
	grafoPai->descendentes += 1;
	
	grafoPai->excentricidade += 1; //2008mar13

	grafoFilho = inserirNovo(nFilho,rFilho,xyFilho,xymin,xymax,xycen,area);
	grafoFilho->irmao   = grafoPai->filho;  //nao precisa
	grafoFilho->pai     = grafoPai;
	grafoPai->filho     = grafoFilho;
	grafoFilho->proximo = grafoPai;
  
	grafoPai->volume += area * (nFilho-nPai);
	grafoPai->altura  = (nFilho-nPai);

	grafoPai->grau++;
	if (grafoPai->grau > *maiorGrau)
		*maiorGrau = grafoPai->grau;

	return grafoFilho;
}


Grafo *MaxTree::inserirNovo(short int n, unsigned int r, XY xy, XY xymin, XY xymax, XY2 xycen, unsigned int area) {   //insere sempre no inicio da lista
	if (nvet == tamvet-1) { //2008fev12
		vet = new Grafo[tamvet]; nvet = 0;
	}
	Grafo *novo = &vet[nvet]; nvet++; //RETIRAR!!!!! 2008fev12

	novo->visitado= false;  //auxiliar para atualizacao da dinamica
	novo->nivel   = n;
	novo->rotulo  = r;
	novo->x       = xy.x;
	novo->y       = xy.y;
	novo->xmin    = xymin.x;  novo->ymin = xymin.y;
	novo->xmax    = xymax.x;  novo->ymax = xymax.y;
	novo->xcen    = xycen.x;  novo->ycen = xycen.y;
	novo->filho   = NULL;
	novo->pai     = NULL;
	novo->irmao   = NULL;
	novo->proximo = grafo;  //???
	novo->area    = area;
	novo->altura  = 0;
	novo->volume  = 0;
	novo->grau    = 0;
	novo->descendentes   = 0;
	novo->excentricidade = 0;
	novo->setParticao(0);
	novo->setOn();
	tamanho++;
	inserir(novo);
	return novo;
}

//------------------fim---classe Maxtree principal-------------------
