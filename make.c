#include "make.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#include "struct.h"

int make_words( int n, int s, int wszystkich_ngramow, struktura_t lista[], char * nazwa_wyj, FILE * out) {

	int slowa = 0;
	int dop = 0; /* ile jest dopasowan */
	struktura_t *dopasowania = malloc(wszystkich_ngramow * sizeof(struktura_t));
	struktura_slow *ostatni = malloc(100 * sizeof(struktura_slow));
	int los, a, i, j;
	int r;

	if (s > 0) {
	/* losowanie i drukowanie pierwszego ngramu */
	los = random(wszystkich_ngramow);
	for (i = 0; i < n-1; i++) {
		if ((strcmp(nazwa_wyj, "stdout")) == 0)
			printf("%s ", lista[los].slowo[i]);
		fprintf(out, "%s ", lista[los].slowo[i]);
		strcat(ostatni[i].slowo, lista[los].slowo[i]);
		slowa++;
	}
	}

	/* AKCJA! */
	/*a = 1; a = 0 - nie mozna znalezc dopasowania, koniec */
	for (a = 1; slowa < s && a != 0; ) {
		clean_struct_adds(dopasowania, wszystkich_ngramow, n);
		dop = 0;
		for (i = 0; i < wszystkich_ngramow; i++) { /* przejdz po wszystkich ngramach */
			r = 0;
			for (j = 0; j < n-1; j++) { /* sprawdz dopasowania dla kazdego slowa */
				if (strcmp(ostatni[j].slowo, lista[i].slowo[j]) == 0)
					r++;
			}
			if (r == n-1) {/* jesli znaleziono dopasowanie */
				for (j = 0; j < n; j++) /* przepisz caly ngram do listy dopasowan */
					strcat(dopasowania[dop].slowo[j], lista[i].slowo[j]);
				dop++;
			}
		}
		if (dop < 1) {
		/*	printf("\n\t***Nie znaleziono dalszych dopasowan.***\n");*/
			a = 0;
		} else {
			los = random(dop); /* wybierz losowo jedno z pasujacych slow */
			if ((strcmp(nazwa_wyj, "stdout")) == 0)
				printf("%s ", dopasowania[los].slowo[n-1]); /*wydrukuj */
			fprintf(out, "%s ", dopasowania[los].slowo[n-1]); /*wydrukuj */
			slowa++;
			ostatni[n-1].slowo[0] = 0;
			strcat(ostatni[n-1].slowo, dopasowania[los].slowo[n-1]);
			for (i = 1; i < n; i++) {
				ostatni[i-1].slowo[0] = 0;
				strcat(ostatni[i-1].slowo, ostatni[i].slowo);
			}
		}
	}
	if ((strcmp(nazwa_wyj, "stdout")) == 0)
		printf("\n");
	fprintf(out,"\n");
	return 0;

}

int make_paragraph( int n, int s, int a, int wszystkich_ngramow, struktura_t lista[], char * nazwa_wyj, FILE * out) {

	int i;
	if (a == -2)
		a = 1;
	int zachowaj = s;

	srand(time(NULL));

	a++;
	for (i = 0; i < a; i++) {
		s = 0;
		if (i > 0) {
			if ((strcmp(nazwa_wyj, "stdout")) == 0)
				printf("\t");
			fprintf(out, "\t");
			s = zachowaj;
		}
		if (make_words(n, s, wszystkich_ngramow, lista, nazwa_wyj, out) == 1)
			return 1;
	}
	return 0;
}

void clean_struct_adds( struktura_t dopasowania [], int wszystkich_ngramow, int n ) {

	int i, j;

	for (i = 0; i < wszystkich_ngramow; i++) {
		dopasowania[i].powtorzenia = 0;
		for (j = 0; j < n; j++)
			dopasowania[i].slowo[j][0] = 0;
	}
}

int random(int up) {

	return ((int)(((double)rand()/RAND_MAX)*up));
}
