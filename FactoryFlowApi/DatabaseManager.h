#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>

class DatabaseManager {
private:
	std::unique_ptr<pqxx::connection> connection;

public:
	bool connect(const std::string& connectionString);
	std::string getEquipmentsJson();
	std::string getWorkOrdersJson();
	std::string getLotsJson();
	std::string getAlarmsJson();
	std::string getSummaryJson();
};