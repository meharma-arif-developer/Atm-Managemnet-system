#include <iostream>
using namespace std;

class User {
private:
    char name[20];
    int pin;
    float balance;

public:
    User() {
        name[0] = '\0';
        pin = 0;
        balance = 0;
    }

    User(const char n[], int p, float b) {
        copyText(name, n);
        pin = p;
        balance = b;
    }

    bool login(int enteredPin) {
        return enteredPin == pin;
    }

    void showMenu() {
        int choice;
        float amount;

        do {
            cout << "\n--- Welcome, " << name << " ---\n";
            cout << "1. Check Balance\n";
            cout << "2. Withdraw\n";
            cout << "3. Change PIN\n";
            cout << "4. View E-Statement\n";
            cout << "5. Logout\n";
            cout << "Select: ";
            cin >> choice;

            switch (choice) {
                case 1: showBalance(); break;
                case 2:
                    cout << "Enter amount: ";
                    cin >> amount;
                    withdraw(amount);
                    break;
                case 3: changePIN(); break;
                case 4: showEStatement(); break;
                case 5: cout << "Logging out...\n"; break;
                default: cout << "Invalid option.\n";
            }
        } while (choice != 5);
    }

    void viewForAdmin() {
        cout << "Name: " << name << " | Balance: $" << balance << " | PIN: " << pin << endl;
    }

private:
    void showBalance() {
        cout << "Current Balance: $" << balance << endl;
    }

    void withdraw(float amount) {
        if (amount <= 0) {
            cout << "Invalid amount.\n";
            return;
        }
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return;
        }

        balance -= amount;
        cout << "Withdrawn: $" << amount << endl;
        saveTransaction(amount);
    }

    void changePIN() {
        int oldPin, newPin;
        cout << "Enter current PIN: ";
        cin >> oldPin;
        if (oldPin == pin) {
            cout << "Enter new PIN: ";
            cin >> newPin;
            pin = newPin;
            cout << "PIN changed successfully.\n";
        } else {
            cout << "Incorrect PIN.\n";
        }
    }

    void saveTransaction(float amount) {
        FILE* file = fopen("transactions.txt", "a");
        if (!file) {
            cout << "Error saving transaction.\n";
            return;
        }

        char buffer[100];
#if defined(_WIN32)
        system("echo %date% %time% > temp.txt");
#else
        system("date '+%Y-%m-%d %H:%M:%S' > temp.txt");
#endif
        FILE* temp = fopen("temp.txt", "r");
        fgets(buffer, sizeof(buffer), temp);
        fclose(temp);
        remove("temp.txt");

        fprintf(file, "User: %s | Withdrawn: $%.2f | Time: %s", name, amount, buffer);
        fclose(file);
    }

    void showEStatement() {
        FILE* file = fopen("transactions.txt", "r");
        if (!file) {
            cout << "No transaction history found.\n";
            return;
        }

        cout << "\n===== E-Statement for " << name << " =====\n";
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            if (lineContainsUser(line)) {
                cout << line;
            }
        }
        fclose(file);
        cout << "========================================\n";
    }

    bool lineContainsUser(const char line[]) {
        int i = 0;
        while (line[i] != '\0') {
            int j = 0;
            while (name[j] != '\0' && line[i + j] == name[j]) {
                j++;
            }
            if (name[j] == '\0') return true;
            i++;
        }
        return false;
    }

    void copyText(char dest[], const char src[]) {
        int i = 0;
        while (src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }
};

// Admin class
class Admin {
private:
    char password[10];

public:
    Admin() {
        copyText(password, "admin");
    }

    bool login() {
        char input[10];
        cout << "Enter admin password: ";
        cin >> input;
        return compareText(password, input);
    }

    void viewAllUsers(User users[], int count) {
        cout << "\n===== ADMIN PANEL =====\n";
        for (int i = 0; i < count; i++) {
            users[i].viewForAdmin();
        }
        cout << "=======================\n";
    }

private:
    void copyText(char dest[], const char src[]) {
        int i = 0;
        while (src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }

    bool compareText(const char a[], const char b[]) {
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] != b[i]) return false;
            i++;
        }
        return a[i] == b[i];
    }
};

// ===== Main Program =====
int main() {
    User users[2] = {
        User("salar", 1111, 1500),
        User("cihan", 2222, 1000)
    };

    Admin admin;
    int choice;

    while (true) {
        cout << "\n===== ATM SYSTEM =====\n";
        cout << "1. Login as User\n";
        cout << "2. Admin Mode\n";
        cout << "3. Exit\n";
        cout << "Select: ";
        cin >> choice;

        if (choice == 1) {
            int pinInput;
            cout << "Enter your PIN: ";
            cin >> pinInput;

            bool found = false;
            for (int i = 0; i < 2; i++) {
                if (users[i].login(pinInput)) {
                    users[i].showMenu();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Invalid PIN.\n";
            }

        } else if (choice == 2) {
            if (admin.login()) {
                admin.viewAllUsers(users, 2);
            } else {
                cout << "Wrong password!\n";
            }

        } else if (choice == 3) {
            cout << "Thank you for using the ATM!\n";
            break;
        } else {
            cout << "Invalid selection.\n";
        }
    }

    return 0;
}


