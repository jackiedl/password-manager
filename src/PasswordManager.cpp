#include "PasswordManager.h"

PasswordManager::PasswordManager(const std::string& dbPath, const std::string& hashPassword)
	:m_db(Database(dbPath))
	, m_hashpassword(hashPassword)
{
	m_db.createEntryTable();
	loadEntries();
}

void PasswordManager::addEntry(const PasswordEntry& entry)
{
	m_db.addEntry(entry);
	loadEntries();
}

void PasswordManager::removeEntry(const int id)
{
	m_db.removeEntry(id);
	loadEntries();
}

void PasswordManager::updateEntry(const int id, const PasswordEntry& entry)
{
	m_db.updateEntry(id, entry);
	loadEntries();
}

void PasswordManager::loadEntries()
{
	m_entries = m_db.loadEntries(m_hashpassword);
}

