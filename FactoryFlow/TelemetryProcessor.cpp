#include "TelemetryProcessor.h"
using namespace std;

TelemetryProcessor::TelemetryProcessor(FactorySystem& factorySystem,
	DatabaseManager& databaseManager) 
	: factorySystem(factorySystem), databaseManager(databaseManager) {}

bool TelemetryProcessor::process(const TelemetryMessage& message) {
	// 수신된 TelemetryMessage를 FactoryFlow의 실제 업무 데이터로 처리

	// 1. 설비 찾기
	Equipment* equipment = factorySystem.findEquipment(message.getEquipmentId());

	// 2. 작업 지시 찾기
	WorkOrder* workOrder = factorySystem.findWorkOrder(message.getWorkOrderId());

	// 3. 존재하지 않으면 false
	if (equipment == nullptr || workOrder == nullptr) return false;

    // 4. ProductionEvent 생성/전달
	if (!factorySystem.enqueueProductionEvent( // Telemetry에 들어 있는 생산량/불량량을 기존 생산 이벤트 큐에 넣어라. 실패하면 전체 처리 실패
		message.getWorkOrderId(),
		message.getEquipmentId(),
		message.getProductionCount(),
		message.getDefectCount()
	)) {
		return false;
	}

    // 5. 기존 FactorySystem 생산 처리
	// 대기큐에 있던 이벤트 하나 꺼내서 처리 후(실적 반영 후) pop
	if (!factorySystem.processNextProductionEvent()) return false;

	// 7. equipment DB 갱신
	if (!databaseManager.upsertEquipment(*equipment)) {
		return false;
	}

	// 8. work_order DB 갱신
	if (!databaseManager.upsertWorkOrder(*workOrder)) {
		return false;
	}

	// 6. telemetry DB 저장
	if (!databaseManager.saveTelemetry(message)) return false;

	// 9. 온도 검사
	if (checkTemperatureAlarm(message)) { // 90도 이상이면

		// 10. 알람 저장
		if (!databaseManager.saveAlarm(message,
			"HIGH TEMPERATURE")) return false;
	}

	return true;

}

bool TelemetryProcessor::checkTemperatureAlarm(const TelemetryMessage& message) const {

	return message.getTemperature() >= 90.0;
}

