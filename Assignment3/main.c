#include <stdio.h>

double get_balance(); /* Reads balance from file */
void update_balance(double new_amount); /* Updates balance in the file */
void check_balance(); /* Displays current balance */
void deposit_money(double deposit); /* Allows user to deposit money */
void withdraw_money(double withdraw); /* Allows user to withdraw money */
void menu(); /* Displays the ATM Menu */

int main()
{  
    double deposit, withdraw;
    int input;
    menu();
    scanf("%d", &input);
    switch(input)
    {
        case 1:
            check_balance();
            break;
        case 2:
            printf("Enter amount to deposit: ");
            scanf("%lf", &deposit);
            deposit_money(deposit);
            printf("Deposit successful! New balance: %.3lf TRY (Saved account.txt)\n", get_balance());
            break;
        case 3:
            printf("Enter amount to withdraw: ");
            scanf("%lf", &withdraw);
            if(get_balance() >= withdraw)
            {
                withdraw_money(withdraw);
                printf("Withdraw successful! New balance: %.3lf TRY (Saved account.txt)\n", get_balance());
            }
            else
            {
                printf("Insufficient funds! Your balance is only %.3lf TRY.\n", get_balance());
            }   
            break;
        case 4:
            printf("The program is closing...");
            return 1;
        default:
            printf("Enter a valid value to select an operation!\n");
    }
    return 0;
}

/* Function to retrieve account balance from file */
double get_balance()
{
    double amount;
    FILE* fp = fopen("account.txt", "r");
    if (fp == NULL)
    {
        fp = fopen("account.txt", "w");
        fprintf(fp, "100.000");
        fclose(fp);
        return 100.000;
    }
    fscanf(fp, "%lf", &amount);
    fclose(fp);
    return amount;
}

/* Function to update account balance in file */
void update_balance(double new_amount)
{
    FILE* fp = fopen("account.txt", "w");
    fprintf(fp, "%.3lf", new_amount);
    fclose(fp);
}

/* Function to display current balance */
void check_balance()
{
    printf("Current balance: %.3lf TRY\n", get_balance());
}

/* Function to deposit money */
void deposit_money(double deposit)
{
    if(deposit < 0){
        printf("The deposit value cannot be smaller than zero. Enter amount to deposit: ");
        double new_balance;
        scanf("%lf", &new_balance);
        return deposit_money(new_balance);
    }
    double new_amount = get_balance() + deposit;
    update_balance(new_amount);
}

/* Function to withdraw money */
void withdraw_money(double withdraw)
{
    if(withdraw < 0){
        printf("The withdraw value cannot be smaller than zero. Enter amount to withdraw: ");
        double new_amount;
        scanf("%lf", &new_amount);
        return withdraw_money(new_amount);
    }
    double new_amount = get_balance() - withdraw;
    update_balance(new_amount);
}

/* Function to display ATM menu */
void menu()
{
    printf("----- Virtual ATM -----\n");
    printf("1. Check Balance\n2. Deposit Money\n3. Withdraw Money\n4. Exit\n");
}