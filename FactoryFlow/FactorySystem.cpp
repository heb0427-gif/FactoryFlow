#include <vector>
#include "FactorySystem.h"
using namespace std;

int FactorySystem::findEquipmentIndex(const std::string& id) const {
	for (int i = 0; i < equipments.size(); i++) {
		if (equipments[i].getId() == id) return i;
	}
	return -1;
}

bool FactorySystem::registerEquipment(const std::string& id, const std::string& name) {
	// equipments??push back ?대씪

	if (id.empty()) return false; // id媛 鍮?臾몄옄?댁씠硫?false
	if (name.empty()) return false; // name??鍮?臾몄옄?댁씠硫?false
	if (findEquipmentIndex(id) != -1) return false; // 以묐났 ?깅줉 諛⑹?

	equipments.push_back(Equipment(id, name));

	return true;
}

const vector<Equipment>& FactorySystem::getEquipments() const {
	return equipments; // Equipment 媛앹껜?ㅼ씠 ??λ맂 踰≫꽣瑜??쎄린 ?꾩슜 李몄“濡?諛섑솚
}

bool FactorySystem::isEquipmentEmpty() const {
	return equipments.empty();
}

const Equipment* FactorySystem::findEquipment(const std::string& id) const {
	int equipmentIndex = findEquipmentIndex(id);

	if (equipmentIndex == -1) return nullptr;
	return &equipments[equipmentIndex];  // 二쇱냼瑜??살뼱???댁꽌 & 遺숈뿬以?
}

bool FactorySystem::changeEquipmentStatus(const std::string& id, EquipmentStatus nextStatus) {
	int equipmentIndex = findEquipmentIndex(id);
	
	if (equipmentIndex == -1) return false;
	return equipments[equipmentIndex].changeStatus(nextStatus);
}