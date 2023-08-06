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


// Function to save financial data to a file
void saveData(struct PersonalFinance pf, const char* filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening the file for writing\n");
        return;
    }

    // Save income and expense data
    fprintf(file, "%f %f\n", pf.income, pf.expense);
    
    // Save income transactions
    fprintf(file, "%d\n", pf.incomeCount);
    for (int i = 0; i < pf.incomeCount; i++) {
        struct Transaction t = pf.transaction_Income[i];
        fprintf(file, "%d %d %d %d %d %s %f\n", t.year, t.month, t.day, t.type, t.category, t.description, t.amount);
    }

    // Save expense transactions
    fprintf(file, "%d\n", pf.expenseCount);
    for (int i = 0; i < pf.expenseCount; i++) {
        struct Transaction t = pf.transaction_Expense[i];
        fprintf(file, "%d %d %d %d %d %s %f\n", t.year, t.month, t.day, t.type, t.category, t.description, t.amount);
    }

    fclose(file);
}


// Function to load financial data from a file
void loadData(struct PersonalFinance *pf, const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file for reading\n");
        return;
    }

    // Load income and expense data
    fscanf(file, "%f %f\n", &pf->income, &pf->expense);
    
    // Load income transactions
    fscanf(file, "%d\n", &pf->incomeCount);
    for (int i = 0; i < pf->incomeCount; i++) {
        char description[255];
        struct Transaction t;
        fscanf(file, "%d %d %d %d %d %254s %f\n", &t.year, &t.month, &t.day, (int*)&t.type, (int*)&t.category, description, &t.amount);
        t.description = strdup(description);
        pf->transaction_Income[i] = t;
    }

    // Load expense transactions
    fscanf(file, "%d\n", &pf->expenseCount);
    for (int i = 0; i < pf->expenseCount; i++) {
        char description[255];
        struct Transaction t;
        fscanf(file, "%d %d %d %d %d %254s %f\n", &t.year, &t.month, &t.day, (int*)&t.type, (int*)&t.category, description, &t.amount);
        t.description = strdup(description);
        pf->transaction_Expense[i] = t;
    }

    fclose(file);
}

int main() {
    struct PersonalFinance *pf = createPersonalFinance();

    // Add some transactions
    struct Transaction *t1 = createTransaction(2023, 7, 25, Income, Salary, "July Salary", 2000.0);
    addTransaction(pf, t1);

    struct Transaction *t2 = createTransaction(2023, 7, 26, Expense, Groceries, "Grocery Shopping", -100.0);
    addTransaction(pf, t2);

    struct Transaction *t3 = createTransaction(2023, 7, 27, Expense, Utilities, "Electricity Bill", -50.0);
    addTransaction(pf, t3);

    printf("Before saving:\n");
    viewTransactions(pf);

    saveData(*pf, "finance_data.txt");

    // Empty the transactions for now to simulate loading from a file
    pf->incomeCount = 0;
    pf->expenseCount = 0;

    loadData(pf, "finance_data.txt");

    printf("\nAfter loading from file:\n");
    viewTransactions(pf);

    float balance = viewBalance(*pf);
    printf("Current Balance: $%.2f\n", balance);

    // Free up the allocated memory
    free(t1->description);
    free(t1);

    free(t2->description);
    free(t2);

    free(t3->description);
    free(t3);

    free(pf);

    return 0;
}
