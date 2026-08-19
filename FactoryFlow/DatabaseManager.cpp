#include "DatabaseManager.h"
#include <iostream>
using namespace std;

bool DatabaseManager::connect(const std::string& connectionString) {
	try {
		connection = std::make_unique<pqxx::connection>(connectionString);

		if (!connection->is_open()) return false;

		return true;
	}

	catch (const exception& e) {
		cout << "Database Connection Error: " << e.what() << "\n";
		return false;
	}
}

bool DatabaseManager::saveTelemetry(const TelemetryMessage& message) {
	
	try {
		// 1. DB 연결 확인
		if (!connection || !connection->is_open()) return false;
		// connection이 아무 pqxx::connection 객체도 가리키지 않는지,
		// 객체는 있지만 DB 연결이 닫혀 있는지 확인

		// 2. transaction 시작
		pqxx::work transaction(*connection);
		// 현재 PostgreSQL 연결을 사용해서 transaction 하나를 시작

		// 3. INSERT 실행
		transaction.exec(
			R"(
				INSERT INTO telemetry (
					equipment_id, work_order_id, timestamp,
					temperature, vibration, production_count,
					defect_count, lot_id
				)
				VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
			)",

			pqxx::params{
				message.getEquipmentId(), message.getWorkOrderId(),
				message.getTimestamp(), message.getTemperature(),
				message.getVibration(), message.getProductionCount(),
				message.getDefectCount(), message.getLotId()
			}
		);

		// 4. commit
		transaction.commit();
		// 실제 DB 변경 확정

		return true;
	}

	catch (const exception& e) {
		cout << "Save Telemetry Error: "
			<< e.what() << "\n";

		return false;
	}
}