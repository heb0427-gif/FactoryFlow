#include "EquipmentMenu.h"
#include "FactorySystem.h"
#include <iostream>
using namespace std;

// 생성자
EquipmentMenu::EquipmentMenu(FactorySystem& system) : system(system) {}

void EquipmentMenu::printMenu() const {
	cout << "================================\n";
	cout << "FactoryFlow v0.1\n";
	cout << "================================\n";
	cout << "1. Register Equipment\n";
	cout << "2. View All Equipments\n";
	cout << "3. View Equipment Details\n";
	cout << "4. Change Equipment Status\n";
	cout << "0. Exit\n";
}

void EquipmentMenu::printEquipment(const Equipment& equipment) const {
	cout << "ID: " << equipment.getId() << "\n";
	cout << "Name: " << equipment.getName() << "\n";
	cout << "Status: " << equipmentStatusToString(equipment.getStatus()) << "\n";
}

void EquipmentMenu::handleDisplayAllEquipments() const {
	const vector<Equipment>& equipments = system.getEquipments(); // 

	if (equipments.empty()) {
		cout << "There're no registered equipments.\n";
		return;
	}

	for (size_t i = 0; i < equipments.size(); i++)
		printEquipment(equipments[i]);
}

void EquipmentMenu::handleDisplayEquipment() const {
	string id;
	cout << "Enter ID: ";
	cin >> id;

	const Equipment* equipmentPtr = system.findEquipment(id);

	if (equipmentPtr == nullptr) {
		cout << "Equipment not found.\n";
		return;
	}

	printEquipment(*equipmentPtr); // 주소가 가리키는 실제 객체를 얻음(역참조)
}

