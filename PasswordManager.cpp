// ============================================================
// FILE: PasswordManager.cpp
// PURPOSE: Full implementation of the PasswordManager class.
//          Includes XOR encryption, password strength checker,
//          masked password display, and encrypt/decrypt tool.
// ============================================================

#include "PasswordManager.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>

// ---------------------------------------------------------------
// XOR encryption key — same key encrypts AND decrypts
// ---------------------------------------------------------------
static const unsigned char XOR_KEY = 0x5A;

// ==============================================================
// encrypt() — XOR every char, store as 2-digit hex
// ==============================================================
std::string PasswordManager::encrypt(const std::string& text) {
    std::string result;
    result.reserve(text.size() * 2);
    for (unsigned char ch : text) {
        unsigned char enc  = ch ^ XOR_KEY;
        char          high = "0123456789ABCDEF"[(enc >> 4) & 0x0F];
        char          low  = "0123456789ABCDEF"[ enc        & 0x0F];
        result += high;
        result += low;
    }
    return result;
}

// ==============================================================
// decrypt() — reverse of encrypt()
// ==============================================================
std::string PasswordManager::decrypt(const std::string& text) {
    if (text.size() % 2 != 0) return "[error]";
    std::string result;
    auto hexVal = [](char c) -> unsigned char {
        if (c >= '0' && c <= '9') return static_cast<unsigned char>(c - '0');
        if (c >= 'A' && c <= 'F') return static_cast<unsigned char>(c - 'A' + 10);
        return 0;
    };
    for (size_t i = 0; i + 1 < text.size(); i += 2) {
        unsigned char enc = static_cast<unsigned char>((hexVal(text[i]) << 4) | hexVal(text[i+1]));
        result += static_cast<char>(enc ^ XOR_KEY);
    }
    return result;
}

// ==============================================================
// checkStrength() — scores the password on 5 criteria
// ==============================================================
PasswordStrength PasswordManager::checkStrength(const std::string& password) {
    int score = 0;

    if (password.length() >= 8)  score++;
    if (password.length() >= 12) score++;

    bool hasUpper   = false;
    bool hasLower   = false;
    bool hasDigit   = false;
    bool hasSpecial = false;

    for (char c : password) {
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper   = true;
        if (std::islower(static_cast<unsigned char>(c))) hasLower   = true;
        if (std::isdigit(static_cast<unsigned char>(c))) hasDigit   = true;
        if (std::ispunct(static_cast<unsigned char>(c))) hasSpecial = true;
    }

    if (hasUpper)   score++;
    if (hasLower)   score++;
    if (hasDigit)   score++;
    if (hasSpecial) score++;

    if (score <= 1) return PasswordStrength::VERY_WEAK;
    if (score == 2) return PasswordStrength::WEAK;
    if (score == 3) return PasswordStrength::MODERATE;
    if (score == 4) return PasswordStrength::STRONG;
    return PasswordStrength::VERY_STRONG;
}

// ==============================================================
// printStrengthReport() — human friendly strength feedback
// ==============================================================
void PasswordManager::printStrengthReport(const std::string& password) {
    PasswordStrength strength = checkStrength(password);

    std::cout << "\n  ............................................\n";
    std::cout << "  Password Strength Result\n";
    std::cout << "  ............................................\n";

    std::cout << "  Strength  :  ";
    switch (strength) {
        case PasswordStrength::VERY_WEAK:   std::cout << "Very Weak   (please change this!)\n";  break;
        case PasswordStrength::WEAK:        std::cout << "Weak        (needs improvement)\n";     break;
        case PasswordStrength::MODERATE:    std::cout << "Moderate    (almost there)\n";          break;
        case PasswordStrength::STRONG:      std::cout << "Strong      (good job!)\n";             break;
        case PasswordStrength::VERY_STRONG: std::cout << "Very Strong (excellent choice!)\n";     break;
    }

    // Visual strength bar
    int level = 0;
    switch (strength) {
        case PasswordStrength::VERY_WEAK:   level = 1; break;
        case PasswordStrength::WEAK:        level = 2; break;
        case PasswordStrength::MODERATE:    level = 3; break;
        case PasswordStrength::STRONG:      level = 4; break;
        case PasswordStrength::VERY_STRONG: level = 5; break;
    }
    std::cout << "  Meter     :  [";
    for (int i = 0; i < 5; i++) std::cout << (i < level ? "#" : "-");
    std::cout << "]  " << level << " out of 5\n";

    // Suggestions if missing any criteria
    bool hasUpper   = false, hasLower   = false;
    bool hasDigit   = false, hasSpecial = false;
    for (char c : password) {
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper   = true;
        if (std::islower(static_cast<unsigned char>(c))) hasLower   = true;
        if (std::isdigit(static_cast<unsigned char>(c))) hasDigit   = true;
        if (std::ispunct(static_cast<unsigned char>(c))) hasSpecial = true;
    }

    bool anyTip = (!hasUpper || !hasLower || !hasDigit || !hasSpecial || password.length() < 8);
    if (anyTip) {
        std::cout << "\n  Here are some tips to make it stronger:\n";
        if (password.length() < 8) std::cout << "    - Use at least 8 characters\n";
        if (!hasUpper)             std::cout << "    - Add some UPPERCASE letters  (e.g. A, B, C)\n";
        if (!hasLower)             std::cout << "    - Add some lowercase letters  (e.g. a, b, c)\n";
        if (!hasDigit)             std::cout << "    - Include numbers             (e.g. 1, 2, 3)\n";
        if (!hasSpecial)           std::cout << "    - Use special symbols         (e.g. @, #, $, !)\n";
    }
    std::cout << "  ............................................\n\n";
}

// ==============================================================
// Constructor — load existing entries from file on startup
// ==============================================================
PasswordManager::PasswordManager(const std::string& filename)
    : m_filename(filename)
{
    loadFromFile();
}

// ==============================================================
// loadFromFile() — read encrypted pairs from disk
// ==============================================================
void PasswordManager::loadFromFile() {
    std::ifstream file(m_filename);
    if (!file.is_open()) return;

    m_entries.clear();
    std::string encWebsite, encPassword;
    while (std::getline(file, encWebsite) && std::getline(file, encPassword)) {
        if (encWebsite.empty() || encPassword.empty()) continue;
        PasswordEntry entry;
        entry.website  = decrypt(encWebsite);
        entry.password = decrypt(encPassword);
        m_entries.push_back(entry);
    }
    file.close();
}

// ==============================================================
// saveToFile() — write encrypted pairs to disk
// ==============================================================
void PasswordManager::saveToFile() const {
    std::ofstream file(m_filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "  Sorry, could not write to file: " << m_filename << "\n";
        return;
    }
    for (const auto& entry : m_entries) {
        file << encrypt(entry.website)  << "\n";
        file << encrypt(entry.password) << "\n";
    }
    file.close();
}

// ==============================================================
// addPassword() — validate, strength check, duplicate check, save
// ==============================================================
void PasswordManager::addPassword(const std::string& website, const std::string& password) {
    // Empty check
    if (website.empty() || password.empty()) {
        std::cout << "  Please make sure both the website name and password are filled in.\n\n";
        return;
    }

    // Duplicate check (case-insensitive)
    for (const auto& entry : m_entries) {
        std::string a = entry.website, b = website;
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);
        if (a == b) {
            std::cout << "  An entry for \"" << entry.website << "\" already exists.\n";
            std::cout << "  If you want to update it, please delete it first and then add it again.\n\n";
            return;
        }
    }

    // Password strength report
    printStrengthReport(password);

    PasswordStrength s = checkStrength(password);
    if (s == PasswordStrength::VERY_WEAK || s == PasswordStrength::WEAK) {
        std::cout << "  Your password looks a bit weak. Do you still want to save it? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore(10000, '\n');
        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "  No problem! Password was not saved. Try creating a stronger one.\n\n";
            return;
        }
    }

    PasswordEntry newEntry;
    newEntry.website  = website;
    newEntry.password = password;
    m_entries.push_back(newEntry);
    saveToFile();

    std::cout << "  Password for \"" << website << "\" has been saved successfully.\n\n";
}

// ==============================================================
// viewAllPasswords() — masked by default, reveal on request
// ==============================================================
void PasswordManager::viewAllPasswords() const {
    if (m_entries.empty()) {
        std::cout << "  You have not saved any passwords yet.\n\n";
        return;
    }

    std::cout << "\n";
    std::cout << "  +------------------------------+------------------------------+\n";
    std::cout << "  |          Website             |          Password            |\n";
    std::cout << "  +------------------------------+------------------------------+\n";

    for (const auto& entry : m_entries) {
        std::cout << "  | " << entry.website;
        int wPad = 29 - (int)entry.website.size();
        if (wPad < 0) wPad = 0;
        std::cout << std::string(wPad, ' ') << "| ";
        std::cout << "**********";
        std::cout << std::string(19, ' ') << "|\n";
    }

    std::cout << "  +------------------------------+------------------------------+\n";
    std::cout << "  Total: " << m_entries.size() << " saved password(s)";
    std::cout << "   (passwords are hidden for your security)\n\n";

    std::cout << "  To see the actual password, type the website name below.\n";
    std::cout << "  Or just press Enter to go back to the menu.\n\n";
    std::cout << "  Website name: ";
    std::string reveal;
    std::getline(std::cin, reveal);

    if (reveal.empty()) {
        std::cout << "\n";
        return;
    }

    std::string revealLower = reveal;
    std::transform(revealLower.begin(), revealLower.end(), revealLower.begin(), ::tolower);

    bool found = false;
    for (const auto& entry : m_entries) {
        std::string entryLower = entry.website;
        std::transform(entryLower.begin(), entryLower.end(), entryLower.begin(), ::tolower);
        if (entryLower == revealLower) {
            std::cout << "\n  +------------------------------+------------------------------+\n";
            std::cout << "  |          Website             |          Password            |\n";
            std::cout << "  +------------------------------+------------------------------+\n";
            std::cout << "  | " << entry.website;
            int wPad = 29 - (int)entry.website.size();
            if (wPad < 0) wPad = 0;
            std::cout << std::string(wPad, ' ') << "| ";
            std::cout << entry.password;
            int pPad = 29 - (int)entry.password.size();
            if (pPad < 0) pPad = 0;
            std::cout << std::string(pPad, ' ') << "|\n";
            std::cout << "  +------------------------------+------------------------------+\n\n";
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "  Could not find any entry for \"" << reveal << "\".\n\n";
    }
}

// ==============================================================
// searchPassword() — case-insensitive substring search
// ==============================================================
void PasswordManager::searchPassword(const std::string& website) const {
    if (website.empty()) {
        std::cout << "  Please enter a website name to search for.\n\n";
        return;
    }

    std::string searchLower = website;
    std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);

    std::vector<const PasswordEntry*> matches;
    for (const auto& entry : m_entries) {
        std::string lower = entry.website;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if (lower.find(searchLower) != std::string::npos)
            matches.push_back(&entry);
    }

    if (matches.empty()) {
        std::cout << "  No saved password found for \"" << website << "\".\n\n";
        return;
    }

    std::cout << "\n  Found " << matches.size() << " matching result(s):\n\n";
    std::cout << "  +------------------------------+------------------------------+\n";
    std::cout << "  |          Website             |          Password            |\n";
    std::cout << "  +------------------------------+------------------------------+\n";

    for (const auto* entry : matches) {
        std::cout << "  | " << entry->website;
        int wPad = 29 - (int)entry->website.size();
        if (wPad < 0) wPad = 0;
        std::cout << std::string(wPad, ' ') << "| ";
        std::cout << entry->password;
        int pPad = 29 - (int)entry->password.size();
        if (pPad < 0) pPad = 0;
        std::cout << std::string(pPad, ' ') << "|\n";
    }
    std::cout << "  +------------------------------+------------------------------+\n\n";
}

// ==============================================================
// deletePassword() — exact match, case-insensitive
// ==============================================================
void PasswordManager::deletePassword(const std::string& website) {
    if (website.empty()) {
        std::cout << "  Please enter the website name you want to delete.\n\n";
        return;
    }

    std::string searchLower = website;
    std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);

    size_t before = m_entries.size();
    m_entries.erase(
        std::remove_if(m_entries.begin(), m_entries.end(),
            [&searchLower](const PasswordEntry& e) {
                std::string lower = e.website;
                std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                return lower == searchLower;
            }),
        m_entries.end()
    );

    if (m_entries.size() == before) {
        std::cout << "  No entry found for \"" << website << "\". Nothing was deleted.\n\n";
    } else {
        saveToFile();
        std::cout << "  The password for \"" << website << "\" has been deleted.\n\n";
    }
}

// ==============================================================
// encryptDecryptTool() — Option 6
// ==============================================================
void PasswordManager::encryptDecryptTool() const {
    std::cout << "\n  ............................................\n";
    std::cout << "  Encrypt / Decrypt Tool\n";
    std::cout << "  ............................................\n\n";
    std::cout << "  Press E - To encrypt some text\n";
    std::cout << "  Press D - To decrypt an encrypted message\n\n";
    std::cout << "  Your choice: ";

    char choice;
    std::cin >> choice;
    std::cin.ignore(10000, '\n');

    if (choice == 'E' || choice == 'e') {
        std::cout << "\n  Great! Enter the text you want to encrypt:\n";
        std::cout << "  > ";
        std::string text;
        std::getline(std::cin, text);
        if (text.empty()) {
            std::cout << "\n  You did not enter anything. Please try again.\n\n";
            return;
        }
        std::string enc = encrypt(text);
        std::cout << "\n  Here is the encrypted form of your text:\n";
        std::cout << "  " << enc << "\n\n";

    } else if (choice == 'D' || choice == 'd') {
        std::cout << "\n  Sure! Paste the encrypted text you want to decrypt:\n";
        std::cout << "  > ";
        std::string hex;
        std::getline(std::cin, hex);
        if (hex.empty()) {
            std::cout << "\n  You did not enter anything. Please try again.\n\n";
            return;
        }
        std::string dec = decrypt(hex);
        std::cout << "\n  Here is the decrypted text of the given input:\n";
        std::cout << "  " << dec << "\n\n";

    } else {
        std::cout << "\n  That was not a valid choice. Please enter E to encrypt or D to decrypt.\n\n";
    }
}