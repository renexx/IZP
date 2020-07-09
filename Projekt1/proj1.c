/**
 * Subor: proj1.c
 * Datum: 4.11.2017
 * Autor: Rene Bolf, xbolfr00@stud.fit.vutbr.cz
 * Projekt 1 : Praca s textom
 * Popis: https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?title=IZP:Projekt1&csid=651706&id=12214
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //kvoli pouzitiu funkcie strlen()
#include <ctype.h>  //kvoli pouzitiu funkcie toupper()

/**
 *Funckia na zoradenie vystupu
 *@param vystup[] jediny parameter funkcie zoradenie.
 *Funkcia je void nema ziadnu navratovu hodnotu.
 */
void zoradenie(char vystup[])
{
    int i;
    int j;
    int pomocna;
    for(i=0; i<vystup[i]; i++) //cyklus v cykle, ktory prechadza, kazdy znak po znaku
    {
        for(j=i+1; j<vystup[j]; j++)
        {

            if(vystup[j]<vystup[i])
            {
                pomocna=vystup[j];
                vystup[j]=vystup[i];
                vystup[i]=pomocna;
            }
        }
    }
}
/**
 *Funkcia sluzi na odstranenie duplicitnych znakov
 *@param vystup[] prvy parameter funkcie
 *@param velkost druhy parameter funkcie
 *Funkcia je void, nema ziadnu navratovu hodnotu
 */
void odstranenie(char vystup[], int velkost)
{
    int i=0;
    int j;
    int k;
    for(i=0; i<velkost; i++) //cyklus na hladanie duplicitnych prvkov v poli, cyklus prejde kazdym prvkom v poli
    {
        for(j=i+1; j<velkost; j++)
        {
            if(vystup[i]==vystup[j])
            {
                for(k=j; k<velkost; k++) // odstrani sucasny duplicitny prvok
                {
                    vystup[k]=vystup[k+1];

                }
                velkost--;      //zmensenie velkosti po vymazani duplicitnych prvkov
                j--;   //ak sa udialo presuvanie prvkov tak neinkrementuj j
            }
        }
    }
}

/**
 *Funkcia na nacitanie miest alebo obci
 *@param mesta[][] parametrom funkcie je dvojrozmerne pole typu char
 *@return int funkcia vracia riadok
 */
int nacitajMesta(char mesta[][100])
{
    int x=0; //riadok
    int y=0; //stlpec

    while((mesta[x][y]=toupper(getchar()))!=EOF) // cyklus sa vykonava dovtedy dokym nenastane end of file (EOF)
    {
        if(mesta[x][y]=='\n')
        {

            mesta[x][y]='\0';
            x++;
            y=0; //stlpcu sa priradi hodnota nula, aby sa vratil na zaciatok riadku
        }
        else
        {
            y++;

        }

    }

    return x;
}
/**
 *Funkcia, ktora sluzi na porovnanie znakov
 *@param mesta[][] parameter je dvojrozmerne pole typu char
 *@param vystup[] parameter je pole typu char
 *@param vystup[] parameter je pole typu char
 *@param *dlzkaVstupu je to ukazatel na int pomocou, ktoreho sa vracia dlzka vstupu do main
 *@param *celeMesto je to ukazatel na int pomocou, ktoreho sa vracia hodnota v ktorej je ulozene cele mesto do main
 *@return int navratova hodnota je typu integer a vracia pocitadlo vystupu
 */
int porovnanieZnakov(char mesta[][100], char vstup[], char vystup[], int dlzkaVstupu, int *celeMesto)
{
    int pocX=0; //pocitadlo riadka
    int pocY=0; //pocitadlo stlpca
    int pocVstupu=0; //pocitadlo vstupu
    int pocVystupu=0; //pocitadlo vystupu


    while(mesta[pocX][0]!='\0')   //cyklus je vykonavany dovtedy dokym na nultom mieste nejakeho riadku nebude znak 0
    {
        if(toupper(vstup[pocVstupu])==mesta[pocX][pocY]) //nacitany vstup je porovnavany od 0. znaku po dlzku vstupu s jednotlivymi riadkami pola tiez od 0. znaku
        {
            pocY++;
            pocVstupu++;
            if(pocY == dlzkaVstupu + 1)
            {
                printf("Found: %s\n", mesta[pocX]);
            }

            else if((toupper(vstup[pocVstupu])!=mesta[pocX][pocY])&&(pocVstupu== dlzkaVstupu)) //porovnanie celeho vstupneho stringu s prislusnym poctom znakov v riadku
            {
                vystup[pocVystupu]=mesta[pocX][pocY];  // zapise znak ktory sa nerovnal do vystupu
                pocVystupu++;
                vystup[pocVystupu] = '\0';
                *celeMesto=pocX;  //pomocna premena v ktorej je ulozeny riadok, aby naslo cele mesto
                pocX++;
                pocY=0;
                pocVstupu=0;

            }

            else if((vstup[pocVstupu]==mesta[pocX][pocY])&&(pocVstupu==dlzkaVstupu)) //inak ak sa rovnaju a zaroven porovnal cely vstupny string s prislusnym poctom znakov v riadku
            {
                *celeMesto=pocX;
            }

        }

        else
        {
            pocX++;
            pocY=0;
            pocVstupu=0;

        }

    }
    return pocVystupu;
}
/**
 *Funkcia na vypisanie moznych zacinajucich prvych pismeniek
 *@param mesta[][] parameter je dvojrozmerne pole datoveho typu char
 *@param polePrvePismena[] parameter je pole typu char, v ktorom su ulozene prve pismena
 *@param x parameter je datoveho typu int a je to pocet riadkov, ktore naplnil vo funkci nacitaj
 */
void prvePismena(char mesta[][100],char polePrvePismena[],int x)
{
    int j=0; //riadok
    int k=0; //stlpec
    int poc=0;

    printf("Enable: ");
    while(j!=x) //cyklus sa vykonava dovtedy dokym pocitadlo j sa nerovna poctu riadkov ktore naplnil vo funkci nacitaj
    {
        polePrvePismena[j]=mesta[j][k]; // pole je ukazatel na pole vystup
        j++;
    }
    zoradenie(polePrvePismena);
    odstranenie(polePrvePismena,j);
    while(polePrvePismena[poc]!='\0') //cyklus sa vykonava dovtedy dokym sa to nerovna 0
    {
        putchar(polePrvePismena[poc]); //vypisanie prvych pismeniek
        poc++;


    }

    printf("\n");
}


int main(int argc, char *argv[])
{
    char mesta[42][100];
    char *vstup=argv[1]; //pole vstup v ktorom je ulozeny argv
    char polePrvePismena[100];  //pomocne pole
    char vystup[100]; //pole vystupu
    int  celeMesto;  //premena v ktorej je ulozene cele mesto
    int  pocVystupu; //pocitadlo vytupu
    int x=nacitajMesta(mesta); //
//osetrenie vstupu ak bude zadanych viac ako 42 adries, program ohlasi chybu
    if(x>42)
    {
        fprintf(stderr,"Maximalny pocet zadanych adries je 42");
        return EXIT_FAILURE;
    }

    if(argc==1)
    {
        prvePismena(mesta,polePrvePismena,x);
        return 0;
    }

    int dlzkaVstupu=strlen(vstup);  //zistenie dlzky vstupu

    if(argc>2) //ak je zadanych viacej argumentov ako 1 program zahlasi chybu
    {
        fprintf(stderr,"Prosim zadajte maximalne 1 argument\n");
        return EXIT_FAILURE;
    }
    pocVystupu=porovnanieZnakov(mesta,vstup,vystup,dlzkaVstupu,&celeMesto);

    if(pocVystupu>1)

    {
        odstranenie(vystup,x);
        zoradenie(vystup);
        printf("Enable: %s\n",vystup);
    }

    if(pocVystupu==1)

    {
        printf("Found: %s\n",mesta[celeMesto]);
    }
    if(pocVystupu==0)
    {
        printf("Not found\n");
    }
    return 0;
}
