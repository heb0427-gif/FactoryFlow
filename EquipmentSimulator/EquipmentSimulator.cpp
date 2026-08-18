#include <iostream>
#include <string>
#include "TcpClient.h"

using namespace std;

int main() {

	TcpClient client;

	cout << "========================================\n";
	cout << "Equipment Simulator TCP Test\n";
	cout << "========================================\n\n";

	cout << "Connecting...\n";

	if (!client.connectToServer("127.0.0.1", 9000)) {
		cout << "Failed to connect to server.\n";
		return 1;
	}

	cout << "Connected.\n\n";


	// ========================================
	// 하드코딩된 Telemetry JSON 3건
	// ========================================

	string json1 =
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-18T18:00:00","status":"RUNNING","temperature":72.4,"vibration":1.82,"productionCount":5,"defectCount":1,"lotId":"LOT-001"})";

	string json2 =
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-18T18:00:01","status":"RUNNING","temperature":73.1,"vibration":1.85,"productionCount":5,"defectCount":0,"lotId":"LOT-001"})";

	string json3 =
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-18T18:00:02","status":"RUNNING","temperature":72.8,"vibration":1.79,"productionCount":5,"defectCount":1,"lotId":"LOT-001"})";


	// ========================================
	// 같은 연결에서 JSON 3건 연속 전송
	// ========================================

	if (!client.sendMessage(json1)) {
		cout << "Failed to send telemetry 1.\n";
		return 1;
	}

	cout << "Telemetry sent.\n";


	if (!client.sendMessage(json2)) {
		cout << "Failed to send telemetry 2.\n";
		return 1;
	}

	cout << "Telemetry sent.\n";


	if (!client.sendMessage(json3)) {
		cout << "Failed to send telemetry 3.\n";
		return 1;
	}

	cout << "Telemetry sent.\n";

	return 0;
}