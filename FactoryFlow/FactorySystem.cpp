#include <vector>
#include <iostream>
#include "FactorySystem.h"
#include "WorkOrder.h"
using namespace std;

FactorySystem::FactorySystem() : nextProductionEventId(1) {}

int FactorySystem::findEquipmentIndex(const std::string& id) const {
	for (int i = 0; i < equipments.size(); i++) {
		if (equipments[i].getId() == id) return i;
	}
	return -1;
}

bool FactorySystem::rejectProductionEvent(const ProductionEvent& productionEvent) {
	rejectedProductionEvents.push_back(productionEvent);
	pendingProductionEvents.pop();
	return false;
}

int FactorySystem::findWorkOrderIndex ( const std::string& id ) const {
	for ( int i = 0; i < workOrders.size ( ); i++ ) {
		if ( workOrders[ i ].getId ( ) == id ) return i;
	}
	return -1;
}

bool FactorySystem::registerEquipment(const std::string& id, const std::string& name) {
	//equipments에 push_back 한다.

	if (id.empty()) return false; // id가 빈 문자열이면 false
	if (name.empty()) return false; // name이 빈 문자열이면 false
	if (findEquipmentIndex(id) != -1) return false; // 중복 등록 방지

	equipments.push_back(Equipment(id, name));

	return true;
}

const vector<Equipment>& FactorySystem::getEquipments() const {
	return equipments; // Equipment 객체들이 저장된 벡터를 읽기 전용 참조로 반환
}

bool FactorySystem::isEquipmentEmpty() const {return equipments.empty();}

const Equipment* FactorySystem::findEquipment(const std::string& id) const {
	int equipmentIndex = findEquipmentIndex(id);

	if (equipmentIndex == -1) return nullptr;
	return &equipments[equipmentIndex];  // 주소를 넘겨주기 위해서 &를 붙여줌
}

Equipment* FactorySystem::findEquipment(const std::string& id) {
	int equipmentIndex = findEquipmentIndex(id);

	if (equipmentIndex == -1) return nullptr;
	return &equipments[equipmentIndex];  // 주소를 넘겨주기 위해서 &를 붙여줌
}

bool FactorySystem::changeEquipmentStatus(const std::string& id, EquipmentStatus nextStatus) {
	int equipmentIndex = findEquipmentIndex(id);
	
	if (equipmentIndex == -1) return false;
	return equipments[equipmentIndex].changeStatus(nextStatus);
}

bool FactorySystem::createWorkOrder ( const std::string& id , 
	const std::string& productCode , int targetQuantity , 
	WorkOrderPriority priority ) {

	if ( ( id.empty() ) || ( productCode.empty() ) || ( targetQuantity <= 0 ) ||
		( findWorkOrderIndex(id) != -1 ) ) return false;

	workOrders.push_back(WorkOrder(id , productCode , targetQuantity, 
		priority));
	
	return true;
}

const vector<WorkOrder>& FactorySystem::getWorkOrders() const {
	return workOrders;
}

const WorkOrder* FactorySystem::findWorkOrder(const string& id) const {
	int workOrderIndex = findWorkOrderIndex (id);

	if ( workOrderIndex == -1 ) return nullptr;

	return &workOrders[workOrderIndex];
}

WorkOrder* FactorySystem::findWorkOrder(const string& id) {
	int workOrderIndex = findWorkOrderIndex(id);

	if (workOrderIndex == -1) return nullptr;

	return &workOrders[workOrderIndex];
}

bool FactorySystem::isWorkOrderEmpty ( ) const {
	return workOrders.empty();
}

bool FactorySystem::isEquipmentAssigned ( const std::string& equipmentId ) const { // 
	if (equipmentId.empty()) return false;

	for ( int i = 0; i < workOrders.size ( ); i++ ) {

		WorkOrderStatus status = workOrders[i].getStatus();

		if ( (equipmentId == workOrders[i].getAssignedEquipmentId()) &&
			((status == WorkOrderStatus::READY) || 
			(status == WorkOrderStatus::RUNNING)||
			((status == WorkOrderStatus::PAUSED)))) {

			return true;
		}
	}

	return false;
}

bool FactorySystem::assignEquipmentToWorkOrder(const std::string& workOrderId,
	const std::string& equipmentId) {

	// 전달된 작업지시 id와 설비 id가 공백
	if ((workOrderId.empty()) || (equipmentId.empty())) return false;

	WorkOrder* workOrder = findWorkOrder(workOrderId);
	const Equipment* equipment = findEquipment(equipmentId);

	if (workOrder == nullptr || equipment == nullptr ||
		workOrder->getStatus() != WorkOrderStatus::WAITING ||
		equipment->getStatus() != EquipmentStatus::STOPPED ||
		isEquipmentAssigned(equipmentId)) return false;

	return workOrder->assignEquipment(equipmentId);
}

bool FactorySystem::startWorkOrder(const std::string& workOrderId) {

	if (workOrderId.empty()) return false;

	WorkOrder* workOrder = findWorkOrder(workOrderId);

	if (workOrder == nullptr) return false;

	if (!workOrder->hasAssignedEquipment()) return false;

	if (workOrder->getStatus() != WorkOrderStatus::READY) return false;

	const string& assignedEquipmentId = workOrder->getAssignedEquipmentId();
	Equipment* equipment = findEquipment(assignedEquipmentId);

	if (equipment == nullptr) return false;

	if (equipment->getStatus() != EquipmentStatus::STOPPED) return false;

	// 설비 상태 변경
	if (!equipment->changeStatus(EquipmentStatus::RUNNING)) return false;

	if (!workOrder->start()) { // 작업 지시 상태 변경
		equipment->changeStatus(EquipmentStatus::STOPPED);
		return false;
	}

	return true;
}

bool FactorySystem::pauseWorkOrder(const std::string& workOrderId) {
	if (workOrderId.empty()) return false;

	WorkOrder* workOrder = findWorkOrder(workOrderId);

	if (workOrder == nullptr) return false;

	if (!workOrder->hasAssignedEquipment()) return false;

	if (workOrder->getStatus() != WorkOrderStatus::RUNNING)	return false;

	const string& assignedEquipmentId = workOrder->getAssignedEquipmentId();
	Equipment* equipment = findEquipment(assignedEquipmentId);

	if (equipment == nullptr) return false;

	if (equipment->getStatus() != EquipmentStatus::RUNNING) return false;

	// 설비 상태 변경
	if (!equipment->changeStatus(EquipmentStatus::PAUSED)) return false;

	if (!workOrder->pause()) { // 작업 지시 상태 변경
		equipment->changeStatus(EquipmentStatus::RUNNING);
		return false;
	}

	return true;
}

bool FactorySystem::resumeWorkOrder(const std::string& workOrderId) {
	if (workOrderId.empty()) return false;

	WorkOrder* workOrder = findWorkOrder(workOrderId);

	if (workOrder == nullptr) return false;

	if (!workOrder->hasAssignedEquipment()) return false;

	if (workOrder->getStatus() != WorkOrderStatus::PAUSED) return false;

	const string& assignedEquipmentId = workOrder->getAssignedEquipmentId();
	Equipment* equipment = findEquipment(assignedEquipmentId);

	if (equipment == nullptr) return false;

	if (equipment->getStatus() != EquipmentStatus::PAUSED) return false;

	// 설비 상태 변경
	if (!equipment->changeStatus(EquipmentStatus::RUNNING)) return false;

	if (!workOrder->resume()) { // 작업 지시 상태 변경
		equipment->changeStatus(EquipmentStatus::PAUSED);
		return false;
	}

	return true;
}

bool FactorySystem::completeWorkOrder(const std::string& workOrderId) {
	if (workOrderId.empty()) return false;

	WorkOrder* workOrder = findWorkOrder(workOrderId);

	if (workOrder == nullptr) return false;

	if (!workOrder->hasAssignedEquipment()) {
		return false;
	}

	if (workOrder->getStatus() != WorkOrderStatus::RUNNING) {
		return false;
	}

	const string& assignedEquipmentId = workOrder->getAssignedEquipmentId();
	Equipment* equipment = findEquipment(assignedEquipmentId);

	if (equipment == nullptr) {
		return false;
	}

	if (equipment->getStatus() != EquipmentStatus::RUNNING) {
		return false;
	}

	// 설비 상태 변경
	if (!equipment->changeStatus(EquipmentStatus::STOPPED)) {
		return false;
	}

	if (!workOrder->complete()) { // 작업 지시 상태 변경
		equipment->changeStatus(EquipmentStatus::RUNNING);
		return false;
	}

	return true;
}

bool FactorySystem::cancelWorkOrder(const std::string& workOrderId) {
	if (workOrderId.empty()) return false;

	WorkOrder* workOrder = findWorkOrder(workOrderId);

	if (workOrder == nullptr) {
		return false;
	}

	if (!workOrder->hasAssignedEquipment()) { // 설비 배정 전이라면

		if (!workOrder->cancel()) { // 작업 지시 상태 변경
			return false;
		}

		return true;
	}

	if ( (workOrder->getStatus() == WorkOrderStatus::COMPLETED) || 
		(workOrder->getStatus() == WorkOrderStatus::CANCELLED) ) {
		return false;
	}

	const string& assignedEquipmentId = workOrder->getAssignedEquipmentId();
	Equipment* equipment = findEquipment(assignedEquipmentId);

	if (equipment == nullptr) {
		return false;
	}

	EquipmentStatus currentEquipmentStatus = equipment->getStatus();

	// 설비 상태 변경
	if (currentEquipmentStatus != EquipmentStatus::STOPPED) {
		if (!equipment->changeStatus(EquipmentStatus::STOPPED)) {
			return false;
		}
	}

	if (!workOrder->cancel()) { // 작업 지시 상태 변경
		
		bool restoreSucceeded = true;

		switch (currentEquipmentStatus) {
		case EquipmentStatus::STOPPED:
			// 원래부터 STOPPED였으므로 복구할 필요 없음
			break;

		case EquipmentStatus::RUNNING:
			// STOPPED → RUNNING
			restoreSucceeded =
				equipment->changeStatus(EquipmentStatus::RUNNING);
			break;

		case EquipmentStatus::PAUSED:
			// STOPPED → PAUSED는 불가능하므로
			// STOPPED → RUNNING → PAUSED 순서로 복구
			if (!equipment->changeStatus(EquipmentStatus::RUNNING)) {
				restoreSucceeded = false;
				break;
			}

			if (!equipment->changeStatus(EquipmentStatus::PAUSED)) {
				restoreSucceeded = false;
			}
			break;

		case EquipmentStatus::ERROR:
			// 현재 상태 전이 규칙상 STOPPED → ERROR가 불가능하므로 복구 불가
			restoreSucceeded = false;
			break;
		}

		if (!restoreSucceeded) {
			return false;
		}

		return false;
	}

	return true;
}

bool FactorySystem::enqueueProductionEvent(const std::string& workOrderId,
	const std::string& equipmentId, int produced, int defects) {

	WorkOrder* workOrder = findWorkOrder(workOrderId);

	if (workOrder == nullptr)
		return false;

	Equipment* equipment = findEquipment(equipmentId);

	if (equipment == nullptr)
		return false;

	if (workOrder->getAssignedEquipmentId() != equipmentId)
		return false;

	if (workOrder->getStatus() != WorkOrderStatus::RUNNING)
		return false;

	if (equipment->getStatus() != EquipmentStatus::RUNNING)
		return false;

	if (!workOrder->canRecordProduction(produced, defects))
		return false;

	if (!equipment->canRecordProduction(produced, defects))
		return false;

	ProductionEvent productionEvent( // 이벤트 객체 만듦
		nextProductionEventId,
		workOrderId,
		equipmentId,
		produced,
		defects
	);

	pendingProductionEvents.push(productionEvent); // 대기큐에 push

	nextProductionEventId++; // 객체 고유번호 증가시킴

	return true;
}

int FactorySystem::getPendingProductionEventCount() const {
	return pendingProductionEvents.size();
}

bool FactorySystem::isProductionEventQueueEmpty() const {
	return pendingProductionEvents.empty();
}

bool FactorySystem::processNextProductionEvent() {

	if (isProductionEventQueueEmpty())
		return false;

	ProductionEvent productionEvent = pendingProductionEvents.front();

	WorkOrder* workOrder = findWorkOrder(productionEvent.getWorkOrderId());
	Equipment* equipment = findEquipment(productionEvent.getEquipmentId());

	// 수정 : nullptr 검사 추가
	if (workOrder == nullptr || equipment == nullptr)
		return rejectProductionEvent(productionEvent);

	const string& assignedEquipmentId = workOrder->getAssignedEquipmentId();

	if (assignedEquipmentId != equipment->getId())
		return rejectProductionEvent(productionEvent);

	if (workOrder->getStatus() != WorkOrderStatus::RUNNING ||
		equipment->getStatus() != EquipmentStatus::RUNNING)
		return rejectProductionEvent(productionEvent);

	if (!workOrder->canRecordProduction(
		productionEvent.getProducedQuantity(),
		productionEvent.getDefectQuantity()) ||

		!equipment->canRecordProduction(
			productionEvent.getProducedQuantity(),
			productionEvent.getDefectQuantity()))
		return rejectProductionEvent(productionEvent);

	// 여기까지는 이벤트 유효성 검사

	// 여기부터는 이미 검증을 통과한 이벤트의 실제 처리

	// 수정 : 각각 성공 여부를 따로 확인

	int produced = productionEvent.getProducedQuantity();
	int defects = productionEvent.getDefectQuantity();

	bool workOrderRecorded =
		workOrder->recordProduction(produced, defects);

	if (!workOrderRecorded) {
		return rejectProductionEvent(productionEvent);
	}

	bool equipmentRecorded =
		equipment->recordProduction(produced, defects);

	if (!equipmentRecorded) {

		// WorkOrder에는 이미 실적이 반영되었으므로 되돌림
		bool workOrderRollbackSucceeded =
			workOrder->rollbackProduction(produced, defects);

		if (!workOrderRollbackSucceeded) {
			// 현재 구조에서는 발생하면 안 되는 내부 오류
			pendingProductionEvents.pop();
			return false;
		}

		// 둘 다 이벤트 반영 전 상태로 돌아왔으므로
		// rejected 이벤트로 처리
		return rejectProductionEvent(productionEvent);
	}

	// 수정 : 설비 종료 → 작업 완료 순으로 변경
	if (workOrder->isTargetReached()) {

		if (!equipment->changeStatus(EquipmentStatus::STOPPED)) {
			workOrder->rollbackProduction(produced, defects);
			equipment->rollbackProduction(produced, defects);

			return rejectProductionEvent(productionEvent);
		}
			

		if (!workOrder->complete()) {

			// 수정 : 실패 시 설비 상태 복구
			workOrder->rollbackProduction(produced, defects);
			equipment->rollbackProduction(produced, defects);

			return rejectProductionEvent(productionEvent);
		}
	}

	// 처리 완료 이력 저장
	processedProductionEvents.push_back(productionEvent);

	// 큐에서 제거
	pendingProductionEvents.pop();

	return true;
}

int FactorySystem::processAllProductionEvents() {
	// 큐가 빌 때까지 processNextProductionEvent()를 반복 호출

	int processedCountBefore = processedProductionEvents.size();

	while (!isProductionEventQueueEmpty()) { //pending 큐 확인
		processNextProductionEvent();
	}

	int processedCountAfter = processedProductionEvents.size();

	return processedCountAfter - processedCountBefore;
}

const vector<ProductionEvent>& FactorySystem::getProcessedProductionEvents() const {
	return processedProductionEvents;
}


const vector<ProductionEvent>& FactorySystem::getRejectedProductionEvents() const {
	return rejectedProductionEvents;
}

int FactorySystem::getTotalProducedQuantity() const {
	// 모든 설비의 누적 생산량을 더해 공장 전체 생산량을 반환

	int totalFactoryProduced = 0;

	for (int i = 0; i < equipments.size(); i++) {
		totalFactoryProduced += equipments[i].getTotalProducedQuantity();
	}

	return totalFactoryProduced;
}

int FactorySystem::getTotalDefectQuantity() const {
	// 모든 설비의 누적 불량량을 더해 공장 전체 불량량을 반환

	int totalFactoryDefected = 0;

	for (int i = 0; i < equipments.size(); i++) {
		totalFactoryDefected += equipments[i].getTotalDefectQuantity();
	}

	return totalFactoryDefected;
}

int FactorySystem::getTotalPassQuantity() const {
	// 전체 생산량에서 전체 불량량을 빼 정상 생산량을 반환

	return getTotalProducedQuantity() - getTotalDefectQuantity();
}

float FactorySystem::getOverallDefectRate() const {

	int totalProducedQuantity = getTotalProducedQuantity();
	if (totalProducedQuantity == 0) return 0.0;

	return (static_cast<float>(getTotalDefectQuantity()) / totalProducedQuantity) * 100;
}

