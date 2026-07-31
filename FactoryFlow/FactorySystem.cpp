#include <vector>
#include "FactorySystem.h"
using namespace std;

int FactorySystem::findEquipmentIndex(const std::string& id) const {
	for (int i = 0; i < equipments.size(); i++) {
		if (equipments[i].getId() == id) return i;
	}
	return -1;
}

int FactorySystem::findWorkOrderIndex ( const std::string& id ) const {
	for ( int i = 0; i < workOrders.size ( ); i++ ) {
		if ( workOrders[ i ].getId ( ) == id ) return i;
	}
	return -1;
}

bool FactorySystem::registerEquipment(const std::string& id, const std::string& name) {
	//equipments에 push_back 한다.

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

const Equipment* FactorySystem::findEquipment(const std::string& id) const {
	int equipmentIndex = findEquipmentIndex(id);

	if (equipmentIndex == -1) return nullptr;
	return &equipments[equipmentIndex];  // 주소를 넘겨주기 위해서 &를 붙여줌
}

bool FactorySystem::changeEquipmentStatus(const std::string& id, EquipmentStatus nextStatus) {
	int equipmentIndex = findEquipmentIndex(id);
	
	if (equipmentIndex == -1) return false;
	return equipments[equipmentIndex].changeStatus(nextStatus);
}

bool FactorySystem::createWorkOrder ( const std::string& id , 
	const std::string& productCode , int targetQuantity , 
	WorkOrderPriority priority ) {

	if ( ( id.empty() ) || ( productCode.empty() ) || ( targetQuantity <= 0 ) ||
		( findWorkOrderIndex(id) != -1 ) ) return false;

	workOrders.push_back(WorkOrder(id , productCode , targetQuantity, 
		priority));
	
	return true;
}

const vector<WorkOrder>& FactorySystem::getWorkOrders() const {
	return workOrders;
}

const WorkOrder* FactorySystem::findWorkOrder(const std::string& id) const {
	int workOrderIndex = findWorkOrderIndex ( id );

	if ( workOrderIndex == -1 ) return nullptr;

	return &workOrders[workOrderIndex];
}

const bool FactorySystem::isWorkOrderEmpty ( ) const {
	return workOrders.empty ( );
}