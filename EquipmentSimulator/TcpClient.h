#pragma once
#include <string>
#include <boost/asio.hpp>

class TcpClient {
private:
	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::socket socket;
public:
	// EquipmentSimulator에서 FactoryFlow 서버에 접속
	TcpClient(); // 생성자

	bool connectToServer(
		const std::string& host, unsigned short port);

	// JSON 한 건을 서버로 전송함
	bool sendMessage(const std::string& message);
};