#include "TcpServer.h"
#include <iostream>
using namespace std;

// Boost라는 라이브러리를 사용함(Boost.asio) Asio = 네트워크 I/O 작업을 편하게 해주는 라이브러리.(파일 읽고 쓰기, 네트워크 송수신 등)

TcpServer::TcpServer() : acceptor(ioContext), socket(ioContext) {}

bool TcpServer::start(unsigned short port) {
	// 지정한 포트에서 TCP 서버 시작

	try {
		// 1. 사용할 port 정보를 가진 TCP IPv4 endpoint 생성
		boost::asio::ip::tcp::endpoint endpoint(
			boost::asio::ip::tcp::v4(), port);

		// 2. acceptor를 endpoint와 같은 프로토콜(TCP IPv4)로 열음
		acceptor.open(endpoint.protocol());

		// 3. acceptor를 endpoint에 bind(묶음. 포트 연결)
		acceptor.bind(endpoint);

		// 여기까지는 서버가 어느 포트를 사용할지까지만 정해짐

		// 4. 클라이언트 연결 요청을 받을 수 있도록 대기 상태로 전환
		acceptor.listen();

		// 클라이언트가 접속할 때까지 기다리고, 연결이 성립되면 socket이 해당 클라이언트와 연결된 상태가 됨
		acceptor.accept(socket);
		// 이제 socket이 EquipmentSimulator와 실제로 연결된 상태임

		return true;
	}

	catch (const boost::system::system_error& e) {
		cout << "TCP Server Error: " << e.what() << "\n";
		return false;
	}
}

bool TcpServer::receiveMessage(string& message) {
	// 연결된 socket에서 데이터를 읽는 역할

	try {
		// 수정:
		// 멤버 receiveBuffer를 사용하므로
		// 다음 메시지까지 미리 읽힌 데이터가 사라지지 않음

		// 2. socket에서 '\n'이 나올 때까지 읽어서 buffer에 저장
		boost::asio::read_until(socket, receiveBuffer, '\n');

		// 3. buffer를 입력 스트림으로 연결
		istream inputStream(&receiveBuffer); // 버퍼는 string이 아니라서..?

		// 4. 한 줄을 message에 저장
		getline(inputStream, message);

		return true;
	}

	catch (const boost::system::system_error& e) {
		cout << "TCP Receive Error: " << e.what() << "\n";
		return false;
	}
}