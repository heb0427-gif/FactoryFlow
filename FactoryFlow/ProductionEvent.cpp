#include "ProductionEvent.h"
using namespace std;

int ProductionEvent::nextEventId = 1;

ProductionEvent::ProductionEvent(const string& workOrderId,
	const string& equipmentId, int producedQuantity,
	int defectQuantity)
	: eventId(nextEventId), workOrderId(workOrderId), equipmentId(equipmentId),
	producedQuantity(producedQuantity), defectQuantity(defectQuantity) {

	nextEventId++;
}

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

