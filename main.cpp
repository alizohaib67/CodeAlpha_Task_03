#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// Forward declaration
class Account;

// ===================== Transaction Class =====================
class Transaction {
public:
    string type;
    double amount;
    string date;
    string targetAccount;

    Transaction(string t, double amt, string target = "") {
        type = t;
        amount = amt;
        targetAccount = target;
        time_t now = time(0);
        //date = ctime(&now);   // current time
            char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);  // safer alternative to ctime
        date = buffer;  // assign the formatted date string
    }

    void showTransaction() {
        cout << "Type: " << type << ", Amount: " << amount;
        if (targetAccount != "")
            cout << ", To: " << targetAccount;
        cout << ", Date: " << date;
    }
};

// ===================== Account Class =====================
class Account {
private:
    string accountNumber;
    double balance;
    string accountType;
    vector<Transaction> history;

public:
    Account(string accNum, string accType) {
        accountNumber = accNum;
        accountType = accType;
        balance = 0.0;
    }

    void deposit(double amount) {
        balance += amount;
        history.push_back(Transaction("Deposit", amount));
        cout << "Deposited Rs." << amount << " successfully.\n";
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!\n";
        }
        else {
            balance -= amount;
            history.push_back(Transaction("Withdraw", amount));
            cout << "Withdrawn Rs." << amount << " successfully.\n";
        }
    }

    void transfer(double amount, Account& toAccount) {
        if (amount > balance) {
            cout << "Insufficient balance for transfer!\n";
        }
        else {
            balance -= amount;
            toAccount.balance += amount;
            history.push_back(Transaction("Transfer", amount, toAccount.accountNumber));
            toAccount.history.push_back(Transaction("Received", amount, accountNumber));
            cout << "Transferred Rs." << amount << " to Account " << toAccount.accountNumber << "\n";
        }
    }

    void showBalance() {
        cout << "Current Balance: Rs." << balance << "\n";
    }

    void showHistory() {
        cout << "---- Transaction History for Account: " << accountNumber << " ----\n";
        for (Transaction& t : history) {
            t.showTransaction();
        }
    }

    string getAccountNumber() {
        return accountNumber;
    }

    friend class Customer;
};

// ===================== Customer Class =====================
class Customer {
private:
    string customerID;
    string name;
    string phone;
    Account account;

public:
    Customer(string id, string n, string p, string accNum, string accType)
        : account(accNum, accType) {
        customerID = id;
        name = n;
        phone = p;
    }

    void depositMoney(double amount) {
        account.deposit(amount);
    }

    void withdrawMoney(double amount) {
        account.withdraw(amount);
    }

    void transferMoney(double amount, Customer& otherCustomer) {
        account.transfer(amount, otherCustomer.account);
    }

    void viewAccountInfo() {
        cout << "Customer ID: " << customerID << "\n";
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";
        account.showBalance();
    }

    void viewTransactions() {
        account.showHistory();
    }

    string getAccountNumber() {
        return account.getAccountNumber();
    }
};

// ===================== Main =====================
int main() {
    cout << "=== Welcome to Simple Bank System ===\n\n";

    // Creating 2 customers
    Customer c1("C001", "Ali Zohaib", "03001234567", "A1001", "Savings");
    Customer c2("C002", "Ahmed Khan", "03009876543", "A1002", "Current");

    c1.depositMoney(1000);
    c1.withdrawMoney(200);
    c1.transferMoney(300, c2);

    cout << "\n--- Ali's Info ---\n";
    c1.viewAccountInfo();
    c1.viewTransactions();

    cout << "\n--- Ahmed's Info ---\n";
    c2.viewAccountInfo();
    c2.viewTransactions();

    return 0;
}