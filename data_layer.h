#ifndef DATA_LAYER_H
#define DATA_LAYER_H
#include <mysql.h>

struct PersonalFinance {
    int personalFinanceId;
    float income;
    float expense;
};
struct Transaction {
    int personalFinanceId
    int transactionId;
    int year;
    int month;
    int day;
    char type[DESC_MAX_LENGTH];
    char category[DESC_MAX_LENGTH];
    float amount;
    bool status;
};
void initializeDataLayer();
void closeDataLayer();

int* addPersonalFinance(struct PersonalFinance* pf); 
int* addTransaction(int personal_finance_id,struct Transaction* t);
int* deleteTransaction(int transaction_id);
int* updateTransaction(struct Transaction* t);
struct PersonalFinance* loadPersonalFinanceData(int personal_finance_id);
struct Transaction* loadAllTransactions(int personal_finance_id);
int getMaxPersonalFinanceId();
int getMaxTransactionId();

#endif 
