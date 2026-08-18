#include <iostream>
#include <string>
#include "TcpServer.h"

using namespace std;

int main() {

	TcpServer server;

	cout << "========================================\n";
	cout << "FactoryFlow TCP Server Test\n";
	cout << "========================================\n\n";

	cout << "Waiting for client on port 9000...\n";

	// start() 내부의 accept()에서
	// EquipmentSimulator가 접속할 때까지 기다림
	if (!server.start(9000)) {
		cout << "Failed to start TCP server.\n";
		return 1;
	}

	cout << "Client connected.\n\n";

	// 같은 TCP 연결에서 JSON 3건 수신
	for (int i = 1; i <= 3; i++) {

		string message;

		if (!server.receiveMessage(message)) {
			cout << "Failed to receive message.\n";
			return 1;
		}

		cout << "[Received " << i << "]\n";
		cout << message << "\n\n";
	}

	cout << "All telemetry messages received.\n";

	return 0;
}