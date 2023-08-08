#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCOME_TRANSACTION 100
#define EXPENSE_TRANSACTION 100
#define DESC_MAX_LENGTH 50

struct Transaction {
    int year;
    int month;
    int day;
    char type[DESC_MAX_LENGTH];
    char category[DESC_MAX_LENGTH];
    float amount;
};

struct Transaction* createTransaction(int year, int month, int day,
                                     char type[DESC_MAX_LENGTH], char* category, float amount) {
    struct Transaction* newTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    newTransaction->year = year;
    newTransaction->month = month;
    newTransaction->day = day;
    strcpy(newTransaction->type, type);
    strcpy(newTransaction->category,category);
    newTransaction->amount = amount;
    return newTransaction;
}

struct PersonalFinance {
    float income;
    float expense;
    int incomeIndex;
    int expenseIndex;
    struct Transaction* transaction_Expense[EXPENSE_TRANSACTION];
    struct Transaction* transaction_Income[INCOME_TRANSACTION];
};

struct PersonalFinance* createPersonalFinance() {
    struct PersonalFinance* newPersonalFinance = (struct PersonalFinance*)malloc(sizeof(struct PersonalFinance));
    newPersonalFinance->income = 0;
    newPersonalFinance->expense = 0;
    newPersonalFinance->incomeIndex = 0;
    newPersonalFinance->expenseIndex= 0;
    return newPersonalFinance;
}

void addTransactionIncome(struct PersonalFinance *pf, struct Transaction* t) {
    pf->transaction_Income[pf->incomeIndex] = t;
    pf->income += t->amount;
    pf->incomeIndex++;
}

void addTransactionExpense(struct PersonalFinance *pf, struct Transaction* t) {
    pf->transaction_Expense[pf->expenseIndex] = t;
    pf->expense += t->amount;
    pf->expenseIndex++;
}

//helper method for sorting by date
int compareDates(struct Transaction* t1, struct Transaction* t2) {
    if (t1->year != t2->year) {
        return t1->year - t2->year;
    } else if (t1->month != t2->month) {
        return t1->month - t2->month;
    } else {
        return t1->day - t2->day;
    }
}

struct PersonalFinance* sortTransactionsByDate(struct PersonalFinance *pf) {
    int i, j;
    int n = pf->incomeIndex;
    //sort income transactions
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (compareDates(pf->transaction_Income[j], pf->transaction_Income[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            struct Transaction* temp = pf->transaction_Income[i];
            pf->transaction_Income[i] = pf->transaction_Income[min_idx];
            pf->transaction_Income[min_idx] = temp;
        }
    }
    //sort expense transactions
    n = pf->expenseIndex;
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (compareDates(pf->transaction_Expense[j], pf->transaction_Expense[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            struct Transaction* temp = pf->transaction_Expense[i];
            pf->transaction_Expense[i] = pf->transaction_Expense[min_idx];
            pf->transaction_Expense[min_idx] = temp;
        }
    }
    return pf;
}

void viewTransactionsByDate(struct PersonalFinance *pf) {
    //sort by date
    struct PersonalFinance* pfDate = sortTransactionsByDate(pf);
    //print income transactions
    printf("Income Transactions:\n");
    for (int i = 0; i < pfDate->incomeIndex; i++) {
        printf("Date: %d/%d/%d\n", pfDate->transaction_Income[i]->year, pfDate->transaction_Income[i]->month, pfDate->transaction_Income[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pfDate->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pfDate->transaction_Income[i]->amount);
    }
    //print expense transactions
    printf("Expense Transactions:\n");
    for (int i = 0; i < pfDate->expenseIndex; i++) {
        printf("Date: %d/%d/%d\n", pfDate->transaction_Expense[i]->year, pfDate->transaction_Expense[i]->month, pfDate->transaction_Expense[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pfDate->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pfDate->transaction_Expense[i]->amount);
    }
}

struct PersonalFinance* sortTransactionByAmount(struct PersonalFinance *pf) {
    int i, j;
    int n = pf->incomeIndex;
    //sort income transactions
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (pf->transaction_Income[j]->amount > pf->transaction_Income[j + 1]->amount) {
                struct Transaction* temp = pf->transaction_Income[j];
                pf->transaction_Income[j] = pf->transaction_Income[j + 1];
                pf->transaction_Income[j + 1] = temp;
            }
        }
    }
    //sort expense transactions
    n = pf->expenseIndex;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (pf->transaction_Expense[j]->amount > pf->transaction_Expense[j + 1]->amount) {
                struct Transaction* temp = pf->transaction_Expense[j];
                pf->transaction_Expense[j] = pf->transaction_Expense[j + 1];
                pf->transaction_Expense[j + 1] = temp;
            }
        }
    }
    return pf;
}

void viewTransactionsAmount(struct PersonalFinance *pf) {
    //sort by amount
    struct PersonalFinance* pfAmount = sortTransactionByAmount(pf);
    printf("Income Transactions:\n");
    for (int i = 0; i < pfAmount->incomeIndex; i++) {
        printf("Date: %d/%d/%d\n", pfAmount->transaction_Income[i]->year, pfAmount->transaction_Income[i]->month, pfAmount->transaction_Income[i]->day);
        printf("Type: Income\n");
        printf("Category: %d\n", pfAmount->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pfAmount->transaction_Income[i]->amount);
    }
    printf("Expense Transactions:\n");
    for (int i = 0; i < pfAmount->expenseIndex; i++) {
        printf("Date: %d/%d/%d\n", pfAmount->transaction_Expense[i]->year, pfAmount->transaction_Expense[i]->month, pfAmount->transaction_Expense[i]->day);
        printf("Type: Expense\n");
        printf("Category: %d\n", pfAmount->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pfAmount->transaction_Expense[i]->amount);
    }
}

void viewTransactionsIncome(struct PersonalFinance *pf) {
    printf("Income Transactions:\n");
    for (int i = 0; i < pf->incomeIndex; i++) {
        printf("Date: %d/%d/%d\n", pf->transaction_Income[i]->year, pf->transaction_Income[i]->month, pf->transaction_Income[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pf->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pf->transaction_Income[i]->amount);
    }
}

void viewTransactionsExpense(struct PersonalFinance *pf) {
    printf("Expense Transactions:\n");
    for (int i = 0; i < pf->expenseIndex; i++) {
        printf("Date: %d/%d/%d\n", pf->transaction_Expense[i]->year, pf->transaction_Expense[i]->month, pf->transaction_Expense[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pf->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pf->transaction_Expense[i]->amount);
    }
}

float viewBalance(struct PersonalFinance *pf) {
    return pf->income - pf->expense;
}

void freeTransaction(struct Transaction* t) {
    free(t);
}

void freePersonalFinance(struct PersonalFinance* pf) {
    for (int i = 0; i < pf->incomeCount; i++) {
        free(pf->transaction_Income[i].description);
    }
    for (int i = 0; i < pf->expenseCount; i++) {
        free(pf->transaction_Expense[i].description);
    }
    free(pf);
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
        struct Transaction t;
        fscanf(file, "%d %d %d %d %d %253s %f\n",
               &t.year, &t.month, &t.day, (int*)&t.type, (int*)&t.category, t.description, &t.amount);
        pf->transaction_Income[i] = t;
    }

    // Load expense transactions
    fscanf(file, "%d\n", &pf->expenseCount);
    for (int i = 0; i < pf->expenseCount; i++) {
        struct Transaction t;
        fscanf(file, "%d %d %d %d %d %253s %f\n",
               &t.year, &t.month, &t.day, (int*)&t.type, (int*)&t.category, t.description, &t.amount);
        pf->transaction_Expense[i] = t;
    }

    fclose(file);
}



int main() {
    struct PersonalFinance pf = createPersonalFinance();

    // Use &pf to pass a pointer to the struct where required
    viewTransactions(&pf);
    saveData(&pf, "finance_data.txt");

    // Directly modify the struct for resetting counts
    pf.incomeCount = 0;
    pf.expenseCount = 0;

    loadData(&pf, "finance_data.txt");
    viewTransactions(&pf);

    float balance = viewBalance(&pf);
    printf("Current Balance: $%.2f\n", balance);

    // The freePersonalFinance function was not provided in the initial code, and may not be needed if dynamic allocation isn't being used. Remove the call or provide the function definition.
    // freePersonalFinance(&pf);

    return 0;
}
