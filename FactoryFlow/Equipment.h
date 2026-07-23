#pragma once
#include <string>

enum class EquipmentStatus {
	STOPPED,
	RUNNING,
	PAUSED,
	ERROR
};

class Equipment {
private :
	std::string id;
	std::string name;
	EquipmentStatus status;
	bool isValidStatusTransition(EquipmentStatus nextStatus) const;

public :
	Equipment(const std::string& id, const std::string& name); // 생성자
	const std::string& getId() const;  // id 출력
	const std::string& getName() const;  // name 출력
	EquipmentStatus getStatus() const;  // 레퍼런스 참조하면 원래 객체가 사라지면 프로그램 죽음
	bool changeStatus(EquipmentStatus nextStatus);  // status 변경
};

std::string equipmentStatusToString(EquipmentStatus status);
// status를 string 타입으로 출력