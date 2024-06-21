#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <iostream>
#include <sqlite3.h>
#include "PasswordEntry.h"

class Database
{
public:
	Database(const std::string& dbPath)
		: m_dbPath{ dbPath }
	{
		open();
	}
	
	~Database()
	{
		if (m_database)
		{
			close();
		}
	}
	
	void createEntryTable();
	
	void addEntry(const PasswordEntry& entry);
	void removeEntry(const int id);
	void updateEntry(const int id, const PasswordEntry& entry);

	std::vector<PasswordEntry> loadEntries(const std::string& key);

	int getLastInsertRowId();

private:
	std::string m_dbPath{};
	sqlite3* m_database{ nullptr };

private:
	int open();
	void close();
};

#endif DATABASE_H