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

	// Railway / Docker 환경에서 cout 로그를 즉시 출력
	cout << unitbuf;


	// ========================================
	// 1. FactorySystem 준비
	// ========================================

	FactorySystem factorySystem;


	// ========================================
	// 2. Equipment 등록
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
	// 3. WorkOrder 생성
	// ========================================

	if (!factorySystem.createWorkOrder(
		"WO001",
		"PRODUCT-001",
		150,
		WorkOrderPriority::NORMAL
	) ||

		!factorySystem.createWorkOrder(
			"WO002",
			"PRODUCT-002",
			180,
			WorkOrderPriority::HIGH
		) ||

		!factorySystem.createWorkOrder(
			"WO003",
			"PRODUCT-003",
			200,
			WorkOrderPriority::HIGH
		) ||

		!factorySystem.createWorkOrder(
			"WO004",
			"PRODUCT-004",
			120,
			WorkOrderPriority::NORMAL
		) ||

		!factorySystem.createWorkOrder(
			"WO005",
			"PRODUCT-005",
			160,
			WorkOrderPriority::LOW
		)) {

		cout << "Failed to create work order.\n";
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
	//
	// WO001 ~ WO004만 시작한다.
	//
	// WO005는 startWorkOrder()를 호출하지 않으므로
	// READY 상태로 유지된다.
	// ========================================

	if (!factorySystem.startWorkOrder("WO001") ||
		!factorySystem.startWorkOrder("WO002") ||
		!factorySystem.startWorkOrder("WO003") ||
		!factorySystem.startWorkOrder("WO004")) {

		cout << "Failed to start work order.\n";
		return 1;
	}


	// ========================================
	// 6. LOT 등록
	// ========================================

	if (!factorySystem.registerLot(
		"LOT-001",
		"MATERIAL-STEEL",
		LotType::RAW_MATERIAL,
		500
	) ||

		!factorySystem.registerLot(
			"LOT-002",
			"MATERIAL-ALUMINUM",
			LotType::RAW_MATERIAL,
			350
		) ||

		!factorySystem.registerLot(
			"LOT-003",
			"MATERIAL-COMPONENT",
			LotType::RAW_MATERIAL,
			400
		) ||

		!factorySystem.registerLot(
			"LOT-101",
			"PRODUCT-001",
			LotType::FINISHED,
			150
		) ||

		!factorySystem.registerLot(
			"LOT-102",
			"PRODUCT-002",
			LotType::FINISHED,
			180
		) ||

		!factorySystem.registerLot(
			"LOT-103",
			"PRODUCT-003",
			LotType::FINISHED,
			200
		) ||

		!factorySystem.registerLot(
			"LOT-104",
			"PRODUCT-004",
			LotType::FINISHED,
			120
		) ||

		!factorySystem.registerLot(
			"LOT-105",
			"PRODUCT-005",
			LotType::FINISHED,
			160
		)) {

		cout << "Failed to register lot.\n";
		return 1;
	}


	// ========================================
	// 7. LOT 관계 등록
	// ========================================

	if (!factorySystem.addLotRelation(
		"LOT-001",
		"LOT-101",
		"WO001",
		120
	) ||

		!factorySystem.addLotRelation(
			"LOT-001",
			"LOT-102",
			"WO002",
			140
		) ||

		!factorySystem.addLotRelation(
			"LOT-002",
			"LOT-103",
			"WO003",
			160
		) ||

		!factorySystem.addLotRelation(
			"LOT-003",
			"LOT-104",
			"WO004",
			100
		) ||

		!factorySystem.addLotRelation(
			"LOT-003",
			"LOT-105",
			"WO005",
			130
		)) {

		cout << "Failed to create lot relation.\n";
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
	// 9. 초기 Equipment 상태 DB 저장
	// ========================================
	//
	// M05는 Telemetry를 보내지 않을 예정이므로
	// 여기서 미리 DB에 저장한다.
	//
	// 다른 설비도 현재 초기 상태를 저장한다.
	// ========================================

	const vector<Equipment>& equipments =
		factorySystem.getEquipments();

	for (int i = 0; i < equipments.size(); i++) {

		if (!databaseManager.upsertEquipment(equipments[i])) {

			cout << "Failed to save initial equipment state.\n";
			return 1;
		}
	}


	// ========================================
	// 10. 초기 WorkOrder 상태 DB 저장
	// ========================================
	//
	// WO005가 READY 상태인 것도 여기서 DB에 저장된다.
	// ========================================

	const vector<WorkOrder>& workOrders =
		factorySystem.getWorkOrders();

	for (int i = 0; i < workOrders.size(); i++) {

		if (!databaseManager.upsertWorkOrder(workOrders[i])) {

			cout << "Failed to save initial work order state.\n";
			return 1;
		}
	}


	// ========================================
	// 11. TelemetryProcessor 생성
	// ========================================

	TelemetryProcessor processor(
		factorySystem,
		databaseManager
	);


	// ========================================
	// 12. TelemetryParser 생성
	// ========================================

	TelemetryParser parser;


	// ========================================
	// 13. TCP 서버 생성
	// ========================================

	TcpServer server;


	// ========================================
	// 14. TCP_PORT 환경변수 읽기
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

		tcpPort =
			static_cast<unsigned short>(port);
	}
	catch (const exception&) {

		cout << "Invalid TCP_PORT.\n";
		return 1;
	}


	// ========================================
	// 15. TCP 서버 시작
	// ========================================

	if (!server.start(tcpPort)) {

		cout << "Failed to start TCP server.\n";
		return 1;
	}

	cout << "\nFactoryFlow TCP server started on port "
		<< tcpPort
		<< ".\n";


	// ========================================
	// 16. 클라이언트 연결 반복 처리
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
		// 17. 현재 클라이언트 메시지 처리
		// ========================================

		while (true) {

			string jsonText;


			// ----------------------------------------
			// 17-1. TCP JSON 수신
			// ----------------------------------------

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
			// 17-2. JSON → TelemetryMessage
			// ----------------------------------------

			TelemetryMessage message;

			if (!parser.parse(jsonText, message)) {

				cout << "Invalid telemetry message.\n\n";
				continue;
			}


			// ----------------------------------------
			// 17-3. FactoryFlow 처리
			// ----------------------------------------

			if (!processor.process(message)) {

				cout << "Failed to process telemetry.\n\n";
				continue;
			}


			cout << "Telemetry processed.\n";

			cout << "Equipment: "
				<< message.getEquipmentId()
				<< "\n";

			cout << "Work Order: "
				<< message.getWorkOrderId()
				<< "\n";

			cout << "Temperature: "
				<< message.getTemperature()
				<< "\n";

			cout << "Production Count: "
				<< message.getProductionCount()
				<< "\n";

			cout << "Defect Count: "
				<< message.getDefectCount()
				<< "\n\n";


			// ========================================
			// 18. 데모 상태 변경
			// ========================================


			// ----------------------------------------
			// WO002
			// 두 번째 Telemetry 이후 일시정지
			//
			// WorkOrder : RUNNING → PAUSED
			// Equipment : RUNNING → PAUSED
			// ----------------------------------------

			if (message.getWorkOrderId() == "WO002" &&
				message.getTimestamp() ==
				"2026-08-27T16:03:00") {

				if (!factorySystem.pauseWorkOrder("WO002")) {

					cout << "Failed to pause WO002.\n";
					continue;
				}

				Equipment* equipment =
					factorySystem.findEquipment("M02");

				WorkOrder* workOrder =
					factorySystem.findWorkOrder("WO002");

				if (equipment == nullptr ||
					workOrder == nullptr) {

					cout << "Failed to find WO002 demo data.\n";
					continue;
				}

				if (!databaseManager.upsertEquipment(*equipment) ||
					!databaseManager.upsertWorkOrder(*workOrder)) {

					cout << "Failed to save paused state.\n";
					continue;
				}

				cout << "WO002 paused.\n\n";
			}


			// ----------------------------------------
			// WO003
			// 두 번째 Telemetry 이후 작업 취소
			//
			// WorkOrder : RUNNING → CANCELLED
			// Equipment : RUNNING → STOPPED
			// ----------------------------------------

			if (message.getWorkOrderId() == "WO003" &&
				message.getTimestamp() ==
				"2026-08-27T16:05:00") {

				if (!factorySystem.cancelWorkOrder("WO003")) {

					cout << "Failed to cancel WO003.\n";
					continue;
				}

				Equipment* equipment =
					factorySystem.findEquipment("M03");

				WorkOrder* workOrder =
					factorySystem.findWorkOrder("WO003");

				if (equipment == nullptr ||
					workOrder == nullptr) {

					cout << "Failed to find WO003 demo data.\n";
					continue;
				}

				if (!databaseManager.upsertEquipment(*equipment) ||
					!databaseManager.upsertWorkOrder(*workOrder)) {

					cout << "Failed to save cancelled state.\n";
					continue;
				}

				cout << "WO003 cancelled.\n\n";
			}
		}
	}

	return 0;
}