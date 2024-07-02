#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>
#define NAZWA_BAZY "nauka.dat"
#define PRZEDMIOTY "przedmioty.txt"
#define ZAJECIA "zajecia.txt"
#define MAX_LP 10
#define MAX_LZ 5

typedef struct baza_danych {
    int id_wpisu;
    int id_studenta;
    char imie[30];
    char nazwisko[30];
    time_t poczatek;
    time_t koniec;
    char przedmiot[50];
    char zajecie[50];
}db;

int liczba_rekordow();
void czas_w_sekundach(float s);
time_t strtotime(char *T);
char* timetostr(time_t time);
bool nowy_wpis(int log, int logid, char *imie, char *nazwisko, int l_przedmiotow, int l_zajec, char (*przedmioty)[50], char (*zajecia)[50]);
bool wyswietl();
bool wyswietl_ucznia();
bool edytuj_wpis(int l_przedmiotow, int l_zajec, char (*przedmioty)[50], char (*zajecia)[50]);
bool usun_wpis();
bool sort_id();
bool sort_id_plik();
bool sredni_czas(char (*przedmioty)[50], int l_przedmiotow, bool q); //q=1 przedmioty
void zaloguj(int *log, int *logid, char *imie, char *nazwisko);
void wypisz_przedmioty(int l_przedmiotow, char (*przedmioty)[50]);
bool usun_przedmiot(int l_przedmiotow, char (*przedmioty)[50]);
bool dodaj_przedmiot(int l_przedmiotow, char (*przedmioty)[50]);
bool dodaj_zajecie(int l_zajec, char (*zajecia)[50]);
bool usun_wpisy_przedmiotu(char *nazwaprzedmiotu);
bool usun_wpisy_zajecia(char *nazwazajecia);
bool usun_zajecie(int l_przedmiotow, char (*przedmioty)[50]);
int zdefiniowane_zajecia(char (*zajecia)[50]);
int zdefiniowane_przedmioty(char (*przedmioty)[50]);
