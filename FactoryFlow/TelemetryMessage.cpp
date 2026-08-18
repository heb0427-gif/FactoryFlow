#include "TelemetryMessage.h"
using namespace std;

TelemetryMessage::TelemetryMessage(
	const std::string& equipmentId,
	const std::string& workOrderId,
	const std::string& timestamp,
	const std::string& status,
	double temperature,
	double vibration,
	int productionCount,
	int defectCount,
	const std::string& lotId)
	: equipmentId(equipmentId),
	workOrderId(workOrderId),
	timestamp(timestamp),
	status(status),
	temperature(temperature),
	vibration(vibration),
	productionCount(productionCount),
	defectCount(defectCount),
	lotId(lotId) {
}

TelemetryMessage::TelemetryMessage()
	: temperature(0.0),
	vibration(0.0),
	productionCount(0),
	defectCount(0) {
}

const string& TelemetryMessage::getEquipmentId() const {
	return equipmentId;
}

const std::string& TelemetryMessage::getWorkOrderId() const {
	return workOrderId;
}

const std::string& TelemetryMessage::getTimestamp() const {
	return timestamp;
}

const std::string& TelemetryMessage::getStatus() const {
	return status;
}

double TelemetryMessage::getTemperature() const {
	return temperature;
}

double TelemetryMessage::getVibration() const {
	return vibration;
}

int TelemetryMessage::getProductionCount() const {
	return productionCount;
}

int TelemetryMessage::getDefectCount() const {
	return defectCount;
}

const std::string& TelemetryMessage::getLotId() const {
	return lotId;
}