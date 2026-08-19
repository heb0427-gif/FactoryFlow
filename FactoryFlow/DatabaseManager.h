#pragma once
#include <string>
#include <memory>
#include <pqxx/pqxx>
#include "TelemetryMessage.h"

class DatabaseManager {
private:
	std::unique_ptr<pqxx::connection> connection; // PostgreSQL과의 실제 DB 연결 객체

public:
	bool connect(const std::string& connectionString); // DB 연결

	bool saveTelemetry(const TelemetryMessage& message);
};