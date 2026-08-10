#include "LotRelation.h"
#include <iostream>
using namespace std;

LotRelation::LotRelation(int relationId, const std::string& inputLotId,
	const std::string& outputLotId, const std::string& workOrderId,
	const std::string& equipmentId, int usedQuantity)
	: relationId(relationId), inputLotId(inputLotId),
	outputLotId(outputLotId), workOrderId(workOrderId),
	equipmentId(equipmentId), usedQuantity(usedQuantity) {}

int LotRelation::getRelationId() const {
	return relationId;
}

const std::string& LotRelation::getInputLotId() const {
	return inputLotId;
}

const std::string& LotRelation::getOutputLotId() const {
	return outputLotId;
}

const std::string& LotRelation::getWorkOrderId() const {
	return workOrderId;
}

const std::string& LotRelation::getEquipmentId() const {
	return equipmentId;
}

int LotRelation::getUsedQuantity() const {
	return usedQuantity;
}

