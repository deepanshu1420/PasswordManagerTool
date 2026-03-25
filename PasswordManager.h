// ============================================================
// FILE: PasswordManager.h
// PURPOSE: Header file — declares the PasswordEntry struct and
//          PasswordManager class with all new features.
// ============================================================

#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <string>
#include <vector>

// ---------------------------------------------------------------
// STRUCT: PasswordEntry
// Represents one saved record: a website and its password.
// ---------------------------------------------------------------
struct PasswordEntry {
    std::string website;   // plaintext in memory
    std::string password;  // plaintext in memory, encrypted on disk
};

// ---------------------------------------------------------------
// ENUM: PasswordStrength
// ---------------------------------------------------------------
enum class PasswordStrength {
    VERY_WEAK,
    WEAK,
    MODERATE,
    STRONG,
    VERY_STRONG
};

// ---------------------------------------------------------------
// CLASS: PasswordManager
// ---------------------------------------------------------------
class PasswordManager {
public:
    PasswordManager(const std::string& filename);

    void addPassword(const std::string& website, const std::string& password);
    void viewAllPasswords() const;        // masked by default, reveal on demand
    void searchPassword(const std::string& website) const;
    void deletePassword(const std::string& website);
    void encryptDecryptTool() const;      // Option 6

    static PasswordStrength checkStrength(const std::string& password);
    static void             printStrengthReport(const std::string& password);

    static std::string encrypt(const std::string& text);
    static std::string decrypt(const std::string& text);

private:
    std::string                m_filename;
    std::vector<PasswordEntry> m_entries;

    void loadFromFile();
    void saveToFile() const;
};

#endif // PASSWORDMANAGER_H