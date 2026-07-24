#include <iostream>
#include "Equipment.h"
#include "FactorySystem.h"
using namespace std;

int main() {
	FactorySystem system;  // 벡터 멤버변수가 있음
	cout << system.registerEquipment("M01", "equipment1") << endl;
	cout << system.findEquipment("M01") << endl;
	cout << system.findEquipment("M99") << endl;
	cout << system.changeEquipmentStatus("M01", EquipmentStatus::RUNNING) << endl;
	cout << "M01's status has changed. " << equipmentStatusToString(EquipmentStatus::RUNNING) << endl;
	cout << system.changeEquipmentStatus("M01", EquipmentStatus::PAUSED) << endl;
	cout << "M01's status has changed. " << equipmentStatusToString(EquipmentStatus::PAUSED) << endl;
	cout << system.changeEquipmentStatus("M99", EquipmentStatus::PAUSED) << endl;
	
	return 0;
}