#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person* position;

typedef struct _person {
    char ime[30];
    int godina;
    position next;
} person;

// Funkcija za stvaranje novog čvora (nije vezan za vezanu listu, vec ga samo stvaramo neovisno o icemu):
position stvaranje(char* ime, int godina) {
    position NoviCvor = (position)malloc(sizeof(person)); //alociranje memorije

    if (NoviCvor == NULL) { //provjera alociranja memorjie       
        printf("Greška u alociranju\n");
        return NULL;
    }

    strcpy(NoviCvor->ime, ime); //spremanje vrijednosti
    NoviCvor->godina = godina;
    NoviCvor->next = NULL;

    return NoviCvor;
}

// Funkcija za dodavanje čvora na početak liste
int dodavanje_pocetak(position head, char* ime, int godina) {
    position NoviCvor = stvaranje(ime, godina); //preko druge funkcije stvaramo NoviCvor
    if (!NoviCvor) return -1; //provjera

    NoviCvor->next = head->next; //dodajemo ga na vezanu listu
    head->next = NoviCvor;

    return 0;
}

//Funkcije za dodavanje cvora na kraj liste
int dodavanje_kraj(position head, char* ime, int godina) {
    position NoviCvor = stvaranje(ime, godina); //preko druge funkcije stvaramo NoviCvor
    if (!NoviCvor) return -1;

    //pronalazenje posljednjeg cvor:
    position temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    //dodavanje cvora na kraj:
    temp->next = NoviCvor;

    return 0;
}

//funkcija za pronalazenje elementa u listi(vraca podatke iz tog cvora)
position pronalazenje(position head, char* ime) {
    position temp = head->next; //preskacemo head, idemo na prvi cvor

    while (temp != NULL) {
        if (strcmp(temp->ime, ime) == 0) {
            return temp; //vraca pokazivac cvora na kojoj je ime
        }
        temp = temp->next; //ide dalje po listi
    }
    return NULL; //nije pronadeno ime

}

//funkcija za pronalazenje elementa u listi(vraca podatke iz cvora ispred)
position pronalaznje_prethodnog(position head, char* ime) {
    position temp = head->next; //prebacivanje na prvi cvor
    position prethodni = head; //prethodni ostaje na headu

    while (temp != NULL) {
        if (strcmp(temp->ime, ime) == 0) {
            return prethodni; //vraca prethodni cvora na kojoj je ime
        }
        prethodni = temp;
        temp = temp->next;
    }
    return NULL;
}

//funkcija za brisanje odabranog elementa
int brisanje_elementa(position head, char* ime) {
    position temp = head->next;
    position prethodni = head;

    while (temp != NULL) {
        if (strcmp(temp->ime, ime) == 0) {
            prethodni->next = temp->next; //preskocimo trenutni cvor
            free(temp); //oslobadanje memorije
            return 0;
        }
        prethodni = temp;
        temp = temp->next;
    }
    return -1;
}

//funkcija za unos iza odredenog elementa
int dodavanje_iza_elementa(position head, char* ime, char* novo_ime, int nova_godina) {
    position trazeniCvor = pronalazenje(head, ime); //trazenje tog cvora
    if (!trazeniCvor) return -1;

    position noviCvor = stvaranje(novo_ime, nova_godina); //stvaranje novog cvora
    if (!noviCvor) return -1;

    noviCvor->next = trazeniCvor->next; //dodajemo ga na vezanu listu
    trazeniCvor->next = noviCvor;

    return 0;
}

//funkcija za unos ispred odredenog elementa
int dodavanje_ispred_elementa(position head, char* ime, char* novo_ime, int nova_godina) {
    position prethodni = pronalaznje_prethodnog(head, ime); //pronalazenje prethodnog

    position noviCvor = stvaranje(novo_ime, nova_godina); //stvaranje novog cvora
    if (!noviCvor) return -1;

    noviCvor->next = prethodni->next; //dodavanje novog cvora ispred (odnosno iza prehodnog)
    prethodni->next = noviCvor;

    return 0;
}

//sortiranje abecedno 
void sortiranje(position head) {
    position i;
    position j;
    char tempIme[30];
    int tempGodina;

    //vanjska for petlja prolazi kroz cijelu listu
    for (i = head->next; i != NULL; i = i->next) {
        //unutarnja for petlja usporeduje imena
        for (j = i->next; j != NULL; j = j->next) {
            //usporedujemo imena i zamjenjuje varijablje
            if (strcmp(i->ime, j->ime) > 0) {

                strcpy(tempIme, i->ime);
                tempGodina = i->godina;

                strcpy(i->ime, j->ime);
                i->godina = j->godina;

                strcpy(j->ime, tempIme);
                j->godina = tempGodina;
            }
        }
    }
}

//funkcija za upis liste u datoteku
void upis(position head, const char* ImeDatoteke) {
    FILE* datoteka = fopen(ImeDatoteke, "w"); //otvranje datoteke
    if (datoteka == NULL) {
        printf("Nesupjelo otvaranje datoteke");
        return -1;
    }
    position temp = head->next;
    while (temp != NULL) {

        fprintf(datoteka, "%s,\t%d\n", temp->ime, temp->godina); //printanje u datoteku
        temp = temp->next;
    }
    fclose(datoteka);
}

//citanje liste iz datoteke
void citanje(position head, const char* ImeDatoteke) {
    FILE* datoteka = fopen(ImeDatoteke, "w"); //otvranje datoteke
    if (datoteka == NULL) {
        printf("Nesupjelo otvaranje datoteke");
        return -1;
    }

    char ime[30];
    int godina;

    while (fscanf(datoteka, "%s %d", ime, &godina) == 2) {
        dodavanje_kraj(&head, ime, godina);
    }
    fclose(datoteka);

}

//funkcija za ispis cile liste
void ispis(position head) {
    position temp = head->next; //prvi stvarni element nakon heada
    while (temp != NULL) {
        printf("cvor: %s, %d\n", temp->ime, temp->godina);
        temp = temp->next;

    }
}

//funkcija za brisanje svih elemenata
int brisi(position head) {

    position temp = head->next; //stavljanje tempa na prvi cvor
    position cvor = NULL;
    while (temp != NULL) {
        cvor = temp;
        temp = temp->next;
        free(cvor);
    }
    head->next = NULL;
    return 0;
}

int main() {
    person head = { .ime = "", .godina = 0, .next = NULL };

    //poziv funkcija:
   
    citanje(&head, "lista.txt");

    ispis(&head);
    printf("\n");

    /*
    char trazenoIme[30];
    scanf("%s", &trazenoIme);
    position trazeniCvor = pronalaznje_prethodnog(&head, trazenoIme);
    if (trazeniCvor) {
        printf("Pronađeni čvor: %s, %d\n", trazeniCvor->ime, trazeniCvor->godina);
    }
    else {
        printf("Čvor s imenom '%s' nije pronađen.\n", trazenoIme);
    }

    char trazenoIme[30] = "niko";
    dodavanje_ispred_elementa(&head, trazenoIme, "luka", 2000);
    */



    return 0;
} 
