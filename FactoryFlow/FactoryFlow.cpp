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

	// 환경변수에서 PostgreSQL 비밀번호 읽기
	
	const char* databaseUrl = std::getenv("DATABASE_URL");

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


	// ========================================
	// TCP_PORT 환경변수 읽기
	// ========================================

	const char* tcpPortText = std::getenv("TCP_PORT");

	if (tcpPortText == nullptr) {
		cout << "TCP_PORT environment variable not found.\n";
		return 1;
	}


	// 문자열 → 숫자 변환
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
	// TCP 서버 시작
	// ========================================

	if (!server.start(tcpPort)) {
		cout << "Failed to start TCP server.\n";
		return 1;
	}

	cout << "\nFactoryFlow TCP server started on port "
		<< tcpPort << ".\n";


	// ========================================
	// 11. 클라이언트 연결 반복 처리
	// ========================================

	int messageCount = 0;

	while (true) {

		cout << "\nWaiting for client...\n";

		// 새로운 EquipmentSimulator 접속 기다리기
		if (!server.waitForClient()) {
			cout << "Failed to accept client.\n";
			continue;
		}

		cout << "Client connected.\n\n";


		// ========================================
		// 12. 현재 클라이언트의 메시지 계속 처리
		// ========================================

		while (true) {

			string jsonText;

			// ----------------------------------------
			// 12-1. TCP JSON 수신
			// ----------------------------------------

			if (!server.receiveMessage(jsonText)) {

				cout << "Waiting for next client...\n";

				// 현재 Simulator가 종료된 것이므로
				// 안쪽 while만 종료
				break;
			}

			messageCount++;

			cout << "[Received "
				<< messageCount
				<< "]\n";

			cout << jsonText << "\n";


			// ----------------------------------------
			// 12-2. JSON → TelemetryMessage
			// ----------------------------------------

			TelemetryMessage message;

			if (!parser.parse(jsonText, message)) {
				cout << "Invalid telemetry message.\n\n";
				continue;
			}


			// ----------------------------------------
			// 12-3. 실제 FactoryFlow 처리
			// ----------------------------------------

			if (!processor.process(message)) {
				cout << "Failed to process telemetry.\n\n";
				continue;
			}


			// ----------------------------------------
			// 12-4. 처리 성공
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