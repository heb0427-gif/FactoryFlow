#define _CRT_SECURE_NO_WARNINGS
#include <crow.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <crow/middlewares/cors.h>

#include "DatabaseManager.h"

using namespace std;

int main() {

	// ========================================
	// 1. PostgreSQL 연결
	// ========================================

	DatabaseManager databaseManager;

	const char* databaseUrl = std::getenv("DATABASE_URL");

	if (databaseUrl == nullptr) {
		cout << "DATABASE_URL environment variable not found.\n";
		return 1;
	}

	string connectionString = databaseUrl;

	if (!databaseManager.connect(connectionString)) {
		cout << "Database connection failed.\n";
		return 1;
	}

	cout << "Database connected.\n";


	// ========================================
	// PORT 환경변수 읽기
	// ========================================

	const char* portText = std::getenv("PORT");

	if (portText == nullptr) {
		cout << "PORT environment variable not found.\n";
		return 1;
	}

	unsigned short port;

	try {
		int portNumber = stoi(portText);

		if (portNumber < 1 || portNumber > 65535) {
			cout << "Invalid PORT.\n";
			return 1;
		}

		port = static_cast<unsigned short>(portNumber);
	}
	catch (const exception&) {
		cout << "Invalid PORT.\n";
		return 1;
	}

	// ========================================
	// 2. Crow HTTP 서버 생성
	// ========================================

	crow::App<crow::CORSHandler> app;

	auto& cors = app.get_middleware<crow::CORSHandler>();

	cors
		.global()
		.origin("http://localhost:5173");


	// ========================================
	// 3. GET /api/health
	// ========================================

	CROW_ROUTE(app, "/api/health")
		([]() {
		return R"({"status":"ok"})";
			});


	// ========================================
	// 4. GET /api/equipments
	// ========================================

	CROW_ROUTE(app, "/api/equipments")
		([&databaseManager]() {

		try {
			string json =
				databaseManager.getEquipmentsJson();

			crow::response response(200, json);

			response.set_header(
				"Content-Type",
				"application/json"
			);

			return response;
		}
		catch (const exception& e) {
			cout << "GET /api/equipments Error: "
				<< e.what() << "\n";

			return crow::response(
				500,
				R"({"error":"Database error"})"
			);
		}
			});


	// ========================================
	// 5. GET /api/work-orders
	// ========================================

	CROW_ROUTE(app, "/api/work-orders")
		([&databaseManager]() {

		try {
			string json =
				databaseManager.getWorkOrdersJson();

			crow::response response(200, json);

			response.set_header(
				"Content-Type",
				"application/json"
			);

			return response;
		}
		catch (const exception& e) {
			cout << "GET /api/work-orders Error: "
				<< e.what() << "\n";

			return crow::response(
				500,
				R"({"error":"Database error"})"
			);
		}
			});


	// ========================================
	// 6. GET /api/lots
	// ========================================

	CROW_ROUTE(app, "/api/lots")
		([&databaseManager]() {

		try {
			string json =
				databaseManager.getLotsJson();

			crow::response response(200, json);

			response.set_header(
				"Content-Type",
				"application/json"
			);

			return response;
		}
		catch (const exception& e) {
			cout << "GET /api/lots Error: "
				<< e.what() << "\n";

			return crow::response(
				500,
				R"({"error":"Database error"})"
			);
		}
			});


	// ========================================
	// 7. GET /api/alarms
	// ========================================

	CROW_ROUTE(app, "/api/alarms")
		([&databaseManager]() {

		try {
			string json =
				databaseManager.getAlarmsJson();

			crow::response response(200, json);

			response.set_header(
				"Content-Type",
				"application/json"
			);

			return response;
		}
		catch (const exception& e) {
			cout << "GET /api/alarms Error: "
				<< e.what() << "\n";

			return crow::response(
				500,
				R"({"error":"Database error"})"
			);
		}
			});


	// ========================================
	// 8. GET /api/summary
	// ========================================

	CROW_ROUTE(app, "/api/summary")
		([&databaseManager]() {

		try {
			string json =
				databaseManager.getSummaryJson();

			crow::response response(200, json);

			response.set_header(
				"Content-Type",
				"application/json"
			);

			return response;
		}
		catch (const exception& e) {
			cout << "GET /api/summary Error: "
				<< e.what() << "\n";

			return crow::response(
				500,
				R"({"error":"Database error"})"
			);
		}
			});


	// ========================================
	// 9. HTTP 서버 실행
	// ========================================

	cout << "FactoryFlow API running on port " << port << "...\n";

	app.port(port).run();

	return 0;
}