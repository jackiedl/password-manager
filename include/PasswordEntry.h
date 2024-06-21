#ifndef PASSWORDENTRY_H
#define PASSWORDENTRY_H

#include <iostream>
#include "Encryption.h"

class PasswordEntry
{
public:
	PasswordEntry(
		const std::string& name,
		const std::string& username,
		const std::string& password,
		const std::string& url)
		: m_name{ name }
		, m_username{ username }
		, m_password{ password }
		, m_url{ url }
	{
	}
	~PasswordEntry() = default;

	const std::string& getName() const { return m_name; }
	const std::string& getUsername() const { return m_username; }
	const std::string& getPassword() const { return m_password; }
	const std::string& getUrl() const { return m_url; }
	const int& getId() const { return m_id; }

	void setId(int id) { m_id = id; }
	void setName(const std::string& name) { m_name = name; }
	void setUsername(const std::string& username) { m_username = username; }
	void setPassword(const std::string& password) { m_password = password; }
	void setUrl(const std::string& url) { m_url = url; }

	void encryptPassword(const std::string& key) { m_password = Encryption::encrypt(m_password, key); }

private:
	std::string m_name;
	std::string m_username;
	std::string m_password;
	std::string m_url;
	int m_id{};
};

#endif PASSWORDENTRY_H