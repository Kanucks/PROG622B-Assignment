#pragma once
#include <string>

// Represents an AI Control Core engineer who may log in to the system.
// All fields are private (encapsulation) and reached only through the
// accessors below.
class Engineer {
private:
    std::string engineerID;         // e.g. "ENG001"
    std::string username;
    std::string encryptedPassword;  // hex-encoded XOR cipher, never plain text
    std::string clearanceLevel;     // "Low", "Medium", "High"

public:
    Engineer();
    Engineer(const std::string& id, const std::string& uname,
             const std::string& rawPassword, const std::string& clearance);
    ~Engineer();

    std::string getEngineerID() const;
    std::string getUsername() const;
    std::string getClearanceLevel() const;
    std::string getEncryptedPassword() const;

    void setClearanceLevel(const std::string& level);
    void setEncryptedPasswordDirect(const std::string& encHex); // used when loading from file

    bool verifyPassword(const std::string& rawPassword) const;

    // File persistence helpers (pipe-delimited record)
    std::string serialize() const;
    static Engineer deserialize(const std::string& line);

    void display() const;
};
