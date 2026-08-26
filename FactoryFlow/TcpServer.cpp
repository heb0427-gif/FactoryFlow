#include "TcpServer.h"
#include <iostream>
using namespace std;

// Boost라는 라이브러리를 사용함(Boost.asio) Asio = 네트워크 I/O 작업을 편하게 해주는 라이브러리.(파일 읽고 쓰기, 네트워크 송수신 등)

TcpServer::TcpServer() : acceptor(ioContext), socket(ioContext) {}

bool TcpServer::start(unsigned short port) { // 9000번 포트에서 서버를 열어두는 것만 함(클라이언트와 연결하지 않음)
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

		// 이제 socket이 EquipmentSimulator와 실제로 연결된 상태임

		return true;
	}

	catch (const boost::system::system_error& e) {
		cout << "TCP Server Error: " << e.what() << "\n";
		return false;
	}
}

bool TcpServer::receiveMessage(string& message) {

	try {
		// '\n'까지 한 메시지 수신
		boost::asio::read_until(
			socket,
			receiveBuffer,
			'\n'
		);

		// receiveBuffer를 입력 스트림으로 연결
		istream inputStream(&receiveBuffer);

		// 한 줄을 message에 저장
		getline(inputStream, message);

		return true;
	}

	catch (const boost::system::system_error& e) {

		cout << "Client disconnected: "
			<< e.what() << "\n";

		if (socket.is_open()) {
			socket.close();
		}

		return false; // 현재 Simulator와의 연결이 끝남
	}
}

bool TcpServer::waitForClient() {
	try {
		// 이전 클라이언트 socket이 남아 있으면 정리
		if (socket.is_open()) socket.close();

		// 이전 연결에서 buffer에 남은 데이터 제거
		receiveBuffer.consume(receiveBuffer.size());

		// 새로운 클라이언트가 접속할 때까지 대기
		acceptor.accept(socket);

		return true;
	}

	catch (const boost::system::system_error& e) {
		cout << "TCP Accept Error: " << e.what() << "\n";
		return false;
	}
}