#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <openssl/sha.h>
#include "Database.h"
#include "Encryption.h"

class PasswordManager
{
public:
	PasswordManager(const std::string& dbPath, const std::string& hashPassword);
	~PasswordManager() = default;

	void addEntry(const PasswordEntry& entry);
	void removeEntry(const int id);
	void updateEntry(const int id, const PasswordEntry& entry);
	const std::vector<PasswordEntry> getEntries() const { return m_entries; }
	const std::string& getHashPassword() const { return m_hashpassword; }
private:
	Database m_db;
	Encryption m_crypt;
	std::string m_hashpassword;
	std::vector<PasswordEntry> m_entries;

private:
	void loadEntries();
};

#endif PASSWORDMANAGER_H