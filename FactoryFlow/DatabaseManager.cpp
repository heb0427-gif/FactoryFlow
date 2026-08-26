#include "DatabaseManager.h"
#include <iostream>
#include <pqxx/version.hxx>

using namespace std;


// ========================================
// PostgreSQL 연결
// ========================================

bool DatabaseManager::connect(const string& connectionString) {

	try {
		connection = make_unique<pqxx::connection>(connectionString);

		if (!connection->is_open()) {
			return false;
		}

		return true;
	}

	catch (const exception& e) {
		cout << "Database Connection Error: "
			<< e.what() << "\n";

		return false;
	}
}


// ========================================
// Telemetry 저장
// ========================================

bool DatabaseManager::saveTelemetry(
	const TelemetryMessage& message) {

	try {

		// 1. DB 연결 확인
		if (!connection || !connection->is_open()) {
			return false;
		}


		// 2. Transaction 시작
		pqxx::work transaction(*connection);


		// 3. INSERT 실행

#if PQXX_VERSION_MAJOR >= 8

		// libpqxx 8 이상
		transaction.exec(
			R"(
				INSERT INTO telemetry (
					equipment_id,
					work_order_id,
					timestamp,
					temperature,
					vibration,
					production_count,
					defect_count,
					lot_id
				)
				VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
			)",

			pqxx::params{
				message.getEquipmentId(),
				message.getWorkOrderId(),
				message.getTimestamp(),
				message.getTemperature(),
				message.getVibration(),
				message.getProductionCount(),
				message.getDefectCount(),
				message.getLotId()
			}
		);

#else

		// libpqxx 7 이하
		transaction.exec_params(
			R"(
				INSERT INTO telemetry (
					equipment_id,
					work_order_id,
					timestamp,
					temperature,
					vibration,
					production_count,
					defect_count,
					lot_id
				)
				VALUES ($1, $2, $3, $4, $5, $6, $7, $8)
			)",

			message.getEquipmentId(),
			message.getWorkOrderId(),
			message.getTimestamp(),
			message.getTemperature(),
			message.getVibration(),
			message.getProductionCount(),
			message.getDefectCount(),
			message.getLotId()
		);

#endif


		// 4. 실제 DB 변경 확정
		transaction.commit();

		return true;
	}

	catch (const exception& e) {
		cout << "Save Telemetry Error: "
			<< e.what() << "\n";

		return false;
	}
}


// ========================================
// Equipment INSERT / UPDATE
// ========================================

bool DatabaseManager::upsertEquipment(
	const Equipment& equipment) {

	try {

		// 1. DB 연결 확인
		if (!connection || !connection->is_open()) {
			return false;
		}


		// 2. Transaction 시작
		pqxx::work transaction(*connection);


		// 3. UPSERT 실행

#if PQXX_VERSION_MAJOR >= 8

		// libpqxx 8 이상
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
				equipmentStatusToString(
					equipment.getStatus()
				),
				equipment.getTotalProducedQuantity(),
				equipment.getTotalDefectQuantity()
			}
		);

#else

		// libpqxx 7 이하
		transaction.exec_params(
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

			equipment.getId(),
			equipment.getName(),
			equipmentStatusToString(
				equipment.getStatus()
			),
			equipment.getTotalProducedQuantity(),
			equipment.getTotalDefectQuantity()
		);

#endif


		// 4. 실제 DB 변경 확정
		transaction.commit();

		return true;
	}

	catch (const exception& e) {
		cout << "Save Equipment Error: "
			<< e.what() << "\n";

		return false;
	}
}


// ========================================
// WorkOrder INSERT / UPDATE
// ========================================

bool DatabaseManager::upsertWorkOrder(
	const WorkOrder& workOrder) {

	try {

		// 1. DB 연결 확인
		if (!connection || !connection->is_open()) {
			return false;
		}


		// 2. Transaction 시작
		pqxx::work transaction(*connection);


		// 3. UPSERT 실행

#if PQXX_VERSION_MAJOR >= 8

		// libpqxx 8 이상
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
				workOrderStatusToString(
					workOrder.getStatus()
				),
				workOrder.getAssignedEquipmentId()
			}
		);

#else

		// libpqxx 7 이하
		transaction.exec_params(
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

			workOrder.getId(),
			workOrder.getProductCode(),
			workOrder.getTargetQuantity(),
			workOrder.getProducedQuantity(),
			workOrder.getDefectQuantity(),
			workOrderStatusToString(
				workOrder.getStatus()
			),
			workOrder.getAssignedEquipmentId()
		);

#endif


		// 4. 실제 DB 변경 확정
		transaction.commit();

		return true;
	}

	catch (const exception& e) {
		cout << "Save Work Order Error: "
			<< e.what() << "\n";

		return false;
	}
}


// ========================================
// Alarm 저장
// ========================================

bool DatabaseManager::saveAlarm(
	const TelemetryMessage& message,
	const string& alarmMessage) {

	try {

		// 1. DB 연결 확인
		if (!connection || !connection->is_open()) {
			return false;
		}


		// 2. Transaction 시작
		pqxx::work transaction(*connection);


		// 3. INSERT 실행

#if PQXX_VERSION_MAJOR >= 8

		// libpqxx 8 이상
		transaction.exec(
			R"(
				INSERT INTO alarm (
					equipment_id,
					temperature,
					message
				)
				VALUES ($1, $2, $3)
			)",

			pqxx::params{
				message.getEquipmentId(),
				message.getTemperature(),
				alarmMessage
			}
		);

#else

		// libpqxx 7 이하
		transaction.exec_params(
			R"(
				INSERT INTO alarm (
					equipment_id,
					temperature,
					message
				)
				VALUES ($1, $2, $3)
			)",

			message.getEquipmentId(),
			message.getTemperature(),
			alarmMessage
		);

#endif


		// 4. 실제 DB 변경 확정
		transaction.commit();

		return true;
	}

	catch (const exception& e) {
		cout << "Save Alarm Error: "
			<< e.what() << "\n";

		return false;
	}
}