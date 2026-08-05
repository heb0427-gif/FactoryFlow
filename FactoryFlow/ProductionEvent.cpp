#include "ProductionEvent.h"
using namespace std;

ProductionEvent::ProductionEvent(int eventId, const string& workOrderId,
	const string& equipmentId, int producedQuantity,
	int defectQuantity)
	: eventId(eventId), workOrderId(workOrderId), equipmentId(equipmentId),
	producedQuantity(producedQuantity), defectQuantity(defectQuantity) {}

int ProductionEvent::getEventId() const {
	return eventId;
}

const string& ProductionEvent::getWorkOrderId() const {
	return workOrderId;
}

const string& ProductionEvent::getEquipmentId() const {
	return equipmentId;
}

int ProductionEvent::getProducedQuantity() const {
	return producedQuantity;
}

int ProductionEvent::getDefectQuantity() const {
	return defectQuantity;
}

int ProductionEvent::getPassQuantity() const {
	return producedQuantity - defectQuantity;
}

