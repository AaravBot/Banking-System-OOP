#include <iostream>
using namespace std;

class Account
{
private:
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
        if (amount <= getBalance() + overdraftLimit) {
            double newBalance = getBalance() - amount;
            Account::withdraw(getBalance());
            cout << "Overdraft used: " << amount - getBalance() << endl;

            cout << "Withdrawn: " << amount << endl;
            cout << "Balance is now negative (overdraft mode)\n";
        } else {
            cout << "Exceeded overdraft limit!\n";
        }
    }
};

int main(){
    CurrentAccount acc2(102, "Aarav", 5000, 2000);
    acc2.withdraw(6000);
    acc2.withdraw(3000);
    acc2.display();
    return 0;
}