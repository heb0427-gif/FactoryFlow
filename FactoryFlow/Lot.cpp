#include "Lot.h"
#include <string>
#include <iostream>
using namespace std;

Lot::Lot(const std::string& id, const std::string& itemCode, LotType type,
	int quantity) : id(id), itemCode(itemCode), type(type), quantity(quantity) {}

const string& Lot::getId() const { return id; }
const string& Lot::getItemCode() const { return itemCode; }
LotType Lot::getType() const { return type; }
int Lot::getQuantity() const { return quantity; }
string Lot::lotTypeToString(LotType type) const {
	switch (type) {
		case LotType::RAW_MATERIAL:
			return "RAW_MATERIAL";

		case LotType::SEMI_FINISHED:
			return "SEMI_FINISHED";

		case LotType::FINISHED:
			return "FINISHED";

		default:
			return "UNKNOWN";
	}
}