#include <crow.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "DatabaseManager.h"

using namespace std;

int main() {

	// ========================================
	// 1. PostgreSQL 연결
	// ========================================

	DatabaseManager databaseManager;

	char* dbPassword = nullptr;
	size_t passwordLength = 0;

	_dupenv_s(
		&dbPassword,
		&passwordLength,
		"FACTORYFLOW_DB_PASSWORD"
	);

	if (dbPassword == nullptr) {
		cout << "Database password environment variable not found.\n";
		return 1;
	}

	string connectionString =
		"host=127.0.0.1 "
		"port=5432 "
		"dbname=factoryflow "
		"user=postgres "
		"password=" + string(dbPassword);

	free(dbPassword);

	if (!databaseManager.connect(connectionString)) {
		cout << "Database connection failed.\n";
		return 1;
	}

	cout << "Database connected.\n";


	// ========================================
	// 2. Crow HTTP 서버 생성
	// ========================================

	crow::SimpleApp app;


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

	cout << "FactoryFlow API running on port 8080...\n";

	app.port(8080).run();

	return 0;
}