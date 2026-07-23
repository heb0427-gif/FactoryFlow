#include <iostream>
#include "Equipment.h"
using namespace std;

int main() {
	Equipment equipment("M01", "Machine 1");
	cout << "Machine ID: " << equipment.getId() << endl;
	cout << "Machine Name: " << equipment.getName() << endl;
	cout << "Machine Status: " << equipmentStatusToString(equipment.getStatus()) << endl;
	
	cout << equipment.changeStatus(EquipmentStatus::RUNNING) << endl;
	cout << equipment.changeStatus(EquipmentStatus::PAUSED) << endl;
	cout << equipment.changeStatus(EquipmentStatus::ERROR) << endl;
	cout << equipment.changeStatus(EquipmentStatus::RUNNING) << endl;
	cout << equipment.changeStatus(EquipmentStatus::STOPPED) << endl;


	return 0;
}