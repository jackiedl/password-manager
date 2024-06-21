#include "Database.h"

int Database::open()
{
	return sqlite3_open(m_dbPath.c_str(), &m_database);
}

void Database::close()
{
	sqlite3_close(m_database);
}

void Database::createEntryTable()
{
	char* messageError;

	std::string entry = "CREATE TABLE PASSWORDENTRY(" \
		"ID INTEGER  PRIMARY	  KEY," \
		"NAME					  TEXT	NOT NULL," \
		"USERNAME				  TEXT	NOT NULL," \
		"HASHPASSWORD			  TEXT	NOT NULL," \
		"URL					  TEXT	NOT NULL); ";

	int rc = sqlite3_exec(m_database, entry.c_str(), NULL, 0, &messageError);

	if (rc != SQLITE_DONE) {
		std::cerr << "Create table Error: " << sqlite3_errmsg(m_database) << '\n';
	}
}

void Database::addEntry(const PasswordEntry& entry)
{
	std::string sql = "INSERT INTO PASSWORDENTRY (NAME, USERNAME, HASHPASSWORD, URL)VALUES(?, ?, ?, ?)";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(m_database, sql.c_str(), -1, &stmt, NULL);
	
	sqlite3_bind_text(stmt, 1, entry.getName().c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, entry.getUsername().c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 3, entry.getPassword().c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 4, entry.getUrl().c_str(), -1, NULL);

	int rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE) {
		std::cerr << "Add Entry Error: " << sqlite3_errmsg(m_database) << '\n';
	}
	sqlite3_finalize(stmt);
}

void Database::removeEntry(const int id)
{
	std::string sql = "DELETE FROM PASSWORDENTRY WHERE ID=?;";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(m_database, sql.c_str(), -1, &stmt, NULL);

	sqlite3_bind_int(stmt, 1, id);

	int rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE) {
		std::cerr << "Remove Entry Error: " << sqlite3_errmsg(m_database) << '\n';
	}
	sqlite3_finalize(stmt);
}

void Database::updateEntry(const int id, const PasswordEntry& entry)
{
	std::string sql =
		"UPDATE PASSWORDENTRY "\
		"SET NAME=?,"\
		"USERNAME=?,"\
		"HASHPASSWORD=?,"\
		"URL=?"\
		"WHERE ID=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(m_database, sql.c_str(), -1, &stmt, NULL);

	sqlite3_bind_text(stmt, 1, entry.getName().c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 2, entry.getUsername().c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 3, entry.getPassword().c_str(), -1, NULL);
	sqlite3_bind_text(stmt, 4, entry.getUrl().c_str(), -1, NULL);
	sqlite3_bind_int(stmt, 5, id);

	int rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE) {
		std::cerr << "Update Entry Error: " << sqlite3_errmsg(m_database) << '\n';
	}
	sqlite3_finalize(stmt);
}

std::vector<PasswordEntry> Database::loadEntries(const std::string& key)
{
	sqlite3_stmt* stmt;
	std::string sql = "SELECT * FROM PASSWORDENTRY";
	std::vector<PasswordEntry> entries;

	int rc = sqlite3_prepare_v2(m_database, sql.c_str(), -1, &stmt, NULL);
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
		std::string username = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
		std::string hashpassword = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
		std::string url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

		PasswordEntry entry{ name, username, Encryption::decrypt(hashpassword, key), url};

		entry.setId(sqlite3_column_int(stmt, 0));
		entries.push_back(entry);
	}
	if (rc != SQLITE_DONE) {
		std::cerr << "Load Entries Error: " << sqlite3_errmsg(m_database) << '\n';
	}
	sqlite3_finalize(stmt);

	return entries;
}

int Database::getLastInsertRowId()
{
	return sqlite3_last_insert_rowid(m_database);
}