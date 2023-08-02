#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCOME_TRANSACTION 100
#define EXPENSE_TRANSACTION 100

enum Type {Income, Expense};
enum Category {Groceries, Utilities, Rent, Salary, Transportation, Dining};

struct Transaction {
    int year;
    int month;
    int day;
    enum Type type;
    enum Type category;
    char* description;
    float amount;
};

struct Transaction* createTransaction(int year, int month, int day,
        enum Type type, enum Type category, char* description, float amount) {
        struct Transaction* newTransaction = (struct Transaction*) malloc(sizeof(struct Transaction));
        newTransaction->year = year;
        newTransaction->month = month;
        newTransaction->day = day;
        newTransaction->type = type;
        newTransaction->category = category;
        newTransaction->description = description;
        newTransaction->amount = amount;
        return newTransaction;
}

struct PersonalFinance {
    float income;
    float expense;
    int incomeCount;
    int expenseCount;
    struct Transaction transaction_Expense[EXPENSE_TRANSACTION];
    struct Transaction transaction_Income[INCOME_TRANSACTION];
};

struct PersonalFinance* createPersonalFinance() {
    struct PersonalFinance* newPersonalFinance = (struct PersonalFinance*)malloc(sizeof(struct PersonalFinance));
    newPersonalFinance->income = 0;
    newPersonalFinance->expense = 0;
    newPersonalFinance->incomeCount = 0;
    newPersonalFinance->expenseCount= 0;
}

void addTransaction(struct PersonalFinance *pf, struct Transaction* t) {
    if (t->type == "Income") {
        pf->transaction_Income[pf->incomeCount] = *t;
        pf->income += t->amount;
        pf->incomeCount += 1;
    }
    else {
        pf->transaction_Expense[pf->expenseCount] = *t;
        pf->expense += t->amount;
        pf->expenseCount += 1;
    }
}

void viewTransactions(struct PersonalFinance *pf) {
    printf("Income Transactions:\n");
    for (int i = 0; i < pf->incomeCount; i++) {
        printf("Date: %d/%d/%d\n", pf->transaction_Income[i].year, pf->transaction_Income[i].month, pf->transaction_Income[i].day);
        printf("Type: Income\n");
        printf("Category: %d\n", pf->transaction_Income[i].category);
        printf("Description: %s\n", pf->transaction_Income[i].description);
        printf("Amount: $%.2f\n\n", pf->transaction_Income[i].amount);
    }
}


float viewBalance(struct PersonalFinance pf) {
    return pf.income - pf.expense;
}

void sortTransactionByAmount(struct PersonalFinance *pf) {
    
    
}
void sortTransactionByDate(struct PersonalFinance *pf) {

}

void saveData(struct PersonalFinance pf) {

}

void loadData(struct PersonalFinance pf) {

}

int main() {
    return 0;
}