#include "items.hpp"

void escape(std::string& str)
{
	size_t pos = 0;
	while((pos = str.find("\\", pos)) != std::string::npos)
	{
		str.insert(pos, "\\");
		pos += 2;
	}
	pos = 0;
	while((pos = str.find("\'", pos)) != std::string::npos)
	{
		str.insert(pos, "\\");
		pos += 2;
	}
	pos = 0;
	while((pos = str.find("\"", pos)) != std::string::npos)
	{
		str.insert(pos, "\\");
		pos += 2;
	}
}

Items::Items()
{
	int rc;
	rc = sqlite3_open("items.db", &_db);
	char *error;
	if(rc)
	{
		throw std::runtime_error(sqlite3_errmsg(_db));
	}	
	const char* sql = "CREATE TABLE IF NOT EXISTS Increments(id INTEGER PRIMARY KEY ASC, trigger INTEGER, per TEXT, what TEXT, howmuch REAL);";
	rc = sqlite3_exec(_db, sql, NULL, NULL, &error);
	if(rc)
	{
		fprintf(stderr, "Error executing Increments statement: %s\n", error);
		sqlite3_free(error);
		throw;
	}
	const char* sql2 = "CREATE TABLE IF NOT EXISTS Items(id INTEGER PRIMARY KEY ASC, name TEXT);";
	rc = sqlite3_exec(_db, sql2, NULL, NULL, &error);
	if(rc)
	{
		fprintf(stderr, "Error executing Items statement: %s\n", error);
		sqlite3_free(error);
		throw;
	}
	const char* sql3 = "CREATE TABLE IF NOT EXISTS Channels(id INTEGER PRIMARY KEY ASC, name TEXT);";
	rc = sqlite3_exec(_db, sql3, NULL, NULL, &error);
	if(rc)
	{
		fprintf(stderr, "Error executing Items statement: %s\n", error);
		sqlite3_free(error);
		throw;
	}
	getIncrements();
}

Items::~Items()
{
	sqlite3_close(_db);
}

int Items::createChannelTable(const std::string& chn)
{
	auto vek = getItems();
	std::string sql = "CREATE TABLE IF NOT EXISTS " + chn + " (id INTEGER PRIMARY KEY ASC, username TEXT UNIQUE";
	for(auto i : vek)
	{
		sql += ", " + i + " INT";
	}
	sql += ");";
	char *error;
	int rc;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement, NULL);
	sqlite3_step(statement);
	rc = sqlite3_finalize(statement);
	if(rc)
	{
		fprintf(stderr, "Error adding %s table: %s\n", chn.c_str(), error);
		sqlite3_free(error);
		return rc;
	}
	else
	{
		auto v = getChannels();
		if(std::find(v.begin(), v.end(), chn) == v.end())
		{
			char *error;
			sqlite3_stmt * statement;
			sqlite3_prepare_v2(_db, "INSERT OR IGNORE INTO Channels('name') VALUES (?);", -1, &statement, NULL);
			sqlite3_bind_text(statement, 1, chn.c_str(), -1, 0);
			sqlite3_step(statement);
			rc = sqlite3_finalize(statement);
			return rc;
		}
	}
	return rc;
}

int Items::addItemCategory(const std::string& item)
{
	auto v = getItems();
	if(std::find(v.begin(), v.end(), item) != v.end())
	{
		return -1;
	}
	char *error;
	int rc;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, "INSERT OR IGNORE INTO Items('name') VALUES (?);", -1, &statement, NULL);
	sqlite3_bind_text(statement, 1, item.c_str(), -1, 0);
	sqlite3_step(statement);
	rc = sqlite3_finalize(statement);
	if(!rc)
	{
		auto channels = getChannels();
		for(const auto& chn : channels)
		{
			std::cout << chn;
			sqlite3_stmt * statement2;
			std::string itemE = item;
			escape(itemE);
			std::string sql = "ALTER TABLE " + chn + " ADD COLUMN " + itemE + " INT;";
			sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement2, NULL);
			sqlite3_step(statement2);
			rc = sqlite3_finalize(statement2);
		}
	}
	return rc;
}

void Items::getIncrements()
{
	_increments.clear();
	int rc;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, "SELECT trigger, per, what, howmuch FROM Increments;", -1, &statement, NULL);
	while((rc = sqlite3_step(statement)) == SQLITE_ROW)
	{
		Increments incr;
		incr.trigger = sqlite3_column_int(statement, 0);
		incr.per = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		incr.what = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		incr.howmuch = sqlite3_column_double(statement, 3);
		
		_increments.push_back(incr);
	}
	rc = sqlite3_finalize(statement);
}

std::vector<std::string> Items::getItems()
{
	int rc;
	std::vector<std::string> vek;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, "SELECT name FROM Items;", -1, &statement, NULL);
	while((rc = sqlite3_step(statement)) == SQLITE_ROW)
	{
		std::string text = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
		vek.push_back(text);
	}
	rc = sqlite3_finalize(statement);
	return vek;
}

std::vector<std::string> Items::getTableNames()
{
	int rc;
	std::vector<std::string> vek;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, "SELECT * FROM sqlite_master;", -1, &statement, NULL);
	while((rc = sqlite3_step(statement)) == SQLITE_ROW)
	{
		std::string text = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		vek.push_back(text);
	}
	rc = sqlite3_finalize(statement);
	return vek;
}

std::vector<std::string> Items::getColumnNames(const std::string& table)
{
	std::vector<std::string> vek;
	auto v = getChannels();
	if(std::find(v.begin(), v.end(), table) != v.end())
	{
		int rc;
		sqlite3_stmt * statement;
		std::string sql = "PRAGMA table_info(" + table + ");";
		sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement, NULL);
		while((rc = sqlite3_step(statement)) == SQLITE_ROW)
		{
			std::string text = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
			vek.push_back(text);
		}
		rc = sqlite3_finalize(statement);
	}
	return vek;	
}

std::vector<std::string> Items::getChannels()
{
	int rc;
	std::vector<std::string> vek;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, "SELECT name FROM Channels;", -1, &statement, NULL);
	while((rc = sqlite3_step(statement)) == SQLITE_ROW)
	{
		std::string text = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
		vek.push_back(text);
		getUsers(text);
	}
	rc = sqlite3_finalize(statement);
	return vek;
}

int Items::addIncrement(int trigger, const std::string& per, const std::string& what, double howmuch)
{
	auto v = getItems();
	if(std::find(v.begin(), v.end(), per) == v.end() && per != "default")
	{
		return -1;
	}
	if(std::find(v.begin(), v.end(), what) == v.end())
	{
		return -2;
	}
	char *error;
	int rc;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, "INSERT INTO Increments('trigger', 'per', 'what', 'howmuch') VALUES (?, ?, ?, ?);", -1, &statement, NULL);
	sqlite3_bind_int(statement, 1, trigger);
	sqlite3_bind_text(statement, 2, per.c_str(), -1, 0);
	sqlite3_bind_text(statement, 3, what.c_str(), -1, 0);
	sqlite3_bind_double(statement, 4, howmuch);
	sqlite3_step(statement);
	rc = sqlite3_finalize(statement);
	if(!rc)
		getIncrements();
	return rc;
}

int Items::deleteIncrement(int id)
{
	char *error;
	int rc;
	sqlite3_stmt * statement;
	sqlite3_prepare_v2(_db, "DELETE FROM Increments WHERE id = ?;", -1, &statement, NULL);
	sqlite3_bind_int(statement, 1, id);
	sqlite3_step(statement);
	rc = sqlite3_finalize(statement);
	if(!rc)
		getIncrements();
	return rc;
}

void Items::insert(const std::string& channel, const std::string& username)
{
	sqlite3_stmt * statement;
	std::string sql = "INSERT INTO " + channel + "(username) VALUES(?);";
	sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement, NULL);
	sqlite3_bind_text(statement, 1, username.c_str(), -1, 0);
	sqlite3_step(statement);
	sqlite3_finalize(statement);
	chnusers[channel].insert(username);
}

//update
void Items::insertOrReplace(const std::string& channel, const std::string& username, const std::string& what, long long howmany)
{
	//vek[0] == channel
	//vek[1] == username
	//vek[2] == what
	//vek[3] == howmany
	sqlite3_stmt * statement;
	std::string sql = "UPDATE " + channel + " SET " + what + " = ? WHERE username = ?;";
	sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement, NULL);
	sqlite3_bind_int64(statement, 1, howmany);
	sqlite3_bind_text(statement, 2, username.c_str(), -1, 0);
	int rc = sqlite3_step(statement);
	sqlite3_finalize(statement);
}

long long Items::getCount(const std::string& channel, const std::string& username, const std::string& what)
{
	sqlite3_stmt * statement;
	std::string sql = "SELECT " + what + " FROM " + channel + " WHERE username = ?;"; 
	sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement, NULL);
	sqlite3_bind_text(statement, 1, username.c_str(), -1, 0);
	long long value = 0;
	if(sqlite3_step(statement) == SQLITE_ROW)
	{
		if(sqlite3_column_type(statement, 0) != SQLITE_NULL)
			value = sqlite3_column_int64(statement, 0);
	}
	return value;
}

bool Items::exists(const std::string& channel, const std::string& username)
{
	sqlite3_stmt * statement;
	std::string sql = "SELECT id FROM " + channel + " WHERE username = ?;"; 
	sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement, NULL);
	sqlite3_bind_text(statement, 1, username.c_str(), -1, 0);
	int value = 0;
	if(sqlite3_step(statement) == SQLITE_ROW)
	{
		if(sqlite3_column_type(statement, 0) != SQLITE_NULL)
			value = sqlite3_column_int(statement, 0);
	}
	if(value != 0)
	{
		return true;
	}
	else return false;
}

void Items::getUsers(const std::string& channel)
{
	begin();
	sqlite3_stmt * statement;
	std::string sql = "SELECT username FROM " + channel + ";"; 
	sqlite3_prepare_v2(_db, sql.c_str(), -1, &statement, NULL);
	while(sqlite3_step(statement) == SQLITE_ROW)
	{
		std::string text = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
		chnusers[channel].insert(text);
	}
	end();
}

void Items::begin()
{
	sqlite3_exec(_db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
}

void Items::end()
{
	sqlite3_exec(_db, "END TRANSACTION;", NULL, NULL, NULL);		
}