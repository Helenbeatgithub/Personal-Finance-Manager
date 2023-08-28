#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define INCOME_TRANSACTION 100
#define EXPENSE_TRANSACTION 100
#define DESC_MAX_LENGTH 50
#define MAX_ID_LENGTH 50
#define FILE_NAME "user_ids.txt"


struct Transaction {
    int year;
    int month;
    int day;
    char type[DESC_MAX_LENGTH];
    char category[DESC_MAX_LENGTH];
    float amount;
    int transactionId;
};


struct User {
    char id[MAX_ID_LENGTH];
};


char* generateUniqueId() {
    srand(time(0));
    static char id[MAX_ID_LENGTH];
    snprintf(id, MAX_ID_LENGTH, "U%d", rand());
    return id;
}



bool idExists(const char* id) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file != NULL) {
        char line[MAX_ID_LENGTH];
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0; // remove newline
            if (strcmp(line, id) == 0) {
                fclose(file);
                return true;
            }
        }
        fclose(file);
    }
    return false;
}


void saveIdToFile(const char* id) {
    FILE *file = fopen(FILE_NAME, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", id);
        fclose(file);
    }
}


void inputUserId(struct User* user) {
    bool isValidId = false;
    while (!isValidId) {
        printf("Enter your ID (or press Enter if you don't have one): ");
        fgets(user->id, sizeof(user->id), stdin);
        user->id[strcspn(user->id, "\n")] = 0; // remove newline

        if (strlen(user->id) == 0) {
            strcpy(user->id, generateUniqueId());
            printf("A new ID has been generated for you: %s\n", user->id);
            saveIdToFile(user->id);
            isValidId = true;
        } else if (!idExists(user->id)) {
            printf("Error: The entered ID doesn't exist! Please try again.\n");
        } else {
            isValidId = true;
        }
    }
}

int currentTransactionId = 0;  // Global variable to hold the current transaction ID

int generateTransactionId() {
    return ++currentTransactionId;  // Increment and return the current transaction ID
}


struct Transaction* createTransaction(int year, int month, int day,
                                      const char type[DESC_MAX_LENGTH], const char category[DESC_MAX_LENGTH], float amount) {
    struct Transaction* newTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    newTransaction->transactionId = generateTransactionId();  // auto-generate transaction ID
    newTransaction->year = year;
    newTransaction->month = month;
    newTransaction->day = day;
    strcpy(newTransaction->type, type);
    strcpy(newTransaction->category, category);
    newTransaction->amount = amount;
    return newTransaction;
}


int isValidDate(int year, int month, int day) {
    // Define the number of days in each month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year in February
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    if (year < 1900 || year > 2023) return 0; 
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > daysInMonth[month]) return 0;

    return 1;
}

struct Transaction* inputTransactionDetails(const char* type) {
    int year, month, day;
    char category[100];
    float amount;

    do {
        printf("Enter the year of the transaction (e.g. between 1900 and 2023):\n");
        scanf("%d", &year);
        printf("Enter the month of the transaction (1-12):\n");
        scanf("%d", &month);
        printf("Enter the day of the transaction:\n");
        scanf("%d", &day);
    } while (!isValidDate(year, month, day));

    printf("Enter the transaction category:\n");
    scanf("%99s", category); 
    printf("Enter the amount of the transaction:\n");
    scanf("%f", &amount);

    struct Transaction* trans = createTransaction(year, month, day, type, category, amount);
    printf("Transaction ID: %d created successfully!\n", trans->transactionId);
    return trans;
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
        printf("Category: %s\n", pfAmount->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pfAmount->transaction_Income[i]->amount);
    }
    printf("Expense Transactions:\n");
    for (int i = 0; i < pfAmount->expenseIndex; i++) {
        printf("Date: %d/%d/%d\n", pfAmount->transaction_Expense[i]->year, pfAmount->transaction_Expense[i]->month, pfAmount->transaction_Expense[i]->day);
        printf("Type: Expense\n");
        printf("Category: %s\n", pfAmount->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pfAmount->transaction_Expense[i]->amount);
    }
}

void viewTransactionsIncome(struct PersonalFinance *pf) {
    printf("Income Transactions:\n");
    for (int i = 0; i < pf->incomeIndex; i++) {
        printf("Transaction ID: %d\n", pf->transaction_Income[i]->transactionId); // <-- Added line
        printf("Date: %d/%d/%d\n", pf->transaction_Income[i]->year, pf->transaction_Income[i]->month, pf->transaction_Income[i]->day);
        printf("Type: Income\n");
        printf("Category: %s\n", pf->transaction_Income[i]->category);
        printf("Amount: $%.2f\n\n", pf->transaction_Income[i]->amount);
    }
}

void viewTransactionsExpense(struct PersonalFinance *pf) {
    printf("Expense Transactions:\n");
    for (int i = 0; i < pf->expenseIndex; i++) {
        printf("Transaction ID: %d\n", pf->transaction_Expense[i]->transactionId); // <-- Added line
        printf("Date: %d/%d/%d\n", pf->transaction_Expense[i]->year, pf->transaction_Expense[i]->month, pf->transaction_Expense[i]->day);
        printf("Type: Expense\n"); // Note: Corrected the type here, it should be Expense instead of Income
        printf("Category: %s\n", pf->transaction_Expense[i]->category);
        printf("Amount: $%.2f\n\n", pf->transaction_Expense[i]->amount);
    }
}


float viewBalance(struct PersonalFinance *pf) {
    return pf->income - pf->expense;
}

void freePersonalFinance(struct PersonalFinance* pf) {
    for (int i = 0; i < pf->incomeIndex; i++) {
        free(pf->transaction_Income[i]);
    }
    for (int i = 0; i < pf->expenseIndex; i++) {
        free(pf->transaction_Expense[i]);
    }
    free(pf);
}

void deleteExpense(struct PersonalFinance *pf, int id) {
    int index = -1;
    for (int i = 0; i < pf->expenseIndex; i++) {
        if (pf->transaction_Expense[i]->transactionId == id) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        pf->expense -= pf->transaction_Expense[index]->amount;
        // Shift the remaining transactions to fill the gap
        for (int i = index; i < pf->expenseIndex - 1; i++) {
            pf->transaction_Expense[i] = pf->transaction_Expense[i + 1];
        }
        pf->expenseIndex--;
    } else {
        printf("Error: Expense with ID %d not found.\n", id);
    }
}

void deleteIncome(struct PersonalFinance *pf, int id) {
    int index = -1;
    for (int i = 0; i < pf->incomeIndex; i++) {
        if (pf->transaction_Income[i]->transactionId == id) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        pf->income -= pf->transaction_Income[index]->amount;
        // Shift the remaining transactions to fill the gap
        for (int i = index; i < pf->incomeIndex - 1; i++) {
            pf->transaction_Income[i] = pf->transaction_Income[i + 1];
        }
        pf->incomeIndex--;
        
    } else {
        printf("Error: Income with ID %d not found.\n", id + 1);
    }
}


void updateTransaction(struct PersonalFinance *pf, int index, int type, int year, int month, int day, char category[DESC_MAX_LENGTH], float amount) {
    struct Transaction *t; // Defining Local Variables
                          //t is a pointer to a Transaction structure. 
                          //It will be used to reference the specific transaction that needs to be updated.


    if (type == 1) { // Income type check
        if (index < 0 || index >= pf->incomeIndex) { //This checks if the provided index is valid. If it's out of the boundaries of the existing income transactions
            printf("Error: Invalid Income Index.\n");
            return;
        }
        t = pf->transaction_Income[index]; // t now points to the income transaction to be updated.
        pf->income -= t->amount; // old amount of that transaction is subtracted from the total income to adjust the sum.

    } else if (type == 2) { // Expense type check
        if (index < 0 || index >= pf->expenseIndex) {
            printf("Error: Invalid Expense Index.\n");
            return;
        }
        t = pf->transaction_Expense[index];
        pf->expense -= t->amount; // Deduct the old amount

    } else {
        printf("Error: Invalid type specified.\n"); // Handling Invalid Transaction Types:
        return;
    }

    //Updating the Transaction Details:
    t->year = year;
    t->month = month;
    t->day = day;
    strcpy(t->category, category);
    t->amount = amount;

    if (type == 1) {
        pf->income += amount; // Add the updated amount
    } else {
        pf->expense += amount; // Add the updated amount
    }
}

int main() {
    
        struct User currentUser;
        inputUserId(&currentUser);
        
        
        struct PersonalFinance *pf = createPersonalFinance();

        int choice;
        while (1) {
            printf("\nPersonal Finance Management System\n");
            printf("1. Add Income Transaction\n");
            printf("2. Add Expense Transaction\n");
            printf("3. View Transactions by Date\n");
            printf("4. View Transactions by Amount\n");
            printf("5. View All Income Transactions\n");
            printf("6. View All Expense Transactions\n");
            printf("7. View Balance\n");
            printf("8. Update Transaction\n");
            printf("9. Delete Income\n");
            printf("10. Delete Expense\n");
            printf("11. Exit\n");
            printf("Enter your choice: \n");
            scanf("%d", &choice);

            switch (choice) {
                case 1: {
                    struct Transaction* t = inputTransactionDetails("Income");
                    addTransactionIncome(pf, t);
                    break;
                }
                case 2: {
                    struct Transaction* t = inputTransactionDetails("Expense");
                    addTransactionExpense(pf, t);
                    break;
                }

                case 3: {
                    viewTransactionsByDate(pf);
                    break;
                }
                case 4: {
                    viewTransactionsAmount(pf);
                    break;
                }
                case 5: {
                    viewTransactionsIncome(pf);
                    break;
                }
                case 6: {
                    viewTransactionsExpense(pf);
                    break;
                }
                case 7: {
                    float balance = viewBalance(pf);
                    printf("Balance: $%.2f\n", balance);
                    break;
                }
                case 8: {
                    int type, index, year, month, day;
                    char category[DESC_MAX_LENGTH];
                    float amount;

                    printf("Update Transaction\n");
                    printf("------------------------------\n");
                    printf("Enter the type of transaction:\n");
                    printf("1. Income\n");
                    printf("2. Expense\n");
                    scanf("%d", &type);

                    if (type == 1) {
                        printf("Enter the ID of the income transaction to update: ");
                    } else if (type == 2) {
                        printf("Enter the ID of the expense transaction to update: ");
                    } else {
                        printf("Error: Invalid type entered.\n");
                        break;
                    }
                    scanf("%d", &index);

                    do {
                        printf("Enter the updated year of the transaction (e.g., 2023):\n");
                        scanf("%d", &year);
                        printf("Enter the updated month of the transaction (1-12):\n");
                        scanf("%d", &month);
                        printf("Enter the updated day of the transaction (1-31):\n");
                        scanf("%d", &day);

                        if(!isValidDate(year, month, day)) {
                            printf("Error: Invalid date entered. Please try again.\n");
                        }
                    } while(!isValidDate(year, month, day));

                    printf("Enter the updated category of the transaction (e.g., Groceries, Rent):\n");
                    scanf("%s", category); // category is already an address, no need for &

                    printf("Enter the updated amount of the transaction (e.g., 150.50):\n");
                    scanf("%f", &amount);

                    updateTransaction(pf, index - 1, type, year, month, day, category, amount);

                    printf("Transaction updated successfully.\n");
                    break;
                }



                case 9: {
                    int incomeIndex;
                    printf("Enter the ID of the income to delete: ");
                    scanf("%d", &incomeIndex);
                    deleteIncome(pf, incomeIndex - 1);
                    break;
                }

                case 10: {
                    int expenseIndex;
                    printf("Enter the ID of the expense to delete: ");
                    scanf("%d", &expenseIndex);
                    deleteExpense(pf, expenseIndex - 1);
                    break;
                }

                case 11: {
                    freePersonalFinance(pf);
                    printf("Exiting the program.\n");
                    return 0;
                }

                default: {
                    printf("Invalid choice. Please select a valid option.\n");
                }
            }
        }
        
        return 0;
}
