#pragma once
#include <boost/asio.hpp>
#include <string>

class TcpServer {
private:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::acceptor acceptor; // start 함수 내 지역변수가 아닌 TcpServer의 멤버변수로 구현
	boost::asio::ip::tcp::socket socket; // 마찬가지

public:
	TcpServer(); // 생성자
	bool start(unsigned short port); // 지정한 포트에서 TCP 서버 시작
	bool receiveMessage(std::string& message);
};