#include "WorkOrder.h"
#include <string>
using namespace std;

// 생성자
WorkOrder::WorkOrder ( const string& id , const string& productCode , int targetQuantity , WorkOrderPriority priority )
	: id ( id ) , productCode ( productCode ) , targetQuantity ( targetQuantity ) ,
	priority ( priority ) , status ( WorkOrderStatus::WAITING ) , assignedEquipmentId ( "" ) {}

const string& WorkOrder::getId ( ) const { return id; }
const string& WorkOrder::getProductCode ( ) const { return productCode; }
int WorkOrder::getTargetQuantity ( ) const { return targetQuantity; }
WorkOrderPriority WorkOrder::getPriority ( ) const { return priority; }
const string& WorkOrder::getAssignedEquipmentId ( ) const { return assignedEquipmentId; }
WorkOrderStatus WorkOrder::getStatus ( ) const { return status; }
bool WorkOrder::hasAssignedEquipment ( ) const {
	if ( !assignedEquipmentId.empty() ) return true;
	return false;
}

string workOrderStatusToString ( WorkOrderStatus status ) {
	switch ( status ) {
	case WorkOrderStatus::WAITING :
		return "WAITING";
	case WorkOrderStatus::READY :
		return "READY";
	case WorkOrderStatus::RUNNING :
		return "RUNNING";
	case WorkOrderStatus::PAUSED :
		return "PAUSED";
	case WorkOrderStatus::COMPLETED :
		return "COMPLETED";
	case WorkOrderStatus::CANCELLED :
		return "CANCELLED";
	default :
		return "UNKNOWN";
	}
}

string workOrderPriorityToString ( WorkOrderPriority priority ) {
	switch ( priority ) {
	case WorkOrderPriority::LOW:
		return "LOW";
	case WorkOrderPriority::NORMAL:
		return "NORMAL";
	case WorkOrderPriority::HIGH:
		return "HIGH";
	default:
		return "UNKNOWN";
	}
}