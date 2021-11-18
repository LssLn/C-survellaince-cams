/*
  Sviluppo di applicazione in ANSI C per gestire un sistema di
  videosorveglianza.

  Su un archivio (allegato all'attivit� 'telecamere.cfg') �
  memorizzato l'elenco dei parametri di configurazione delle
  telecamere che compongono il sistema. La configurazione di una
  telecamera comprende le seguenti informazioni, identificativo
  telecamera, identificativo dell'area monitorata, orario inizio e
  fine registrazione.

  Implementare le seguenti funzioni:

  1. CaricaConfigurazione: carica l'elenco dei parametri di
  configurazione delle telecamere in una lista di telecamere.

  2. CreaAree: a partire dalla lista delle telecamere, costruisce una
  lista di aree in cui, per ogni area monotorata, ci sia il numero e
  la lista delle telecamere che monitorano quella specifica area.

  3. RegistrazioniDisponibili: dato un orario, e l'identificativo di
  un'area, restituisce la lista delle telecamere che sono in modalit�
  registrazione a quell'ora.

  La descrizione dettagliata di ogni funzione � riportata come
  commento immediatamente prima della definizione della funione.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_FILE  "telecamere.cfg"

typedef struct Telecamera
{
    char  id_telecamera[16];
    char  id_area[16];
    char  ora_inizio[6]; // formato hh:mm
    char  ora_fine[6];   // formato hh:mm
} TTelecamera;

typedef struct NodoListaTelecamere
{
    TTelecamera                 telecamera;
    struct NodoListaTelecamere* succ;
} TNodoListaTelecamere;

typedef TNodoListaTelecamere* PNodoListaTelecamere;

typedef struct NodoListaAree
{
    char                  id_area[16];
    int                   num_telecamere;
    PNodoListaTelecamere  lista_telecamere;
    struct NodoListaAree* succ;
} TNodoListaAree;

typedef TNodoListaAree* PNodoListaAree;


/* AggiungiTelecamera inserisce un elemento in testa alla lista passata
   come primo parametro. Restituisce 1 se l'operazione va a buon fine,
   0 altrimenti */
int AggiungiTelecamera(PNodoListaTelecamere* telecamere, TTelecamera telecamera)
{
    PNodoListaTelecamere aux;

    aux = (PNodoListaTelecamere)malloc(sizeof(TNodoListaTelecamere));

    if (aux == 0)
        return 0;

    aux->telecamera = telecamera;
    aux->succ = *telecamere;

    *telecamere = aux;

    return 1;
}

/*
  1. CaricaTelecamere: carica l'elenco delle telecamere in una lista
  di telecamere.

  Parametri di ingresso: nome del file
  Parametri di uscita: lista delle telecamere (PNodoListaTelecamere)
  Valore restituito: 1 operazione andata a buon fine, 0 altrimenti
 */
int CaricaTelecamere(char* nome_file, PNodoListaTelecamere* telecamere)
{
    // Implementare il corpo della funzione
    FILE* f;
    TTelecamera t;

    f = fopen(nome_file, "r");
    if (f == NULL) return 0;

    while (fscanf(f, "%s%s%s%s", t.id_telecamera, t.id_area, t.ora_inizio, t.ora_fine) == 4) {
        AggiungiTelecamera(telecamere, t);
    }
    return 1;
}

/*
   AggiungiArea inserisce un elemento in testa alla lista passata
   come parametro. Restituisce la nuova testa della lista
*/
PNodoListaAree AggiungiArea(PNodoListaAree aree, char* id_area)
{
    PNodoListaAree aux;

    aux = (PNodoListaAree)malloc(sizeof(TNodoListaAree));

    strcpy(aux->id_area, id_area);
    aux->num_telecamere = 0;
    aux->lista_telecamere = NULL;
    aux->succ = aree;

    return aux;
}

/*
   2. CreaAree: a partire dalla lista delle telecamere, costruisce una
   lista di aree in cui, per ogni area monotorata, ci sia il numero e
   la lista delle telecamere che monitorano quella specifica area.

   Parametri di ingresso: lista delle telecamere
   Parametri di uscita: lista delle aree
*/
PNodoListaAree CreaAree(PNodoListaTelecamere telecamere)
{
    // Implememtare il corpo della funzione
    PNodoListaAree aree = (PNodoListaAree)malloc(sizeof(PNodoListaAree));
    PNodoListaTelecamere aux;
    aree = NULL;
    aux = telecamere;


    while (aux != NULL) {
        while (aree != NULL) {
            if (strcmp(aux->telecamera.id_area, aree->id_area) == 0) {
                AggiungiTelecamera(&(aree->lista_telecamere), telecamere->telecamera);
                aree->num_telecamere++;
            }
            
            aree=aree->succ;
        }
        aree = AggiungiArea(aree, telecamere->telecamera.id_area);
        AggiungiTelecamera(&(aree->lista_telecamere), telecamere->telecamera);
        aree->num_telecamere++;
         
        aux = aux->succ;
    }

    return aree;
}

/*
  3. RegistrazioniDisponibili: dato un orario, e l'identificativo di
  un'area, restituisce la lista delle telecamere che sono in modalit�
  registrazione a quell'ora.

  Parametri di ingresso: lista delle are, orario, identificativo area
  Parametri di uscita: lista delle telecamere
*/

PNodoListaTelecamere RegistrazioniDisponibili(PNodoListaAree aree, char* orario, char* id_area) {
    // Implememtare il corpo della funzione
    PNodoListaTelecamere lista = (PNodoListaTelecamere)malloc(sizeof(PNodoListaTelecamere));
    lista = NULL;
    int hh1, mm1, hh2, mm2, hh3, mm3;
    PNodoListaTelecamere tc = aree->lista_telecamere;

    while (aree != NULL) {
        if (strcmp(aree->id_area, id_area) == 0) { //seleziono l'area di telecamere
            tc = aree->lista_telecamere; //MOdificato ***** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //while (tc != NULL) { //seleziono quali sono in uso nell'orario
            for (int i = 0; i < aree->num_telecamere; i++) {
                sscanf(tc->telecamera.ora_inizio, "%d:%d", &hh1, &mm1);
                sscanf(tc->telecamera.ora_fine, "%d:%d", &hh3, &mm3);
                sscanf(orario, "%d:%d", &hh2, &mm2);
                //printf("\n%d:%d  - %d:%d  -  %d:%d\n", hh1, mm1, hh2, mm2, hh3, mm3);
                if (hh1 <= hh2 && hh3 >= hh2) {
                    /*if (hh1 == hh2 && mm1 <= mm2) {
                        if (hh3 == hh2 && mm3 > mm2) {
                            AggiungiTelecamera(&lista, aree->lista_telecamere->telecamera);
                            printf("\nAggiunta telecamera %s", aree->lista_telecamere->telecamera.id_area);
                            continue;
                        }
                        else continue;
                    }
                    else continue;
                    */
                    AggiungiTelecamera(&lista, tc->telecamera);
                    printf("\nAggiunta telecamera %s", tc->telecamera.id_area);


                }
                //tc = aree->lista_telecamere->succ;
                //tc= tc->succ;

            }
            //} //chiude 2nd while

        }
        aree = aree->succ;
    }
    return lista;
}

/*
  VisualizzaTelecamere visualizza le informazioni delle telecamere
  nella lista di telecamere
 */
void VisualizzaTelecamere(PNodoListaTelecamere telecamere)
{
    printf("\n\n");
    while (telecamere != NULL)
    {
        printf("%-15s %-15s %-5s %-5s\n",
            telecamere->telecamera.id_telecamera,
            telecamere->telecamera.id_area,
            telecamere->telecamera.ora_inizio,
            telecamere->telecamera.ora_fine);

        telecamere = telecamere->succ;
    }
}

int Menu()
{
    int scelta;

    printf("\n\n*** M E N U ***\n"
        "1 - Carica telecamere\n"
        "2 - Crea aree\n"
        "3 - Registrazioni disponibili\n"
        "4 stampa\n"
        "0 - Uscita\n\n"
        "Scelta: ");
    scanf("%d", &scelta);

    return scelta;
}

void DisallocaListaTelecamere(PNodoListaTelecamere lista)
{
    PNodoListaTelecamere p;

    while (lista != NULL)
    {
        p = lista;
        lista = lista->succ;
        free(p);
    }
}

void DisallocaListaAree(PNodoListaAree lista)
{
    PNodoListaAree p;

    while (lista != NULL)
    {
        p = lista;
        lista = lista->succ;
        free(p);
    }
}

int main(void)
{
    PNodoListaTelecamere telecamere = NULL;
    PNodoListaTelecamere telecamere_disponibili = NULL;
    PNodoListaAree       aree = NULL;
    int                  scelta;
    char                 orario[16], id_area[16];


    do {
        scelta = Menu();

        switch (scelta)
        {
        case 1:
            // Invocare la funzione CaricaTelecamere per ottenere la lista telecamere
            if (CaricaTelecamere(NOME_FILE, &telecamere) == 1)
                printf("\nCaricamento del file eseguito");
            else
                printf("Errore");
            break;

        case 2:
            // Invocare la funzione CreaAree per ottenre la lista di liste aree
            aree = CreaAree(telecamere);
            printf("\nAree create con successo");
            break;

        case 3:
            // Acquisire l'orario e l'id_area
            printf("Inserire orario:");
            scanf("%s", orario);
            printf("Inserire l'area id: ");
            scanf("%s", id_area);
            // Invocare la funzione RegistrazioniDisponibili per ottenere la lista telecamere_disponibili
            telecamere_disponibili = RegistrazioniDisponibili(aree, orario, id_area);
            // Visualizzare la lista telecamere_disponibili
            VisualizzaTelecamere(telecamere_disponibili);
            break;
        case 4:
            VisualizzaTelecamere(telecamere);
            break;
        }

    } while (scelta != 0);

    // Didsallocare le liste
    DisallocaListaTelecamere(telecamere);
    DisallocaListaAree(aree);
    return 0;
}

