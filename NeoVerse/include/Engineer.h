#pragma once
#include <string>

using namespace std;

// Represents an AI Control Core engineer who may log in to the system.
// All fields are private (encapsulation) and reached only through the
// accessors below.
class Engineer {
private:
    string engineerID;         // e.g. "ENG001"
    string username;
    string encryptedPassword;  // hex-encoded XOR cipher, never plain text
    string clearanceLevel;     // "Low", "Medium", "High"

public:
    Engineer();
    Engineer(const string& id, const string& uname,
             const string& rawPassword, const string& clearance);
    ~Engineer();

    string getEngineerID() const;
    string getUsername() const;
    string getClearanceLevel() const;
    string getEncryptedPassword() const;

    void setClearanceLevel(const string& level);
    void setEncryptedPasswordDirect(const string& encHex); // used when loading from file

    bool verifyPassword(const string& rawPassword) const;

    // File persistence helpers (pipe-delimited record)
    string serialize() const;
    static Engineer deserialize(const string& line);

    void display() const;
};
