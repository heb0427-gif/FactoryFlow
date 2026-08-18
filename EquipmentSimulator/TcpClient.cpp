#include "TcpClient.h"
#include <iostream>
using namespace std;

TcpClient::TcpClient() : socket(ioContext) {}

bool TcpClient::connectToServer(
	const std::string& host, unsigned short port) {

	try {
		/* make_address()가 host 문자열을 boost가 사용할 수 있는
		IP 주소 객체로 바꿔줌 */

		boost::asio::ip::address address = boost::asio::ip::make_address(host);

		// 내가 접속할 상대 위치 정보를 가진 TCP IPv4 endpoint 생성
		boost::asio::ip::tcp::endpoint endpoint(address, port);

		// 그럼 서버에서는 acceptor.accept(socket) 이 기다리고 있음.

		// 클라이언트와 서버의 실제 연결
		socket.connect(endpoint); // socket아, 이 endpoint에 있는 서버로 연결해

		// 연결 성공 후에는 클라이언트/서버 모두 자기 socket을 통해 데이터를 주고 받음

		return true;
	}

	catch (const boost::system::system_error& e) {
		cout << "TCP Client Error: " << e.what() << "\n";
		return false;
	}
}

bool TcpClient::sendMessage(const std::string& message) {
	// message를 소켓으로 서버에 보내야 함

	try {
		string data = message + '\n';

		// 이제 data를 소켓으로 보내야 함

		boost::asio::write(socket, boost::asio::buffer(data));

		return true;
	}

	catch (const boost::system::system_error& e) {
		cout << "TCP Send Error: " << e.what() << "\n";
		return false;
	}
}