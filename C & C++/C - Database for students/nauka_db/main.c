#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>
#define NAZWA_BAZY "nauka.dat"
#define PRZEDMIOTY "przedmioty.txt"
#define ZAJECIA "zajecia.txt"
#include "nauka_db_h.h"

int main()
{
    char przedmioty[MAX_LP][50];
    int l_przedmiotow=0;
    char zajecia[MAX_LZ][50];
    int l_zajec=0;

    int s=zdefiniowane_przedmioty(przedmioty);
    if(s<0)
    {
        printf(u8"Wystapil blad");
        exit(0);
    }
    else
        l_przedmiotow=s;

    s=zdefiniowane_zajecia(zajecia);
    if(s<0)
    {
        printf(u8"Wystapil blad");
        exit(0);
    }
    else
        l_zajec=s;





    printf("  Zdefiniowane przedmioty:");
    wypisz_przedmioty(l_przedmiotow,przedmioty);
    printf("\n  Zdefiniowane zajecia:");
    wypisz_przedmioty(l_zajec,zajecia);


    int log=0;
    char logimie[20];
    char lognazwisko[30];
    int logid;
    int *log_w=&log;
    int *logid_w=&logid;
    char *logimie_w=logimie;
    char *lognazwisko_w=lognazwisko;

    int opcja;
    do{
    printf("\n0. ZAKONCZ");
    printf("\n1. UTWORZ NOWY REKORD");
    printf("\n2. WYSWIETL BAZE");
    printf("\n3. WYSWIETL REKORDY UCZNIA");
    printf("\n4. EDYTUJ REKORD");
    printf("\n5. USUN REKORD");
    printf("\n6. SORTUJ PO INDEKSIE - na wyjscie");
    printf("\n7. SORTUJ PO INDEKSIE - w pliku");
    printf("\n8. ZESTAWIENIE: PRZEDMIOTY - SREDNI CZAS NAUKI STUDENTOW");
    printf("\n9. ZESTAWIENIE: POSZCZEGOLNE ZAJECIA - SREDNI CZAS POSWIECONY PRZEZ STUDENTOW");
    if(log==0)
        printf("\n10. ZALOGUJ");
    else
    {
        printf("\n10. WYLOGUJ");
        printf(u8"\n \n  Zalogowany student: %d %s %s \n", logid, logimie, lognazwisko );
    }
    printf("\n11. DODAJ PRZEDMIOT");
    printf("\n12. USUN PRZEDMIOT");
    printf("\n13. WYSWIETL PRZEDMIOTY");
    printf("\n14. DODAJ ZAJECIE");
    printf("\n15. USUN ZAJECIE");
    printf("\n16. WYSWIETL ZAJECIA");





    printf("\n \nWybierz numer jednej z powyzszych opcji \n");
    scanf("%d", &opcja);
    fflush(stdin);
    switch(opcja)
    {
        case 1:
            if(!nowy_wpis(log,logid,logimie,lognazwisko, l_przedmiotow, l_zajec, przedmioty, zajecia))
            {
                    printf(u8"Wystapil blad");
                    opcja=0;
            }
            break;
        case 2:
            if(!wyswietl())
            {
                    printf(u8"Wystapil blad");
                    opcja=0;
            }
            break;
        case 3:
            if(!wyswietl_ucznia())
            {
                    printf(u8"Wystapil blad");
                    opcja=0;
            }
            break;
        case 4:
            if(!edytuj_wpis(l_przedmiotow, l_zajec, przedmioty, zajecia))
            {
                printf(u8"Wystapil blad");
                opcja=0;
            }
            break;
        case 5:
            if(!usun_wpis())
            {
                    printf(u8"Wystapil blad");
                    opcja=0;
            };
            break;
        case 6:
            if(!sort_id())
            {
                    printf(u8"Wystapil blad");
                    opcja=0;
            };
            break;
        case 7:
            if(!sort_id_plik())
            {
                    printf(u8"Wystapil blad");
                    opcja=0;
            };
            break;
        case 8:
            if(!sredni_czas(przedmioty, l_przedmiotow, 1))
                {
                        printf(u8"Wystapil blad");
                        opcja=0;
                };
            break;
         case 9:
            if(!sredni_czas(zajecia, l_zajec, 0))
            {
                    printf(u8"Wystapil blad");
                    opcja=0;
            };
            break;
        case 10:
           zaloguj(log_w, logid_w, logimie_w, lognazwisko_w);
            break;
        case 11:
            {
                l_przedmiotow++;
                if(!dodaj_przedmiot(l_przedmiotow, przedmioty))
                {
                    printf(u8"Wystapil blad");
                        opcja=0;
                        l_przedmiotow--;
                }

            }
            break;
        case 12:
            {

                if(!usun_przedmiot(l_przedmiotow, przedmioty))
                {
                    printf(u8"Wystapil blad");
                        opcja=0;

                }
                else l_przedmiotow--;

            }
            break;
        case 13:
            {
                putchar('\n');
                wypisz_przedmioty(l_przedmiotow, przedmioty);
                putchar('\n');
            }
            break;
        case 14:
            {
                l_zajec++;
                if(!dodaj_zajecie(l_zajec, zajecia))
                   {
                       printf(u8"Wystapil blad");
                        opcja=0;
                        l_zajec--;
                   }



            }

            break;
        case 15:
           {

                if(!usun_zajecie(l_zajec, zajecia))
                {
                    printf(u8"Wystapil blad");
                        opcja=0;

                }
                else l_zajec--;

            }
            break;
        case 16:
        {
            putchar('\n');
           wypisz_przedmioty(l_zajec, zajecia);
           putchar('\n');
        }
        break;

    }
    }
    while(opcja!=0);
    return 0;
}
