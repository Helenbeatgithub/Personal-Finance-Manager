#ifndef DATA_LAYER_H
#define DATA_LAYER_H

#include "personal_finance.c"

void initializeDataLayer();
void closeDataLayer();

int* addPersonalFinance(struct PersonalFinance* pf); 
int* addTransaction(int personal_finance_id，struct Transaction* t);
int* deleteTransaction(int transaction_id);
int* updateTransaction(struct Transaction* t);
struct PersonalFinance* loadPersonalFinanceData(int personal_finance_id);
struct Transaction* loadAllTransactions(int personal_finance_id);

#endif 
