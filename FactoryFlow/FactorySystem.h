#pragma once
#include "Equipment.h"
#include "WorkOrder.h"
#include <string>
#include <vector>

class FactorySystem {  // 공장에 등록된 전체 설비 목록 관리
private:
	std::vector<Equipment> equipments; // 설비들을 저장하는 벡터
	int findEquipmentIndex(const std::string& id) const;
	std::vector<WorkOrder> workOrders; // 작업 지시들을 저장하는 벡터
	int findWorkOrderIndex ( const std::string& id ) const;

public:
	// 새로운 설비 등록 함수
	bool registerEquipment(const std::string& id, const std::string& name);
	const std::vector<Equipment>& getEquipments() const;
	const Equipment* findEquipment(const std::string& id) const;  // 해당 id를 가진 객체의 주소 반환
	bool changeEquipmentStatus(const std::string& id, EquipmentStatus nextStatus);
	bool isEquipmentEmpty() const;

	bool createWorkOrder ( const std::string& id , 
		const std::string& productCode , int& targetQuantity , 
		WorkOrderPriority& priority );

	const std::vector<WorkOrder>& getWorkOrders ( ) const;
	const WorkOrder* findWorkOrders (const std::string& id) const;

};