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

#include "mm.h"


using std::cout;
using std::endl;
using std::cin;


typedef struct {
	unsigned int x;
	unsigned int y;
} XY2;

class FilaHierarquica {
	private:
		FilaXY *fila[256];
	public:
		FilaHierarquica(unsigned int *histograma, Grafo **hash[257]);
		int vazia(int alvo);
		void insere(int alvo, XY *e);
		int remove(int alvo, XY *e);
		~FilaHierarquica();
};

//----------------------classe Maxtree principal-------------------
class MaxTree {
	public:
		Grafo *vet; unsigned int tamvet; unsigned int nvet; //RETIRAR!!!!! 2008fev12
		unsigned char      **img;  //matriz da imagem
		unsigned short int   altura; //dimensões da imagem
		unsigned short int   largura;
		unsigned short int   maiorGrau; //ordem maxima de um no da max-tree
		short int            menorValor;
		int                **status;
		unsigned int        *rotuloNo;  //vetor de 256 posicoes
		bool                *nivelNo;   //vetor de 256 posicoes
		XY                  *xyNo;      //vetor de 256 posicoes
		unsigned short int   alturaVizinho; //size_y e size_x no codigo python
		unsigned short int   larguraVizinho;
		unsigned int         vizinhos_quantidade; //quantidade de vizinhos
		XY                  *vizinhos;            //deslocamentos para os vizinhos
		Grafo               *grafo; //self.graph = {}
		unsigned int         tamanho;
		unsigned int         tamanho_original;
		Grafo              **hash[257];
		PilhaXY             *pilha; //pilha estatica usada no retornaRegiao para melhor desempenho
		FilaGrafo           *fila;  //fila  estatica usada no retornaFilhos e atualizaAtributos para melhor desempenho  (tamanho maximo = numero de nos)
		unsigned int        *histograma;
		FilaHierarquica     *filas;
		short int flood(short int h);
		MaxTree(int **vz, int vzAltura, int vzLargura, const Imagem& imag);
		Lista *retornaFolhas();
		int  retornaPixel(Pontos pt, int pos);
		int  retornaStatus(Pontos pt, int pos);
		void retornaRegiao(Grafo *g, Imagem *cc);
		void retornaRegiao(int nivel, unsigned int rotulo, Imagem *cc);
		unsigned int  retornaArea(Grafo *g);
		unsigned int  retornaArea(int nivel, unsigned int rotulo);
		Grafo *retornaPai(Grafo *filho);
		Grafo *retornaPai(int nivel, unsigned int rotulo);
		Grafo *retornaFilho(Grafo *pai);
		Grafo *retornaFilho(int nivel, unsigned int rotulo);
		Lista *retornaFilhos(Grafo *pai);
		void atualizaAtributos();
		void atualizaPonteiros();  //reconstroi arvore apenas com nos ativos ("true")
		void lista();
		void lista2();
		void escreve(char *nomeArquivoDot, bool exibeFiltro = true, bool gerarPS = false);  //graphviz: preprocessor for drawing directed graphs ($ dot -Tps grafo.dot -o grafo.ps)
		void escreveTouchGraph(char *nomeArquivoTG = "graph.txt");
		void destroi();
		Imagem retornaImagem(int particao = 0);
		Imagem ext2img();
		void zeraParticoes();
		short int pertencePerimetro(short int n, short int x, short int y);
		bool inserir(Grafo *p);
		Grafo *buscar(int nivel, unsigned int rotulo);
		Grafo *inserir(short int nPai, unsigned short int rPai, XY xyPai, short int nFilho, unsigned short int rFilho, XY xyFilho, XY xymin, XY xymax, XY2 xycen, unsigned int area, unsigned short int *maiorGrau);  //retorna sempre o primeiro da lista
		Grafo *inserirNovo(short int n, unsigned int r, XY xy, XY xymin, XY xymax, XY2 xycen, unsigned int area);   //insere sempre no inicio da lista
};
//------------------fim---classe Maxtree principal-------------------


//----------------- MAIN FUNCTIONS ------------------
MaxTree *img2tree(const Imagem& f, const Estruturante& se = eeCruz());

//----------------- EXTINCTION FUNCTIONS ------------------
void extinction_height(MaxTree *mt);
void extinction_area(MaxTree *mt);
void extinction_volume(MaxTree *mt);
void extinction_desc(MaxTree *mt);
void extinction_htop(MaxTree *mt);
void extinction_hbbox(MaxTree *mt);
void extinction_wbbox(MaxTree *mt);
