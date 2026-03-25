# 🔐 Password Manager Tool

![C++](https://img.shields.io/badge/Language-C++-blue)
![Platform](https://img.shields.io/badge/Platform-CLI-lightgrey)
![Status](https://img.shields.io/badge/Status-Active-success)

A secure, console-based **Password Manager** built in C++ that stores credentials locally using encryption.  
Designed with a focus on simplicity, privacy, and core system design concepts.

---

## 📌 Overview
- Stores passwords securely using **XOR encryption**
- Works completely **offline (no cloud)**
- Clean and interactive **menu-driven CLI**

---

## ✨ Features
- 🔒 **Secure Storage** – Encrypts and saves data in `passwords.dat`
- 📊 **Password Strength Analyzer** – Checks strength + gives suggestions
- 👁️ **Masked Display** – Passwords hidden by default
- 🔍 **Smart Search** – Case-insensitive search
- ❌ **Delete Functionality** – Remove saved entries
- 🚫 **Duplicate Prevention** – Avoids overwriting existing data
- 🔄 **Encrypt/Decrypt Tool** – Manual text encryption utility
- 💾 **Auto Save** – Loads & saves automatically

---

## 🚀 Advantages
- 🖥️ **100% Offline & Private** – No external servers
- ⚡ **Lightweight** – Pure C++ (no frameworks)
- 🧑‍💻 **User-Friendly CLI** – Simple and intuitive menus
- 🛡️ **Security Awareness** – Warns for weak passwords

---

## 🛠️ Tech Stack
- **Language:** C++
- **Concepts:** OOP, File Handling, STL

**Libraries Used:**
- iostream, fstream, string, vector  
- algorithm, cctype, limits, sstream  

---

## 📂 Project Structure
PasswordManager/
│── main.cpp
│── PasswordManager.cpp
│── PasswordManager.h
│── .gitignore
│── README.md

---

## ▶️ Getting Started

### 1. Clone
```bash
git clone https://github.com/your-username/password-manager.git
cd password-manager
```

### 2. Compile
```bash
g++ main.cpp PasswordManager.cpp -o PasswordManager
```
👉 This will create an executable file (.exe):
- On Windows: PasswordManager.exe
- On Mac/Linux: PasswordManager

### 3. Run
```bash
./PasswordManager
```
