#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data_layer.h"

#define TRANSACTIONS 100
#define DESC_MAX_LENGTH 50



struct PersonalFinance* createPersonalFinanceAccount(int personalFinanceId) {
    struct PersonalFinance* newPersonalFinance = (struct PersonalFinance*)malloc(sizeof(struct PersonalFinance));
    newPersonalFinance->personalFinanceId=personalFinanceId;
    newPersonalFinance->income = 0;
    newPersonalFinance->expense = 0;

    int* a=addPersonalFinance(newPersonalFinance);
    printf("Exiting the program.\n");
    return newPersonalFinance;
}




int main() {

    initializeDataLayer();
    struct PersonalFinance *pf = createPersonalFinanceAccount(1);

    

    return 0;
}