#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Account {
protected:
    int accountNumber;
    string accHolderName;
    double balance;

public:
    Account(int accNo, string name, double bal) {
        accountNumber = accNo;
        accHolderName = name;
        balance = bal;
    }

    virtual ~Account() {}

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: " << amount << endl;
            cout << "Current Balance: " << balance << endl;
        } else {
            cout << "Invalid deposit amount!" << endl;
        }
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "Withdrawn: " << amount << endl;
            cout << "Current Balance: " << balance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    int getAccountNumber() {
        return accountNumber;
    }

    virtual string getType() = 0;

    virtual void display() {
        cout << "\nAccount Details:\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << accHolderName << endl;
        cout << "Balance: " << balance << endl;
    }

    virtual void save(ofstream &out) = 0;
};

class SavingsAccount : public Account {
private:
    double minimumBalance;

public:
    SavingsAccount(int accNo, string name, double bal, double minBal)
        : Account(accNo, name, bal) {
        minimumBalance = minBal;
    }

    void withdraw(double amount) override {
        if (balance - amount >= minimumBalance) {
            balance -= amount;
            cout << "Withdrawn: " << amount << endl;
            cout << "Current Balance: " << balance << endl;
        } else {
            cout << "Cannot withdraw! Minimum balance must be maintained.\n";
        }
    }

    string getType() override {
        return "S";
    }

    void save(ofstream &out) override {
        out << "S " << accountNumber << " " << accHolderName << " " << balance << " " << minimumBalance << endl;
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(int accNo, string name, double bal, double limit)
        : Account(accNo, name, bal) {
        overdraftLimit = limit;
    }

    void withdraw(double amount) override {
        if (amount <= balance + overdraftLimit) {
            balance -= amount;
            cout << "Withdrawn: " << amount << endl;
            if (balance < 0) {
                cout << "Overdraft used: " << -balance << endl;
            }
            cout << "Current Balance: " << balance << endl;
        } else {
            cout << "Exceeded overdraft limit!\n";
        }
    }

    string getType() override {
        return "C";
    }

    void save(ofstream &out) override {
        out << "C " << accountNumber << " " << accHolderName << " " << balance << " " << overdraftLimit << endl;
    }
};

Account* findAccount(vector<Account*> &accounts, int accNo) {
    for (auto acc : accounts) {
        if (acc->getAccountNumber() == accNo) return acc;
    }
    return nullptr;
}

bool accountExists(vector<Account*> &accounts, int accNo) {
    return findAccount(accounts, accNo) != nullptr;
}

void deleteAccount(vector<Account*> &accounts, int accNo) {
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i]->getAccountNumber() == accNo) {
            delete accounts[i];
            accounts.erase(accounts.begin() + i);
            cout << "Account deleted successfully\n";
            return;
        }
    }
    cout << "Account not found\n";
}

void saveToFile(vector<Account*> &accounts) {
    ofstream out("accounts.txt");
    for (auto acc : accounts) {
        acc->save(out);
    }
    out.close();
}

void loadFromFile(vector<Account*> &accounts) {
    ifstream in("accounts.txt");
    if (!in) return;

    char type;
    while (in >> type) {
        int accNo;
        string name;
        double bal;

        if (type == 'S') {
            double minBal;
            in >> accNo >> name >> bal >> minBal;
            accounts.push_back(new SavingsAccount(accNo, name, bal, minBal));
        } else if (type == 'C') {
            double limit;
            in >> accNo >> name >> bal >> limit;
            accounts.push_back(new CurrentAccount(accNo, name, bal, limit));
        }
    }
    in.close();
}

int main() {
    vector<Account*> accounts;
    loadFromFile(accounts);

    int choice;

    do {
        cout << "\n--- Banking System Menu ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Display Account\n";
        cout << "5. Delete Account\n";
        cout << "6. Save to File\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        int accNo;
        double amount;

        switch (choice) {

        case 1: {
            int type;
            string name;
            double bal;

            cout << "1. Savings  2. Current\n";
            cin >> type;

            cout << "Enter Account Number: ";
            cin >> accNo;

            if (accountExists(accounts, accNo)) {
                cout << "Account number already exists!\n";
                break;
            }

            cout << "Enter Name: ";
            cin >> name;

            cout << "Enter Initial Balance: ";
            cin >> bal;

            if (type == 1) {
                double minBal;
                cout << "Enter Minimum Balance: ";
                cin >> minBal;
                accounts.push_back(new SavingsAccount(accNo, name, bal, minBal));
            } else {
                double limit;
                cout << "Enter Overdraft Limit: ";
                cin >> limit;
                accounts.push_back(new CurrentAccount(accNo, name, bal, limit));
            }
            break;
        }

        case 2:
            cout << "Enter Account Number: ";
            cin >> accNo;
            if (auto acc = findAccount(accounts, accNo)) {
                cout << "Enter amount: ";
                cin >> amount;
                acc->deposit(amount);
            } else cout << "Account not found\n";
            break;

        case 3:
            cout << "Enter Account Number: ";
            cin >> accNo;
            if (auto acc = findAccount(accounts, accNo)) {
                cout << "Enter amount: ";
                cin >> amount;
                acc->withdraw(amount);
            } else cout << "Account not found\n";
            break;

        case 4:
            cout << "Enter Account Number: ";
            cin >> accNo;
            if (auto acc = findAccount(accounts, accNo)) {
                acc->display();
            } else cout << "Account not found\n";
            break;

        case 5:
            cout << "Enter Account Number: ";
            cin >> accNo;
            deleteAccount(accounts, accNo);
            break;

        case 6:
            saveToFile(accounts);
            cout << "Data saved\n";
            break;

        case 7:
            saveToFile(accounts);
            cout << "Auto-saving before exit...\n";
            break;

        default:
            cout << "Invalid choice\n";
        }

    } while (choice != 7);

    for (auto acc : accounts) delete acc;

    return 0;
}