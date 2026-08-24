#include "DatabaseManager.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

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

string DatabaseManager::getEquipmentsJson() {

	// 1. DB 연결 확인
	if (!connection || !connection->is_open()) { // connection이 없거나, 있지만 open 안 된 경우
		throw runtime_error("Database is not connected.");
	}

	// 조회 전용 trransaction
	pqxx::read_transaction transaction(*connection); // 이게 뭐지? transaction?

	// 2. SELECT 실행(equipment 테이블 SELECT)
	pqxx::result result = transaction.exec( // PostgreSQL에서 SELECT 실행 후 결과를 result로 받음
		R"(
			SELECT
					equipment_id, name, status, total_produced,
					total_defect
			FROM equipment
			ORDER BY equipment_id
		)"
	);

	// 3. equipment 행들 받기
	nlohmann::json jsonArray = nlohmann::json::array();

	// 4. 각 행을 JSON 객체로 변환(SELECT 결과의 각 행을 하나씩 처리
	for (const auto& row : result) {
		nlohmann::json equipmentJson = {
			{"id", row["equipment_id"].as<string>()},
			{"name", row["name"].as<string>()},
			{"status", row["status"].as<string>()},
			{"totalProduced", row["total_produced"].as<int>()},
			{"totalDefect", row["total_defect"].as<int>()}
		};

		// 5. JSON 배열로 만들기
		jsonArray.push_back(equipmentJson);
	}

	// 6. string으로 반환(JSON 객체 -> JSON 문자열)
	return jsonArray.dump();
}

string DatabaseManager::getWorkOrdersJson() {

	// 1. DB 연결 확인
	if (!connection || !connection->is_open()) { // connection이 없거나, 있지만 open 안 된 경우
		throw runtime_error("Database is not connected.");
	}

	// 조회 전용 trransaction
	pqxx::read_transaction transaction(*connection); // 이게 뭐지? transaction?

	// 2. SELECT 실행(workOrder 테이블 SELECT)
	pqxx::result result = transaction.exec( // PostgreSQL에서 SELECT 실행 후 결과를 result로 받음
		R"(
			SELECT
				work_order_id, product_code, target_quantity,
				produced_quantity, defect_quantity, status,
				equipment_id
			FROM work_order
			ORDER BY work_order_id;
		)"
	);

	// 3. workOrder 행들 받기
	nlohmann::json jsonArray = nlohmann::json::array();

	// 4. 각 행을 JSON 객체로 변환(SELECT 결과의 각 행을 하나씩 처리
	for (const auto& row : result) {
		nlohmann::json workOrderJson = {
			{"id", row["work_order_id"].as<string>()},
			{"productCode", row["product_code"].as<string>()},
			{"targetQuantity", row["target_quantity"].as<int>()},
			{"producedQuantity", row["produced_quantity"].as<int>()},
			{"defectQuantity", row["defect_quantity"].as<int>()},
			{"status", row["status"].as<string>()},
			{"equipmentId", row["equipment_id"].as<string>()},
		};

		// 5. JSON 배열로 만들기
		jsonArray.push_back(workOrderJson);
	}

	// 6. string으로 반환(JSON 객체 -> JSON 문자열)
	return jsonArray.dump();

}

string DatabaseManager::getLotsJson() {

	// 1. DB 연결 확인
	if (!connection || !connection->is_open()) { // connection이 없거나, 있지만 open 안 된 경우
		throw runtime_error("Database is not connected.");
	}

	// 조회 전용 trransaction
	// PostgreSQL 연결을 이용해 SELECT를 수행하기 위한 읽기 전용 transaction 생성
	pqxx::read_transaction transaction(*connection);

	// 2. SELECT 실행(lot 테이블 SELECT)
	pqxx::result result = transaction.exec( // PostgreSQL에서 SELECT 실행 후 결과를 result로 받음
		R"(
			SELECT
				lot_id, item_code, type, quantity
			FROM lot
			ORDER BY lot_id;
		)"
	);

	// 3. lot 행들 받기
	nlohmann::json jsonArray = nlohmann::json::array();

	// 4. 각 행을 JSON 객체로 변환(SELECT 결과의 각 행을 하나씩 처리
	for (const auto& row : result) {
		nlohmann::json lotJson = {
			{"id", row["lot_id"].as<string>()},
			{"itemCode", row["item_code"].as<string>()},
			{"type", row["type"].as<string>()},
			{"quantity", row["quantity"].as<int>()}
		};

		// 5. JSON 배열로 만들기
		jsonArray.push_back(lotJson);
	}

	// 6. string으로 반환(JSON 객체 -> JSON 문자열)
	return jsonArray.dump();
}

string DatabaseManager::getAlarmsJson() {

	// 1. DB 연결 확인
	if (!connection || !connection->is_open()) { // connection이 없거나, 있지만 open 안 된 경우
		throw runtime_error("Database is not connected.");
	}

	// 조회 전용 trransaction
	// PostgreSQL 연결을 이용해 SELECT를 수행하기 위한 읽기 전용 transaction 생성
	pqxx::read_transaction transaction(*connection);

	// 2. SELECT 실행(alarm 테이블 SELECT)
	pqxx::result result = transaction.exec( // PostgreSQL에서 SELECT 실행 후 결과를 result로 받음
		R"(
			SELECT
				id, equipment_id, temperature, message, created_at
			FROM alarm
			ORDER BY created_at DESC
		)"
	);

	// 3. alarm 행들 받기
	nlohmann::json jsonArray = nlohmann::json::array();

	// 4. 각 행을 JSON 객체로 변환(SELECT 결과의 각 행을 하나씩 처리
	for (const auto& row : result) {
		nlohmann::json alarmJson = {
			{"id", row["id"].as<long long>()},
			{"equipmentId", row["equipment_id"].as<string>()},
			{"temperature", row["temperature"].as<double>()},
			{"message", row["message"].as<string>()},
			{"createdAt", row["created_at"].as<string>()}
		};

		// 5. JSON 배열로 만들기
		jsonArray.push_back(alarmJson);
	}

	// 6. string으로 반환(JSON 객체 -> JSON 문자열)
	return jsonArray.dump();

}

string DatabaseManager::getSummaryJson() {
	// Dashboard에 표시할 전체 생산량과 불량량을 계산해서 JSON으로 반환

	// 1. DB 연결 확인
	if (!connection || !connection->is_open()) { // connection이 없거나, 있지만 open 안 된 경우
		throw runtime_error("Database is not connected.");
	}

	// 조회 전용 trransaction
	// PostgreSQL 연결을 이용해 SELECT를 수행하기 위한 읽기 전용 transaction 생성
	pqxx::read_transaction transaction(*connection);

	pqxx::result equipmentResult = transaction.exec( // PostgreSQL에서 SELECT 실행 후 결과를 result로 받음
		R"(
			SELECT COUNT(*) AS equipment_count
			FROM equipment;
		)"
	);

	pqxx::result workOrderResult = transaction.exec( // PostgreSQL에서 SELECT 실행 후 결과를 result로 받음
		R"(
			SELECT COUNT(*) AS work_order_count,
				COALESCE(SUM(produced_quantity), 0) AS total_produced,
				COALESCE(SUM(defect_quantity), 0) AS total_defect
			FROM work_order;
		)"
	);

	int equipmentCount = equipmentResult[0]["equipment_count"].as<int>();
	int workOrderCount = workOrderResult[0]["work_order_count"].as<int>();
	int totalProduced = workOrderResult[0]["total_produced"].as<int>();
	int totalDefect = workOrderResult[0]["total_defect"].as<int>();

	double defectRate = 0.0;

	if (totalProduced > 0) {
		defectRate = static_cast<double>(totalDefect) / totalProduced * 100.0;
	}

	// Json 객체 생성
	nlohmann::json summaryJson = {
		{"equipmentCount", equipmentCount},
		{"workOrderCount", workOrderCount},
		{"totalProduced", totalProduced},
		{"totalDefect", totalDefect},
		{"defectRate", defectRate}
	};

	
	// Json -> 문자열
	return summaryJson.dump();
}
