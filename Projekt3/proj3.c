/**
 * @file:       proj3.c
 * @date:       9.12.2017
 * @author:     Rene Bolf, xbolfr00@stud.fit.vutbr.cz
 * Projekt 3:   Jednoducha zhlukova analyza
 * @brief:      https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?title=IZP:Projekt3&csid=651706&id=12214
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - zhluk objektu:
 *      pocet objektu ve zhluku,
 *      kapacita zhluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole zhluku.
 */
/**
 * Struktura objetku
 * @var id datovy typ int je to identifikator objektu
 * @var x datovy typ float je to suradnica x
 * @var y datovy typ float je to suradnica y
 */
struct obj_t
{
    int id;
    float x;
    float y;
};
/**
 * Zhluk objektu
 * @var size datovy typ int udava pocet objektov v zhluku
 * @var capacity datovy typ int je to kapacita zhluku(pocet objektov, pre ktore je rezervovane miesto v poli)
 * @var obj je to ukazatel na struct obj_t
 */
struct cluster_t
{
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/**
 * Inicializacia zhluku 'c'. Alokuje pamat pre cap objektov (kapacitu).
 * Ukazatel NULL u pole objektov znamena kapacitu 0.
 * @param c Ukazatel na struct cluster_t
 * @param cap je datoveho typu integer a reprezentuje kapacitu zhluku(pocet objektov, pre ktore je rezervovane miesto v poli)
 * @pre c na zaciatku nema hodnotu NULL
 * @pre cap  ma hodnotu vacsiu ako 0 alebo hodnotu rovnu 0
 * @pre zhluk c nieje inicializovany a pamat nie je alokovana
 * @post c je inicializovane a spravila sa alokacia pamati pre cap objektov
 * @defgroup zhluk_objetku
 */
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->obj = NULL;
    c->size = 0;
    c->capacity = cap;

    if(cap>0)
        c->obj=malloc(sizeof(struct obj_t)*cap);
    if(c->obj==NULL)
        c->capacity=0;
}
/**
 * Odstranenie vsetkych objektov zhluku a inicializacia na prazdny zhluk.
 * @param c Ukazatel na struct cluster_t
 * @pre c je inicializovane
 * @post uvolnenie pamati a inicializovanie c na 0
 * @defgroup zhluk_objektu
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/**
 * Zmena kapacity zhluku 'c' na kapacitu 'new_cap'
 * @param c Ukazatel na struct cluster_t
 * @param new_cap datovy typ int je to novo vzniknuta kapacita zhluku
 * @pre new_cap  ma hodnotu vacsiu ako 0 alebo hodnotu rovnu 0
 * @pre capacity je vacia alebo rovna 0
 * @post zmena cap na new_cap
 * @defgroup zhluk_objektu
 * @return c vratenie zhluku c
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}
/**
 * Prida objekt 'obj' na koniec zhluku 'c'. Rozsiri zhluk, pokial sa do neho objekt nevojde.
 * @param c Ukazatel na struct cluster_t
 * @param obj je to struct obj_t  je to objekt so struktury objektu
 * @pre nebol pridany objekt na koniec zhluku
 * @post pridanie objektu na koniec zhluku
 * @defgroup zhluk_objektu
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c->capacity <= c->size)
        if(!resize_cluster(c,c->capacity + CLUSTER_CHUNK)) //kontrola ci sa to podarilo realocovat
            return;

    c->obj[c->size] = obj;  //ulozi objekt na index c->size
    c->size++;   //zvacsi size
}
/**
 * Seradi objekty ve zhluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/**
 * Do zhluku 'c1' prida objekty 'c2'. zhluk 'c1' bude v pripade nutnosti rozsireny.
 * Objekty v zhluku 'c1' budu zoradene vzostupne podla identifikacneho cisla.
 * zhluk 'c2' bude nezmeneny.
 * @param c1 Ukazatel na struct cluster_t
 * @param c2 Ukazatel na struct cluster_t
 * @pre c1 na zaciatku nema hodnotu NULL
 * @pre c2 na zaciatku nema hodnotu NULL
 * @pre objekt je pridany na koniec
 * @post do zhluku c1 je pridany objekt c2, zluci jednotlive objekty a objekty v c1 budu zoradene vzostupne podla ID
 * @defgroup zhluk_objektu
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for(int i = 0; i < c2->size; i++)     // vsetky objekty z zhluku c2
    {
        append_cluster(c1,c2->obj[i]);   // pridat do zhluku c1 objekt s indexem [i] z c2
    }
    sort_cluster(c1); //zoradenie objektov v zhluku c1
}

/**********************************************************************/
/* Prace s polem zhluku */

/**
 * Odstrani zhluk z pola zhluku 'carr'. Pole zhluku obsahuje 'narr' poloziek (zhluku).
 * zhluk pre odstranenie sa nachadza na indexe 'idx'.
 * @param carr Ukazatel na struct cluster_t
 * @param narr datovy typ int definuje n poloziek v zhluku
 * @param idx datovy typ int definuje index, na ktorom sa zhluk nachadza
 * @pre zhluk je zluceny a zoradeny vzostupne
 * @post odstrani sa zhluk z pola zhluku carr vrati sa nova velkost ktora bude o 1 mensia
 * @defgroup pole_zhluku
 * @return Funkcia vracia novy pocet zhluku v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(&carr[idx]); // uvolnenie pamäti pre zhluk na indexu idx
    for(int i = idx; i < narr-1; i++) // posunutie zhluku v poli od indexu idx smerom dolava
    {
        carr[i] = carr[i + 1];  //na dane miesto sa prida iny aby neostalo prazdne miesto
    }

    return narr-1;  //nova velkost bude o 1 mensia, pretoze sa jeden zhluk odstranil
}

/**
 * Pocita Euklidovsku vzdialenost medzi dvomi objektami.
 * @param o1 Ukazatel na struct obj_t
 * @param  o2 Ukazatel na struct obj_t
 * @pre o1 sa nerovna NULL
 * @pre o2 sa nerovna NULL
 * @post vypocet Euklidovskej vzdialenosti
 * @defgroup matematicky_vypocet
 * @return Funkcia vrati vypocet Euklidovskej vzdialenosti
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
    // vypocet Euklidovskej vzdialenosti mezi objketami `o1` a `o2`
    return sqrtf((o1->x - o2->x) * (o1->x - o2->x) + (o1->y - o2->y) * (o1->y - o2->y));
}
int premium_case;
/**
 * Pocita vzdalialenost dvoch zhlukov tromi zlukovymi metodami
 * Metodou "Unweighted pair-group average"
 * Metodou najblizsieho suseda
 * Metodou najvzdialenejsieho suseda
 * @param c1 Ukazatel na struct cluster_t
 * @param  c2 Ukazatel na struct cluster_t
 * @pre c1 sa nerovna NULL a velkost je vacsia ako 0
 * @pre c2 sa nerovna NULL a velkost je vacsia ako 0
 * @pre je vyratana Euklidovska vzdialenost, ktora sa pouzije pri vypocte vzdialenosti
 * @post vypocet dvoch zhlukov tromi zhlukovymi metodami vychozia hodnota je metoda Unweighted pair-group average
 * @defgroup pole_zhluku
 * @return Funkcia vrati vypocet najblizsieho suseda ak sa premium_case = 2
 * @return Funkcia vrati vypocet najvzdialenejsieho suseda ak sa premium_case = 3
 * @return Funkcia vrati vypocet Unweighted pair-group average ak sa premium_case = 1 je to vychodzia hodnota
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float distance = 0.0;
    float max_distance = 0.0;
    float min_distance = INFINITY;

    for (int i = 0; i < c1->size; i++)
    {
        for (int j = 0; j < c2->size; j++)
        {
            if(premium_case == 1)
            distance = distance + obj_distance(&c1->obj[i], &c2->obj[j]);

            else
            {
                distance = obj_distance(&c1->obj[i], &c2->obj[j]);

                if(distance > max_distance)
                max_distance = distance;

                if(distance < min_distance)
                min_distance = distance;
            }
        }
    }

    switch(premium_case)
    {
      case 2: return min_distance;
          break;
      case 3: return max_distance;
          break;
    }
    float average = distance / (c1->size * c2->size);
    return average;
}
/**
 * Funkcia najde dva najblizsie zhluky. V poli zhluku 'carr' o velkosti 'narr' hlada dva nejblizsi zhluky.
 * Najdene zhluky identifikuje podla ich indexov v poli 'carr'.
 * Funkcia najdene zhluky (indexy do pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
 * @param carr Ukazatel na struct cluster_t reprezentuje pole zhluku
 * @param  narr datovy typ int reprezentuje velkost velkost pole zhluku carr
 * @param c1 Ukazatel na datovy typ int sluzi na ukladanie zhulov(indexov do pola carr)
 * @param c2 Ukazatel na datovy typ int sluzi na ukladanie zhulov(indexov do pola carr)
 * @pre je vypocitana cluster_distance, velkost pola carr musi byt vacsia ako 0
 * @post najde dva najblizsie zhluky a najdene zhluky sa ulozia do pamati na adresu c1 resp c2
 * @defgroup pole_zhluku
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float min_distance = INT_MAX;
    float distance;
    // pocitanie vzdialenosti medzi vsetkými zhlukmi navzajom
    for (int i = 0; i < narr; i++)
    {
        for (int j = i + 1; j < narr; j++)
        {
            // vypocet vzdialenosti
            distance = cluster_distance(&carr[i], &carr[j]);
            if (distance < min_distance)
            {
                min_distance = distance;
                *c1 = i;
                *c2 = j;
            }
        }
    }
}

// pomocna funkce pro razeni zhluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/**
 * Zoradenie objektov vo zhluku vzostupne podla ich identifikatorov.
 * @param c Ukazatel na struct cluster_t
 * @pre objekty v zhluku niesu zoradene
 * @post objekty v zhluku su zoradenie podla ich identifikatorov
 * @defgroup zhluk_objektu
 */
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/**
 * Vypis zhluku 'c' na stdout.
 * @param c Ukazatel na struct cluster_t
 * @defgroup vypis_zhluku
 */
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/**
 * Zo suboru 'filename' nacita objekty. Pre kazdy objekt vytvori zhluk a ulozi ho do pola zhluku.
 * Alokuje priestor pre pole vsetkych zhlukov a ukazatel na prvu polozku pole (ukalazatel na prvni zhluk v alokovanem poli)
 * ulozi ho do pameti, kde sa odkazuje parameter 'arr'.
 * V pripade nejake chyby na adresu arr ulozi NULL
 * @param filename ukazatel na char je to nazov suboru
 * @param arr ukazatel na ukazatel na struct cluster_t
 * @pre arr sa nerovna NULL
 * @post subor je otvoreny, pocet zaznamov zo zaciatku suboru je naskenovane a miesto je na alokovane pre pole zhlukov
 * @post pre kazdy riadok v textovom subore je vytvoreny novy objekt, inicializuje sa cluster
 * @defgroup pole_zhluku
 * @return Funkcia vracia pocet nactenych objektu (zhluku).
 */
int load_clusters(char *filename, struct cluster_t **arr)
{
    //skontroluje ci arr nie je NULL
    assert(arr != NULL);

//otvori a skontroluje subor
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        fprintf(stderr, "ERROR: Nepodarilo sa otvorit subor\n");
        return -1;
    }

//naskenuje cislo (pocet zaznamov) zo zaciatku suboru
    int n = 0;
    int code = fscanf(file, "count=%d", &n);
    if( code != 1)
    {
        fprintf(stderr, "ERROR\n");
        return -1;
    }

//alokuje miesto pre pole zhlukov o velkosti n (pocet zaznamov)
    *arr = malloc(sizeof(struct cluster_t) * n);
    if(*arr == NULL)
    {
        fprintf(stderr, "ERROR : Nepodarilo sa alokovat miesto\n");
        return -1;
    }

//pre kazdy riadok v textovom subore
    for(int i = 0; i < n; i++)
    {
        //vytvory novy objekt
        struct obj_t obj;
        //naskenuje do objektu ID, x, y
        int code = fscanf(file ,"%d %f %f", &obj.id, &obj.x, &obj.y);
        if(code != 3)
        {
            fprintf(stderr, "ERROR : Nepodarilo sa naskenovat\n");
            return -1;
        }
        //inicializuje cluster na i pozicii velkostou CLUSTER_CHUNK
        init_cluster(&(*arr)[i], CLUSTER_CHUNK);
        //ulozi hodnotu obj do objektu arr[i]
        *(*arr)[i].obj = obj;
        //nastavi velkost clusteru na 1
        (*arr)[i].size = 1;
    }
//skusi zavriet subor
    if(fclose(file) == EOF)
    {
        fprintf(stderr, "ERROR : Nepodarilo sa zavriet subor\n");
        return -1;
    }
//vrati pocet nacitanych clusterov ak nenastala chyba
    return n;
}

/**
 * Vypis pola zhluku. Parameter 'carr' je ukazatel na prvu polozku (zhluk).
 * Vypise sa prvy 'narr' zhluku.
 * @param carr ukazatel na struct cluster_t
 * @param narr datovy typ int pocet polov
 * @defgroup vypis_zhluku
 */
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;  //pole zhluku
    int n;
    char *err_n;
    int count_clusters;
    if(argc == 1)
    {
        fprintf(stderr, "ERROR: Nebol zadany ziadny argument\n");
          return -1;
    }
    else if(argc == 3)
        premium_case = 1; //ak nebude zvolenty argument [METHOD] bude to robit vychodziu metodu pair-group average
    else if(argc == 4)
    {
        if(strcmp(argv[3], "--avg") == 0)
            premium_case = 1;

        else if(strcmp(argv[3], "--min") == 0)
            premium_case = 2;

        else if(strcmp(argv[3], "--max") == 0)
            premium_case = 3;
        else
        {
            fprintf(stderr, "ERROR: Chybovy vstup\n");
            return EXIT_FAILURE;
        }
    }

    else
    {
        fprintf(stderr, "ERROR: Nespravny pocet argumentov\n");
        return -1;
    }

    strtol(argv[2],&err_n,10);      //10 = base bude to v 10 sustave
    if(*err_n != '\0')
    {
        fprintf(stderr, "ERROR: Zadana hodnota musi byt cislo\n");
        return -1;
    }
    n = atoi(argv[2]);
    if(n <= 0)
    {
        fprintf(stderr, "ERROR: Zadana hodnota nesmie byt zaporna alebo mat nulovu hodnotu\n");
        return -1;
    }

    if((count_clusters = load_clusters(argv[1], &clusters)) == -1)
        return EXIT_FAILURE;

    if (count_clusters < n)
    {
        fprintf(stderr, "ERROR: Pocet objektov je mensi ako N\n");
        return -1;
    }
    while(count_clusters > n)
    {
        int c1, c2;
        find_neighbours(clusters, count_clusters , &c1, &c2);
        merge_clusters(clusters + c1, clusters + c2);
        count_clusters = remove_cluster(clusters, count_clusters, c2);
    }
    print_clusters(clusters, count_clusters);
    for(int i=0; i < count_clusters; i++)
    {
      clear_cluster(&clusters[i]);
    }
    free(clusters);

    return 0;
}
