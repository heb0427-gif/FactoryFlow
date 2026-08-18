#include "TelemetryParser.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

bool TelemetryParser::parse(const std::string& jsonText,
	TelemetryMessage& message) const {

	json data; // json 데이터를 담을 객체를 하나 만듦
	// string 타입의 jsonText를 실제 JSON으로 파싱해야 함

	try {
		// jsonText를 JSON으로 분석
		data = json::parse(jsonText); // JSON 라이브러리가 이미 만들어놓은 함수

		// 존재 여부 검사
		if (!data.contains("equipmentId")) return false;
		if (!data.contains("workOrderId")) return false;
		if (!data.contains("timestamp")) return false;
		if (!data.contains("status")) return false;
		if (!data.contains("temperature")) return false;
		if (!data.contains("vibration")) return false;
		if (!data.contains("productionCount")) return false;
		if (!data.contains("defectCount")) return false;
		if (!data.contains("lotId")) return false;

		// 자료형 검사
		if (!data["equipmentId"].is_string()) return false;
		if (!data["workOrderId"].is_string()) return false;
		if (!data["timestamp"].is_string()) return false;
		if (!data["status"].is_string()) return false;
		if (!data["temperature"].is_number()) return false;
		if (!data["vibration"].is_number()) return false;
		if (!data["productionCount"].is_number_integer()) return false;
		if (!data["defectCount"].is_number_integer()) return false;
		if (!data["lotId"].is_string()) return false;

		// status 값 자체를 검사
		string status = data["status"].get<string>();

		if (status != "STOPPED" &&
			status != "RUNNING" &&
			status != "PAUSED" &&
			status != "ERROR") {
			return false;
		}

		// 값 추출
		string equipmentId = data["equipmentId"].get<string>();
		string workOrderId = data["workOrderId"].get<string>();
		string timestamp = data["timestamp"].get<string>();

		double temperature = data["temperature"].get<double>();
		double vibration = data["vibration"].get<double>();

		int productionCount = data["productionCount"].get<int>();
		int defectCount = data["defectCount"].get<int>();

		string lotId = data["lotId"].get<string>();

		message = TelemetryMessage(
			equipmentId,
			workOrderId,
			timestamp,
			status,
			temperature,
			vibration,
			productionCount,
			defectCount,
			lotId
		);

		return true;
	}
	
	catch (...) {
		return false;
	}
}

std::string TelemetryParser::toJson(const TelemetryMessage& message) const {
	// C++ TelemetryMessage를 TCP로 보낼 JSON 문자열로 바꾼다.

	json data; // 비어있음
	data["equipmentId"] = message.getEquipmentId();
	data["workOrderId"] = message.getWorkOrderId();
	data["timestamp"] = message.getTimestamp();
	data["status"] = message.getStatus();
	data["temperature"] = message.getTemperature();
	data["vibration"] = message.getVibration();
	data["productionCount"] = message.getProductionCount();
	data["defectCount"] = message.getDefectCount();
	data["lotId"] = message.getLotId();

	return data.dump(); 
	// dump()로 JSON 객체를 JSON 형식의 문자열로 직렬화해서 반환
}