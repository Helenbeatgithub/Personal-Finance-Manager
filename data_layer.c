#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "data_layer.h"

// Database configuration
#define DB_HOST "localhost"
#define DB_USER "username"
#define DB_PASS "password"
#define DB_NAME "finance_manager"

#define CREATE_SUCCESS 0
#define CREATE_FAILURE 1

// Database connection object
static MYSQL* connection;


void initializeDatabase() {
    connection = mysql_init(NULL);

    if (connection == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(connection));
        mysql_close(connection);
        exit(1);
    }

    // Create tables if they don't exist
    const char* createIncomeTableSQL =
        "CREATE TABLE IF NOT EXISTS income ("
        "  id INT AUTO_INCREMENT PRIMARY KEY,"
        "  year INT NOT NULL,"
        "  month INT NOT NULL,"
        "  day INT NOT NULL,"
        "  category INT NOT NULL,"
        "  description VARCHAR(255) NOT NULL,"
        "  amount FLOAT NOT NULL"
        ");";

    const char* createExpenseTableSQL =
        "CREATE TABLE IF NOT EXISTS expense ("
        "  id INT AUTO_INCREMENT PRIMARY KEY,"
        "  year INT NOT NULL,"
        "  month INT NOT NULL,"
        "  day INT NOT NULL,"
        "  category INT NOT NULL,"
        "  description VARCHAR(255) NOT NULL,"
        "  amount FLOAT NOT NULL"
        ");";

    if (mysql_query(connection, createIncomeTableSQL) != 0) {
        fprintf(stderr, "Error creating income table: %s\n", mysql_error(connection));
    }

    if (mysql_query(connection, createExpenseTableSQL) != 0) {
        fprintf(stderr, "Error creating expense table: %s\n", mysql_error(connection));
    }
}

void closeDatabase() {
    mysql_close(connection);
}

int createNewPersonalFinance(struct PersonalFinance* pf) {
    // Implement SQL INSERT query and execute it
    char query[256];
    sprintf(query, "INSERT INTO personal_finance (total_income, total_expense, income_count, expense_count) "
                   "VALUES (%f, %f, %d, %d)", pf->total_income, pf->total_expense, pf->income_count, pf->expense_count);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Insert error: %s\n", mysql_error(conn));
        return CREATE_FAILURE; 
    }

    return CREATE_SUCCESS;
}

int addTransactionToPersonalFinance(struct Transaction* t) {
    // Implement SQL INSERT query and execute it
    char query[256];
    sprintf(query, "INSERT INTO transactions (personal_finance_id, year, month, day, type, category, description, amount) "
                   "VALUES (%d, %d, %d, %d, '%s', '%s', '%s', %f)",
            t->personal_finance_id, t->year, t->month, t->day, t->type == Income ? "Income" : "Expense",
            t->category == Groceries ? "Groceries" : t->category == Utilities ? "Utilities" :
            t->category == Rent ? "Rent" : t->category == Salary ? "Salary" :
            t->category == Transportation ? "Transportation" : "Dining",
            t->description, t->amount);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Insert error: %s\n", mysql_error(conn));
        return CREATE_FAILURE;
    }

    return CREATE_SUCCESS;
}

int updatePersonalFinanceData(struct PersonalFinance* pf) {
    // Implement SQL UPDATE query and execute it
    char query[256];
    sprintf(query, "UPDATE personal_finance SET total_income = %f, total_expense = %f, income_count = %d, expense_count = %d "
                   "WHERE id = %d", pf->total_income, pf->total_expense, pf->income_count, pf->expense_count, pf->id);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Update error: %s\n", mysql_error(conn));
        return CREATE_FAILURE;
    }
    return CREATE_SUCCESS;
}
、

int updateTransactionData(struct Transaction* t) {
    // Implement SQL UPDATE query and execute it
    char query[256];
    sprintf(query, "UPDATE transactions SET year = %d, month = %d, day = %d, type = '%s', category = '%s', "
                   "description = '%s', amount = %f WHERE id = %d",
            t->year, t->month, t->day, t->type == Income ? "Income" : "Expense",
            t->category == Groceries ? "Groceries" : t->category == Utilities ? "Utilities" :
            t->category == Rent ? "Rent" : t->category == Salary ? "Salary" :
            t->category == Transportation ? "Transportation" : "Dining",
            t->description, t->amount, t->id);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Update error: %s\n", mysql_error(conn));
        return CREATE_FAILURE;
    }
    return CREATE_SUCCESS;
}

struct Transaction* loadAllTransactions(int personal_finance_id) {
    char query[512];
    sprintf(query, "SELECT * FROM transactions WHERE personal_finance_id = %d", personal_finance_id);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Select error: %s\n", mysql_error(conn));
        return NULL; // Return NULL to indicate an error
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Store result error: %s\n", mysql_error(conn));
        return NULL; // Return NULL to indicate an error
    }

    int num_transactions = mysql_num_rows(result);
    struct Transaction* transactions = (struct Transaction*)malloc(num_transactions * sizeof(struct Transaction));

    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        struct Transaction* t = &transactions[i];
        t->id = atoi(row[0]);
        t->personal_finance_id = atoi(row[1]);
        t->year = atoi(row[2]);
        t->month = atoi(row[3]);
        t->day = atoi(row[4]);
        t->type = (strcmp(row[5], "Income") == 0) ? Income : Expense;
        t->category = parseCategory(row[6]);
        strncpy(t->description, row[7], sizeof(t->description) - 1);
        t->amount = atof(row[8]);
        i++;
    }

    mysql_free_result(result);
    return transactions;
}

// Load a specific PersonalFinance entry by its ID
struct PersonalFinance* loadPersonalFinanceData(int personal_finance_id) {
    char query[256];
    sprintf(query, "SELECT * FROM personal_finance WHERE id = %d", personal_finance_id);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Select error: %s\n", mysql_error(conn));
        return NULL; // Return NULL to indicate an error
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Store result error: %s\n", mysql_error(conn));
        return NULL; // Return NULL to indicate an error
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL) {
        struct PersonalFinance* pf = (struct PersonalFinance*)malloc(sizeof(struct PersonalFinance));
        pf->id = atoi(row[0]);
        pf->total_income = atof(row[1]);
        pf->total_expense = atof(row[2]);
        pf->income_count = atoi(row[3]);
        pf->expense_count = atoi(row[4]);

        mysql_free_result(result);
        return pf;
    } else {
        fprintf(stderr, "Personal finance entry with ID %d not found.\n", personal_finance_id);
        mysql_free_result(result);
        return NULL; // Return NULL to indicate an error or not found
    }
}



int deletePersonalFinanceData(struct PersonalFinance* pf) {
    // Implement SQL DELETE query and execute it
    char query[256];
    sprintf(query, "DELETE FROM personal_finance WHERE id = %d", pf->id);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Delete error: %s\n", mysql_error(conn));
        return CREATE_FAILURE;
    }
    return CREATE_SUCCESS;
}

// Delete a specific transaction entry based on ID
int deleteTransactionData(struct Transaction* t) {
    // Implement SQL DELETE query and execute it
    char query[256];
    sprintf(query, "DELETE FROM transactions WHERE id = %d", t->id);

    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Delete error: %s\n", mysql_error(conn));
        return CREATE_FAILURE;
    }
    return CREATE_SUCCESS;
}