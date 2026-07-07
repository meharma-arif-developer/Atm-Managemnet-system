#include <iostream>
using namespace std;

#define MAX_USERS 5
#define MAX_TXN 20

// GLOBAL TIME (declare ONLY ONCE)
int globalTime = 1;

/* ================= USER STRUCT ================= */
struct User {
    string name;
    int pin;
    int balance;

    string txnType[MAX_TXN];
    int txnAmount[MAX_TXN];
    int txnTime[MAX_TXN];   // logical date & time
    int top;
};

/* ================= LINEAR SEARCH ================= */
int searchUser(User users[], string name) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].name == name)
            return i;
    }
    return -1;
}

/* ================= USER LOGIN ================= */
int userLogin(User users[], int pin) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].pin == pin)
            return i;
    }
    return -1;
}

/* ================= STACK PUSH ================= */
void pushTransaction(User &u, string type, int amount) {
    if (u.top < MAX_TXN - 1) {
        u.top++;
        u.txnType[u.top] = type;
        u.txnAmount[u.top] = amount;
        u.txnTime[u.top] = globalTime++;   // DATE & TIME
    }
}

/* ================= USER MENU ================= */
void userMenu(User users[], int index) {
    User &u = users[index];
    int choice, amount;
    string receiver;

    while (true) {
        cout << "\n------------- Welcome ( " << u.name << " )----------\n";
        cout << "1. Check Balance\n";
        cout << "2. Withdraw\n";
        cout << "3. Transfer Money\n";
        cout << "4. Change PIN\n";
        cout << "5. Transaction History\n";
        cout << "6. Logout\n";
        cout<<"----------------------------------------\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Balance: " << u.balance << endl;
        }

        else if (choice == 2) {
            cout << "Enter amount: ";
            cin >> amount;

            if (amount > 0 && amount <= u.balance) {
                u.balance -= amount;
                pushTransaction(u, "Withdraw", amount);
                cout << "Withdraw successful\n";
            } else {
                cout << "Invalid amount\n";
            }
        }

        else if (choice == 3) {
            cout << "Enter receiver name: ";
            cin >> receiver;

            int rIndex = searchUser(users, receiver);

            if (rIndex == -1) {
                cout << "User not found\n";
            } else {
                cout << "Enter amount: ";
                cin >> amount;

                if (amount > 0 && amount <= u.balance) {
                    u.balance -= amount;
                    users[rIndex].balance += amount;

                pushTransaction(u, "Transfer Sent to " + receiver, amount);
pushTransaction(users[rIndex], "Transfer Received from " + u.name, amount);

                    cout << "Transfer successful\n";
                } else {
                    cout << "Invalid amount\n";
                }
            }
        }

        else if (choice == 4) {
            int oldPin, newPin;
            cout << "Enter old PIN: ";
            cin >> oldPin;

            if (oldPin == u.pin) {
                cout << "Enter new PIN: ";
                cin >> newPin;
                u.pin = newPin;
                pushTransaction(u, "PIN Change", 0);
                cout << "PIN updated\n";
            } else {
                cout << "Wrong PIN\n";
            }
        }

        else if (choice == 5) {
            if (u.top == -1) {
                cout << "No transactions\n";
            } else {
                cout << "\n--- Transaction History (STACK) ---\n";
                for (int i = u.top; i >= 0; i--) {
                    cout << u.txnType[i]
                         << " | Amount: " << u.txnAmount[i]
                         << " | Time Slot: " << u.txnTime[i]
                         << endl;
                }
            }
        }

        else if (choice == 6) {
            break;
        }

        else {
            cout << "Invalid option\n";
        }
    }
}

/* ================= ADMIN MODE ================= */
bool adminLogin() {
    int pass;
    cout << "Enter Admin Password: ";
    cin >> pass;
    return pass == 9999;
}

void adminPanel(User users[]) {
    cout << "\n------------- ADMIN PANEL ---------------\n";
    for (int i = 0; i < MAX_USERS; i++) {
        cout << users[i].name
             << " | PIN: " << users[i].pin
             << " | Balance: " << users[i].balance << endl;
    }
    cout<<"-----------------------------------------\n";
}

/* ================= MAIN ================= */
int main() {
	cout<<"                         =========================================================================\n";
   cout << "                         ============================ ATM SYSTEM =================================\n";
   cout<<"                         =========================================================================\n";
    User users[MAX_USERS] = {
        {"Arwa", 1111, 3000, {}, {}, {}, -1},
        {"Salar", 2222, 2000, {}, {}, {}, -1},
        {"Cihan", 3333, 2500, {}, {}, {}, -1},
        {"Meharma", 4444, 1800, {}, {}, {}, -1},
        {"Arhum", 5555, 5000, {}, {}, {}, -1}
    };

    int choice, pin;

    while (true) {
        cout<<"                         =========================================================================\n";
        cout << "                         ===                          1. User Login                            ===\n";
        cout << "                         ===                          2. Admin Mode                            ===\n";
        cout << "                         ===                          3. Exit                                  ===\n";
        cout<<"                         =========================================================================\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter PIN: ";
            cin >> pin;

            int index = userLogin(users, pin);
            if (index != -1)
                userMenu(users, index);
            else
                cout << "Invalid PIN\n";
        }

        else if (choice == 2) {
            if (adminLogin())
                adminPanel(users);
            else
                cout << "Wrong password\n";
        }

        else if (choice == 3) {
            break;
        }
    }
    return 0;
}


