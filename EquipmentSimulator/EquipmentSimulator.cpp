#include <iostream>
#include <string>
#include "TcpClient.h"

using namespace std;

int main() {

	TcpClient client;

	cout << "========================================\n";
	cout << "Equipment Simulator Integration Test\n";
	cout << "========================================\n\n";

	cout << "Connecting...\n";

	if (!client.connectToServer("altaria.proxy.rlwy.net", 53807)) {
		cout << "Failed to connect to server.\n";
		return 1;
	}

	cout << "Connected.\n\n";


	// ========================================
	// 1. 정상 Telemetry
	// ========================================

	string normalJson1 =
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-26T14:00:00","status":"RUNNING","temperature":72.4,"vibration":1.82,"productionCount":5,"defectCount":1,"lotId":"LOT-001"})";


	// ========================================
	// 2. 잘못된 JSON
	// ========================================

	string invalidJson =
		R"({ ABC)";


	// ========================================
	// 3. 존재하지 않는 설비
	// ========================================

	string unknownEquipmentJson =
		R"({"equipmentId":"M999","workOrderId":"WO001","timestamp":"2026-08-26T14:00:01","status":"RUNNING","temperature":73.1,"vibration":1.85,"productionCount":5,"defectCount":0,"lotId":"LOT-001"})";


	// ========================================
	// 4. 다시 정상 Telemetry
	// ========================================

	string normalJson2 =
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-26T14:00:02","status":"RUNNING","temperature":74.0,"vibration":1.80,"productionCount":5,"defectCount":0,"lotId":"LOT-001"})";


	// ========================================
	// 5. 고온 Telemetry
	// ========================================

	string highTemperatureJson =
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-26T14:00:03","status":"RUNNING","temperature":95.2,"vibration":1.79,"productionCount":5,"defectCount":1,"lotId":"LOT-001"})";


	// ========================================
	// 메시지 전송
	// ========================================

	if (!client.sendMessage(normalJson1)) {
		cout << "Failed to send normal telemetry 1.\n";
		return 1;
	}

	cout << "[1] Normal telemetry sent.\n";


	if (!client.sendMessage(invalidJson)) {
		cout << "Failed to send invalid JSON.\n";
		return 1;
	}

	cout << "[2] Invalid JSON sent.\n";


	if (!client.sendMessage(unknownEquipmentJson)) {
		cout << "Failed to send unknown equipment telemetry.\n";
		return 1;
	}

	cout << "[3] Unknown equipment telemetry sent.\n";


	if (!client.sendMessage(normalJson2)) {
		cout << "Failed to send normal telemetry 2.\n";
		return 1;
	}

	cout << "[4] Normal telemetry sent.\n";


	if (!client.sendMessage(highTemperatureJson)) {
		cout << "Failed to send high temperature telemetry.\n";
		return 1;
	}

	cout << "[5] High temperature telemetry sent.\n";


	cout << "\nAll integration test messages sent.\n";

	return 0;
}