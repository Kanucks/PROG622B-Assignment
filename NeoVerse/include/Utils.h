#pragma once
#include <string>

using namespace std;

// Small helper functions shared across the project.
namespace Utils {
    // Simple reversible XOR "encryption" used to avoid storing engineer
    // passwords as plain text, as required by the brief. This is NOT
    // cryptographically secure - it exists purely to demonstrate the
    // concept of encoding sensitive data before it is stored/saved.
    string encrypt(const string& plain, char key = 0x5A);
    string decrypt(const string& cipherHex, char key = 0x5A);

    // Returns the current date/time as "YYYY-MM-DD HH:MM:SS".
    string currentTimestamp();
}
