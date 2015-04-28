#include <stdio.h>
#include <string.h>

#include "make.h"
#include "struct.h"
int make_log ( int n, int s, int a, char * nazwa_wyj) {

	FILE * log;
	char sciezka[100];

	sciezka[0] = 0;
	strcat(sciezka, "LOGS/");
	strcat(sciezka, nazwa_wyj);
	strcat(sciezka, ".log");

	log = fopen(sciezka, "w");
	if (log == NULL) {
		printf("BLAD: Nie mozna utworzyc pliku %s.log z ustawieniami wejsciowymi\n", nazwa_wyj);
		return 1;
	} else {
			fprintf(log, "-n %d -s %d", n, s);
		if (s == -2)
			fprintf(log, "-n %d -a %d", n, a);
		printf("INFORMACJA: Utworzono plik %s.log z ustawieniami wejsciowymi.\n", nazwa_wyj);
	}
	fclose(log);	
	return 0;
}

int count_words ( FILE * plik ) {

	int licznik = 0;
	char bufor[100];

	bufor[0] = 0;
	while (fscanf(plik, "%s", bufor) != EOF)
		licznik++;
	return licznik;
}

void druk_strukt( int n, int liczba_ngramow,  struktura_t lista[]) {
	/* drukowanie struktury */

	int i, j;

	printf("\n");
	for (i = 0; i < liczba_ngramow; i++) {
	printf("[");
		for (j = 0; j < n; j++)
			printf(" %s", lista[i].slowo[j]);
		printf(" ]");
		if (j == n)
			printf("\t%d\n", lista[i].powtorzenia);
	}

}

int make_data ( int n, int lin, FILE *in[], char * nazwa_wyj) {
	int slowa;
	int i, liczba_ngramow, ilengramow, j, k, l;
	FILE * data;
	char sciezka[100];
	int wszystkichngramow = 0;



	sciezka[0] = 0;
	for (i = 0; i < lin; i++) {
		rewind(in[i]);
		wszystkichngramow += count_words(in[i]) - n + 1;
		rewind(in[i]);
	}
	struktura_t *lista = malloc(wszystkichngramow * sizeof(struktura_t));
	
	strcat(sciezka, "DATA/");
	strcat(sciezka, nazwa_wyj);
	strcat(sciezka, ".data");
	if ((data = fopen(sciezka, "w")) == NULL) {
		printf("BLAD: Nie mozna utworzyc pliku %s\n", sciezka);
		return 1;
	} else {
		fclose(data);
		if ((data = fopen(sciezka, "w+")) == NULL) {
			printf("BLAD: Nie mozna utworzyc pliku %s\n", sciezka);
			return 1;
		}
		liczba_ngramow = 0;
		ilengramow = 0;
		/* TWORZENIE PLIKU Z N-GRAMAMI */
		j = 0;
		for (i = 0; i < lin; i++) {
			slowa = count_words(in[i]);
			ilengramow = slowa - n + 1;
			liczba_ngramow += slowa - n + 1;
			rewind(in[i]);
			/* printf("\ti = %d, slowa = %d, ilengramow = %d, liczba ngramow = %d, wszystkich ngramow = %d\n", i, slowa, ilengramow, liczba_ngramow, wszystkichngramow); */
			for ( ; j < liczba_ngramow; j++) { /* wpisywanie pierwszych slow dla kazdego n-gramu */
				fscanf(in[i], "%s", lista[j].slowo[0]);
				lista[j].powtorzenia = 1;
				lista[j].r = n;
			}
			j--; /* dopisywanie reszty slow w ostatnim ngramie */
			for (k = 1; k < n; k++) {
				fscanf(in[i], "%s", lista[j].slowo[k]);
			}
			/* dopisywanie brakujacych slow do poprzednich ngramow */
			for (k = 1; k < ilengramow; k++) {
				for (l = 1; l < n; l++) {
					strcat(lista[j-k].slowo[l], lista[j-k+1].slowo[l-1]);
				}
			}
			j++;
		}

		/* sprawdzanie powtarzajacych sie ngramow
		 * jesli ngram sie powtarza: r = 0, powtorzenia = 0 */
		for (i = 0; i < wszystkichngramow; i++) { /*przejscie po ngramach do porownania */
			if (lista[i].powtorzenia > 0) {
				for (j = i + 1; j < wszystkichngramow + 1; j++) { /* przejscie po ngramach ponizej */
					if (lista[j].powtorzenia > 0) {
						lista[j].r = n;
						for (k = n; k == lista[j].r && k > 0; k--) { /* porownywanie konkretnych slow */
							/*printf("k = %d, i = %d, j = %d, lista[j].r = %d\n", k, i, j, lista[j].r);
							printf("   %s - %s\n", lista[i].slowo[k-1], lista[j].slowo[k-1]);*/
							if (strcmp(lista[i].slowo[k-1], lista[j].slowo[k-1]) == 0) {
								lista[j].r--;
							}
						}
						if (lista[j].r == 0) {
							lista[j].powtorzenia = 0;
							lista[i].powtorzenia++;
						}
					}
				}
			}
		}
		/*druk_strukt(n, wszystkichngramow, lista);*/
		for (i = 0; i < liczba_ngramow; i++) {
			if (lista[i].powtorzenia > 0) {
				for (j = 0; j < n; j++)
					fprintf(data, "%s ", lista[i].slowo[j]);
				if (j == n)
					fprintf(data, "%d\n", lista[i].powtorzenia);
			}
		}
	}
	fclose(data);
	printf("INFORMACJA: Utworzono plik %s.data.\n", nazwa_wyj);

	return 0;
}

int stat_wej( int n, int lin, FILE *in[], char * nazwa_wyj, struktura_t lista[], int wszystkichngramow) {

	FILE * wyjscie;
	char sciezka[100];
	char bufor[100];
	int wszystkichslow = 0;
	int i, j, k, l, a, ileslow; /* j - ilosc wczytanych do struktury slow */
	int r; /* czy slowo wystapilo 0-nie, 1-tak */
	int listap[wszystkichngramow];
	double prawdopodobienstwo;

	bufor[0] = 0;
	sciezka[0] = 0;
	strcat(sciezka, "STATS/");
	strcat(sciezka, nazwa_wyj);
	strcat(sciezka, "_in");

	if ((wyjscie = fopen(sciezka, "w")) == NULL) {
		printf("BLAD: Nie mozna utworzyc pliku ze statystykami plikow wejsciowych %s\n", sciezka);
		return 1;
	} else {
		printf("INFORMACJA: Utworzono plik ze statystykami plikow wejsciowych: %s\n", sciezka);
	}

	fprintf(wyjscie, "\tSTATYSTYKI DLA PLIKÓW WEJŚCIOWYCH:\n\n");
	fprintf(wyjscie, "Liczba plików źródłowych: %d.\n", lin);
	for (i = 0; i < lin; i++) {
		rewind(in[i]);
		wszystkichslow += count_words(in[i]);
	}
	fprintf(wyjscie, "Liczba wszystkich słów: %d.\nLiczba wygenerowanych n-gramów: %d.\n\n", wszystkichslow, wszystkichngramow);
	struktura_slow *slowa = malloc(wszystkichngramow * sizeof(struktura_slow));
	for (i = 0; i < wszystkichslow; i++) { /* 'czyszczenie' struktury */
		slowa[i].slowo[0] = 0;
		slowa[i].powtorzenia = 0;
	}
	/* NAJCZESCIEJ WYSTEPUJACE SLOWA */
	j = 0;
	for (i = 0; i < lin; i++) { /* sprawdz kazdy plik */
		rewind(in[i]);
		ileslow = count_words(in[i]);
		rewind(in[i]);
		for (l = 0; l < ileslow; l++) { /* jezeli wczyta slowo */
			fscanf(in[i], "%s", bufor);
			r = 0;
			for (k = 0; k < j && r == 0 ; k++) { /* sprawdz czy juz sie powtarza */
				if (strcmp(bufor, slowa[k].slowo) == 0) { /* jesli slowo sie powtarza*/
					slowa[k].powtorzenia++;
					r = 1;
				}
			}
			if (r == 0) { /* slowo nie wystapilo */
				strcat(slowa[j].slowo, bufor);
				slowa[j].powtorzenia++;
				j++;
			}
		}	
	}

	/* NAJCZESCIEJ WYSTEPUJACE NGRAMY */

	/* kopiowanie listy powtorzen */
	for (k = 0; k < wszystkichngramow; k++) {
		listap[k] = lista[k].powtorzenia;
	}

	if (wszystkichngramow > 9) {
		i = 10;
	} else {
		i = wszystkichngramow;
	}

	fprintf(wyjscie, "Najczęściej powtarzające się ngramy (liczba powtórzeń x n-gram (prawdopodobieństwo wystąpienia, wartość wskaźnika PMI):\n");
	for (k = 0; k < i; k++) {
		l = 0; /* indeks z najwieksza liczba */
		for(r = 0; r < j; r++) {
			if (listap[r] > listap[l]) 
				l = r;
		}
		fprintf(wyjscie, "\t%dx", listap[l]);
		prawdopodobienstwo = 1;
		for (a = 0; a < n; a++) {
			fprintf(wyjscie, " %s", lista[l].slowo[a]);
			for (r = 0; r < wszystkichslow; r++) {
				if((strcmp(lista[l].slowo[a], slowa[r].slowo)) == 0) {
					prawdopodobienstwo *= ((double)slowa[r].powtorzenia/(double)wszystkichslow); /* !!! */
				}
			}
		}
		fprintf(wyjscie, " (%.4f, PMI: %.2f)\n", (double)listap[l]/(double)wszystkichngramow, ((double)listap[l]/(double)wszystkichngramow)/prawdopodobienstwo);
		listap[l] = 0;
	}




	/* drukowanie listy wystepujacych slow i ilosci ich powtorzen
	for (i = 0; i < j; i++) {
		printf("\t%d %s\n", slowa[i].powtorzenia, slowa[i].slowo);
	} */

	if (j > 9) { /* ile slow do statystyk (max 10) */
		i = 10;
	} else {
		i = j;
	}

	fprintf(wyjscie, "\nNajczęściej powtarzające się słowa (liczba powtórzeń x słowo (prawdopodobieństwo wystąpienia)):\n");
	for (k = 0; k < i; k++) {
		l = 0; /* indeks z najwieksza liczba */
		for(r = 0; r < j; r++) {
			if (slowa[r].powtorzenia > slowa[l].powtorzenia) 
				l = r;
		}
		fprintf(wyjscie, "\t%dx %s (%.4f)\n", slowa[l].powtorzenia, slowa[l].slowo, (double)slowa[l].powtorzenia/(double)wszystkichslow);
		slowa[l].powtorzenia = 0;
	}


	return 0;
}

int stat_wyj(FILE *out, char * nazwa_wyj) {

	FILE * wyjscie;
	char sciezka[100];
	char bufor[100];
	int wszystkichslow = count_words(out);
	int i, j, k, l, ileslow; /* j - ilosc wczytanych do struktury slow */
	int r; /* czy slowo wystapilo 0-nie, 1-tak */

	bufor[0] = 0;
	sciezka[0] = 0;
	strcat(sciezka, "STATS/");
	strcat(sciezka, nazwa_wyj);
	strcat(sciezka, "_out");

	rewind(out);
	wszystkichslow = count_words(out);
	if ((wyjscie = fopen(sciezka, "w")) == NULL) {
		printf("\nBLAD: Nie mozna utworzyc pliku ze statystykami pliku wyjsciowego %s\n", sciezka);
		return 1;
	} else {
		printf("\nINFORMACJA: Utworzono plik ze statystykami pliku wyjsciowego: %s\n", sciezka);
	}

	fprintf(wyjscie, "\tSTATYSTYKI DLA PLIKU WYJŚCIOWEGO:\n\n");

	fprintf(wyjscie, "Liczba wszystkich wygenerowanych słów: %d.\n\n", wszystkichslow);
	struktura_slow *slowa = malloc(wszystkichslow * sizeof(struktura_slow));
	for (i = 0; i < wszystkichslow; i++) { /* 'czyszczenie' struktury */
		slowa[i].slowo[0] = 0;
		slowa[i].powtorzenia = 0;
	}
	/* NAJCZESCIEJ WYSTEPUJACE SLOWA */
	j = 0;

	rewind(out);
	ileslow = count_words(out);
	rewind(out);
	for (l = 0; l < ileslow; l++) { /* jezeli wczyta slowo */
		fscanf(out, "%s", bufor);
		r = 0;
		for (k = 0; k < j && r == 0 ; k++) { /* sprawdz czy juz sie powtarza */
			if (strcmp(bufor, slowa[k].slowo) == 0) { /* jesli slowo sie powtarza*/
				slowa[k].powtorzenia++;
				r = 1;
			}
		}
		if (r == 0) { /* slowo nie wystapilo */
			strcat(slowa[j].slowo, bufor);
			slowa[j].powtorzenia++;
			j++;
		}
	}	

	/* drukowanie listy wystepujacych slow i ilosci ich powtorzen
	for (i = 0; i < j; i++) {
		printf("\t%d %s\n", slowa[i].powtorzenia, slowa[i].slowo);
	} */

	if (j > 9) { /* ile slow do statystyk (max 10) */
		i = 10;
	} else {
		i = j;
	}

	fprintf(wyjscie, "Najczęściej powtarzające się słowa (liczba powtórzeń x słowo):\n");
	for (k = 0; k < i; k++) {
		l = 0; /* indeks z najwieksza liczba */
		for(r = 0; r < j; r++) {
			if (slowa[r].powtorzenia > slowa[l].powtorzenia) 
				l = r;
		}
		fprintf(wyjscie, "\t%dx %s\n", slowa[l].powtorzenia, slowa[l].slowo);
		slowa[l].powtorzenia = 0;
	}

	return 0;
}
