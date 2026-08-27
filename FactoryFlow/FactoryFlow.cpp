#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <string>
#include "FactorySystem.h"
#include "DatabaseManager.h"
#include "TelemetryProcessor.h"
#include "TelemetryParser.h"
#include "TelemetryMessage.h"
#include "TcpServer.h"

using namespace std;

int main() {

	cout << unitbuf;

	// ========================================
	// 1. FactorySystem 준비
	// ========================================

	FactorySystem factorySystem;


	// ========================================
	// 2. 데모 Equipment 등록
	// ========================================

	if (!factorySystem.registerEquipment("M01", "CNC Machine") ||
		!factorySystem.registerEquipment("M02", "Milling Machine") ||
		!factorySystem.registerEquipment("M03", "Assembly Robot") ||
		!factorySystem.registerEquipment("M04", "Inspection Machine") ||
		!factorySystem.registerEquipment("M05", "Packaging Machine")) {

		cout << "Failed to register equipment.\n";
		return 1;
	}


	// ========================================
	// 3. 데모 WorkOrder 생성
	// ========================================

	if (!factorySystem.createWorkOrder(
		"WO001", "PRODUCT-001", 150, WorkOrderPriority::NORMAL) ||

		!factorySystem.createWorkOrder(
			"WO002", "PRODUCT-002", 180, WorkOrderPriority::HIGH) ||

		!factorySystem.createWorkOrder(
			"WO003", "PRODUCT-003", 200, WorkOrderPriority::HIGH) ||

		!factorySystem.createWorkOrder(
			"WO004", "PRODUCT-004", 120, WorkOrderPriority::NORMAL) ||

		!factorySystem.createWorkOrder(
			"WO005", "PRODUCT-005", 160, WorkOrderPriority::NORMAL)) {

		cout << "Failed to create work orders.\n";
		return 1;
	}


	// ========================================
	// 4. WorkOrder에 Equipment 배정
	// ========================================

	if (!factorySystem.assignEquipmentToWorkOrder("WO001", "M01") ||
		!factorySystem.assignEquipmentToWorkOrder("WO002", "M02") ||
		!factorySystem.assignEquipmentToWorkOrder("WO003", "M03") ||
		!factorySystem.assignEquipmentToWorkOrder("WO004", "M04") ||
		!factorySystem.assignEquipmentToWorkOrder("WO005", "M05")) {

		cout << "Failed to assign equipment.\n";
		return 1;
	}


	// ========================================
	// 5. WorkOrder 시작
	// ========================================

	if (!factorySystem.startWorkOrder("WO001") ||
		!factorySystem.startWorkOrder("WO002") ||
		!factorySystem.startWorkOrder("WO003") ||
		!factorySystem.startWorkOrder("WO004") ||
		!factorySystem.startWorkOrder("WO005")) {

		cout << "Failed to start work orders.\n";
		return 1;
	}


	// ========================================
	// 6. 데모 LOT 등록
	// ========================================

	if (!factorySystem.registerLot(
		"LOT-001", "MATERIAL-STEEL", LotType::INPUT, 500) ||

		!factorySystem.registerLot(
			"LOT-002", "MATERIAL-ALUMINUM", LotType::INPUT, 350) ||

		!factorySystem.registerLot(
			"LOT-003", "MATERIAL-COMPONENT", LotType::INPUT, 400) ||

		!factorySystem.registerLot(
			"LOT-101", "PRODUCT-001", LotType::OUTPUT, 150) ||

		!factorySystem.registerLot(
			"LOT-102", "PRODUCT-002", LotType::OUTPUT, 180) ||

		!factorySystem.registerLot(
			"LOT-103", "PRODUCT-003", LotType::OUTPUT, 200) ||

		!factorySystem.registerLot(
			"LOT-104", "PRODUCT-004", LotType::OUTPUT, 120) ||

		!factorySystem.registerLot(
			"LOT-105", "PRODUCT-005", LotType::OUTPUT, 160)) {

		cout << "Failed to register lots.\n";
		return 1;
	}


	// ========================================
	// 7. LOT 추적 관계 생성
	// ========================================

	if (!factorySystem.addLotRelation(
		"LOT-001", "LOT-101", "WO001", 120) ||

		!factorySystem.addLotRelation(
			"LOT-001", "LOT-102", "WO002", 140) ||

		!factorySystem.addLotRelation(
			"LOT-002", "LOT-103", "WO003", 160) ||

		!factorySystem.addLotRelation(
			"LOT-003", "LOT-104", "WO004", 100) ||

		!factorySystem.addLotRelation(
			"LOT-003", "LOT-105", "WO005", 130)) {

		cout << "Failed to create lot relations.\n";
		return 1;
	}

	cout << "FactorySystem initialized.\n";


	// ========================================
	// 8. PostgreSQL 연결
	// ========================================

	DatabaseManager databaseManager;

	const char* databaseUrl = getenv("DATABASE_URL");

	if (databaseUrl == nullptr) {
		cout << "DATABASE_URL environment variable not found.\n";
		return 1;
	}

	string connectionString = databaseUrl;

	if (!databaseManager.connect(connectionString)) {
		cout << "Database connection failed.\n";
		return 1;
	}

	cout << "Database connected.\n";


	// ========================================
	// 9. TelemetryProcessor 생성
	// ========================================

	TelemetryProcessor processor(
		factorySystem,
		databaseManager
	);


	// ========================================
	// 10. TelemetryParser 생성
	// ========================================

	TelemetryParser parser;


	// ========================================
	// 11. TCP 서버 생성
	// ========================================

	TcpServer server;


	// ========================================
	// 12. TCP_PORT 환경변수 읽기
	// ========================================

	const char* tcpPortText = getenv("TCP_PORT");

	if (tcpPortText == nullptr) {
		cout << "TCP_PORT environment variable not found.\n";
		return 1;
	}

	unsigned short tcpPort;

	try {

		int port = stoi(tcpPortText);

		if (port < 1 || port > 65535) {
			cout << "Invalid TCP_PORT.\n";
			return 1;
		}

		tcpPort = static_cast<unsigned short>(port);
	}
	catch (const exception&) {

		cout << "Invalid TCP_PORT.\n";
		return 1;
	}


	// ========================================
	// 13. TCP 서버 시작
	// ========================================

	if (!server.start(tcpPort)) {
		cout << "Failed to start TCP server.\n";
		return 1;
	}

	cout << "\nFactoryFlow TCP server started on port "
		<< tcpPort << ".\n";


	// ========================================
	// 14. 클라이언트 연결 반복 처리
	// ========================================

	int messageCount = 0;

	while (true) {

		cout << "\nWaiting for client...\n";

		if (!server.waitForClient()) {
			cout << "Failed to accept client.\n";
			continue;
		}

		cout << "Client connected.\n\n";


		// ========================================
		// 15. 현재 클라이언트 메시지 처리
		// ========================================

		while (true) {

			string jsonText;

			if (!server.receiveMessage(jsonText)) {

				cout << "Waiting for next client...\n";
				break;
			}

			messageCount++;

			cout << "[Received "
				<< messageCount
				<< "]\n";

			cout << jsonText << "\n";


			// ----------------------------------------
			// JSON → TelemetryMessage
			// ----------------------------------------

			TelemetryMessage message;

			if (!parser.parse(jsonText, message)) {
				cout << "Invalid telemetry message.\n\n";
				continue;
			}


			// ----------------------------------------
			// FactoryFlow 처리
			// ----------------------------------------

			if (!processor.process(message)) {
				cout << "Failed to process telemetry.\n\n";
				continue;
			}


			// ----------------------------------------
			// 처리 성공
			// ----------------------------------------

			cout << "Telemetry processed.\n";

			cout << "Equipment: "
				<< message.getEquipmentId() << "\n";

			cout << "Work Order: "
				<< message.getWorkOrderId() << "\n";

			cout << "Temperature: "
				<< message.getTemperature() << "\n";

			cout << "Production Count: "
				<< message.getProductionCount() << "\n";

			cout << "Defect Count: "
				<< message.getDefectCount() << "\n\n";
		}
	}

	return 0;
}