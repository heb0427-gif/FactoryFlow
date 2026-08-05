#pragma once
#include <string>

/*
Event ID: 1
Work Order ID: WO001
Equipment ID: M01
Produced Quantity: 10
Defect Quantity: 2
Pass Quantity: 8
*/

class ProductionEvent { // 아직 처리되지 않은 생산실적 요청 한 건
private:
	int eventId;
	std::string workOrderId;
	std::string equipmentId;
	int producedQuantity;
	int defectQuantity;

public:
	ProductionEvent(int eventId, const std::string& workOrderId,
		const std::string& equipmentId, int producedQuantity, 
		int defectQuantity);

	int getEventId() const;
	const std::string& getWorkOrderId() const;
	const std::string& getEquipmentId() const;
	int getProducedQuantity() const;
	int getDefectQuantity() const;
	int getPassQuantity() const;
};

