#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>

using namespace std;

struct atmAccount {
    int accountNumber;
    string accountName;
    string contactNum;
    string birthday;
    string pinCode;
    double balance;
};

class ATM {
private:
    static const int MAX_ACCOUNTS = 10;
    atmAccount accounts[MAX_ACCOUNTS];
    int accountCount = 0;

public:
    void menu(int accountIndex);
    void putAccounts();
    void saveAccounts();
    void createAcc();
    void login();
    void withdraw(int accountIndex);
    void deposit(int accountIndex);
    void balance(int accountIndex);
    void fundTransfer(int senderIndex);
    void changePin(int accountIndex);
};

void ATM::menu(int accountIndex) {
    int choice;
    while (true) {
        system("cls");
        cout << "--------Transaction Mode--------\n";
        cout << "1. Withdraw\n";
        cout << "2. Check Balance\n";
        cout << "3. Deposit\n";
        cout << "4. Fund Transfer\n";
        cout << "5. Change PIN\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system("cls");
                withdraw(accountIndex);
                system("pause");
                break;
            case 2:
                system("cls");
                balance(accountIndex);
                system("pause");
                break;
            case 3:
                system("cls");
                deposit(accountIndex);
                system("pause");
                break;
            case 4:
                system("cls");
                balance(accountIndex);
                fundTransfer(accountIndex);
                system("pause");
                break;
            case 5:
                system("cls");
                changePin(accountIndex);
                system("pause");
                break;
            case 6:
                return;
            default:
                cout << "Invalid choice. 1 to 6 Only!!" << endl;
        }
    }
}

void ATM::putAccounts() {
    ifstream inputFile("D:\\MLJAS.txt");
    if (inputFile.is_open()) {
        atmAccount account;
        while (inputFile >> account.accountNumber >> account.accountName >> account.contactNum >>
               account.birthday >> account.pinCode >> account.balance) {
            if (accountCount < MAX_ACCOUNTS) {
                accounts[accountCount++] = account;
            } else {
                cerr << "Max account limit reached." << endl;
            }
        }
        inputFile.close();
    }
}

void ATM::saveAccounts() {
    ofstream outputFile("D:\\MLJAS.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < accountCount; i++) {
            outputFile << accounts[i].accountNumber << " " << accounts[i].accountName << " " << accounts[i].contactNum << " " <<
            accounts[i].birthday << " " << accounts[i].pinCode << " " << accounts[i].balance << endl;
        }
        outputFile.close();
    } else {
        cerr << "Failed to open the file for saving accounts." << endl;
    }
}

void ATM::createAcc() {
    if (accountCount < MAX_ACCOUNTS) {
        atmAccount newAccount;
        cout << "--------CREATING ACCOUNT--------\n";
        cout << "Enter account number (5 digits or less): ";
        cin >> newAccount.accountNumber;
        cout << "Enter account name: ";
        cin.ignore();
        getline(cin, newAccount.accountName);
        cout << "Enter contact number: ";
        cin >> newAccount.contactNum;
        cout << "Enter birthday (YYYY-MM-DD): ";
        cin >> newAccount.birthday;
        cout << "Enter initial balance: ";
        cin >> newAccount.balance;

        if (newAccount.balance < 5000) {
            cerr << "Initial deposit must be at least 5000. Account creation failed." << endl;
            return;
        }

        cout << "Enter PIN: ";
        cin >> newAccount.pinCode;

        accounts[accountCount++] = newAccount;
        saveAccounts();
        cout << "\nAccount successfully created.\n";
    } else {
        cerr << "Max account limit reached. Cannot create more accounts." << endl;
    }
}

void ATM::login() {
    int accountNumber;
    string pinCode;
    cout << "--------LOGIN--------\n";
    cout << "Enter account number: ";
    cin >> accountNumber;
    cout << "Enter PIN: ";

    char c;
    pinCode = "";
    while (true) {
        c = _getch();
        if (c == 13)
            break;
        else if (c == 8) {
            if (!pinCode.empty()) {
                cout << "\b \b";
                pinCode.pop_back();
            }
        } else if (c >= '0' && c <= '9') {
            cout << '*';
            pinCode += c;
        }
    }

    int accountIndex = -1;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber && accounts[i].pinCode == pinCode) {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex != -1) {
        menu(accountIndex);
    } else {
        cout << "Login failed. Incorrect account number or PIN." << endl;
    }
}

void ATM::withdraw(int accountIndex) {
    int withdrawAmount;
    cout << "Enter the amount to withdraw: ";
    cin >> withdrawAmount;

    if (withdrawAmount > 0 && withdrawAmount <= accounts[accountIndex].balance) {
        accounts[accountIndex].balance -= withdrawAmount;
        cout << "Withdrawal successful. New balance: " << accounts[accountIndex].balance << endl;
        saveAccounts();
    } else {
        cout << "Invalid withdrawal amount or insufficient balance." << endl;
    }
}

void ATM::deposit(int accountIndex) {
    double depositAmount;
    cout << "Enter the amount to deposit: ";
    cin >> depositAmount;

    if (depositAmount > 0) {
        accounts[accountIndex].balance += depositAmount;
        cout << "Deposit successful. New balance: " << accounts[accountIndex].balance << endl;
        saveAccounts();
    } else {
        cout << "Invalid deposit amount." << endl;
    }
}

void ATM::balance(int accountIndex) {
    cout << "Account Balance: " << accounts[accountIndex].balance << endl;
}

void ATM::fundTransfer(int senderIndex) {
    int receiverAccountNumber;
    double transferAmount;

    cout << "Enter the recipient's account number: ";
        saveAccounts();
    cin >> receiverAccountNumber;

    int receiverIndex = -1;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == receiverAccountNumber) {
            receiverIndex = i;
            break;
        }
    }

    if (receiverIndex == -1) {
        cout << "Receiver account not found." << endl;
        return;
    }

    cout << "Enter the amount to transfer: ";
    cin >> transferAmount;

    if (transferAmount <= 0 || transferAmount > accounts[senderIndex].balance) {
        cout << "Invalid transfer amount or insufficient balance." << endl;
    } else {
        accounts[senderIndex].balance -= transferAmount;
        accounts[receiverIndex].balance += transferAmount;
        saveAccounts();
        cout << "Fund transfer successful. New balance: " << accounts[senderIndex].balance << endl;
    }
}

void ATM::changePin(int accountIndex) {
    string newPin;
    string currentPin;

    cout << "Enter your current PIN: ";
    char c;
    currentPin = "";
    while (true) {
        c = _getch();
        if (c == '\r')
            break;
        else if (c == 8) {
            if (!currentPin.empty()) {
                cout << "\b \b";
                currentPin.pop_back();
            }
        } else if (c >= '0' && c <= '9') {
            cout << '*';
            currentPin += c;
        }
    }

    if (currentPin != accounts[accountIndex].pinCode) {
        cout << "\nIncorrect current PIN." << endl;
        return;
    }

    cout << "\nEnter your new PIN: ";
    newPin = "";
    while (true) {
        c = _getch();
        if (c == '\r')
            break;
        else if (c == 8) {
            if (!newPin.empty()) {
                cout << "\b \b";
                newPin.pop_back();
            }
        } else if (c >= '0' && c <= '9') {
            cout << '*';
            newPin += c;
        }
    }

    accounts[accountIndex].pinCode = newPin;
    cout << "\nPIN code changed successfully." << endl;
    saveAccounts();
}

int main(){
    ATM atm;
    atm.putAccounts();

    while (true){
        system("cls");
        int choice;
        cout << "--------ATM--------\n";
        cout << "1. Create Account\n";
        cout << "2. Transaction Mode\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system("cls");
                atm.createAcc();
                system("pause");
                break;
            case 2:
                system("cls");
                atm.login();
                system("pause");
                break;
            case 3:
                return 0;
            default:
                cout << "Invalid choice. 1 to 3 Only!!" << endl;
        }
    }
}
