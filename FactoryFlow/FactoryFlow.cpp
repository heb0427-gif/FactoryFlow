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

	// ========================================
	// 1. 기존 FactorySystem 준비
	// ========================================

	FactorySystem factorySystem;


	// ========================================
	// 2. 테스트용 Equipment 등록
	// ========================================

	if (!factorySystem.registerEquipment(
		"M01",
		"CNC Machine"
	)) {
		cout << "Failed to register equipment.\n";
		return 1;
	}


	// ========================================
	// 3. 테스트용 WorkOrder 생성
	// ========================================

	if (!factorySystem.createWorkOrder(
		"WO001",
		"PRODUCT-001",
		100,
		WorkOrderPriority::NORMAL
	)) {
		cout << "Failed to create work order.\n";
		return 1;
	}


	// ========================================
	// 4. WorkOrder에 Equipment 배정
	// ========================================

	if (!factorySystem.assignEquipmentToWorkOrder(
		"WO001",
		"M01"
	)) {
		cout << "Failed to assign equipment.\n";
		return 1;
	}


	// ========================================
	// 5. WorkOrder 시작
	// ========================================

	if (!factorySystem.startWorkOrder("WO001")) {
		cout << "Failed to start work order.\n";
		return 1;
	}

	cout << "FactorySystem initialized.\n";


	// ========================================
	// 6. PostgreSQL 연결
	// ========================================

	DatabaseManager databaseManager;

	string connectionString =
		"host=127.0.0.1 "
		"port=5432 "
		"dbname=factoryflow "
		"user=postgres "
		"password=MY_PASSWORD";

	if (!databaseManager.connect(connectionString)) {
		cout << "Database connection failed.\n";
		return 1;
	}

	cout << "Database connected.\n";


	// ========================================
	// 7. TelemetryProcessor 생성
	// ========================================

	TelemetryProcessor processor(
		factorySystem,
		databaseManager
	);


	// ========================================
	// 8. TelemetryParser 생성
	// ========================================

	TelemetryParser parser;


	// ========================================
	// 9. TCP 서버 생성
	// ========================================

	TcpServer server;

	cout << "\nWaiting for client on port 9000...\n";


	// ========================================
	// 10. EquipmentSimulator 연결 기다리기
	// ========================================

	if (!server.start(9000)) {
		cout << "Failed to start TCP server.\n";
		return 1;
	}

	cout << "Client connected.\n\n";


	// ========================================
	// 11. 같은 TCP 연결에서 JSON 3건 처리
	// ========================================

	for (int i = 1; i <= 3; i++) {

		string jsonText;


		// ----------------------------------------
		// 11-1. TCP로 JSON 문자열 수신
		// ----------------------------------------

		if (!server.receiveMessage(jsonText)) {
			cout << "Failed to receive message.\n";
			return 1;
		}

		cout << "[Received " << i << "]\n";
		cout << jsonText << "\n";


		// ----------------------------------------
		// 11-2. JSON → TelemetryMessage 변환
		// ----------------------------------------

		TelemetryMessage message;

		if (!parser.parse(jsonText, message)) {
			cout << "Invalid telemetry message.\n\n";

			// 잘못된 메시지 하나 때문에
			// 서버 전체를 종료하지 않고 다음 메시지를 받음
			continue;
		}


		// ----------------------------------------
		// 11-3. TelemetryMessage 실제 처리
		// ----------------------------------------

		if (!processor.process(message)) {
			cout << "Failed to process telemetry.\n\n";

			// 처리 실패 역시 다음 메시지는 계속 받을 수 있게 함
			continue;
		}


		// ----------------------------------------
		// 11-4. 처리 성공
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


	// ========================================
	// 12. 테스트 완료
	// ========================================

	cout << "All telemetry messages processed.\n";

	return 0;
}