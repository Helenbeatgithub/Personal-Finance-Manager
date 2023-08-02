#ifndef DATA_LAYER_H
#define DATA_LAYER_H

#include "personal_finance.c"

void initializeDataLayer();
void closeDataLayer();

int* createNewPersonalFinance(struct PersonalFinance* pf);
int* addTransactionToPersonalFinance(struct Transaction* t);

int* updatePersonalFinanceData(struct PersonalFinance* pf);
int* updateTransactionData(struct Transaction* t);

struct PersonalFinance* loadPersonalFinanceData(int personal_finance_id);
struct Transaction* loadAllTransactions(int personal_finance_id);

int* deletePersonalFinanceData(struct PersonalFinance* pf);
int* deleteTransactionData(struct Transaction* t);
#endif 
