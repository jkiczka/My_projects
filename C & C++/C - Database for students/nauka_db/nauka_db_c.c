
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


int liczba_rekordow()
{   FILE *fp;
    fp=fopen(NAZWA_BAZY, "r+b");
    fseek(fp,0,SEEK_END);
    int n=ftell(fp)/sizeof(db);
    fclose(fp);
    return n;

}

void czas_w_sekundach(float s)
{
    if(s==0)
        printf("0");
    double minuty, godziny, dni;
    if(s/(60.00*60.00*24)>=1)
       {
           dni=floor(s/(60.00*60.00*24));
            s=s-dni*(60.00*60.00*24);
            if(dni)
                printf("1 dzien ");
            else
                printf("%0.0f dni ",dni);


        }
    if(s/(60.00*60.00)>=1)
        {
            godziny=floor(s/(60.00*60.00));
            s=s-godziny*60.00*60.00;
            printf("%0.0f godz ",godziny);
        }
    if(s/60.00>=1)
    {
        minuty=floor(s/60.00);
        s=s-minuty*60.00;
        printf("%0.0f min ",minuty);

    }
    if (s>0)
        printf("%0.0f sek ",s);


    putchar('\n');



}

time_t strtotime(char *T)
{
    time_t timet=0;

   int year=0,month=0,day=0,hour=0,min=0;

   if (sscanf(T,"%4d.%2d.%2d,%2d:%2d",&year,&month,&day,&hour,&min)==5)
       {
        struct tm breakdown={0};
       breakdown.tm_year=year-1900;
       breakdown.tm_mon=month-1;
       breakdown.tm_mday=day;
       breakdown.tm_hour=hour;
       breakdown.tm_min=min;
       timet=mktime(&breakdown);
       return timet;
       }
    else
        {printf(u8"Niepoprawny format daty");
        return 0;}


}

char* timetostr(time_t time)
{

   char*t=asctime(localtime(&time));
   if(t[strlen(t)-1]=='\n')
    t[strlen(t)-1]='\0';
   return t;
}

bool nowy_wpis(int log, int logid, char *imie, char *nazwisko, int l_przedmiotow, int l_zajec, char (*przedmioty)[50], char (*zajecia)[50])
{
    FILE *fp;
    fp=fopen(NAZWA_BAZY,"a+b");
    if(fp==NULL)
    return 0;

    fseek(fp,0,SEEK_END);

    db *w_nowy;
    w_nowy=malloc(sizeof(db));

    if(w_nowy==NULL)
    return 0;

    int indeks;
        char string [50];
        char string2[50];
        time_t czas;
        time_t czas2;
        w_nowy->id_wpisu=liczba_rekordow()+1;
    if (log)
    {
       w_nowy->id_studenta=logid;
        strcpy(w_nowy->imie,imie);
        strcpy(w_nowy->nazwisko,nazwisko);

    }
    else
    {

         printf("Nr indeksu: ");
        scanf("%d",&indeks);
        w_nowy->id_studenta=indeks;
        fflush(stdin);


        printf("Imie: ");
         scanf("%s",string);
        strcpy(w_nowy->imie,string);
        fflush(stdin);

        printf("Nazwisko: ");
        scanf("%s",string);
        strcpy(w_nowy->nazwisko,string);
        fflush(stdin);
    }

        printf("Poczatek pracy w formacie [rrrr.mm.dd,gg:mm]: ");
        scanf("%s",string);
        czas=strtotime(string);
        while(czas==0)
        {
            printf("Blad wczytywania daty. Sprobuj jeszcze raz \n");
            printf("Poczatek pracy w formacie [rrrr.mm.dd,gg:mm]: \n");
            scanf("%s",string);
            czas=strtotime(string);
            fflush(stdin);
        }
        w_nowy->poczatek=czas;
        fflush(stdin);


        printf("Koniec pracy w formacie [rrrr.mm.dd,gg:mm]: ");
        scanf("%s",string2);

        czas2=strtotime(string2);

        while(czas2==0)
        {
            printf("Blad wczytywania daty. Sprobuj jeszcze raz \n");
            printf("Koniec pracy w formacie [rrrr.mm.dd,gg:mm]: \n");
            scanf("%s",string2);
            czas2=strtotime(string2);
            fflush(stdin);
        }
        w_nowy->koniec=czas2;


        fflush(stdin);

        printf("Przedmiot:");
        wypisz_przedmioty(l_przedmiotow,przedmioty);
        scanf("%d",&indeks);
        while(indeks>l_przedmiotow || indeks<1)
        {
             printf("Wybierz przedmiot z zakresu 1-%d. \n",l_przedmiotow);
             scanf("%d",&indeks);
              fflush(stdin);
        }
        strcpy(w_nowy->przedmiot,przedmioty[indeks]);
        fflush(stdin);

        printf("Zajecie:");
        wypisz_przedmioty(l_zajec, zajecia);
        scanf("%d",&indeks);
        while(indeks>l_zajec || indeks<1)
        {
             printf("Wybierz zajecie z zakresu 1-%d. \n",l_zajec);
             scanf("%d",&indeks);
            fflush(stdin);
        }
        strcpy(w_nowy->zajecie,zajecia[indeks]);
        fflush(stdin);
        fwrite(w_nowy,sizeof(db),1,fp);



fclose(fp);
free(w_nowy);
return 1;

}

bool wyswietl()
{
    db wpis;
    FILE *fp;
    fp=fopen(NAZWA_BAZY, "r+b");
    if(fp==NULL) //pusta baza
       {
          fp=fopen(NAZWA_BAZY, "w+b");
          fclose(fp);
          printf(u8"Baza jest pusta, plik utworzono \n");
       }
    else
    {
        fseek(fp,0,SEEK_END);
        if (0==ftell(fp))
        printf(u8"Plik istnieje, ale baza jest pusta. \n");
    }

    fseek(fp,0,SEEK_SET);
    int czy_dalej=1;
    int licznik=0;
    while(fread(&wpis, sizeof(db),1,fp) && czy_dalej==1)
    {
        printf("\n%d %d %s %s %s ",
               wpis.id_wpisu, wpis.id_studenta, wpis.imie,wpis.nazwisko,\
                timetostr(wpis.poczatek));


                 printf("%s %s %s \n",timetostr(wpis.koniec),
               wpis.przedmiot, wpis.zajecie);

        licznik++;
        if(licznik==20)
        {
            printf(u8"Czy chcesz wyswietlac dalej? \n Jesli tak - wybierz 1, \n jesli nie - wybierz 0 \n");
            scanf("%d", &czy_dalej);
            licznik=0;

        }
    }


    fclose(fp);
    return 1;


}

bool wyswietl_ucznia()
{
    bool found=0;
db wpis;
    FILE *fp;
    fp=fopen(NAZWA_BAZY, "r+b");
    if(fp==NULL)
        return 0;
            fseek(fp,0,SEEK_SET);
    int podaneid;
    printf(u8"Podaj id ucznia, ktorego chcesz wyswietlic \n");
    scanf("%d",&podaneid);
    fflush(stdin);
    while(fread(&wpis, sizeof(db),1,fp))
    {
        if(wpis.id_studenta==podaneid)
        { printf("\n%d %d %s %s %s ",
               wpis.id_wpisu, wpis.id_studenta, wpis.imie,wpis.nazwisko,\
                timetostr(wpis.poczatek));


                 printf("%s %s %s \n",timetostr(wpis.koniec),
               wpis.przedmiot, wpis.zajecie);

        found=1;
        }

    }

    if(found==0)
        printf("Nie znalezniono ucznia o podanym id \n");


    fclose(fp);
    return 1;

}
bool edytuj_wpis(int l_przedmiotow, int l_zajec, char (*przedmioty)[50], char (*zajecia)[50])
{
db wpis;
bool found=0;
    FILE *fp, *fp1;
    fp=fopen(NAZWA_BAZY, "r+b");
    fp1=fopen("temp.dat", "w+b");

    if(fp==NULL)
        return 0;
            fseek(fp,0,SEEK_SET);
    int podaneid;
    printf(u8"Podaj numer wpisu, ktory chcesz edytowac \n");
    scanf("%d",&podaneid);
    fflush(stdin);

    while(fread(&wpis, sizeof(db),1,fp))
    {
        if(wpis.id_wpisu==podaneid)
        {
                     found=1;
                     int indeks;
                     char string[50];
                     char string2[50];
                     time_t czas2;
                     time_t czas;

                        printf("\n%d %d %s %s %s ",
                            wpis.id_wpisu, wpis.id_studenta, wpis.imie,wpis.nazwisko,\
                            timetostr(wpis.poczatek));


                        printf("%s %s %s \n",timetostr(wpis.koniec),
                                wpis.przedmiot, wpis.zajecie);
                        printf("Nowy nr indeksu: ");
                        scanf("%d",&indeks);
                        wpis.id_studenta=indeks;
                        fflush(stdin);


                        printf("Nowe imie: ");
                         scanf("%s",string);
                        strcpy(wpis.imie,string);
                        fflush(stdin);

                        printf("Nowe nazwisko: ");
                        scanf("%s",string);
                        strcpy(wpis.nazwisko,string);
                        fflush(stdin);

                        printf("Nowy poczatek pracy w formacie [rrrr.mm.dd,gg:mm]: ");
                        scanf("%s",string);
                        czas=strtotime(string);
                        while(czas==0)
                        {
                            printf("Blad wczytywania daty. Sprobuj jeszcze raz \n");
                            printf("Poczatek pracy w formacie [rrrr.mm.dd,gg:mm]: \n");
                            scanf("%s",string);
                            czas=strtotime(string);
                            fflush(stdin);
                        }
                        wpis.poczatek=czas;
                        fflush(stdin);


                        printf("Nowy koniec pracy w formacie [rrrr.mm.dd,gg:mm]: ");
                        scanf("%s",string2);
                        czas2=strtotime(string2);
                        while(czas2==0)
                        {
                            printf("Blad wczytywania daty. Sprobuj jeszcze raz \n");
                            printf("Koniec pracy w formacie [rrrr.mm.dd,gg:mm]: \n");
                            scanf("%s",string2);
                            czas2=strtotime(string2);
                            fflush(stdin);
                        }
                        wpis.koniec=czas2;
                        fflush(stdin);

                        printf("Nowy przedmiot:");
                        wypisz_przedmioty(l_przedmiotow,przedmioty);
                        scanf("%d",&indeks);
                        while(indeks>l_przedmiotow || indeks<1)
                        {
                             printf("Wybierz przedmiot z zakresu 1-%d. \n",l_przedmiotow);
                             scanf("%d",&indeks);
                              fflush(stdin);
                        }
                        strcpy(wpis.przedmiot,przedmioty[indeks]);
                        fflush(stdin);

                        printf("Nowe zajecie:");
                        wypisz_przedmioty(l_zajec, zajecia);
                        scanf("%d",&indeks);
                        while(indeks>l_zajec || indeks<1)
                        {
                             printf("Wybierz zajecie z zakresu 1-%d. \n",l_zajec);
                             scanf("%d",&indeks);
                            fflush(stdin);
                        }
                        strcpy(wpis.zajecie,zajecia[indeks]);
                        fflush(stdin);
        }


        fwrite(&wpis,sizeof(db),1,fp1);




    }
    fclose(fp);
    fclose(fp1);

    if(found)
    {
        fp1=fopen("temp.dat", "r+b");
        fp=fopen(NAZWA_BAZY, "w+b");

        if(fp==NULL || fp1==NULL)
            return 0;

        while (fread(&wpis, sizeof(db),1,fp1))
            fwrite(&wpis, sizeof(db),1,fp);
    }
    else
        printf("Nie znaleziono wpisu o podanym numerze");
    fclose(fp);
    fclose(fp1);
    return 1;
}
bool usun_wpis()
{
db wpis;
bool found=0;
    FILE *fp, *fp1;
    fp=fopen(NAZWA_BAZY, "r+b");
    fp1=fopen("temp.dat", "w+b");

    if(fp==NULL)
        return 0;
            fseek(fp,0,SEEK_SET);
    int podaneid;
    printf(u8"Podaj numer wpisu, ktory chcesz usunac \n");
    scanf("%d",&podaneid);
    fflush(stdin);

    while(fread(&wpis, sizeof(db),1,fp))
    {
        if(wpis.id_wpisu==podaneid)
                     found=1;
        else
                    fwrite(&wpis,sizeof(db),1,fp1);
    }
    fclose(fp);
    fclose(fp1);

    if(found)
    {
        fp1=fopen("temp.dat", "r+b");
        fp=fopen(NAZWA_BAZY, "w+b");

        if(fp==NULL || fp1==NULL)
            return 0;

        while (fread(&wpis, sizeof(db),1,fp1))
            fwrite(&wpis, sizeof(db),1,fp);
    }
    else
        printf("Nie znaleziono wpisu o podanym numerze");
    fclose(fp);
    fclose(fp1);
    return 1;
}

//typedef struct baza_danych *wsk_na_baze;
bool sort_id()
{

    db *w_nowy,bazadanych;
    FILE *fp;
    fp=fopen(NAZWA_BAZY, "r+b");
    if(fp==NULL)
        return 0;

    fseek(fp,0,SEEK_END);
    int n=ftell(fp)/sizeof(db);
    w_nowy=(db*)calloc(n,sizeof(db));
    rewind(fp);
    for(int i=0; i<n; i++)
        fread(&w_nowy[i],sizeof(db),1,fp);

//    for(int i=0; i<n; i++)
//        for(int j=i+1;j<n;j++)
//        {
//            if(w_nowy[i].id_studenta>w_nowy[j].id_studenta)
//            {
//                bazadanych=w_nowy[i];
//                w_nowy[i]=w_nowy[j];
//                w_nowy[j]=bazadanych;
//            }
//
//        }

    for (int i=1; i<n; i++)
    {
        int x=w_nowy[i].id_studenta;
            bazadanych=w_nowy[i];
        int j=i-1;
        while (j>=0 && x<w_nowy[j].id_studenta)
        {
            w_nowy[j+1]=w_nowy[j];
            j--;

        }
        w_nowy[j+1]=bazadanych;
    }

    for(int i=0;i<n;i++)
    {
        bazadanych=w_nowy[i];
        printf("\n %d %d %s %s %s %s %s %s",\
               bazadanych.id_wpisu, bazadanych.id_studenta, bazadanych.imie,bazadanych.nazwisko,\
                timetostr(bazadanych.poczatek), timetostr(bazadanych.koniec),\
               (bazadanych.przedmiot), (bazadanych.zajecie));
    }


    fclose(fp);
    free(w_nowy);
    return 1;



}
bool sort_id_plik()
{

    db*w_nowy,bazadanych;
    FILE *fp;
    fp=fopen(NAZWA_BAZY, "r+b");
    if(fp==NULL)
        return 0;

    fseek(fp,0,SEEK_END);
    int n=ftell(fp)/sizeof(db);
    w_nowy=(db*)calloc(n,sizeof(db));
    rewind(fp);
    for(int i=0; i<n; i++)
        fread(&w_nowy[i],sizeof(db),1,fp);

    fclose(fp);

    fp=fopen(NAZWA_BAZY, "w+b");
    if(fp==NULL)
        return 0;

//    for(int i=0; i<n; i++)
//        for(int j=i+1;j<n;j++)
//        {
//            if(w_nowy[i].id_studenta>w_nowy[j].id_studenta)
//            {
//                bazadanych=w_nowy[i];
//                w_nowy[i]=w_nowy[j];
//                w_nowy[j]=bazadanych;
//            }
//
//        }

        for (int i=1; i<n; i++)
    {
        int x=w_nowy[i].id_studenta;
            bazadanych=w_nowy[i];
        int j=i-1;
        while (j>=0 && x<w_nowy[j].id_studenta)
        {
            w_nowy[j+1]=w_nowy[j];
            j--;

        }
        w_nowy[j+1]=bazadanych;
    }

    for(int i=0;i<n;i++)
    {
        bazadanych=w_nowy[i];
        printf("\n%d %d %s %s %s %s %s %s",\
               bazadanych.id_wpisu, bazadanych.id_studenta, bazadanych.imie,bazadanych.nazwisko,\
                timetostr(bazadanych.poczatek), timetostr(bazadanych.koniec),\
               (bazadanych.przedmiot),(bazadanych.zajecie));
        fwrite(&w_nowy[i],sizeof(db),1,fp);
    }


    fclose(fp);
    free(w_nowy);
    return 1;



}

bool sredni_czas(char (*przedmioty)[50], int l_przedmiotow, bool q) //q=1 przedmioty
{
long long int suma[500];//suma
int ile[500];//l.wystapien
char nazwy[500][50];
int ile_roznych=l_przedmiotow;

for (int i=0; i<500; i++)
    suma[i]=ile[i]=0;
for (int i=1; i<=l_przedmiotow; i++)
    strcpy(nazwy[i],przedmioty[i]);

db wpis;

    FILE *fp;
    fp=fopen(NAZWA_BAZY, "r+b");
    if(fp==NULL)
        return 0;
    //fseek(fp,0,SEEK_SET);
    rewind(fp);
    while(fread(&wpis, sizeof(db),1,fp))
    {
        bool znaleziono=0;
        for (int i=1; i<=ile_roznych; i++)
           {

                    if(q==1 && strcmp(nazwy[i],wpis.przedmiot)==0)
                    {
                        znaleziono=1;
                        ile[i]=ile[i]+1;
                        time_t delta=wpis.koniec-wpis.poczatek;
                        //printf("%s ", timetostr(wpis.poczatek));
                        suma[i]=suma[i]+delta;

                    }


                    if(q==0 && strcmp(nazwy[i],wpis.zajecie)==0)
                    {
                        znaleziono=1;
                        ile[i]=ile[i]+1;
                        time_t delta=wpis.koniec-wpis.poczatek;
                        //printf("%s ", timetostr(wpis.poczatek));
                        suma[i]=suma[i]+delta;

                    }



            }

            if(znaleziono==0)
            {
                 //printf("%s", wpis.przedmiot);
                    ile_roznych++;
                    strcpy(nazwy[ile_roznych],wpis.przedmiot);
                    ile[ile_roznych]=ile[ile_roznych]+1;
                    time_t delta=wpis.koniec-wpis.poczatek;
                    suma[ile_roznych]=suma[ile_roznych]+delta;
           }


    }
    fclose(fp);



    for(int i=1; i<=ile_roznych; i++)
    {float srednia;
   // printf("%d %lld\n",ile[i], suma[i]);
        if(ile[i]!=0)
            srednia=suma[i]/ile[i];
        else
            srednia=0.00;
        if(q)
                printf("Przedmiot: %s, \n sredni czas sesji: ",nazwy[i]);
        else
                printf("Aktywnosc: %s, \n sredni czas sesji: ",nazwy[i]);
        czas_w_sekundach(srednia);
        //printf("%f \n",srednia);
    }

    return 1;
}

void zaloguj(int *log, int *logid, char *imie, char *nazwisko)
{
    if (*log==1)
    {
        *log=0;
      printf(u8"Poprawnie wylogowano");
    }
    else
    {
        printf(u8"Podaj swoje dane: \n");

         printf("Nr indeksu: ");
        scanf("%d",logid);
        fflush(stdin);


        printf("Imie: ");
         scanf("%s", imie);
        fflush(stdin);

        printf("Nazwisko: ");
        scanf("%s", nazwisko);
        fflush(stdin);

        *log=1;


        printf(u8"\nPoprawnie zalogowano jako %d %s %s\n", *logid, imie, nazwisko );
        printf(u8"\n Gdy jestes zalogowany, funkcja UTWORZ NOWY REKORD() tworzy rekordy z Twoimi danymi\n");
    }


}

void wypisz_przedmioty(int l_przedmiotow, char (*przedmioty)[50])
{
    //printf(u8"\nLiczba przedmiotow %d \n", l_przedmiotow);
    for (int i=1; i<=l_przedmiotow; i++)
    {
     printf(" %d - %s", i, przedmioty[i]);
    }
    putchar('\n');
}

bool usun_przedmiot(int l_przedmiotow, char (*przedmioty)[50])
{
    if (l_przedmiotow==1)
        printf(u8"Nie mozna usunac wszystkich przedmiotow \n");
    else
    {

    wypisz_przedmioty(l_przedmiotow,przedmioty);
    printf(u8"\n Podaj numer przedmiotu, ktory chcesz usunac \n");
    int p;
    scanf("%d", &p);

    while(p<1 || p>l_przedmiotow)
    {
        scanf("%d", &p);
        printf(u8"Niepoprawny numer przedmiotu. Podaj numer z zakresu 1-%d \n", l_przedmiotow);
    }
    char nazwaprzedmiotu[50];
    strcpy(nazwaprzedmiotu,przedmioty[p]);

    for (int i=p; i<l_przedmiotow; i++)
    {
        strcpy(przedmioty[i],przedmioty[i+1]);

    }


    //usun z pliku
    FILE *fp1=fopen(PRZEDMIOTY, "w");
    if (fp1==NULL)
        return 0;
    for (int i=1; i<=l_przedmiotow-1; i++)
    {
        fputs(przedmioty[i],fp1);
        fputs(" ",fp1);
    }

    fclose(fp1);

    printf(u8"\n Czy chcesz usunac wszystkie wpisy zawierajace ten przedmiot? \n");
    printf(u8"\n Jesli tak - wybierz 1, jesli nie - wybierz 0 \n");
    int czy;
    scanf("%d", &czy);
    if (czy)
        {
            if(!usun_wpisy_przedmiotu(nazwaprzedmiotu))
                return 0;
        }


    }

     return 1;
}

bool usun_zajecie(int l_przedmiotow, char (*przedmioty)[50])
{
    if (l_przedmiotow==1)
        printf(u8"Nie mozna usunac wszystkich przedmiotow \n");
    else
    {

    wypisz_przedmioty(l_przedmiotow,przedmioty);
    printf(u8"\n Podaj numer przedmiotu, ktory chcesz usunac \n");
    int p;
    scanf("%d", &p);

    while(p<1 || p>l_przedmiotow)
    {
        scanf("%d", &p);
        printf(u8"Niepoprawny numer przedmiotu. Podaj numer z zakresu 1-%d \n", l_przedmiotow);
    }
    char nazwaprzedmiotu[50];
    strcpy(nazwaprzedmiotu,przedmioty[p]);

    for (int i=p; i<l_przedmiotow; i++)
    {
        strcpy(przedmioty[i],przedmioty[i+1]);

    }


    //usun z pliku
    FILE *fp1=fopen(ZAJECIA, "w");
    if (fp1==NULL)
        return 0;
    for (int i=1; i<=l_przedmiotow-1; i++)
    {
        fputs(przedmioty[i],fp1);
        fputs(" ",fp1);
    }

    fclose(fp1);

    printf(u8"\n Czy chcesz usunac wszystkie wpisy zawierajace ta aktywnosc? \n");
    printf(u8"\n Jesli tak - wybierz 1, jesli nie - wybierz 0 \n");
    int czy;
    scanf("%d", &czy);
    if (czy)
        {
            if(!usun_wpisy_zajecia(nazwaprzedmiotu))
                return 0;
        }


    }

     return 1;
}




bool usun_wpisy_przedmiotu(char *nazwaprzedmiotu)
{
    db wpis;
    FILE *fp, *fp1;
    fp=fopen(NAZWA_BAZY, "r+b");
    fp1=fopen("temp.dat", "w+b");

    if(fp==NULL)
        return 0;
            fseek(fp,0,SEEK_SET);


    while(fread(&wpis, sizeof(db),1,fp))
    {
        if(strcmp(wpis.przedmiot,nazwaprzedmiotu)!=0)
                    fwrite(&wpis,sizeof(db),1,fp1);
    }
    fclose(fp);
    fclose(fp1);

        fp1=fopen("temp.dat", "r+b");
        fp=fopen(NAZWA_BAZY, "w+b");

        if(fp==NULL || fp1==NULL)
            return 0;

        while (fread(&wpis, sizeof(db),1,fp1))
            fwrite(&wpis, sizeof(db),1,fp);

    fclose(fp);
    fclose(fp1);
    return 1;
}

bool usun_wpisy_zajecia(char *nazwazajecia)
{
    db wpis;
    FILE *fp, *fp1;
    fp=fopen(NAZWA_BAZY, "r+b");
    fp1=fopen("temp.dat", "w+b");

    if(fp==NULL)
        return 0;
            fseek(fp,0,SEEK_SET);


    while(fread(&wpis, sizeof(db),1,fp))
    {
        if(strcmp(wpis.zajecie,nazwazajecia)!=0)
                    fwrite(&wpis,sizeof(db),1,fp1);
    }
    fclose(fp);
    fclose(fp1);

        fp1=fopen("temp.dat", "r+b");
        fp=fopen(NAZWA_BAZY, "w+b");

        if(fp==NULL || fp1==NULL)
            return 0;

        while (fread(&wpis, sizeof(db),1,fp1))
            fwrite(&wpis, sizeof(db),1,fp);

    fclose(fp);
    fclose(fp1);
    return 1;
}

bool dodaj_przedmiot(int l_przedmiotow, char (*przedmioty)[50])
{
    char string[50];
    printf("\n Podaj nazwe przedmiotu (ciag znakow bez spacji)\n");
    scanf("%s", string);
    //fflush(stdin);
    strcpy(przedmioty[l_przedmiotow], string);
    FILE *wsk;
    wsk=fopen(PRZEDMIOTY,"a+");
    if (wsk==NULL)
        return 0;
    fputs(string,wsk);
    fputs(" ",wsk);


    fclose(wsk);
    return 1;

}


bool dodaj_zajecie(int l_zajec, char (*zajecia)[50])
{
    char string[50];
    printf("\n Podaj nazwe aktywnosci (ciag znakow bez spacji)\n");
    scanf("%s", string);
    //fflush(stdin);
    strcpy(zajecia[l_zajec], string);
    FILE *wsk;
    wsk=fopen(ZAJECIA,"a+");
    if (wsk==NULL)
        return 0;
    fputs(string,wsk);
    fputs(" ",wsk);
    fclose(wsk);
    return 1;

}

int zdefiniowane_przedmioty(char (*przedmioty)[50])
{
    char string[50];
    int l_przedmiotow=0;
    FILE *wsk=fopen(PRZEDMIOTY,"a+");
    if (wsk==NULL)
        return -1;
    int i=1;
    while(!feof(wsk))
    {
        fscanf(wsk,"%s",string);
            strcpy(przedmioty[i], string);
              l_przedmiotow++;
              i++;
    }
    l_przedmiotow--;
    fclose(wsk);
      if (l_przedmiotow==0)
      {
          printf("Plik 'przedmioty.txt' nie zawiera zdefiniowanych przedmiotow. Podaj ile z nich chcesz zdefiniowac \n");
          int liczba;
          scanf("%d", &liczba);
          for (int i=1; i<=liczba; i++)
          {
              l_przedmiotow++;
              dodaj_przedmiot(l_przedmiotow, przedmioty);
          }
      }


      return l_przedmiotow;

}


int zdefiniowane_zajecia(char (*zajecia)[50])
{
    char string[50];
    int l_zajec=0;
    FILE *wsk=fopen(ZAJECIA,"a+");
    int i=1;
    if (wsk==NULL)
        return -1;
    while(!feof(wsk))
    {
        fscanf(wsk,"%s",string);

            strcpy(zajecia[i],string);
              l_zajec++;
              i++;
    }

    fclose(wsk);
    l_zajec--;

     if (l_zajec==0)
      {
          printf("Plik 'zajecia.txt' nie zawiera zdefiniowanych zajec. Podaj ile z nich chcesz zdefiniowac \n");
          int liczba;
          scanf("%d", &liczba);
          for (int i=1; i<=liczba; i++)
          {
              l_zajec++;
              dodaj_zajecie(l_zajec, zajecia);
          }
      }

      return l_zajec;
}
