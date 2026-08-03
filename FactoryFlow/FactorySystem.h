#pragma once
#include "Equipment.h"
#include "WorkOrder.h"
#include <string>
#include <vector>

class FactorySystem {  // 怨듭옣???깅줉???꾩껜 ?ㅻ퉬 紐⑸줉 愿由?
private:
	std::vector<Equipment> equipments; // ?ㅻ퉬?ㅼ쓣 ??ν븯??踰≫꽣
	int findEquipmentIndex(const std::string& id) const;
	std::vector<WorkOrder> workOrders; // ?묒뾽 吏?쒕뱾????ν븯??踰≫꽣
	int findWorkOrderIndex ( const std::string& id ) const;

public:
	// ?ㅻ퉬 ?깅줉 ?⑥닔
	bool registerEquipment(const std::string& id, const std::string& name);
	const std::vector<Equipment>& getEquipments() const;
	const Equipment* findEquipment(const std::string& id) const;  // ?대떦 id瑜?媛吏?媛앹껜??二쇱냼 諛섑솚
	Equipment* findEquipment(const std::string& id);
	bool changeEquipmentStatus(const std::string& id, EquipmentStatus nextStatus);
	bool isEquipmentEmpty() const;

	// ?묒뾽 吏???깅줉 ?⑥닔
	bool createWorkOrder ( const std::string& id , 
		const std::string& productCode , int targetQuantity , 
		WorkOrderPriority priority );
	const std::vector<WorkOrder>& getWorkOrders ( ) const;
	const WorkOrder* findWorkOrder (const std::string& id) const;
	WorkOrder* findWorkOrder(const std::string& id);
	bool isWorkOrderEmpty ( ) const;

	// ?쒖뒪???꾩껜 洹쒖튃 援ы쁽(湲곗〈 Equipment????WorkOrder ?곌껐
	bool assignEquipmentToWorkOrder(const std::string& workOrderId,
		const std::string& equipmentId );

	bool isEquipmentAssigned ( const std::string& equipmentId ) const;

	bool startWorkOrder(const std::string& workOrderId);
	bool pauseWorkOrder(const std::string& workOrderId);
	bool resumeWorkOrder(const std::string& workOrderId);
	bool completeWorkOrder(const std::string& workOrderId);
	bool cancelWorkOrder(const std::string& workOrderId);
};