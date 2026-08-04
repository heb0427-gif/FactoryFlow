#include "WorkOrder.h"
#include <string>
using namespace std;

// 생성자
WorkOrder::WorkOrder ( const string& id , const string& productCode , int targetQuantity , WorkOrderPriority priority )
	: id ( id ) , productCode ( productCode ) , targetQuantity ( targetQuantity ) ,
	priority ( priority ) , status ( WorkOrderStatus::WAITING ) , 
	assignedEquipmentId ( "" ), producedQuantity(0), defectQuantity(0) {}

const string& WorkOrder::getId ( ) const { return id; }
const string& WorkOrder::getProductCode ( ) const { return productCode; }
int WorkOrder::getTargetQuantity ( ) const { return targetQuantity; }
WorkOrderPriority WorkOrder::getPriority ( ) const { return priority; }
const string& WorkOrder::getAssignedEquipmentId ( ) const { return assignedEquipmentId; }
WorkOrderStatus WorkOrder::getStatus ( ) const { return status; }
bool WorkOrder::hasAssignedEquipment ( ) const {
	return !assignedEquipmentId.empty();
}

bool WorkOrder::assignEquipment(const std::string& equipmentId) {
	if (status != WorkOrderStatus::WAITING) { // WAITING 상태에서만 설비 등록 가능
		return false; }

	if (equipmentId.empty()) return false;

	assignedEquipmentId = equipmentId;
	status = WorkOrderStatus::READY;
	return true;
}

bool WorkOrder::start ( ) {
	if ( status != WorkOrderStatus::READY ) { // READY 상태에서만 start 가능
		return false;
	}

	status = WorkOrderStatus::RUNNING;
	return true;
}

bool WorkOrder::pause ( ) {
	if ( status != WorkOrderStatus::RUNNING ) { // RUNNING 상태에서만 pause 가능
		return false;
	}

	status = WorkOrderStatus::PAUSED;
	return true;
}

bool WorkOrder::resume ( ) {
	if ( status != WorkOrderStatus::PAUSED ) { // PAUSED 상태에서만 resume 가능
		return false;
	}

	status = WorkOrderStatus::RUNNING;
	return true;
}

bool WorkOrder::complete ( ) {
	if ( status != WorkOrderStatus::RUNNING ) { // RUNNING 상태에서만 complete 가능
		return false;
	}

	if (!isTargetReached()) return false;

	status = WorkOrderStatus::COMPLETED;
	return true;
}

bool WorkOrder::cancel ( ) {
	if ((status == WorkOrderStatus::COMPLETED) || 
		(status == WorkOrderStatus::CANCELLED )) {
		return false;
	}

	status = WorkOrderStatus::CANCELLED;
	return true;
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

int WorkOrder::getProducedQuantity() const { 
	return producedQuantity; 
}

int WorkOrder::getDefectQuantity() const {
	return defectQuantity;
}

int WorkOrder::getPassQuantity() const {
	return producedQuantity - defectQuantity;
}

int WorkOrder::getRemainingQuantity() const {
	return targetQuantity - producedQuantity;
}

float WorkOrder::getProgressRate() const {
	// 목표 수량 대비 생산 수량의 비율 백분율 반환

	return (static_cast<float>(producedQuantity) / targetQuantity) * 100;
}

float WorkOrder::getDefectRate() const {
	// 전체 생산량 대비 불량 수량 비율 백분율 반환

	if (producedQuantity == 0) return 0.0;

	return (static_cast<float>(defectQuantity) / producedQuantity) * 100;
}

bool WorkOrder::isTargetReached() const {
	return producedQuantity == targetQuantity;
}

bool WorkOrder::canRecordProduction(int produced, int defects) const {
	if (status != WorkOrderStatus::RUNNING) return false;
	if (produced < 1) return false;
	if (defects < 0) return false;
	if (defects > produced) return false;
	if ((produced + producedQuantity) > targetQuantity) return false;

	return true;
}

bool WorkOrder::recordProduction(int produced, int defects) {
	if (!canRecordProduction(produced, defects)) {
		return false;
	}

	producedQuantity += produced;
	defectQuantity += defects;

	return true;
}