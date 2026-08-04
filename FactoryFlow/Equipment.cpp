#include "Equipment.h"
using namespace std;

// 생성자 정의
Equipment::Equipment(const string& id, const string& name) 
	: id(id), name(name), status(EquipmentStatus::STOPPED),
	totalProducedQuantity(0), totalDefectQuantity(0) { }  // 스탑 상태로 초기화

// Getter 메서드 정의
const string& Equipment::getId() const { return id; }
const string& Equipment::getName() const { return name; }
EquipmentStatus Equipment::getStatus() const { return status; }
bool Equipment::changeStatus(EquipmentStatus nextStatus) {
	if (!isValidStatusTransition(nextStatus)) return false;

	status = nextStatus;
	return true;
}

bool Equipment::isValidStatusTransition(EquipmentStatus nextStatus) const {
	if (status == nextStatus) return false; // 동일 상태로 변경 불가
	if ((status == EquipmentStatus::STOPPED) && (nextStatus == EquipmentStatus::PAUSED))
		return false; // STOPPED -> PAUSED 불가
	if ((status == EquipmentStatus::STOPPED) && (nextStatus == EquipmentStatus::ERROR))
		return false; // STOPPED -> ERROR 불가
	if ((status == EquipmentStatus::ERROR) && (nextStatus == EquipmentStatus::RUNNING))
		return false; // ERROR -> RUNNING 불가
	if ((status == EquipmentStatus::ERROR) && (nextStatus == EquipmentStatus::PAUSED))
		return false; // ERROR -> PAUSED 불가
	return true;
}

string equipmentStatusToString(EquipmentStatus status) {
	if (status == EquipmentStatus::STOPPED)
		return "STOPPED!";
	if (status == EquipmentStatus::RUNNING)
		return "RUNNING!";
	if (status == EquipmentStatus::PAUSED)
		return "PAUSED!";
	if (status == EquipmentStatus::ERROR)
		return "ERROR!";
	return "UNKNOWN!";
}

int Equipment::getTotalProducedQuantity() const {
	return totalProducedQuantity;
}

int Equipment::getTotalDefectQuantity() const {
	// 설비에서 발생한 누적 불량 수량 반환
	return totalDefectQuantity;
}

int Equipment::getTotalPassQuantity() const {
	// 설비에서 발생한 정상 수량 반환
	return totalProducedQuantity - totalDefectQuantity;
}

float Equipment::getDefectRate() const {
	// 설비의 누적 불량률(0이면 0 반환)

	if (totalProducedQuantity == 0) return 0.0;
	return (static_cast<float>(totalDefectQuantity) / totalProducedQuantity) * 100;
}

bool Equipment::canRecordProduction(int produced, int defects) const {
	// 현재 설비가 해당 생산실적을 받을 수 있는지 검사(값은 변경 안 함)

	if (status != EquipmentStatus::RUNNING) return false;
	if (produced < 1) return false;
	if (defects < 0) return false;
	if (defects > produced) return false;

	return true;
}

bool Equipment::recordProduction(int produced, int defects) {
	if (!canRecordProduction(produced, defects)) {
		return false;
	}

	totalProducedQuantity += produced;
	totalDefectQuantity += defects;

	return true;
}