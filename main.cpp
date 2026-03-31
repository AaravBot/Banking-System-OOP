#include <iostream>
using namespace std;

class Account
{
protected:
    int accountNumber;
    string accHolderName;
    double balance;

public:
    Account(int accNo, string name, double bal)
    {
        accountNumber = accNo;
        accHolderName = name;
        balance = bal;
    }
    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            cout << "Deposited: " << amount << endl;
        }
        else
        {
            cout << "Invalid deposit amount!" << endl;
        }
    }

    double getBalance()
    {
        return balance;
    }

    virtual void withdraw(double amount)
    {
        if (amount <= balance)
        {
            balance -= amount;
            cout << "Withdrawn: " << amount << endl;
        }
        else
        {
            cout << "Insufficient balance!" << endl;
        }
    }
    void display()
    {
        cout << "\nAccount Details:\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << accHolderName << endl;
        cout << "Balance: " << balance << endl;
    }
};
class SavingsAccount : public Account{
    private:
        double minimumBalance;
    public:
    SavingsAccount(int accNo, string name, double bal, double minBal): Account(accNo, name, bal){
        minimumBalance = minBal;
    }
    void withdraw(double amount) override{
        if (getBalance() - amount >= minimumBalance){
            Account::withdraw(amount);
        }else{
            cout << "Cannot withdraw! Minimum balance must be maintained.\n";
        }
    }
};
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(int accNo, string name, double bal, double limit): Account(accNo, name, bal){
        overdraftLimit = limit;
    }

    void withdraw(double amount) override {
    if (amount <= balance + overdraftLimit) {
        balance -= amount;

        cout << "Withdrawn: " << amount << endl;

        if (balance < 0) {
            cout << "Overdraft used: " << -balance << endl;
        }

    } else {
        cout << "Exceeded overdraft limit!\n";
    }
}
};

int main(){
    int choice;
    SavingsAccount acc1(101, "Aarav", 5000, 1000);
    do {
        cout << "\n--- Banking System Menu ---\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Display Account\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        double amount;
        switch(choice) {
            case 1:
                cout << "Enter amount to deposit: ";
                cin >> amount;
                acc1.deposit(amount);
                break;

            case 2:
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                acc1.withdraw(amount);
                break;

            case 3:
                acc1.display();
                break;

            case 4:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while(choice != 4);

    return 0;
}