#include <iostream>
#include <string>
#include "TcpServer.h"
#include "DatabaseManager.h"
#include "TelemetryMessage.h"

using namespace std;

int main() {

	DatabaseManager dbManager;

	// 1. PostgreSQL 연결
	if (!dbManager.connect(
		"host=127.0.0.1 port=5432 dbname=factoryflow user=postgres password=eunbin0427*")) {

		cout << "Database connection failed.\n";
		return 1;
	}

	cout << "Database connected.\n";

	// 2. 테스트용 TelemetryMessage 생성
	TelemetryMessage message(
		"M01",
		"WO001",
		"2026-08-19T19:10:00",
		"RUNNING",
		72.4,
		1.82,
		5,
		1,
		"LOT-001"
	);

	// 3. telemetry 테이블에 저장
	if (!dbManager.saveTelemetry(message)) {
		cout << "Failed to save telemetry.\n";
		return 1;
	}

	cout << "Telemetry saved.\n";

	return 0;
}