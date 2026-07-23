#include <iostream>
#include "Equipment.h"
using namespace std;

// 생성자 정의
Equipment::Equipment(const string& id, const string& name) 
	: id(id), name(name), status(EquipmentStatus::STOPPED) { }  // 스탑 상태로 초기화

// Getter 메서드 정의
const string& Equipment::getId() const {
	return id;
}

const string& Equipment::getName() const {
	return name;
}

EquipmentStatus Equipment::getStatus() const {
	return status;
}

bool Equipment::changeStatus(EquipmentStatus nextStatus) {
	if (status == nextStatus) return false;
	if ((status == EquipmentStatus::ERROR) && (nextStatus == EquipmentStatus::RUNNING))
		return false; // ERROR -> RUNNING 불가
	status = nextStatus;
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