#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAZIV 50
#define FILE_NOT_OPENED (-1)
#define MEMORY_NOT_ALLOCATED (-2)

typedef struct _Artikl* ArtiklHead;
typedef struct _Artikl {
    char naziv[MAX_NAZIV];
    int kolicina;
    double cijena;
    ArtiklHead next;
} Artikl;

typedef struct _Racun* RacunHead;
typedef struct _Racun {
    int godina;
    int mjesec;
    int dan;
    ArtiklHead artikli;
    RacunHead rnext;
} Racun;

int compareDates(RacunHead r1, RacunHead r2);
int insertArtiklSorted(ArtiklHead* head, ArtiklHead novi);
int insertRacunSorted(RacunHead* head, RacunHead novi);
int readRacuni(const char* filename, RacunHead* head);
int queryItem(RacunHead head);
int freeAll(RacunHead head);
int dateInRange(RacunHead r, int g1,int m1,int d1,int g2,int m2,int d2);

int main() {
    RacunHead racuniHead = NULL; 

    if(readRacuni("racuni.txt", &racuniHead) != 0){
        printf("Greska prilikom ucitavanja racuna.\n");
        return FILE_NOT_OPENED;
    }

    queryItem(racuniHead);
    freeAll(racuniHead);

    return 0;
}


int compareDates(RacunHead r1, RacunHead r2){       
    if(r1->godina != r2->godina) return r1->godina - r2->godina;
    if(r1->mjesec != r2->mjesec) return r1->mjesec - r2->mjesec;
    return r1->dan - r2->dan;
}

int insertArtiklSorted(ArtiklHead* head, ArtiklHead novi){   //ubacuje artikl u listu abecedno, zbraja kolicine ako artikl vec postoji
    ArtiklHead* curr = head;

    while(*curr && strcmp((*curr)->naziv, novi->naziv) < 0)
        curr = &(*curr)->next;

    if(*curr && strcmp((*curr)->naziv, novi->naziv) == 0){
        (*curr)->kolicina += novi->kolicina;
        free(novi);
        return 0;
    } else {
        novi->next = *curr;
        *curr = novi;
        return 0;
    }
}

int insertRacunSorted(RacunHead* head, RacunHead novi){   //ubacuje racun u listu sortirano po datumu
    RacunHead* curr = head;

    while(*curr && compareDates(*curr, novi) < 0)
        curr = &(*curr)->rnext;

    novi->rnext = *curr;
    *curr = novi;
    return 0;
}

int readRacuni(const char* filename, RacunHead* head){     //cita sve racune iz racuni.txt i svaku datoteku pojedinacno
    FILE* fp = fopen(filename, "r");
    if(!fp) return FILE_NOT_OPENED;

    char buffer[MAX_LINE];
    while(fscanf(fp, "%s", buffer) == 1){
        buffer[strcspn(buffer, "\n")] = 0;

        FILE* racunFile = fopen(buffer, "r");  //da otvorimo pojedini racun
        if(!racunFile){
            printf("Nemoguce otvoriti %s\n", buffer);
            continue;
        }

        RacunHead novi_racun = (RacunHead)malloc(sizeof(Racun));   //alociramo novi racun
        if(!novi_racun){
            printf("Greska alokacije memorije za racun.\n");
            fclose(racunFile);
            continue;
        }
        novi_racun->artikli = NULL;
        novi_racun->rnext = NULL;

        fscanf(racunFile, "%d-%d-%d", &novi_racun->godina, &novi_racun->mjesec, &novi_racun->dan);   //ovdje citamo datum racuna

        char naziv[MAX_NAZIV];
        int kolicina;
        double cijena;

        while(fscanf(racunFile, "%s %d %lf", naziv, &kolicina, &cijena) == 3){   //ovdje citamo artikle liniju po liniju
            if(kolicina <=0 || cijena <=0) continue;

            ArtiklHead novi_artikl = (ArtiklHead)malloc(sizeof(Artikl));
            if(!novi_artikl){
                printf("Greska alokacije memorije za artikl.\n");
                continue;
            }
            strcpy(novi_artikl->naziv, naziv);
            novi_artikl->kolicina = kolicina;
            novi_artikl->cijena = cijena;
            novi_artikl->next = NULL;

            insertArtiklSorted(&novi_racun->artikli, novi_artikl);  //ubacujemo artikl u sortiranu listu
        }

        fclose(racunFile);
        insertRacunSorted(head, novi_racun);
    }

    fclose(fp);
    return 0;
}

int dateInRange(RacunHead r, int g1,int m1,int d1,int g2,int m2,int d2){
    if(r->godina < g1 || (r->godina == g1 && r->mjesec < m1) || (r->godina==g1 && r->mjesec==m1 && r->dan<d1)) return 0;
    if(r->godina > g2 || (r->godina == g2 && r->mjesec > m2) || (r->godina==g2 && r->mjesec==m2 && r->dan>d2)) return 0;
    return 1;
}

int queryItem(RacunHead head){
    char trazeni[MAX_NAZIV];
    int g1,m1,d1,g2,m2,d2;

    printf("Unesi naziv artikla: ");
    scanf("%s", trazeni);

    printf("Unesi pocetni datum (YYYY-MM-DD): ");
    scanf("%d-%d-%d",&g1,&m1,&d1);

    printf("Unesi krajnji datum (YYYY-MM-DD): ");
    scanf("%d-%d-%d",&g2,&m2,&d2);

    int ukupnaKolicina=0;
    double ukupnaCijena=0;

    RacunHead r = head;
    while(r){     //prolazimo kroz sve racune
        if(dateInRange(r,g1,m1,d1,g2,m2,d2)){
            ArtiklHead a = r->artikli;
            while(a){     //prolazimo sve artikle tog racuna
                if(strcmp(a->naziv,trazeni)==0){
                    ukupnaKolicina += a->kolicina;
                    ukupnaCijena += a->kolicina * a->cijena;
                }
                a = a->next;
            }
        }
        r = r->rnext;
    }

    printf("\nUkupno kupljeno: %d kom\n", ukupnaKolicina);
    printf("Ukupno potroseno: %.2lf EUR\n", ukupnaCijena);

    return 0;
}

int freeAll(RacunHead head){
    while(head){
        RacunHead toDel = head;
        head = head->rnext;

        ArtiklHead a = toDel->artikli;
        while(a){
            ArtiklHead temp = a;
            a = a->next;
            free(temp);
        }

        free(toDel);
    }
    return 0;
}
