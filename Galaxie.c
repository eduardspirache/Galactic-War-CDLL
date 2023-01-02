// Copyright 2021 Spirache Eduard

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Galaxie.h"

int main(void) {
    int M, i;
    doubly_linked_list_t* galaxie = dll_create(sizeof(info));
    char comanda[4];
    scanf("%d", &M);
    for (i = 0; i < M; i ++) {
        scanf("%s", comanda);
        if (!strcmp(comanda, "ADD")) {
            ADD(galaxie);
        } else if (!strcmp(comanda, "BLH")) {
            BLH(galaxie);
        } else if (!strcmp(comanda, "UPG")) {
            UPG(galaxie);
        } else if (!strcmp(comanda, "EXP")) {
            EXP(galaxie);
        } else if (!strcmp(comanda, "RMV")) {
            RMV(galaxie);
        } else if (!strcmp(comanda, "COL")) {
            COL(galaxie);
        } else if (!strcmp(comanda, "ROT")) {
            ROT(galaxie);
        } else if (!strcmp(comanda, "SHW")) {
            SHW(galaxie);
        }
    }

    free_all(&galaxie);
    free(galaxie);
    return 0;
}
