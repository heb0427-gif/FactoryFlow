#pragma once
#include "TelemetryMessage.h"
#include "FactorySystem.h"
#include "DatabaseManager.h"

// 이 클래스가 TCP로 받은 데이터를 실제 FactoryFlow 업무 데이터로 처리함

class TelemetryProcessor {
private:
	FactorySystem& factorySystem;
	DatabaseManager& databaseManager;

	bool checkTemperatureAlarm(const TelemetryMessage& message) const;

public:
	TelemetryProcessor(FactorySystem& factorySystem,
		DatabaseManager& databaseManager);

	bool process(const TelemetryMessage& message);
	
};