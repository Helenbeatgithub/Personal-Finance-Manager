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
    int i, j;
    int n = pf->incomeCount;
    
    // Sort Income Transactions using Bubble Sort
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (pf->transaction_Income[j].amount > pf->transaction_Income[j + 1].amount) {
                // Swap the transactions
                struct Transaction temp = pf->transaction_Income[j];
                pf->transaction_Income[j] = pf->transaction_Income[j + 1];
                pf->transaction_Income[j + 1] = temp;
            }
        }
    }

    // Sort Expense Transactions using Bubble Sort
    n = pf->expenseCount;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (pf->transaction_Expense[j].amount > pf->transaction_Expense[j + 1].amount) {
                // Swap the transactions
                struct Transaction temp = pf->transaction_Expense[j];
                pf->transaction_Expense[j] = pf->transaction_Expense[j + 1];
                pf->transaction_Expense[j + 1] = temp;
            }
        }
    }
}



void sortTransactionByDate(struct PersonalFinance *pf) {
    int i, j;
    int n = pf->incomeCount;

    // Sort Income Transactions using Selection Sort
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (compareDates(pf->transaction_Income[j], pf->transaction_Income[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            // Swap the transactions
            struct Transaction temp = pf->transaction_Income[i];
            pf->transaction_Income[i] = pf->transaction_Income[min_idx];
            pf->transaction_Income[min_idx] = temp;
        }
    }

    // Sort Expense Transactions using Selection Sort
    n = pf->expenseCount;
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (compareDates(pf->transaction_Expense[j], pf->transaction_Expense[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            // Swap the transactions
            struct Transaction temp = pf->transaction_Expense[i];
            pf->transaction_Expense[i] = pf->transaction_Expense[min_idx];
            pf->transaction_Expense[min_idx] = temp;
        }
    }
}

// Helper function to compare two dates (earlier date < later date)
int compareDates(struct Transaction t1, struct Transaction t2) {
    if (t1.year != t2.year) {
        return t1.year - t2.year;
    }
    if (t1.month != t2.month) {
        return t1.month - t2.month;
    }
    return t1.day - t2.day;
}


void saveData(struct PersonalFinance pf) {

}

void loadData(struct PersonalFinance pf) {

}

int main() {
    return 0;
}
