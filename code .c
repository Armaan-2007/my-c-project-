#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 typedef struct {
    char category[50];
    float amount;
    char date[15];
    char note[100];
 } Expense;
 Expense *expenses = NULL;
 int count = 0;
 void addExpense() {
    expenses = realloc(expenses, (count + 1) * sizeof(Expense));
    if (expenses == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    printf("Enter Category: ");
    scanf("%s", expenses[count].category);
    printf("Enter Amount: ");
    scanf("%f", &expenses[count].amount);
    printf("Enter Date (dd/mm/yyyy): ");
    scanf("%s", expenses[count].date);
    printf("Enter Note: ");
    getchar();
    fgets(expenses[count].note, sizeof(expenses[count].note), stdin);
    expenses[count].note[strcspn(expenses[count].note, "\n")] = '\0';
    count++;
    printf("Expense added successfully!\n");
 }
 void displayExpenses() {
    if (count == 0) {
        printf("No expenses to display.\n");
        return;
    }
    printf("\n%-5s %-15s %-10s %-12s %-30s\n", "S.No", "Category", "Amount", "Date", "Note");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-15s %-10.2f %-12s %-30s\n",
               i + 1, expenses[i].category, expenses[i].amount,
               expenses[i].date, expenses[i].note);
    }
 }
 void calculateTotal() {
    float total = 0;
    for (int i = 0; i < count; i++) total += expenses[i].amount;
    printf("Total Expenses: %.2f\n", total);
 }
 void saveToFile() {
    FILE *fp = fopen("expenses.txt", "w");
    if (fp == NULL) { printf("Error opening file!\n"); return; }
    for (int i = 0; i < count; i++)
        fprintf(fp, "%s %.2f %s %s\n",
                expenses[i].category, expenses[i].amount,
                expenses[i].date, expenses[i].note);
    fclose(fp);
    printf("Expenses saved to file successfully!\n");
 }
 void loadFromFile() {
    FILE *fp = fopen("expenses.txt", "r");
    if (fp == NULL) { printf("No saved file found.\n"); return; }
    Expense temp;
    while (fscanf(fp, "%s %f %s %[^
 ]", temp.category, &temp.amount, temp.date, temp.note) == 4) {
        expenses = realloc(expenses, (count + 1) * sizeof(Expense));
        expenses[count++] = temp;
    }
    fclose(fp);
    printf("Expenses loaded from file successfully!\n");
 }
int main() {
    int choice;
    loadFromFile();
    while (1) {
        printf("\n====== Personal Expense Tracker ======\n");
        printf("1. Add Expense\n2. View Expenses\n3. Calculate Total\n4. Save to File\n5. Load from File\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addExpense(); break;
            case 2: displayExpenses(); break;
            case 3: calculateTotal(); break;
            case 4: saveToFile(); break;
            case 5: loadFromFile(); break;
            case 6:
                saveToFile(); free(expenses);
                printf("Exiting program... Goodbye!\n");
                exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}
