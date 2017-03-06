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

#include <math.h>
#include "mm.h"



Pontos::Pontos(const int& tamanho) {
	tam_max = tamanho;
	a = new int[tam_max];
	b = new int[tam_max];
	tam = 0;
	altura  = 0;
	largura = 0;
}
void Pontos::insere(const int& x, const int& y) {
	int m, n;
	if (tam == tam_max)
		return;
	a[tam] = x;
	b[tam] = y;
	tam++;
	m = (int) 2*abs(x) + 1;
	n = (int) 2*abs(y) + 1;
	if (m > altura)  altura  = m;
	if (n > largura) largura = n;
}
int Pontos::getX(const int& pos) {
	return a[pos];
}
int Pontos::getY(const int& pos) {
	return b[pos];
}
Pontos& Pontos::operator=(const Pontos& p) {
	for (int k=0; k < p.tam; k++) {
		a[k] = p.a[k];
		b[k] = p.b[k];
	}
	tam_max = p.tam_max;
	tam     = p.tam;
    return *this;
}
void Pontos::destroi() {
	delete a;
	delete b;
}	
Pontos::~Pontos() {
//	if (a != NULL) {
//		delete a;
//		delete b;
//		a = b = NULL;
//		tam_max = tam = 0;
//	}
}





Imagem::Imagem(const int& imAltura, const int& imLargura, const int& imProfundidade, const int& valor, const int& tipo) {
	int i, j;
	//????? printf("Constructor (from whd): %08X\n", this);
	altura       = imAltura;
	largura      = imLargura;
	profundidade = imProfundidade;
	//--------aloca matriz de pixels--------
	matriz = new int*[altura];
	for (i=0; i<altura; i++) {
		matriz[i] = new int[largura];
		for (j=0; j<largura; j++)
			matriz[i][j] = valor;
	}
	//--------------------------------------
	bits = tipo;
}

// le imagem de um arquivo
Imagem::Imagem(char *nomeArquivo) {
    //????? printf("Constructor (from file): %08X\n", this);
	if (nomeArquivo == NULL) {
		bits = NULA;
		return;
	}
#ifdef OPENCV
	IplImage *x = cvLoadImage(nomeArquivo, 0);
	if (!x) {
		fprintf(stderr, "Não foi possível abrir o arquivo %s\n", nomeArquivo);
		exit(-1);
	}
	int i, j;
	altura  = x->height;
	largura = x->width;
    uchar *data = (uchar *)x->imageData;
    int step = x->widthStep;

	//--------aloca matriz de pixels--------
	matriz = new int*[altura];
	for (i=0; i<altura; i++) {
		matriz[i] = new int[largura];
		for (j=0; j < largura; j++) {
            matriz[i][j] = (int)data[j];
        }
        data += step;
    }
	cvReleaseImage(&x);
    profundidade = 1;
	bits = CINZA;
#else
	FILE *fp;
	int valor, MAX = INFINITO;
	int h = 0, w = 0;
#ifdef WIN32
		fopen_s(&fp, nomeArquivo,"rb");
#else
		fp = fopen(nomeArquivo,"rb");
#endif
	if (fp==NULL) { 
		fprintf(stderr, "Não foi possível abrir o arquivo %s\n", nomeArquivo);
		exit(-1);
	}
	fscanf(fp, "%s", formato); //P5 -> int   |  P2 -> char
	if((strcmp(formato,"P5") != 0) && (strcmp(formato,"P2") != 0)) {
		fprintf(stderr, "Arquivo P5 ou P2 esperado! \n");
		exit(-1);
	}
	//-------leitura dos comentarios e largura, altura e MAX------
	int i=fgetc(fp);
	while(isspace(i)) i=fgetc(fp);
	if(i=='#')  {
		while(i!=10) i=fgetc(fp);
		fscanf(fp,"%d%d%d",&largura,&altura,&MAX);
	}
	else {
		rewind(fp);
		fscanf(fp,"%s%d%d%d",formato,&largura,&altura,&MAX);
	}
	fgetc(fp);
	//--------aloca matriz de pixels--------
	matriz = new int*[altura];
	for (i=0; i<altura; i++)
		matriz[i] = new int[largura];
	//--------------------------------------
	if(strcmp(formato,"P5") == 0) {
		valor=fgetc(fp);
		while(!feof(fp)) {
			matriz[h][w] = valor;
			valor=fgetc(fp);
			w++;
			if(w == largura) { w = 0; h++; }
		}
	}
	else {  //P2
		fscanf(fp,"%d",&valor);
		while(!feof(fp)) {
			matriz[h][w] = valor;
			fscanf(fp,"%d",&valor);
			w++;
			if(w == largura) { w = 0; h++; }
		}
	}
	if (MAX == 255) bits = CINZA; else if (MAX == 1) bits = BINARIA; else bits = INTEIRA;
	profundidade = 1;
#endif
}

Imagem::Imagem(const Imagem& f) {
	int i, j;
    //????? printf("Constructor (copy): %08X\n", this);
	bits = f.bits;
    altura = f.altura;
    largura = f.largura;
    profundidade = f.profundidade;
	matriz = new int*[altura];
	for (i=0; i<altura; i++) {
		matriz[i] = new int[largura];
		for (j=0; j < largura; j++) {
			matriz[i][j] = f.matriz[i][j];
        }
    }
}

Imagem::~Imagem() {
    //????? printf("Destructor: %08X\n", this);
	if (matriz != NULL) {
		for (int i=0; i<altura; i++)
			delete matriz[i];
		delete matriz;
		matriz = NULL;
	}
}

Imagem& Imagem::operator=(const Imagem& f) {
	///* VERIFICAR !!!!!
	int i, j;
	bits = f.bits;
	altura = f.altura;
	largura = f.largura;
	profundidade = f.profundidade;
	//matriz = new int*[altura];
	for (i=0; i<altura; i++) {
		//matriz[i] = new int[largura];
		for (j=0; j < largura; j++) {
			matriz[i][j] = f.matriz[i][j];
		}
	}
	/// VERIFICAR !!!!! */
	return *this;
}



// escreve imagem de uma matriz para arquivo
void Imagem::write(char *image_filename) {
	if (strstr(image_filename,".pgm") || strstr(image_filename,".PGM")) {
		FILE *fp;
		int a, b, i, MAX = INFINITO;
		if (bits == CINZA) MAX = 255; else if (bits == BINARIA) MAX = 1;
#ifdef WIN32
		fopen_s(&fp, image_filename,"w+b");
#else
		fp = fopen(image_filename,"w+b");
#endif
		if (fp==NULL) {
			fprintf(stderr, "Não foi possível abrir o arquivo %s\n", image_filename);
			exit(-1);
		}
		if (MAX > 255)
			i = fputs("P2\n", fp);
		else
			i = fputs("P5\n", fp);
		i = fputs("#", fp);
		i = fputs(" Created", fp);
		i = fputs(" by", fp);
		i = fputs(" LAPIS/UDESC\n", fp);
		fprintf(fp, "%d %d\n", largura,altura);
		fprintf(fp, "%d\n", MAX);
		if (MAX > 255) {
			for(a=0; a < altura; a++) {
				for(b=0; b < largura; b++) {
					i = fprintf(fp, "%i ",matriz[a][b]);
				}
			}
		}
		else {
			for(a=0; a < altura; a++) {
				for(b=0; b < largura; b++) {
					i = fputc(matriz[a][b], fp);
				}
			}
		}
		fclose(fp);
	}
#ifdef OPENCV
	else {
		IplImage *x = cvCreateImage( cvSize(largura,altura), IPL_DEPTH_8U, 1);
		for (int i=0; i < altura; i++) 
		for (int j=0; j < largura; j++) 
		CV_IMAGE_ELEM(x, uchar, i, j) = (uchar) matriz[i][j];
		cvSaveImage(image_filename, x);
		cvReleaseImage(&x);
	}
#endif
#ifndef OPENCV
	else {
		printf("Formato de imagem não suportado.\n");
	}
#endif
}
void Imagem::zera() {
	int i, j;
	for (i=0; i < altura; i++)
		for (j=0; j < largura; j++)
			matriz[i][j] = 0;
}
bool Imagem::ehNula() {
	return (bits == NULA);
}
bool Imagem::ehBinaria() {
	return (bits == BINARIA);
}
bool Imagem::ehCinza() {
	return (bits == CINZA);
}
bool Imagem::ehInteira() {
	return (bits == INTEIRA);
}
void Imagem::comoBinaria() {
	int i, j;
	if (bits != BINARIA) {
		for (i=0; i < altura; i++)
			for (j=0; j < largura; j++)
				if (matriz[i][j] > 0)
					matriz[i][j] = 1;
		else
			matriz[i][j] = 0;
		bits = BINARIA;
	}
}
void Imagem::comoCinza() {
	int i, j;
	if (bits != BINARIA && bits != CINZA) {
		for (i=0; i < altura; i++)
			for (j=0; j < largura; j++)
				if (matriz[i][j] > 255)
					matriz[i][j] = 255;
		else if (matriz[i][j] < 0)
			matriz[i][j] = 0;
	}
	bits = CINZA;
}
void Imagem::asgray() {
	int i, j;
	if (bits != BINARIA && bits != CINZA) {
		for (i=0; i < altura; i++)
			for (j=0; j < largura; j++)
				if (matriz[i][j] > 255)
					matriz[i][j] = 255;
		else if (matriz[i][j] < 0)
			matriz[i][j] = 0;
	}
	bits = CINZA;
}
void Imagem::comoInteira() {
	bits = INTEIRA;
}
void Imagem::exibe(const char *nomeJanela) {
#ifdef OPENCV
	IplImage *x = cvCreateImage( cvSize(largura,altura), IPL_DEPTH_8U, 1);

    char *data = x->imageData;
	for (int i=0; i < altura; i++) {
        for (int j=0; j < largura; j++) {
            int p = matriz[i][j];
            data[j] = (uchar)p;
            // CV_IMAGE_ELEM(x, uchar, i, j) = (uchar) matriz[i][j];
        }
        data += x->widthStep;
    }
	cvNamedWindow(nomeJanela, CV_WINDOW_AUTOSIZE);
	cvShowImage(nomeJanela, x);
	cvWaitKey();
	cvReleaseImage(&x);
#endif
#ifndef OPENCV
	printf("Visualização não suportada!\n");
#endif
}

void Estruturante::imagem(const Imagem& f) {
	int i, j;
	int ic = f.altura  / 2;
	int jc = f.largura / 2;
	img = new Imagem(f.altura, f.largura, f.profundidade, 0, f.bits);
	viz = new Pontos(imArea(f));
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++) {
		img->matriz[i][j] = f.matriz[i][j];
		if (f.matriz[i][j] > 0)
			viz->insere(i - ic, j - jc);
		}
		tam = viz->tam;
		altura  = f.altura;
		largura = f.largura;
}
void Estruturante::pontos(Pontos p) {
	int k, a, b;
	int ic = p.altura  / 2;
	int jc = p.largura / 2;
	Imagem f(p.altura, p.largura, 1);
	for (k=0; k < p.tam; k++) {
		a = p.getX(k);
		b = p.getY(k);
		f.matriz[ic + a][jc + b] = 1;
	}
	imagem(f);
}
Estruturante::Estruturante(char *tipo, const int& raio, float angulo) {
	if (strcmp(tipo,"caixa")==0) {
		int dim = 2*raio+1;
		Imagem f(dim, dim, 1, 1);
		imagem(f);
	}
	else if (strcmp(tipo,"disco")==0) {
		int i, j, dim = 2*raio+1;
		Imagem f(dim, dim, 1);
		for (i=0; i<dim; i++)
			for (j=0; j<dim; j++)
				f.matriz[i][j] = ( (pow(1.*i-raio,2) + pow(1.*j-raio,2)) <= pow(raio+0.5,2) );
		imagem(f);
	}
	else if (strcmp(tipo,"linha")==0) {
		angulo  = (float) (angulo * PI/180);  //transf. para radianos
		float seno, cosseno;
		int i, j;
		int m = (int) ceil(raio * fabs(sin(angulo)) - 0.5);
		int n = (int) ceil(raio * fabs(cos(angulo)) - 0.5);
		if (m == 0) m = 1; if (n == 0) n = 1;
		Imagem f(m, n, 1);
		if (sin(angulo) < 0)
			angulo = (float) (angulo + PI);
		seno    = sin(angulo);
		cosseno = cos(angulo);	
		if (seno >= 0 && seno <= sqrt(2.)/2+EPS) {
			if (cosseno < 0) {
				for (j=0; j<n; j++) {
					i = (int) ceil(1.*(m-1)/(n-1+EPS)*j -0.5);
					f.matriz[i][j] = 1;
				}
			}
			else {
				for (j=0; j<n; j++) {
					i = (int) ceil(1.*(1-m)/(n-1+EPS)*j + (m-1) - 0.5);
					f.matriz[i][j] = 1;
				}
			}
		}
		else if (seno > sqrt(2.)/2 && seno <= 1) {
			if (cosseno < 0) {
				for (i=0; i<m; i++) {
					j = (int) ceil(1.*(n-1)/(m-1+EPS)*i - 0.5);
					f.matriz[i][j] = 1;
				}
			}
			else {
				for (i=0; i<m; i++) {
					j = (int) ceil(1.*(n-1)/(1-m+EPS)*i + (n-1) - 0.5);
					f.matriz[i][j] = 1;
				}
			}
		}
		imagem(f);
	}
	else { // "cruz"
		int i, j, dim = 2*raio+1;
		Imagem f(dim, dim, 1);
		for (i=0; i<dim; i++)
			for (j=0; j<dim; j++)
				f.matriz[i][j] = ( (abs(raio-i) + abs(raio-j)) <= raio);
		imagem(f);
	}
}
Estruturante::Estruturante(const Imagem& f) {
	imagem(f);
}
Estruturante::Estruturante(const Pontos& p) {
	pontos(p);
}
int Estruturante::vizX(const int& pos) {
	return viz->getX(pos);
}
int Estruturante::vizY(const int& pos) {
	return viz->getY(pos);
}
void Estruturante::destroi() {
	if (img != NULL) {
		delete img; //2008mar14
		tam = 0;
	}
	if (viz != NULL) {  //2008mar14
		viz->destroi();
		delete viz;
	}
}


// ELEMENTOS ESTRUTURANTES
Estruturante eeImagem(const Imagem& f) {
	Estruturante e(f);
	return e;
}
Estruturante eeCruz(const int& raio) {
	Estruturante e("cruz",raio);
	return e;
}
Estruturante eeCaixa(const int& raio) {
	Estruturante e("caixa",raio);
	return e;
}
Estruturante eeDisco(const int& raio) {
	Estruturante e("disco",raio);
	return e;
}
Estruturante eeLinha(const int& tamanho, const float& angulo) {
	Estruturante e("linha",tamanho,angulo);
	return e;
}
Estruturante eeUniao(const Estruturante& e1, const Estruturante& e2) {
	int k, ic, jc, m = e1.altura, n = e1.largura;
	if (e2.altura  > e1.altura ) m = e2.altura;
	if (e2.largura > e1.largura) n = e2.largura;
	Imagem aux(m, n, 1);
	ic = m / 2;
	jc = n / 2;
	for (k=0; k < e1.viz->tam; k++)
		aux.matriz[ic + e1.viz->getX(k)][jc + e1.viz->getY(k)] = 1;
	for (k=0; k < e2.viz->tam; k++)
		aux.matriz[ic + e2.viz->getX(k)][jc + e2.viz->getY(k)] = 1;
	Estruturante eu(aux);
	return eu;
}
Estruturante eeReflete(const Estruturante& e) {
	int i, j;
	Imagem aux(e.altura, e.largura, 1);
	for (i=0; i < e.altura; i++)
		for (j=0; j < e.largura; j++)
			aux.matriz[i][j] = e.img->matriz[e.altura-1-i][e.largura-1-j];
	Estruturante er(aux);
	return er;
}
Estruturante eeTranslada(const Estruturante& e, const int& x, const int& y) {
	int k;
	Pontos aux(e.viz->tam);
	for (k=0; k < e.viz->tam; k++)
		aux.insere(e.viz->getX(k) + x, e.viz->getY(k) + y);
	Estruturante et(aux);
	return et;
}
Estruturante eeRotaciona(const Estruturante& e, const float& angulo) {
	int   k;
	float x0, x1;
	float a = (float) (angulo * PI/180);
	Pontos aux(e.viz->tam);
	for (k=0; k < e.viz->tam; k++) {
		x0 = e.viz->getY(k)*cos(a) - e.viz->getX(k)*sin(a);
		x1 = e.viz->getY(k)*sin(a) + e.viz->getX(k)*cos(a);
		aux.insere( (int) ((x1+0.5)*(x1>=0) + (x1-0.5)*(x1<0)), 
			    (int) ((x0+0.5)*(x0>=0) + (x0-0.5)*(x0<0)) );
	}
	Estruturante er(aux);
	return er;
}    
Estruturante eeDilata(Estruturante e1, Estruturante e2)  {
	int k;
	Estruturante ed = eeCruz(0);
	for (k=0; k < e2.viz->tam; k++) {
		Estruturante et = eeTranslada(e1, e2.vizX(k), e2.vizY(k));
		ed = eeUniao(ed, et);
	}
	return ed;
}
Estruturante eeSoma(Estruturante e, const int& n) { // n adicoes de Minkowski
	int k;
	if (n == 0)
		return eeCruz(0);
	Estruturante es = e;
	for (k=0; k < n-1; k++)
		es = eeDilata(es, e);
	return es;
}
Estruturante eeDiferenca(const Estruturante& e1, const Estruturante& e2) {  // conjunto(e1) - conjunto(e2)
	int k, ic, jc, m = e1.altura, n = e1.largura;
	if (e2.altura  > e1.altura ) m = e2.altura;
	if (e2.largura > e1.largura) n = e2.largura;
	Imagem aux(m, n, 1);
	ic = m / 2;
	jc = n / 2;
	for (k=0; k < e1.viz->tam; k++)
		aux.matriz[ic + e1.viz->getX(k)][jc + e1.viz->getY(k)] = 1;
	for (k=0; k < e2.viz->tam; k++)
		aux.matriz[ic + e2.viz->getX(k)][jc + e2.viz->getY(k)] = 0;
	Estruturante eu(aux);
	return eu;
}

Estruturante eeHomoA() {
	Imagem a(3,3,1);
	a.matriz[0][0] = 0; a.matriz[0][1] = 0; a.matriz[0][2] = 0;
	a.matriz[1][0] = 0; a.matriz[1][1] = 1; a.matriz[1][2] = 0;
	a.matriz[2][0] = 1; a.matriz[2][1] = 1; a.matriz[2][2] = 1;
	Estruturante ea(a);
	return ea;
}    
Estruturante eeHomoB() {
	Imagem b(3,3,1);
	b.matriz[0][0] = 1; b.matriz[0][1] = 1; b.matriz[0][2] = 1;
	b.matriz[1][0] = 0; b.matriz[1][1] = 0; b.matriz[1][2] = 0;
	b.matriz[2][0] = 0; b.matriz[2][1] = 0; b.matriz[2][2] = 0;
	Estruturante eb(b);
	return eb;
}
Estruturante eeFimA() {
	Imagem a(3,3,1);
	a.matriz[0][0] = 0; a.matriz[0][1] = 0; a.matriz[0][2] = 0;
	a.matriz[1][0] = 0; a.matriz[1][1] = 1; a.matriz[1][2] = 0;
	a.matriz[2][0] = 0; a.matriz[2][1] = 0; a.matriz[2][2] = 0;
	Estruturante ea(a);
	return ea;
}    
Estruturante eeFimB() {
	Imagem b(3,3,1);
	b.matriz[0][0] = 0; b.matriz[0][1] = 0; b.matriz[0][2] = 0;
	b.matriz[1][0] = 1; b.matriz[1][1] = 0; b.matriz[1][2] = 1;
	b.matriz[2][0] = 1; b.matriz[2][1] = 1; b.matriz[2][2] = 1;
	Estruturante eb(b);
	return eb;
}

// LEITURA DE UMA IMAGEM E A TRANSFORMACAO PARA NIVEIS DE CINZA
Imagem imLe(char *nomeArquivo) {
	return Imagem(nomeArquivo);
};
Imagem imRead(char *image_filename) {
	return Imagem(image_filename);
};

// CLONE DE UMA IMAGEM
Imagem imClone(const Imagem& f) {
	int i, j;
	Imagem g(f.altura, f.largura, f.profundidade, 0, f.bits);
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = f.matriz[i][j];
	return g;
}

// CRIACAO DE UMA IMAGEM COM VALOR CONSTANTE
Imagem imConstante(const int& altura, const int& largura, const int& profundidade, const int& valor, const int& tipo) {
	Imagem f(altura, largura, profundidade, valor, tipo);
	return f;
}

// CRIACAO DE UMA IMAGEM COM ZEROS
Imagem imZeros(const int& altura, const int& largura, const int& profundidade, const int& tipo) {
	Imagem f(altura, largura, profundidade, 0, tipo);
	return f;
}

// TRANSFORMACAO DE IMAGEM PARA BINARIA (BIT) OU PARA NIVEIS DE CINZA (BYTE)
Imagem imBinaria(const Imagem& f) {
	Imagem g = imClone(f);
	if (!g.ehBinaria())
		g.comoBinaria();
	return g;
}
Imagem imCinza(const Imagem& f) {
	Imagem g = imClone(f);
	if (!g.ehCinza())
		g.comoCinza();
	return g;
}
Imagem imInteira(const Imagem& f) {
	Imagem g = imClone(f);
	if (!g.ehInteira())
		g.comoInteira();
	return g;
}

Imagem imNula(void) {
	Imagem f(NULL);
	return f;
}


// NORMALIZACAO
Imagem imNormaliza(const Imagem& f, const int& inf, const int& sup) {
	int i, j, d1, d2, MIN, MAX;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, f.bits);
	MIN = imMinimo(f);
	MAX = imMaximo(f);
	d1 = MAX - MIN;
	d2 = sup - inf;
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = (int) ( inf + 1.0*(f.matriz[i][j] - MIN) * d2 / d1  + 0.5 );
	return g;
}
Imagem imNormalize(const Imagem& f, const int& inf, const int& sup) {
	int i, j, d1, d2, MIN, MAX;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, f.bits);
	MIN = imMinimo(f);
	MAX = imMaximo(f);
	d1 = MAX - MIN;
	d2 = sup - inf;
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = (int) ( inf + 1.0*(f.matriz[i][j] - MIN) * d2 / d1  + 0.5 );
	return g;
}

// QUANTIDADE DOS PIXELS DIFERENTES DE ZERO
int imArea(const Imagem& f) {
	int i, j, area=0;
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] != 0)
				area++;
	return area;
}

// MAXIMO E MINIMO DA IMAGEM/ENTRE IMAGEM E CONSTANTE/ENTRE IMAGENS
int imMaximo(const Imagem& f) {
	int i, j, maximo=-INFINITO;
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] > maximo)
				maximo = f.matriz[i][j];
	return maximo;
}
Imagem imMaximo(const Imagem& f, const int& c) {
	Imagem fc = imConstante(f.altura, f.largura, f.profundidade, c, f.bits);
	Imagem g  = imMaximo(f, fc);
	return g;
}
Imagem imMaximo(const Imagem& f1, const Imagem& f2) {
	int i, j;
	Imagem g = imZeros(f1.altura, f1.largura, f1.profundidade, f1.bits);
	for (i=0; i<f1.altura; i++)
		for (j=0; j<f1.largura; j++)
			if (f1.matriz[i][j] >= f2.matriz[i][j])
				g.matriz[i][j] = f1.matriz[i][j];
	else
		g.matriz[i][j] = f2.matriz[i][j];
	return g;
}
int imMinimo(const Imagem& f) {
	int i, j, minimo=INFINITO;
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] < minimo)
				minimo = f.matriz[i][j];
	return minimo;
}
Imagem imMinimo(const Imagem& f, const int& c) {
	Imagem fc = imConstante(f.altura, f.largura, f.profundidade, c, f.bits);
	Imagem g  = imMaximo(f, fc);
	return g;
}
Imagem imMinimo(const Imagem& f1, const Imagem& f2) {
	int i, j;
	Imagem g = imZeros(f1.altura, f1.largura, f1.profundidade, f1.bits);
	for (i=0; i<f1.altura; i++)
		for (j=0; j<f1.largura; j++)
			if (f1.matriz[i][j] <= f2.matriz[i][j])
				g.matriz[i][j] = f1.matriz[i][j];
	else
		g.matriz[i][j] = f2.matriz[i][j];
	return g;
}

// LIMIARIZACAO
Imagem imLimiariza(const Imagem& f, const int& inf, const int& sup) {
	int i, j;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, BINARIA);
	for (i=0; i<f.altura; i++)
		for (j=0; j<f.largura; j++)
			if (f.matriz[i][j] >= inf && f.matriz[i][j] <= sup)
				g.matriz[i][j] = 1;
	return g;
}
Imagem imThreshold(const Imagem& f, const int& inf, const int& sup) {
	int i, j;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, BINARIA);
	for (i=0; i<f.altura; i++)
		for (j=0; j<f.largura; j++)
			if (f.matriz[i][j] >= inf && f.matriz[i][j] <= sup)
				g.matriz[i][j] = 1;
	return g;
}



#ifdef OPENCV

Imagem cv2im(IplImage *f) {
  int i, j;
  Imagem g = imZeros(f->height, f->width, 1);
  for (i=0; i < f->height; i++)
    for (j=0; j < f->width; j++)
      g.matriz[i][j] = CV_IMAGE_ELEM(f, uchar, i, j);
  return g;
}

IplConvKernel *eeim2eecv(Estruturante e) {
  int *valores = new int[e.altura*e.largura];
  int kcv=0;
  for (int i=0; i < e.altura; i++)
    for (int j=0; j < e.largura; j++) {
      valores[kcv] = e.img->matriz[i][j];
      kcv++;
    }
  IplConvKernel* ecv = cvCreateStructuringElementEx(e.largura,e.altura,e.largura/2,e.altura/2,CV_SHAPE_CUSTOM,valores);
  return ecv;
}

#endif
