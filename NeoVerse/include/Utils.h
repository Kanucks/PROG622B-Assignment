#pragma once
#include <string>

// Small helper functions shared across the project.
namespace Utils {
    // Simple reversible XOR "encryption" used to avoid storing engineer
    // passwords as plain text, as required by the brief. This is NOT
    // cryptographically secure - it exists purely to demonstrate the
    // concept of encoding sensitive data before it is stored/saved.
    std::string encrypt(const std::string& plain, char key = 0x5A);
    std::string decrypt(const std::string& cipherHex, char key = 0x5A);

    // Returns the current date/time as "YYYY-MM-DD HH:MM:SS".
    std::string currentTimestamp();
}
