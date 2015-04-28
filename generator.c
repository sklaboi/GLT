/* SZKIELET GENERATORA */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generator.h"
#include "stat.h"
#include "make.h"

int generator ( int n, int s, int a, FILE *in[], FILE *out, FILE *open, int lin, int lopen, char * nazwa_wyj, char * nazwa_wej) {
	
	char sciezka[100];
	char bufor[100];
	FILE * data;
	int i = 1;
	int liczba_ngramow = 0; /*ilosc ngramow */
	int wszystkichngramow = 0;

	for (i = 0; i < lin; i++) {
		rewind(in[i]);
		wszystkichngramow += count_words(in[i]) - n + 1;
		rewind(in[i]);
	}

	if (lopen > 0) {
		sciezka[0] = 0;
		strcat(sciezka, "DATA/");
		strcat(sciezka, nazwa_wej);
		strcat(sciezka, ".data");

		if ((data = fopen(sciezka, "r")) == NULL) {
			printf("BLAD: Nie mozna wczytac pliku z ngramami: %s.\n", sciezka);
			return 1;
		}

		rewind(open);
		wszystkichngramow += count_words(data) - n + 1;
		rewind(open);
	}

	printf("\nMODUL GENERATOR:  n = %d, s = %d, a = %d, lin = %d, lopen = %d, nazwa_wyj = %s\n\n", n, s, a, lin, lopen, nazwa_wyj);

	if (lopen == 0) { /* jesli nie podano pliku do openenia */
		if (make_log(n, s, a, nazwa_wyj) == 1) /* smake log */
			return 1;
		if (make_data( n, lin, in, nazwa_wyj) == 1)
			return 1;
	}

	/* wczytywanie n-gramow z pliku .data do struktury */
	sciezka[0] = 0;
	strcat(sciezka, "DATA/");
	strcat(sciezka, nazwa_wyj);
	strcat(sciezka, ".data");

	if (lopen > 0) {
		sciezka[0] = 0;
		strcat(sciezka, "DATA/");
		strcat(sciezka, nazwa_wej);
		strcat(sciezka, ".data");
		if (make_log(n, s, a, nazwa_wyj) == 1) /* smake log */
			return 1;
		fclose(data);
		if ((data = fopen(sciezka, "r")) == NULL) {
			printf("BLAD: Nie udalo sie wczytac pliku z ngramami %s.data\n", nazwa_wyj);
			return 1;
		}
		wszystkichngramow = 0;
		wszystkichngramow += (count_words(data)/(n+1));
		printf("wszystkich ngramow z wczytywanego pliku: %d\n", wszystkichngramow);
		fclose(data);
	}
	/* mallocowanie pamieci dla struktury */
	struktura_t *lista = malloc(wszystkichngramow * sizeof(struktura_t));
	
	if ((data = fopen(sciezka, "r")) == NULL) {
		printf("BLAD: Nie mozna wczytac pliku z ngramami: %s.\n", sciezka);
		return 1;
	} else {
		printf("INFORMACJA: wczytano plik %s\n", sciezka);

		rewind(data);
		for (i = 0; fscanf(data, "%s", bufor) != EOF; i++) {
			/* WCZYTYWANIE */
			if (i == n) { /*wczytano liczbe*/
				lista[liczba_ngramow].powtorzenia = atoi(bufor);
				liczba_ngramow++;
				i = -1;
				bufor[0] = 0;
			} else {
				lista[liczba_ngramow].slowo[i][0] = 0;
				strcat(lista[liczba_ngramow].slowo[i], bufor);
				bufor[0] = 0;
			}
		}
	}

	if (lopen == 0) {
		if (stat_wej(n, lin, in, nazwa_wyj, lista, wszystkichngramow) == 1)
			return 1;
	}

/*	drukuj_strukt(n, liczba_ngramow, lista);*/

	/* GENEROWANIE TEKSTU!!! */

	if (make_paragraph(n, s, a, liczba_ngramow, lista, nazwa_wyj, out) == 1)
		return 1;
	
	/* makeenie statystyk wyrzucic do funkcji generujacej tekst */
	fclose(out);
	sciezka[0] = 0;
	strcat(sciezka, "OUT/");
	strcat(sciezka, nazwa_wyj);
	if ((out = fopen(sciezka, "r")) == NULL) {
		printf("BLAD: Nie mozna bylo omakeyc wygenerowanego pliku z tekstem w celu umakeenia statystyk.\n");
		return 1;
	}
	if (stat_wyj(out, nazwa_wyj) == 1)
		return 1;

	return 0;
}
