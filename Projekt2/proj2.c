/**
* Subor:     proj2.c
* Datum:     18.11.2017
* Autor:     Rene Bolf, xbolfr00@stud.fit.vutbr.cz
* Projekt 2: Iteracne vypocty
* Popis:     https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?title=IZP:Projekt2&csid=651706&id=12214
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/**Kody chyb programu*/
enum ERROR
{
    ERROR_OK,
    ERROR_NUMBER,
    ERROR_INTERVAL,
    ERROR_BAD_PARAM
};
/**
 *Funckia pre vypisanie napovedy
 *@param argc je datoveho typu int a je to argc z main
 *@return je hodnota 0
 */
int help(int argc)
{
    if(argc>2)
    {
        fprintf(stderr,"Nespravny pocet argumentov\n");
        return ERROR_BAD_PARAM;
    }
    printf("Vitajte v napovede k programu Iteracne vypocty\n");
    printf("Program vytvoril Rene Bolf (xbolfr00)\n");
    printf("Program sluzi na vypocitanie vzdialenosti a vysky velkych objektov\n");
    printf("Program spustajte prosim vas v nasledujjucej podobe:\n ./proj2 --help \n ./proj2 --tan A N M \n ./proj2 [-c X] -m A [B] \n");
    printf("\nPopis argumentov:\n \n--help vypise na obrazovku napovedu k programu \n");
    printf("--tan   porovna presnost vypoctu tangens uhlu A (v radianoch)\n");
    printf("N a M udavaju v ktorych iteraciach ma porovnanie prebiehat\n");
    printf("Argument [-c X] je nepovinny\n -c nastavuje vysku meracieho pristroja\n");
    printf("-m  vypocita a zmeria vzdialenosti\n");
    printf("A -uhol alfa. Program vypocita vzdialenost meraneho objektu\n");
    printf("B -uhol beta je nepovinny. Program vypocita vysku meraneho objektu\n");
    return 0;
}
/**
 *Funkcia pre vypocet tangens uhla pomoocu Taylorovho polynomu
 *@param x je datoveho typu double
 *@param n je typu unsigned int
 *@return tanx funkcia vracia vypocitany tangens
 */
double taylor_tan(double x, unsigned int n)
{
    double tanx;      //tanx je predchadzajuci cely clen
    double mocX=x;    //mocX je mocnina x, ktora sa zvysuje
    double citatel[13]= {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
    double menovatel[13]= {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    for(unsigned int i=0; i<n; ++i)
    {
        tanx=tanx+mocX*citatel[i]/menovatel[i];
        mocX=mocX*x*x;
    }
    return tanx;
}
/**
 *Funkcia pre vypocet tangens uhla pomoocu zretazeneho zlomku
 *@param x je datoveho typu double
 *@param n je typu unsigned int
 *@return cf funkcia vracia vysledok zretazeneho zlomku
 */
double cfrac_tan(double x, unsigned int n)
{
    double a=1.0;
    double b;
    double cf=0;
    while(n>0)
    {
        b=(2*n-1)/x;
        cf=a/(b-cf);
        n--;
    }
    return cf;
}
/**
 *Funkcia pre vypocet vzdialenosti meraneho objektu
 *@param alfa je datoveho typu double a predstavuje uhol alfa
 *@param vyskaPristroja je datoveho typu double a je to vyska pristroja
 *@return dlzka funkcia vracia vypocitanu dlzku(vzdialenost) meraneho objektu
 */
double vzdialenost(double alfa, double vyskaPristroja)
{
    double dlzka;
    dlzka=vyskaPristroja/cfrac_tan(alfa,10);
    printf("%.10e\n",dlzka);
    return dlzka;
}
/**
 *Funkcia pre vypocet vzdialenosti meraneho objektu
 *@param beta je datoveho typu double a predstavuje uhol beta
 *@param dlzka je datoveho typu double a je to hodnota pouzita z funkcie pre vypocet vzdialenosti
 *@param vyskaPristroja je datoveho typu double a je to vyska pristroja
 *@return v funkcia vracia vypocitanu vysku meraneho objektu
 */
double vyska(double beta, double dlzka, double vyskaPristroja)
{
    double v;
    v=cfrac_tan(beta,10)*dlzka+vyskaPristroja;
    printf("%.10e\n",v);
    return v;
}
/**
 *Funkcia pre vypis porovnania tangens uhlu A(v radianoch) medzi volanim tan z mat. kniznice a vypocet tangens pomocou taylorovho polynomu a zretazeneho zlomku
 *@param afla je datoveho typu double a predstavuje uhol alfa
 *@param n je datoveho typu int
 *@param m je datoveho typu int
 *Tieto parametre udavaju, v ktorych interaciach iteracneho vypoctu ma porovnanie prebiehat v intervale 0<N=M<14
 *Funckia je void nema ziadnu navratovu hodnotu
 */
void vypisTan(double alfa, int n, int m)
{
    double matTan,t,te,c,ce;
    while(n<=m)
    {
        matTan=tan(alfa);
        t=taylor_tan(alfa,n);
        te=fabs(t-matTan);
        c=cfrac_tan(alfa,n);
        ce=fabs(c-matTan);
        printf("%d %e %e %e %e %e\n",n,matTan,t,te,c,ce);
        ++n;
    }
}
/**
 *Funkcia pre kontrolu uhlov ci su v spravnom intervale a ci su to cisla.
 *@param uhol je to ukazatel na double a reprezentuje uhly alfa a beta
 *@param str je string, ktory sluzi k pouzitiu sscanf na zistenie ci je to cislo
 *@return ERROR_OK vracia 0 ked je vsetko v poriadku
 *@return ERROR_INTERVAL vracia chybove hlasenie, ked je zadany zly interval
 *@return ERROR_NUMBER vracia chybove hlasenie, ked hodnoty niesu cisla
 */
int kontrolaUhlov(double *uhol, char *str)
{
    int a1=sscanf(str,"%lf",uhol);
    if(a1==1)
    {
        if(0<*uhol && *uhol <=1.4)
            return ERROR_OK;
        else
        {
            fprintf(stderr, "zadajte v intervale 0<A<=1.4<Ï?/2.\n");
            return ERROR_INTERVAL;
        }
    }
    else
    {
        fprintf(stderr,"Zadane hodnoty niesu cisla, prosim zadajte cisla \n");
        return ERROR_NUMBER;
    }
}
/**
 *Funkcia pre kontrolu vysky senzora ci je hodnota v spravnom intervale a ci je to cislo
 *@param vyskaPristroja je ukazatel na double a definuje vysku pristroja
 *@param argv je ukazatel na string argv z main
 *@return ERROR_OK vracia 0 ked je vsetko v poriadku
 *@return ERROR_INTERVAL vracia chybove hlasenie, ked je zadany zly interval
 *@return ERROR_NUMBER vracia chybove hlasenie, ked hodnoty niesu cisla
 */
int kontrolaVyskySenzora(double *vyskaPristroja,char **argv)
{
    int x=sscanf(argv[2],"%lf",vyskaPristroja);
    if(x==1)
    {
        if(0<*vyskaPristroja && *vyskaPristroja <=100)
            return ERROR_OK;
        else
        {
            fprintf(stderr, "zadajte v intervale 0<X<=100\n");
            return ERROR_INTERVAL;
        }
    }
    else
    {
        fprintf(stderr,"Zadane hodnoty niesu cisla, prosim zadajte cisla \n");
        return ERROR_NUMBER;
    }
}
/**
 *Funkcia argumentTan sluzi na spracovanie argumentu --tan a pre kontrolu intervalu a ci su dane hodnoty cisla
 *@param argc je datoveho typu int a je to argc z main
 *@param argv je ukazatel na string argv z main
 *@param alfa je ukazatel na double
 *@param n je ukazatel na int
 *@param m je ukazatel na int
 *@return ERROR_OK vracia 0 ked je vsetko v poriadku
 *@return ERROR_INTERVAL vracia chybove hlasenie, ked je zadany zly interval
 *@return ERROR_NUMBER vracia chybove hlasenie, ked hodnoty niesu cisla
 *@return ERROR_BAD_PARAM vracia chybove hlasenie, ked su zadane zle parametre
 */
int argumentTan(int argc,char **argv,double *alfa,int *n,int *m)
{
    if(argc==5)
    {
        int a1=sscanf(argv[2],"%lf",alfa);
        int m1=sscanf(argv[3],"%d",n);
        int n1=sscanf(argv[4],"%d",m);
        if(a1==1 && m1==1 && n1==1)
        {
            if(0<*n&&*n<=*m&&*m<14)
                return ERROR_OK;
            else
            {
                fprintf(stderr, "zadajte v intervale 0<N<=M<14\n");
                return ERROR_INTERVAL;
            }
        }
        else
        {
            fprintf(stderr,"Zadane hodnoty niesu cisla, prosim zadajte cisla \n");
            return ERROR_NUMBER;
        }
    }
    else
    {
        fprintf(stderr, "Zle zadane Argumenty\n");
        return ERROR_BAD_PARAM;
    }
}
/**
 *Funkcia argumentM sluzi na volanie a spracovanie dalsich funkcii, ked je prvy argument -m
 *@param argc je datoveho typu int a je to argc z main
 *@param argv je ukazatel na string argv z main
 *@return ERROR_OK vracia 0 ked je vsetko v poriadku
 *@return ERROR_BAD_PARAM vracia chybove hlasenie, ked su zadane zle parametre
 */
int argumentM(int argc, char **argv)
{
    double alfa,beta;
    if(argc==3)
    {
        int a = kontrolaUhlov(&alfa,argv[2]);
        if(a != 0)
            return a;
        vzdialenost(alfa,1.5);
    }
    else if(argc==4)
    {
        int a = kontrolaUhlov(&alfa,argv[2]);
        int b = kontrolaUhlov(&beta,argv[3]);
        if(a != 0 || b != 0)
            return a;
        double dlzka = vzdialenost(alfa,1.5);
        vyska(beta,dlzka,1.5);
    }
    else
    {
        fprintf(stderr, "Zly pocet argumentov\n");
        return ERROR_BAD_PARAM;
    }
    return ERROR_OK;
}
/**
 *Funkcia argumentC sluzi na volanie a spracovanie dalsich funkcii, ked je prvy argument -c
 *@param argc je datoveho typu int a je to argc z main
 *@param argv je ukazatel na string argv z main
 *@return ERROR_OK vracia 0 ked je vsetko v poriadku
 *@return ERROR_BAD_PARAM vracia chybove hlasenie, ked su zadane zle parametre
 */
int argumentC(int argc, char **argv)
{
    if(argc>=5 && argc<=6)
    {
        double vyskaPristroja,alfa,beta;
        int x = kontrolaVyskySenzora(&vyskaPristroja,argv);
        if(x!=0)
            return x;
        if(strcmp(argv[3],"-m")==0)
        {
            if(argc==5)
            {
                int a=kontrolaUhlov(&alfa,argv[4]);
                if(a!=0)
                    return a;
                vzdialenost(alfa,vyskaPristroja);
            }
            else
            {
                int a=kontrolaUhlov(&alfa,argv[4]);
                int b=kontrolaUhlov(&beta,argv[5]);
                if(a!=0||b!=0)
                    return a;
                double dlzka=vzdialenost(alfa,vyskaPristroja);
                vyska(beta,dlzka,vyskaPristroja);
            }
        }
        else
        {
            fprintf(stderr, "Treti argument musi byt -m\n");
            return ERROR_BAD_PARAM;
        }
        return ERROR_OK;
    }
    else
    {
        fprintf(stderr, "Zly pocet argumentov\n");
        return ERROR_BAD_PARAM;
    }
}
int main(int argc, char *argv[])
{
    if(argc==1)
    {
        fprintf(stderr,"Ak ste nezadali ziadny argument a neviete pokracovat zadajte --help\n");
        return 0;
    }
    if(strcmp(argv[1], "--help")==0)
    {
        return help(argc);
    }
    if(strcmp(argv[1], "--tan")==0)
    {
        if(argc == 5)
        {
            double alfa;
            int n,m;
            if(argumentTan(argc,argv,&alfa,&n,&m) == ERROR_OK)
                vypisTan(alfa,n,m);
            else
                EXIT_FAILURE;
        }
        else
        {
            fprintf(stderr,"Zadali ste zly pocet argumentov.Ak neviete pokracovat zadajte --help\n");
            return EXIT_FAILURE;
        }
    }
    if(strcmp(argv[1],"-c")==0)
    {
        argumentC(argc,argv);
    }
    if(strcmp(argv[1],"-m")==0)
    {
        argumentM(argc,argv);
    }
    else
        return EXIT_FAILURE;
    return 0;
}
