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

	int totalProducedQuantity;  // 설비가 생산한 총 누적 수량
	int totalDefectQuantity;  // 설비에서 발생한 누적 불량 수량

public :
	Equipment(const std::string& id, const std::string& name); // 생성자
	const std::string& getId() const;  // id 출력
	const std::string& getName() const;  // name 출력
	EquipmentStatus getStatus() const;  // 레퍼런스 참조하면 원래 객체가 사라지면 프로그램 죽음
	bool changeStatus(EquipmentStatus nextStatus);  // status 변경

	int getTotalProducedQuantity() const; // 설비의 총 누적 수량 반환
	int getTotalDefectQuantity() const; // 설비에서 발생한 누적 불량 수량 반환
	int getTotalPassQuantity() const; // 설비에서 발생한 정상 수량 반환
	float getDefectRate() const; // 설비의 누적 불량률(0이면 0 반환)
	bool canRecordProduction(int produced, int defects) const; // 현재 설비가 해당 생산실적을 받을 수 있는지 검사(값은 변경 안 함)
	bool recordProduction(int produced, int defects); // 검증된 생산 수량과 불량 수량을 설비 누적 실적에 반영


};

std::string equipmentStatusToString(EquipmentStatus status);
// status를 string 타입으로 출력

