#pragma once
#include <string>

// 어떤 LOT가 어떤 LOT를 만드는 데 사용됐는지 기록한다.

class LotRelation {
private:
	int relationId;
	std::string inputLotId;
	std::string outputLotId;
	std::string workOrderId;
	std::string equipmentId;
	int usedQuantity;

public:
	LotRelation(int relationId, const std::string& inputLotId,
		const std::string& outputLotId, const std::string& workOrderId,
		const std::string& equipmentId, int usedQuantity);
	int getRelationId() const;
	const std::string& getInputLotId() const;
	const std::string& getOutputLotId() const;
	const std::string& getWorkOrderId() const;
	const std::string& getEquipmentId() const;
	int getUsedQuantity() const;
};