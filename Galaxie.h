// Copyright 2021 Spirache Eduard

#ifndef GALAXIE_H_
#define GALAXIE_H_

#pragma once

#include "./utils.h"
#include "./CircularDoublyLinkedList.h"

#define DEFAULT_VAL 1
#define MAX_NAME_SIZE 21

typedef struct
{
    char nume[MAX_NAME_SIZE];
    int numarScuturi;
    int planeteDistruse;
    doubly_linked_list_t *listaScuturi;
} info;

void DELETE(doubly_linked_list_t *galaxie, int indice);

/*  ADD
 * Se adauga in galaxie planeta <nume_planetă> 
 * pe poziția <indice_planeta> (initial galaxia este goala)
 * Această planetă va avea <număr_scuturi> scuturi => toate 1
 * daca <indice_planeta> > numărul de planete din galaxie => out of b
*/
void ADD(doubly_linked_list_t *galaxie)
{
    info planetaNoua;
    int i, numarScuturi, val = DEFAULT_VAL;
    unsigned int  indice;
    scanf("%s%d%d", planetaNoua.nume, &indice, &numarScuturi);
    // copiez detaliile planetei

    if (indice > galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    planetaNoua.numarScuturi = numarScuturi;
    planetaNoua.planeteDistruse = 0;

    // creez o lista de scuturi si intializez scuturile cu 1
    planetaNoua.listaScuturi = dll_create(sizeof(int));
    for (i = 0; i < numarScuturi; i++) {
        dll_add_nth_node(planetaNoua.listaScuturi, i, &val);
    }

    // adaug planeta in galaxie
    dll_add_nth_node(galaxie, indice, &planetaNoua);
    printf("The planet %s has joined the galaxy.\n", planetaNoua.nume);
}

/*  BLH
 * Se sterge planeta de pe pozitia indice
 * Daca indicele > numarul de planete => out of bounds
*/
void BLH(doubly_linked_list_t *galaxie)
{
    unsigned int indice;
    dll_node_t *eaten;
    info* planeta;
    scanf("%d", &indice);

    if (indice >= galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    // salvez planeta in node-ul "eaten" pentru eliberare
    eaten = dll_remove_nth_node(galaxie, indice);
    planeta = (info *)eaten->data;

    printf("The planet %s has been eaten by the vortex.\n",
           planeta->nume);

    // eliberez memoria pentru planeta
    dll_free(&(planeta->listaScuturi));
    free(eaten->data);
    free(eaten);
}

/*  UPG
 * Planeta de la pozitia <indice_planeta> 
 * va adauga <valoare_upgrade> la scutul de pe poziția <indice_scut>
 * Dacă <indice_planeta> este mai mare decât numărul de planete => out of bounds
 * Dacă <indice_scut> este mai mare decât numărul de scuturi => shield out of bo
*/
void UPG(doubly_linked_list_t *galaxie)
{
    unsigned int indice_planeta;
    int indice_scut, valoare_upgrade;
    int *val;
    info *planeta;
    scanf("%d%d%d", &indice_planeta, &indice_scut, &valoare_upgrade);

    if (indice_planeta >= galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    planeta = (info *)dll_get_nth_node(galaxie, indice_planeta)->data;

    if (indice_scut >= planeta->numarScuturi) {
        printf("Shield out of bounds!\n");
        return;
    }

    dll_node_t *upgraded = dll_get_nth_node(planeta->listaScuturi, indice_scut);
    val = (int *)upgraded->data;
    *val += valoare_upgrade;
}

/*  EXP
 * Planeta aflata la pozitia <indice_planeta> va adăuga un scut cu 
 * valoarea <valoare_scut> la finalul listei de scuturi.
 * dacă <indice_planeta> > numărul de planete din galaxie => out of b
*/
void EXP(doubly_linked_list_t *galaxie)
{
    unsigned int indice_planeta;
    int valoare_scut;
    info *planeta;
    scanf("%d%d", &indice_planeta, &valoare_scut);

    if (indice_planeta >= galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    planeta = (info *)dll_get_nth_node(galaxie, indice_planeta)->data;

    dll_add_nth_node(planeta->listaScuturi,
    planeta->numarScuturi, &valoare_scut);

    planeta->numarScuturi++;
}

/*  RMV
 * Planeta aflata la pozitia <indice_planeta>
 * isi va pierde scutul de pe pozitia <indice_scut>.
 * Dacă <indice_planeta> > numărul de planete din galaxie. => out of b
 * Dacă <indice_scut> > numărul de scuturi ale planetei => shield out of b
 * O planetă nu poate avea mai putin de 4 scuturi
*/
void RMV(doubly_linked_list_t *galaxie)
{
    unsigned int indice_planeta;
    int indice_scut;
    info *planeta;
    dll_node_t *eaten;
    scanf("%d%d", &indice_planeta, &indice_scut);

    if (indice_planeta >= galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    planeta = (info *)dll_get_nth_node(galaxie, indice_planeta)->data;

    if (indice_scut >= planeta->numarScuturi) {
        printf("Shield out of bounds!\n");
        return;
    }

    if (planeta->numarScuturi <= 4) {
        printf("A planet cannot have less than 4 shields!\n");
        return;
    }

    eaten = dll_remove_nth_node(planeta->listaScuturi, indice_scut);
    free(eaten->data);
    free(eaten);
    planeta->numarScuturi--;
}

/*  COL
 *  planet1→next = planet2
 *  Planetele se vor ciocni în 1 unitate de scut.
 * I) Scutul lui planet1 ce se va ciocni va fi 
 * elementul de pe pozitia size / 4 din lista de scuturi
 * II) Scutul lui planet2 ce se va ciocni va fi elementul de pe poziția
 * (size / 4) * 3, unde size = dimensiunea listei de scuturi a planetei.
 *  La coliziune, fiecare scut afectat va pierde o unitate.
 *  Când o planetă face coliziune într-un loc în care scutul are valoarea 0, 
 * planeta face implozie si este eliminata din galaxie. 
 *  Dacă ambele planete au scuturi în punctul de ciocnire cu valoarea 0, 
 * vor imploda amândouă, prima oară afisându-se mesajul pentru planet1.
 *   In cazul in care o planeta moare => planeta->planeteDistruse ++
 *  Dacă <indice_planeta1> sau <indice_planeta2> > numarul de planete => out
*/
void COL(doubly_linked_list_t *galaxie)
{
    unsigned int indice1, indice2;
    int punct_ciocnire1, punct_ciocnire2;
    info *planeta1, *planeta2;
    int *putere_scut1, *putere_scut2;
    scanf("%d%d", &indice1, &indice2);

    if (indice1 > galaxie->size || indice2 > galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    planeta1 = (info *)dll_get_nth_node(galaxie, indice1)->data;
    planeta2 = (info *)dll_get_nth_node(galaxie, indice2)->data;

    // setam indexul scuturilor care se vor ciocni
    punct_ciocnire1 = planeta1->numarScuturi / 4;
    punct_ciocnire2 = (planeta2->numarScuturi / 4) * 3;

    // pointeri la scuturile care se vor ciocni
    dll_node_t *scut1 = dll_get_nth_node(planeta1->listaScuturi,
                        punct_ciocnire1);
    dll_node_t *scut2 = dll_get_nth_node(planeta2->listaScuturi,
                        punct_ciocnire2);

    // pointeri la valorile scuturilor pentru a le putea modifica
    putere_scut1 = (int *)scut1->data;
    putere_scut2 = (int *)scut2->data;

    // scadem puterea scuturilor
    *putere_scut1 -= 1;
    *putere_scut2 -= 1;

    if (*putere_scut1 < 0) {
        printf("The planet %s has imploded.\n", planeta1->nume);
        planeta2->planeteDistruse++;
    }

    if (*putere_scut2 < 0) {
        printf("The planet %s has imploded.\n", planeta2->nume);
        DELETE(galaxie, indice2);
        planeta1->planeteDistruse++;
    }

    if (*putere_scut1 < 0) {
        DELETE(galaxie, indice1);
    }
}

/*  ROT
 *  Se rotește planeta aflata la pozitia <indice_planeta> 
 * cu un numar de <unitati> unitati în direcția <sens>
 *  Sens = c - ceasornic ; t - trigonometric
 *  Dacă <indice_planeta> > numărul de planete din galaxie => Out
 *  Orice alt caracter în afară de c și t va fi considerat invalid
 *  Rotirea trebuie realizata eficient. De exemplu, daca o planetă 
 * cu 10 scuturi trebuie să se rotească cu 1043 de unitați, se obtine acelasi
 * rezultat ca in cazul in care se realizeaza rotirea cu 3 unitati.
*/
void ROT(doubly_linked_list_t *galaxie)
{
    unsigned int indice_planeta;
    int unitati;
    int numar_rotiri;
    char sens;
    info *planeta;
    dll_node_t *current;
    scanf("%d %c %d", &indice_planeta, &sens, &unitati);

    if (indice_planeta >= galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }
    if (sens != 'c' && sens != 't') {
        printf("Not a valid direction!\n");
        return;
    }

    planeta = (info *)dll_get_nth_node(galaxie, indice_planeta)->data;
    current = planeta->listaScuturi->head;

    numar_rotiri = unitati % planeta->numarScuturi;

    for (int i = 0; i < numar_rotiri; i++) {
        if (sens == 'c') {
            current = current->prev;
        } else {
            current = current->next;
        }
    }
    planeta->listaScuturi->head = current;
}

/* SHW
 * NAME: <nume_planeta>
 * CLOSEST: <nume_vecin_stanga> (prev) and <nume_vecin_dreapta> (next)
 * SHIELDS: <valoare_shield0> <valoare_shield1> <valoare_shield2> ...
 * KILLED: <numarul_de_planete_distruse>
 * dacă o planetă are un singur vecin (sunt doar 2 planete în galaxie)
 * CLOSEST: <nume_vecin>
 * dacă o planeta nu are vecini (exista o singura planeta in galaxie)
 * CLOSEST: none
*/
void SHW(doubly_linked_list_t *galaxie)
{
    unsigned int indicePlaneta, i;
    int j;
    scanf("%d", &indicePlaneta);

    if (indicePlaneta >= galaxie->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    dll_node_t *current = galaxie->head;

    for (i = 0; i < indicePlaneta; i++) {
        if (current->next != galaxie->head)
            current = current->next;
    }

    info planeta = *(info *)(current->data);
    // afisam detaliile
    printf("NAME: %s\n", planeta.nume);
    if (galaxie->size < 2) {
        printf("CLOSEST: none\n");
    } else if (galaxie->size == 2) {
        if (indicePlaneta == 1) {
            printf("CLOSEST: %s\n", (*(info *)(current->prev->data)).nume);
        } else {
            printf("CLOSEST: %s\n", (*(info *)(current->next->data)).nume);
        }
    } else {
        printf("CLOSEST: %s and %s\n", (*(info *)(current->prev->data)).nume,
               (*(info *)(current->next->data)).nume);
    }
    printf("SHIELDS: ");

    dll_node_t *secondCurrent = planeta.listaScuturi->head;

    for (j = 0; j < planeta.numarScuturi; j++) {
        printf("%d ", *(int *)(secondCurrent->data));
        secondCurrent = secondCurrent->next;
    }
    printf("\n");
    printf("KILLED: %d\n", planeta.planeteDistruse);
}

//////////////// FUNCTII SECUNDARE ////////////////

// Functie care sterge o planeta
void DELETE(doubly_linked_list_t *galaxie, int indice)
{
    dll_node_t *eaten;
    info* planeta;

    eaten = dll_remove_nth_node(galaxie, indice);
    planeta = (info *)eaten->data;

    dll_free(&planeta->listaScuturi);
    free(eaten->data);
    free(eaten);
}

// galaxia e doubly linked list
// trebuie sa parcurg galaxia pentru fiecare planeta in parte si
// dealoc lista de scuturi si structura in sine

// Functie de free
void free_all(doubly_linked_list_t **galaxie)
{
    unsigned int i;
    info* planeta;
    dll_node_t *after = NULL;
    dll_node_t *current = (*galaxie)->head;

    // parcurgem galaxia si dealocam fiecare planeta
    for (i = 0; i < (*galaxie)->size; i++) {
        planeta = (info *)(current->data);
        after = current->next;
        dll_free(&planeta->listaScuturi);
        free(current->data);
        free(current);
        current = after;
    }
}
/*
// GALAXIA
struct doubly_linked_list_t
{
    dll_node_t* head; ->data = PLANETA ->lista scuturi
    unsigned int data_size;
    unsigned int size;
};
// PLANETA
typedef struct 
{
    char nume[MAX_NAME_SIZE];
    int numarScuturi;
    int planeteDistruse;
    doubly_linked_list_t* listaScuturi;
}info;
// LISTA SCUTURI
struct doubly_linked_list_t
{
    dll_node_t* head;
    unsigned int data_size;
    unsigned int size;
};
*/

#endif  // GALAXIE_H_
