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
	// equipments에 push back 해라

	if (id.empty()) return false; // id가 빈 문자열이면 false
	if (name.empty()) return false; // name이 빈 문자열이면 false
	if (findEquipmentIndex(id) != -1) return false; // 중복 등록 방지

	equipments.push_back(Equipment(id, name));

	return true;
}

const vector<Equipment>& FactorySystem::getEquipments() const {
	return equipments; // Equipment 객체들이 저장된 벡터를 읽기 전용 참조로 반환
}

bool FactorySystem::isEquipmentEmpty() const {
	return equipments.empty();
}