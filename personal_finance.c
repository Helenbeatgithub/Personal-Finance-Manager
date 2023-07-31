#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Type {Income, Expense};

struct Transaction {
    int year;
    int month;
    int day;
    enum Type type;
    char* category;
    char* description;
    float amount;
};

struct Transaction* createTransaction(int year, int month, int day,
        enum Type type, char* category, char* description, float amount) {
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
    int transactionCount;
    struct Transaction transactions[];
};

struct PersonalFinance* createPersonalFinance() {
    struct PersonalFinance* newPersonalFinance = (struct PersonalFinance*)malloc(sizeof(struct PersonalFinance));
    newPersonalFinance->income = 0;
    newPersonalFinance->expense = 0;
    newPersonalFinance->transactionCount = 0;
}

void addTransaction(struct PersonalFinance *pf, struct Transaction t) {

}

struct Transaction* viewTransactions(struct PersonalFinance pf) {

}

float viewBalance(struct PersonalFinance pf) {
    return pf.income - pf.expense;
}

void saveData(struct PersonalFinance pf) {

}

void loadData(struct PersonalFinance pf) {

}

int main() {
    return 0;
}