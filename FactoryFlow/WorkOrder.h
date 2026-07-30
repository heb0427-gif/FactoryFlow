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

public :
	WorkOrder (const std::string& id , const std::string& productCode , int targetQuantity , WorkOrderPriority priority );
	const std::string& getId ( ) const;
	const std::string& getProductCode ( ) const;
	int getTargetQuantity ( ) const;
	WorkOrderPriority getPriority ( ) const;
	const std::string& getAssignedEquipmentId ( ) const;
	WorkOrderStatus getStatus ( ) const;
	bool hasAssignedEquipment ( ) const;
};

std::string workOrderStatusToString ( WorkOrderStatus status );
std::string workOrderPriorityToString ( WorkOrderPriority priority );