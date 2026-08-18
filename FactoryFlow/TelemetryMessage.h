#pragma once
#include <string>

// 외부 설비가 TCP로 보내온 메시지 자체를 표현하는 객체
// 외부에서 가상의 공장 설비가 보내는 JSON 데이터

class TelemetryMessage {
private:
	std::string equipmentId;
	std::string workOrderId;
	std::string timestamp;
	std::string status;
	double temperature;
	double vibration;
	int productionCount;
	int defectCount;
	std::string lotId;

public:
	TelemetryMessage();
	TelemetryMessage(
		const std::string& equipmentId,
		const std::string& workOrderId,
		const std::string& timestamp,
		const std::string& status,
		double temperature,
		double vibration,
		int productionCount,
		int defectCount,
		const std::string& lotId
	);

	const std::string& getEquipmentId() const;
	const std::string& getWorkOrderId() const;
	const std::string& getTimestamp() const;
	const std::string& getStatus() const;
	double getTemperature() const;
	double getVibration() const;
	int getProductionCount() const;
	int getDefectCount() const;
	const std::string& getLotId() const;
};