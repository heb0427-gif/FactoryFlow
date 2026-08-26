#include "TcpClient.h"
#include <iostream>
using namespace std;

TcpClient::TcpClient() : socket(ioContext) {}

bool TcpClient::connectToServer(
	const std::string& host, unsigned short port) {

	try {
		/*
		resolver는 host 이름을 실제 접속 가능한 IP 주소로 변환한다.

		예:
		altaria.proxy.rlwy.net
				↓
			DNS 조회
				↓
			실제 IP 주소
		*/

		boost::asio::ip::tcp::resolver resolver(ioContext);

		/*
		resolve()에
		1. 접속할 서버 주소
		2. 접속할 포트
		를 전달한다.

		port는 숫자이므로 문자열로 변환한다.
		*/
		auto endpoints = resolver.resolve(
			host,
			std::to_string(port)
		);

		/*
		DNS 조회 결과로 얻은 endpoint들 중
		연결 가능한 endpoint에 socket을 연결한다.
		*/
		boost::asio::connect(socket, endpoints);

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