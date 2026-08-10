#pragma once
#include "Equipment.h"
#include "WorkOrder.h"
#include "ProductionEvent.h"
#include "Lot.h"
#include "LotRelation.h"
#include <string>
#include <vector>
#include <queue>

class FactorySystem {  // 공장에 등록된 전체 설비 목록 관리
private:
	std::vector<Equipment> equipments; // 설비들을 저장하는 벡터
	int findEquipmentIndex(const std::string& id) const;
	std::vector<WorkOrder> workOrders; // 작업 지시들을 저장하는 벡터
	int findWorkOrderIndex ( const std::string& id ) const;

	std::queue<ProductionEvent> pendingProductionEvents; // 미처리 생산실적들 큐(FIFO)
	int nextProductionEventId;  // 이벤트마다 고유 번호 자동 발급
	std::vector<ProductionEvent> processedProductionEvents; // 처리된 생산실적들 벡터
	std::vector<ProductionEvent> rejectedProductionEvents; // 거부된 생산실적들 벡터
	bool rejectProductionEvent(const ProductionEvent& productionEvent);

	int findLotIndex(const std::string& lotId) const; // 내부 검색용

	std::vector<Lot> lots;
	
	std::vector<LotRelation> lotRelations;
	int nextLotRelationId;

public:
	FactorySystem();
	// 설비 등록 함수
	bool registerEquipment(const std::string& id, const std::string& name);
	const std::vector<Equipment>& getEquipments() const;
	const Equipment* findEquipment(const std::string& id) const;  // 해당 id를 가진 객체의 주소 반환
	Equipment* findEquipment(const std::string& id);
	bool changeEquipmentStatus(const std::string& id, EquipmentStatus nextStatus);
	bool isEquipmentEmpty() const;

	// 작업 지시 등록 함수
	bool createWorkOrder ( const std::string& id , 
		const std::string& productCode , int targetQuantity , 
		WorkOrderPriority priority );
	const std::vector<WorkOrder>& getWorkOrders ( ) const;
	const WorkOrder* findWorkOrder (const std::string& id) const;
	WorkOrder* findWorkOrder(const std::string& id);
	bool isWorkOrderEmpty ( ) const;

	// 시스템 전체 규칙 구현(기존 Equipment와 WorkOrder 연결)
	bool assignEquipmentToWorkOrder(const std::string& workOrderId,
		const std::string& equipmentId );

	bool isEquipmentAssigned ( const std::string& equipmentId ) const;

	bool startWorkOrder(const std::string& workOrderId);
	bool pauseWorkOrder(const std::string& workOrderId);
	bool resumeWorkOrder(const std::string& workOrderId);
	bool completeWorkOrder(const std::string& workOrderId);
	bool cancelWorkOrder(const std::string& workOrderId);

	bool enqueueProductionEvent(const std::string& workOrderId, 
		const std::string& equipmentId, int produced, int defects);
	int getPendingProductionEventCount() const;
	bool isProductionEventQueueEmpty() const;

	bool processNextProductionEvent();
	int processAllProductionEvents(); //큐가 빌 때까지 processNextProductionEvent() 반복 호출 
	const std::vector<ProductionEvent>& getProcessedProductionEvents() const;
	const std::vector<ProductionEvent>& getRejectedProductionEvents() const;

	int getTotalProducedQuantity() const;
	int getTotalDefectQuantity() const;
	int getTotalPassQuantity() const;
	float getOverallDefectRate() const;

	bool registerLot(const std::string& lotId, const std::string& itemCode,
		LotType type, int quantity);
	const Lot* findLot(const std::string& lotId) const;
	Lot* findLot(const std::string& lotId);
	const std::vector<Lot>& getLots() const;
	bool isLotEmpty() const;

	bool addLotRelation(
		const std::string& inputLotId,
		const std::string& outputLotId,
		const std::string& workOrderId,
		int usedQuantity
	);
};

