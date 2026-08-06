#pragma once
#include <string>

enum class WorkOrderStatus {
	WAITING,
	READY,
	RUNNING,
	PAUSED,
	COMPLETED,
	CANCELLED
};

enum class WorkOrderPriority {
	LOW, 
	NORMAL, 
	HIGH
};

class WorkOrder {
private :
	std::string id;
	std::string productCode;
	int targetQuantity;
	WorkOrderPriority priority;
	WorkOrderStatus status;
	std::string assignedEquipmentId;
	
	int producedQuantity; // 현재까지 생산한 전체 수량
	int defectQuantity;  // 전체 생산량 중 불량 수량

public :
	WorkOrder (const std::string& id , const std::string& productCode , int targetQuantity , WorkOrderPriority priority );
	const std::string& getId ( ) const;
	const std::string& getProductCode ( ) const;
	int getTargetQuantity ( ) const;
	WorkOrderPriority getPriority ( ) const;
	const std::string& getAssignedEquipmentId ( ) const;
	WorkOrderStatus getStatus ( ) const;
	bool hasAssignedEquipment ( ) const;

	bool assignEquipment (const std::string& equipmentId );
	bool start ( );
	bool pause ( );
	bool resume ( );
	bool complete ( );
	bool cancel ( );

	int getProducedQuantity() const; // 전체 생산 수량 반환
	int getDefectQuantity() const; // 불량 수량 반환
	int getPassQuantity() const; // 정상 수량 반환
	int getRemainingQuantity() const; // 남은 생산 수량 반환
	float getProgressRate() const; // 목표 수량 대비 생산 수량의 비율 백분율 반환
	float getDefectRate() const; // 전체 생산량 대비 불량 수량 비율 백분율 반환
	bool isTargetReached() const; // 생산 수량이 목표 수량과 같아졌는지 확인

	// 입력된 생산실적을 현재 작업에 반영할 수 있는지 검사. 실제 값은 변경하지 않는다.
	bool canRecordProduction(int produced, int defects) const;
	bool recordProduction(int produced, int defects);

};

std::string workOrderStatusToString ( WorkOrderStatus status );
std::string workOrderPriorityToString ( WorkOrderPriority priority );