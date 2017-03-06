/***************************************************************************
 *   Copyright (C) 2008 by Alexandre Gonçalves Silva                       *
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
#include <string.h>


double getTime();




int main(int argc, char **argv) {

	int i, option, threshold;
	char *options[7] = {"height","area","volume","desc","htop","hbbox","wbbox"};
	double t1, t2, t3, t4;

	if (argc < 4  ||  argc > 5) {
		printf("\nUsage:\n    extinction option image_in.pgm image_out.pgm [threshold]\n");
		printf("\nOptions:\n    height\n    area\n    volume\n    desc\n    htop\n    hbbox\n    wbbox\n");
		printf("\nExample:\n    $ ./extinction desc images/tomatoes.pgm images/result.pgm 300\n\n");
		return 0;
	}
	
	for (i=0; i < 7; i++)
		if (strcmp(argv[1],options[i])==0) break;
	option = i;
	if (option == 7) {
		printf("Invalid option.\n");
		return 0;
	}

	Imagem f = imRead(argv[2]);

	t1 = getTime();
	MaxTree *mt = img2tree(f);
	t2 = getTime();
	switch (option) {
		case 0:
			extinction_height(mt);
			break;
		case 1:
			extinction_area(mt);
			break;
		case 2:
			extinction_volume(mt);
			break;
		case 3:
			extinction_desc(mt);
			break;
		case 4:
			extinction_htop(mt);
			break;
		case 5:
			extinction_hbbox(mt);
			break;
		case 6:
			extinction_wbbox(mt);
			break;
	}
	t3 = getTime();
	Imagem out = mt->ext2img();
	t4 = getTime();

	if (argc == 5) {
		threshold = atoi( argv[4] );
		out = imThreshold(out, threshold);
	}

	out.asgray(); out = imNormalize(out);
	out.write(argv[3]);

	printf("\nImage dimensions  \t = %d lines x %d columns\nNumber of pixels \t = %d pixels\nNumber of components \t = %d nodes\n", \
		f.altura, f.largura, f.altura*f.largura, mt->tamanho);
	printf("Construction time \t = %lf seconds\nExtinction time \t = %lf seconds\nReconstruction time \t = %lf seconds\n\n", \
		(t2-t1)/1e3, (t3-t2)/1e3, (t4-t3)/1e3);

	return 0;
}
