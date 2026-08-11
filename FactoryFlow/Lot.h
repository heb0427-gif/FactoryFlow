#pragma once
#include <string>

enum class LotType {
	RAW_MATERIAL, // 외부에서 들어온 "원자재" LOT
	SEMI_FINISHED, // 공장에서 생산한 "반제품" LOT
	FINISHED // 공장에서 생산한 "완제품" LOT
};

class Lot {
private:
	std::string id;
	std::string itemCode;
	LotType type;
	int quantity;

public:
	Lot(const std::string& id, const std::string& itemCode, LotType type,
		int quantity);
	const std::string& getId() const;
	const std::string& getItemCode() const;
	LotType getType() const;
	int getQuantity() const;
	std::string lotTypeToString(LotType type);
};