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

#ifdef OPENCV
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#endif


#include <time.h>
 
 
#include <iostream>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lista.h"

#define NULA    -1
#define BINARIA  1
#define CINZA    8
#define INTEIRA 32

#define INFINITO 2147483647
#define PI       3.14159265358979323846
#define EPS      10e-20


typedef struct {
	int x;
	int y;
	int valor;
} Pixel;



class Pontos {
	private:
		int *a;
		int *b;
		int tam_max;
	public:
		int tam;
		int altura;
		int largura;
		Pontos(const int& tamanho);
		void insere(const int& x, const int& y);
		int getX(const int& pos = 0);
		int getY(const int& pos = 0);
		Pontos& operator=(const Pontos& p);
		void destroi();
		~Pontos();
};

 

class Imagem {
	public:
		int   bits; //numero de bits (binaria=1, niveis de cinza=8, ...)
		char  formato[3];
		int   altura;
		int   largura;
		int   profundidade;
		int **matriz;
		Imagem(const int& imAltura, const int& imLargura, const int& imProfundidade, const int& valor = 0, const int& tipo = CINZA);
		Imagem(char *nomeArquivo);
		Imagem(const Imagem& img);
		void write(char *image_filename);
		void lista(void);
		void zera();
		bool ehNula();
		bool ehBinaria();
		bool ehCinza();
		bool ehInteira();
		void comoBinaria();
		void comoCinza();
		void comoInteira();
		void asgray();
		Imagem& operator=(const Imagem& f);
		~Imagem();
		void exibe(const char *nomeJanela = "OpenCV");
		void exibeRGB(const char *nomeJanela = "OpenCV", Imagem R = Imagem(NULL), Imagem G = Imagem(NULL), Imagem B = Imagem(NULL), const char *nomeArquivo = NULL);
};



class Estruturante {
	public:
		Imagem *img;
		Pontos *viz; 
		int     altura;
		int     largura;
		int     tam;
	private:
		void imagem(const Imagem& f);
		void pontos(Pontos p);
	public:
		Estruturante(char *tipo = "cruz", const int& raio = 1, float angulo = 0.0);
		Estruturante(const Imagem& f);
		Estruturante(const Pontos& p);
		int vizX(const int& pos);
		int vizY(const int& pos);
		void destroi();
/*
		Estruturante& operator=(const Estruturante& e);
		~Estruturante();
}
*/
};



Estruturante eeImagem(const Imagem& f);
Estruturante eeCaixa(const int& raio = 1);
Estruturante eeCruz (const int& raio = 1);
Estruturante eeDisco(const int& raio = 3);
Estruturante eeLinha(const int& tamanho = 5, const float& angulo = 0.0);
Estruturante eeUniao(const Estruturante& e1, const Estruturante& e2);
Estruturante eeReflete(const Estruturante& e);
Estruturante eeTranslada(const Estruturante& e, const int& x = 0, const int& y = 0);
Estruturante eeRotaciona(const Estruturante& e, const float& angulo = 45.0); //angulo deve ser multiplo de 45
Estruturante eeDilata(Estruturante e1, Estruturante e2);
Estruturante eeSoma(Estruturante e = eeCruz(), const int& n = 1);
Estruturante eeDiferenca(const Estruturante& e1, const Estruturante& e2);

Estruturante eeHomoA();
Estruturante eeHomoB();
Estruturante eeFimA();
Estruturante eeFimB();

Imagem imNula(void);

Imagem imInsere(const Imagem& f, Pontos p);
Pontos imNaozero(const Imagem& f);
Imagem imNormaliza(const Imagem& f, const int& inf = 0, const int& sup = 255);
Imagem imNormalize(const Imagem& f, const int& inf = 0, const int& sup = 255);

Imagem imAplica(const Imagem& fr, int (* operador)(const Imagem& f));

int imArea(const Imagem& f);
int imPerimetro(const Imagem& f);
int imMaximo(const Imagem& f);
Imagem imMaximo(const Imagem& f, const int& c);
Imagem imMaximo(const Imagem& f1, const Imagem& f2);
int imMinimo(const Imagem& f);
Imagem imMinimo(const Imagem& f, const int& c);
Imagem imMinimo(const Imagem& f1, const Imagem& f2);

Imagem imCentroide(const Imagem& f);
Imagem imRetangulomin(const Imagem& f);
Imagem imCirculomin(const Imagem& f);
Imagem imFechoconvexo(const Imagem& f);

Imagem imSoma(const Imagem& f, const int& c);
Imagem imSoma(const Imagem& f1, const Imagem& f2);

Imagem imSubtrai(const Imagem& f, const int& c);
Imagem imSubtrai(const Imagem& f1, const Imagem& f2);

Imagem imMultiplica(const Imagem& f, const int& c);
Imagem imMultiplica(const Imagem& f1, const Imagem& f2);

Imagem imReflete(const Imagem& f);
Imagem imEscala(const Imagem& f, const int& nova_altura, const int& nova_largura);

Imagem imLe(char *nomeArquivo);
Imagem imRead(char *image_filename);
Imagem imClone(const Imagem& f);
Imagem imConstante(const int& altura, const int& largura, const int& profundidade, const int& valor = 0, const int& tipo = CINZA);
Imagem imZeros(const int& altura, const int& largura, const int& profundidade = 1, const int& tipo = CINZA);

Imagem imBinaria(const Imagem& f);
Imagem imCinza(const Imagem& f);
Imagem imInteira(const Imagem& f);

Imagem imNega(const Imagem& f);
Imagem imLimiariza(const Imagem& f, const int& inf, const int& sup = INFINITO);
Imagem imThreshold(const Imagem& f, const int& inf, const int& sup = INFINITO);

Imagem imContorno(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imDilata(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imErode(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imAbre(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imAbreth(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imFecha(const Imagem& f, const Estruturante& e = eeCruz());

Imagem imGradiente(const Imagem& f, const Estruturante& edil = eeCruz(), const Estruturante& eero = eeCruz());
Imagem imDistancia(const Imagem& f, const Estruturante& e = eeCruz());

Imagem imUniao(const Imagem& f, const int& c);
Imagem imUniao(const Imagem& f1, const Imagem& f2);
Imagem imIntersecao(const Imagem& f, const int& c);
Imagem imIntersecao(const Imagem& f1, const Imagem& f2);

Imagem imReconstroi(const Imagem& f, const Imagem& m, const Estruturante& e = eeCruz());
Imagem imMaxregional(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imHmax(const Imagem& f, const int& h = 1, const Estruturante& e = eeCruz());

Imagem imBorda(const Imagem& f, const int& tam_vert=1, const int& tam_horiz=1, const int& valor_borda = 255, const int& valor_fundo = 0);
Imagem imBuraco(const Imagem& f, const Estruturante& e = eeCruz());

Imagem imAfina(const Imagem& f, const Estruturante& e1 = eeHomoA(), const Estruturante& e2 = eeHomoB(), int n = -1, const float& angulo = 45);
Imagem imEsqueleto(const Imagem& f, const Estruturante& e = eeCruz(), const int& opcao = BINARIA);

Imagem imRotula(const Imagem& f, Estruturante e = eeCruz());

Imagem imTudonada(const Imagem& f, const Estruturante& e1 = eeCruz(), const Estruturante& e2 = eeCruz());
Imagem imTudonadavolta(const Imagem& f, const Estruturante& e1 = eeCruz(), const Estruturante& e2 = eeCruz());

bool imSaoiguais(const Imagem& f1, const Imagem& f2);

Pontos imLimites(Pontos p);
Pontos imLimitescirculo(Pontos p, int *raio);  //devolve coordenadas do centro + raio minimo circunscrito

Imagem imDiques(const Imagem& f, const Imagem& mrotulado, Estruturante e = eeCruz());




#ifdef OPENCV
Imagem cv2im(IplImage *f);
//IplImage *im2cv(Imagem f);
IplConvKernel *eeim2eecv(Estruturante e);
#endif
