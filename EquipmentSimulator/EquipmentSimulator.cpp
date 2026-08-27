#include <iostream>
#include <string>
#include <vector>
#include "TcpClient.h"

using namespace std;

int main() {

	TcpClient client;

	cout << "========================================\n";
	cout << "FactoryFlow Equipment Simulator\n";
	cout << "========================================\n\n";

	cout << "Connecting...\n";

	if (!client.connectToServer("altaria.proxy.rlwy.net", 53807)) {
		cout << "Failed to connect to server.\n";
		return 1;
	}

	cout << "Connected.\n\n";


	// ========================================
	// 데모 Telemetry 데이터
	// ========================================

	vector<string> telemetryMessages = {

		// M01 - CNC Machine
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-27T16:00:00","status":"RUNNING","temperature":72.4,"vibration":1.82,"productionCount":18,"defectCount":1,"lotId":"LOT-101"})",

		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-27T16:01:00","status":"RUNNING","temperature":74.1,"vibration":1.86,"productionCount":16,"defectCount":0,"lotId":"LOT-101"})",


		// M02 - Milling Machine
		R"({"equipmentId":"M02","workOrderId":"WO002","timestamp":"2026-08-27T16:02:00","status":"RUNNING","temperature":68.7,"vibration":1.43,"productionCount":22,"defectCount":1,"lotId":"LOT-102"})",

		R"({"equipmentId":"M02","workOrderId":"WO002","timestamp":"2026-08-27T16:03:00","status":"RUNNING","temperature":71.3,"vibration":1.51,"productionCount":20,"defectCount":0,"lotId":"LOT-102"})",


		// M03 - Assembly Robot
		R"({"equipmentId":"M03","workOrderId":"WO003","timestamp":"2026-08-27T16:04:00","status":"RUNNING","temperature":76.2,"vibration":1.12,"productionCount":25,"defectCount":1,"lotId":"LOT-103"})",

		// 고온 Alarm
		R"({"equipmentId":"M03","workOrderId":"WO003","timestamp":"2026-08-27T16:05:00","status":"RUNNING","temperature":94.8,"vibration":1.18,"productionCount":21,"defectCount":2,"lotId":"LOT-103"})",


		// M04 - Inspection Machine
		R"({"equipmentId":"M04","workOrderId":"WO004","timestamp":"2026-08-27T16:06:00","status":"RUNNING","temperature":64.5,"vibration":0.82,"productionCount":17,"defectCount":0,"lotId":"LOT-104"})",

		// 고온 Alarm
		R"({"equipmentId":"M04","workOrderId":"WO004","timestamp":"2026-08-27T16:07:00","status":"RUNNING","temperature":92.6,"vibration":0.91,"productionCount":15,"defectCount":1,"lotId":"LOT-104"})",


		// M05 - Packaging Machine
		R"({"equipmentId":"M05","workOrderId":"WO005","timestamp":"2026-08-27T16:08:00","status":"RUNNING","temperature":69.8,"vibration":0.74,"productionCount":20,"defectCount":0,"lotId":"LOT-105"})",

		// 고온 Alarm
		R"({"equipmentId":"M05","workOrderId":"WO005","timestamp":"2026-08-27T16:09:00","status":"RUNNING","temperature":97.1,"vibration":0.88,"productionCount":18,"defectCount":1,"lotId":"LOT-105"})",


		// 추가 고온 Alarm - M01
		R"({"equipmentId":"M01","workOrderId":"WO001","timestamp":"2026-08-27T16:10:00","status":"RUNNING","temperature":91.4,"vibration":1.94,"productionCount":14,"defectCount":1,"lotId":"LOT-101"})"
	};


	// ========================================
	// Telemetry 순차 전송
	// ========================================

	for (int i = 0; i < telemetryMessages.size(); i++) {

		if (!client.sendMessage(telemetryMessages[i])) {

			cout << "Failed to send telemetry "
				<< i + 1
				<< ".\n";

			return 1;
		}

		cout << "["
			<< i + 1
			<< "] Telemetry sent.\n";
	}


	cout << "\n========================================\n";
	cout << "All demo telemetry messages sent.\n";
	cout << "========================================\n";

	return 0;
}