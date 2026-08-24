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

bool DatabaseManager::upsertEquipment(const Equipment& equipment) {
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
				INSERT INTO equipment (
					equipment_id,
					name,
					status,
					total_produced,
					total_defect
				)
				VALUES ($1, $2, $3, $4, $5)

				 ON CONFLICT (equipment_id)
				 DO UPDATE SET
					name = EXCLUDED.name,
					status = EXCLUDED.status,
					total_produced = EXCLUDED.total_produced,
					total_defect = EXCLUDED.total_defect,
					updated_at = CURRENT_TIMESTAMP
			)",

			pqxx::params{
				 equipment.getId(),
				 equipment.getName(),
				 equipmentStatusToString(equipment.getStatus()),
				 equipment.getTotalProducedQuantity(),
				 equipment.getTotalDefectQuantity()
			}
		);

		// 4. commit
		transaction.commit();
		// 실제 DB 변경 확정

		return true;
	}

	catch (const exception& e) {
		cout << "Save Equipment Error: "
			<< e.what() << "\n";

		return false;
	}
}

bool DatabaseManager::upsertWorkOrder(const WorkOrder& workOrder) {
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
				INSERT INTO work_order (
					work_order_id,
					product_code,
					target_quantity,
					produced_quantity,
					defect_quantity,
					status,
					equipment_id
				)
				VALUES ($1, $2, $3, $4, $5, $6, $7)

				 ON CONFLICT (work_order_id)
				 DO UPDATE SET
					product_code = EXCLUDED.product_code,
					target_quantity = EXCLUDED.target_quantity,
					produced_quantity = EXCLUDED.produced_quantity,
					defect_quantity = EXCLUDED.defect_quantity,
					status = EXCLUDED.status,
					equipment_id = EXCLUDED.equipment_id
			)",

			pqxx::params{
				workOrder.getId(),
				workOrder.getProductCode(),
				workOrder.getTargetQuantity(),
				workOrder.getProducedQuantity(),
				workOrder.getDefectQuantity(),
				workOrderStatusToString(workOrder.getStatus()),
				workOrder.getAssignedEquipmentId()
			}
		);

		// 4. commit
		transaction.commit();
		// 실제 DB 변경 확정

		return true;
	}

	catch (const exception& e) {
		cout << "Save Work Order Error: "
			<< e.what() << "\n";

		return false;
	}
}

bool DatabaseManager::saveAlarm(const TelemetryMessage& message,
	const string& alarmMessage) {

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
				INSERT INTO alarm (
					equipment_id,
					temperature,
					message
				)
				VALUES ($1, $2, $3)
			)",

			pqxx::params {
				message.getEquipmentId(),
				message.getTemperature(),
				alarmMessage
			}
		);

		// 4. commit
		transaction.commit();
		// 실제 DB 변경 확정

		return true;
	}

	catch (const exception& e) {
		cout << "Save Alarm Error: "
			<< e.what() << "\n";

		return false;
	}
}