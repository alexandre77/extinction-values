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




typedef struct {
	unsigned short int x;
	unsigned short int y;
} XY;
//----------------------classe arvore-------------------
class Grafo {
	private:
		unsigned int        particao;   //rotulo do noh conforme marcadores ... usado tambem como valor de extincao nas folhas
		bool                ativo;      //ativado=1, desativado=0
	public:
		bool                visitado;   //auxiliar para atualizacao da dinamica
		unsigned short int  x,y;        //linha e coluna da semente de reconstrucao
		unsigned short int  xmin,ymin; //2008jan
		unsigned short int  xmax,ymax; //2008jan
		unsigned int        xcen,ycen; //2008jan_
		short int           nivel;
		unsigned short int  rotulo;
		unsigned short int  grau;       //numero de filhos
		unsigned int        area;
		unsigned int        volume;
		unsigned char       altura;
		unsigned int        descendentes;    //numero de descendentes do noh
		unsigned char       excentricidade;  //altura da sub-arvore
		Grafo *pai;
		Grafo *filho;                  //armazena sempre o filho mais novo
		Grafo *irmao;                  //imediatamente mais velho q o noh corrente!!! o filho mais velho nao tem irmao!!
		Grafo *proximo;                //sequencia de construção das regioes!
		bool          isOn();
		void          setOn();
		void          setOff();
		unsigned int  getParticao();
		void          setParticao(unsigned int valor);
		Grafo();
};
//--------------fim------classe arvore------------------






  
 /* LISTA DINAMICA DUPLAMENTE ENCADEADA */

 
#define SIM  1
#define NAO  0
#define INI  0
#define FIM -1
#define MAIOR  1
#define MENOR -1
#define IGUAL  0


class NoLista {
	public:
		void *info;
		NoLista *anterior;
		NoLista *proximo;
		NoLista(unsigned int tamanhoInformacao);
		~NoLista();
};


class Lista {  //DINAMICA (PODE SER USADA COMO PILHA DINAMICA E FILA DINAMICA)
	private:
		unsigned int tamInfo; /* quantidade de bytes do elemento */
		NoLista *inicio;      /* ponteiro para o primeiro noh */
		NoLista *fim;         /* ponteiro para o ultimo noh */
	public:
		int tamanho;          /* numero de elementos */

		/* pos = 0   --> primeira  posicao */
		/* pos = 1   --> segunda   posicao */
		/* ...                             */
		/* pos = -1  --> ultima    posicao */
		/* pos = -2  --> penultima posicao */
		/* ...                             */
		
		Lista(unsigned int tamanhoInformacao);
		int vazia();  /* testa se esta vazia ou nao */
		int empilha(void *novo);
		int desempilha(void *novo);
		int insere(void *novo, int pos = FIM);
		int inserePrioridade(void *novo, int (*compara)(void *e1, void *e2));
		int remove(void *destino, int pos = INI);
		int acessa(void *destino, int pos);
		int troca(void *novo, int pos); //substitui elemento na posicao 'pos' pelo novo
		void concatena(Lista *lista); //acrescenta "lista" no fim ("lista" eh zerada no processo)
		void inverte();
		void ordena(int (*compara)(void *e1, void *e2), int decrescente = SIM);
		void limpa();
		~Lista();
};




class NoListaE {
	public:
		void *info;
		int anterior;
		int proximo;
		
		NoListaE(unsigned int tamanhoInformacao);
		~NoListaE();
};


class ListaE {  //ESTATICA
	private:
		unsigned int tamInfo; /* quantidade de bytes do elemento */
		unsigned int tamVet;  /* tamanho do vetor (quantidade maxima de elementos) */
		int livre;       /* ponteiro para o primeiro noh da lista de posicoes livres */
		int inicio;      /* ponteiro para o primeiro noh */
		int fim;         /* ponteiro para o ultimo noh */
		NoListaE **vet;
	public:
		int tamanho;          /* numero de elementos */

		/* pos = 0   --> primeira  posicao */
		/* pos = 1   --> segunda   posicao */
		/* ...                             */
		/* pos = -1  --> ultima    posicao */
		/* pos = -2  --> penultima posicao */
		/* ...                             */
		
		int aloca();
		void desaloca(int posicao);
		
		ListaE(unsigned int tamanhoInformacao, unsigned int tamanhoVetor);
		int vazia();  /* testa se esta vazia ou nao */
		int empilha(void *novo);
		int desempilha(void *novo);
		int insere(void *novo, int pos = FIM);
		int inserePrioridade(void *novo, int (*compara)(void *e1, void *e2));
		int remove(void *destino, int pos = INI);
		int acessa(void *destino, int pos);
		//void concatena(Lista *lista); //acrescenta "lista" no fim ("lista" eh zerada no processo)
		void limpa();
		~ListaE();
};





class Fila {  //ESTATICA CIRCULAR
	private:
		unsigned int tamInfo;
		unsigned int tamVet;
		int inicio;
		int fim;
		void **vet;
	public:
		int tamanho;
		Fila(unsigned int tamanhoInformacao, unsigned int tamanhoVetor);
		int vazia();
		int insere(void *novo);
		int remove(void *novo);
		int remove();
		int acessa(void *novo);
		void limpa();
		~Fila();
};




class Pilha {  //ESTATICA
	private:
		unsigned int tamInfo;
		unsigned int tamVet;
		int topo;
		void **vet;
	public:
		int tamanho;
		Pilha(unsigned int tamanhoInformacao, unsigned int tamanhoVetor);
		int vazia();
		int empilha(void *novo);
		int desempilha(void *novo);
		int desempilha();
		int acessa(void *novo);
		void limpa();
		~Pilha();
};







/***********************************************************

  OUTRAS LISTAS / FILAS / PILHAS ESPECIFICAS PARA A MAXTREE

************************************************************/


class FilaXY {  //ESTATICA CIRCULAR
	private:
		unsigned int tamVet;
		int inicio;
		int fim;
		XY *vet;
	public:
		int tamanho;
		FilaXY(unsigned int tamanhoVetor);
		int vazia();
		int insere(XY *novo);
		int remove(XY *novo);
		int remove();
		int acessa(XY *novo);
		void limpa();
		~FilaXY();
};
class PilhaXY {  //ESTATICA
	private:
		unsigned int tamVet;
		int topo;
		XY *vet;
	public:
		int tamanho;
		PilhaXY(unsigned int tamanhoVetor);
		int vazia();
		int empilha(XY *novo);
		int desempilha(XY *novo);
		int desempilha();
		int acessa(XY *novo);
		void limpa();
		~PilhaXY();
};
class FilaGrafo {  //ESTATICA CIRCULAR
	private:
		unsigned int tamVet;
		int inicio;
		int fim;
		Grafo **vet;
	public:
		int tamanho;
		FilaGrafo(unsigned int tamanhoVetor);
		int vazia();
		int insere(Grafo **novo);
		int remove(Grafo **novo);
		int remove();
		int acessa(Grafo **novo);
		void limpa();
		~FilaGrafo();
};
