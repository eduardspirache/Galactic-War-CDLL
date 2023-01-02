// Copyright 2021 Spirache Eduard

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./CircularDoublyLinkedList.h"
#include "./utils.h"

/*
 * Functie care trebuie apelata pentru alocarea si initializarea unei liste.
 * (Setare valori initiale pentru campurile specifice structurii LinkedList).
 */
doubly_linked_list_t *
dll_create(unsigned int data_size)
{
    doubly_linked_list_t *list = malloc(sizeof(*list));
    list->head = NULL;
    list->data_size = data_size;
    list->size = 0;
    return list;
}

/*
 * Functia intoarce un pointer la nodul de pe pozitia n din lista.
 * Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
 * afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
 * pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
 * trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
 * afla pozitia dorita fara sa simulam intreaga parcurgere? Daca n < 0, eroare.
 */
dll_node_t *
dll_get_nth_node(doubly_linked_list_t *list, unsigned int n)
{
    unsigned int i;

    dll_node_t *current = list->head;

    for (i = 0; i < n; i++) {
        current = current->next;
    }

    return current;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
 * get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
 * care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
 * adaugam nodul nou la finalul listei. Daca n < 0, eroare.
 */
void
dll_add_nth_node(doubly_linked_list_t *list, unsigned int n, const void *data)
{
    dll_node_t *node = malloc(sizeof(dll_node_t));
    dll_node_t *current = list->head;

    node->data = malloc(list->data_size);
    memcpy(node->data, data, list->data_size);

    if (list->size == 0) {
        list->head = node;
        list->head->next = list->head;
        list->head->prev = list->head;
    } else if (n == 0 && list->size > 0) {
        node->prev = current->prev;
        node->next = current;
        current->prev = node;
        node->prev->next = node;
        list->head = node;
    }  else {
        current = dll_get_nth_node(list, n);
        node->prev = current->prev;
        node->next = current;
        node->next->prev = node;
        node->prev->next = node;
    }

    list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
 * proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
 * finalul listei. Daca n < 0, eroare. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
dll_node_t *
dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n)
{
    dll_node_t *current = list->head;
    dll_node_t *before = NULL;
    dll_node_t *after = NULL;

    if (list->size == 1) {
        list->head->next = list->head;
        list->head->prev = list->head;
    } else if (list->size == 2) {
        if (n == 1) {
            current = current->next;
            list->head->next = list->head;
            list->head->prev = list->head;
        } else {
            after = current->next;
            list->head = after;
            after->next = after;
            after->prev = after;
        }
    } else if (list->size > 2 && n == 0) {
        current->prev->next = current->next;
        current->next->prev = current->prev;
        list->head = current->next;
    } else {
        current = dll_get_nth_node(list, n);
        before = current->prev;
        after = current->next;
        before->next = after;
        after->prev = before;
    }

    list->size--;
    return current;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
dll_get_size(doubly_linked_list_t *list)
{
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void dll_free(doubly_linked_list_t **pp_list)
{
    unsigned int i;

    if (*pp_list == NULL) {
        return;
    }

    dll_node_t *after = NULL;
    dll_node_t *current = (*pp_list)->head;

    for (i = 0; i < (*pp_list)->size; i++) {
        after = current->next;
        free(current->data);
        free(current);
        current = after;
    }

    free(*pp_list);
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista separate printr-un spatiu, incepand de la primul nod din lista.
 */
void dll_print_int_list(doubly_linked_list_t *list)
{
    unsigned int i;
    dll_node_t *current = list->head;
    for (i = 0; i < list->size; i++) {
        printf("%d ", *(int *)current->data);
        current = current->next;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista separate printr-un spatiu, incepand de la primul nod din
 * lista.
 */
void dll_print_string_list(doubly_linked_list_t *list)
{
    unsigned int i;
    dll_node_t *current = list->head;
    for (i = 0; i < list->size; i++) {
        printf("%s ", (char *)current->data);
        current = current->next;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void dll_print_ints_left_circular(dll_node_t *start)
{
    dll_node_t *current = start->prev;
    printf("%d ", *(int *)start);
    while (current != start) {
        printf("%d ", *(int *)current);
        current = current->prev;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
 * stocate in nodurile din lista, separate printr-un spatiu, incepand de la
 * nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
 * circulara, pana cand sunt afisate valorile tuturor nodurilor.
 */
void dll_print_ints_right_circular(dll_node_t *start)
{
    dll_node_t *current = start->next;
    printf("%d ", *(int *)start);
    while (current != start) {
        printf("%d ", *(int *)current);
        current = current->next;
    }
    printf("\n");
}
