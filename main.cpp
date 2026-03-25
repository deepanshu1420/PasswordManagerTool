// ============================================================
// FILE: main.cpp
// PURPOSE: Entry point — clean menu-driven interface.
//          All ASCII characters for Windows PowerShell support.
// ============================================================

#include <iostream>
#include <string>
#include <limits>
#include "PasswordManager.h"

// ---------------------------------------------------------------
// getLine() — reads input safely, trims whitespace
// ---------------------------------------------------------------
static std::string getLine(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    size_t start = input.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = input.find_last_not_of(" \t\r\n");
    return input.substr(start, end - start + 1);
}

// ---------------------------------------------------------------
// printBanner() — shown once at startup
// ---------------------------------------------------------------
static void printBanner() {
    std::cout << "\n";
    std::cout << "  ============================================================\n";
    std::cout << "        Welcome to Deepanshu's Password Manager Tool\n";
    std::cout << "                  [ Secure. Simple. Yours. ]\n";
    std::cout << "  ============================================================\n";
    std::cout << "\n";
}

// ---------------------------------------------------------------
// printMenu() — shown every loop iteration
// ---------------------------------------------------------------
static void printMenu() {
    std::cout << "  +---------------------------------------+\n";
    std::cout << "  |             MAIN MENU                 |\n";
    std::cout << "  +---------------------------------------+\n";
    std::cout << "  |  1.  Add a new password               |\n";
    std::cout << "  |  2.  View all passwords               |\n";
    std::cout << "  |  3.  Search password by website       |\n";
    std::cout << "  |  4.  Delete a password                |\n";
    std::cout << "  |  5.  Exit                             |\n";
    std::cout << "  |  6.  Encrypt / Decrypt Tool           |\n";
    std::cout << "  +---------------------------------------+\n";
    std::cout << "  Enter your choice: ";
}

// ---------------------------------------------------------------
// printDivider() — separator between sections
// ---------------------------------------------------------------
static void printDivider() {
    std::cout << "  ----------------------------------------\n";
}

// ==============================================================
// main()
// ==============================================================
int main() {
    const std::string DATA_FILE = "passwords.dat";

    PasswordManager manager(DATA_FILE);

    printBanner();

    int choice = 0;

    do {
        std::cout << "\n";
        printMenu();

        // Handle non-numeric input
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n  That was not a valid option. Please enter a number from 1 to 6.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n";
        printDivider();

        switch (choice) {

            // ---- 1. Add a password ---------------------------
            case 1: {
                std::cout << "  Add a New Password\n\n";
                std::cout << "  Please enter the details below:\n\n";
                std::string website  = getLine("  Website name  : ");
                std::string password = getLine("  Password      : ");
                manager.addPassword(website, password);
                break;
            }

            // ---- 2. View all passwords -----------------------
            case 2: {
                std::cout << "  All Saved Passwords\n";
                manager.viewAllPasswords();
                break;
            }

            // ---- 3. Search -----------------------------------
            case 3: {
                std::cout << "  Search Password by Website\n\n";
                std::string website = getLine("  Enter the website name to search: ");
                manager.searchPassword(website);
                break;
            }

            // ---- 4. Delete -----------------------------------
            case 4: {
                std::cout << "  Delete a Password\n\n";
                std::string website = getLine("  Enter the website name to delete: ");
                manager.deletePassword(website);
                break;
            }

            // ---- 5. Exit -------------------------------------
            case 5: {
                printDivider();
                std::cout << "\n  Goodbye, Deepanshu! Your passwords are safe and sound.\n\n";
                printDivider();
                break;
            }

            // ---- 6. Encrypt/Decrypt Tool ---------------------
            case 6: {
                manager.encryptDecryptTool();
                break;
            }

            default: {
                std::cout << "  That option does not exist. Please choose a number between 1 and 6.\n\n";
                break;
            }
        }

    } while (choice != 5);

    return 0;
}