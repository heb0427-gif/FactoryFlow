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

void EquipmentMenu::handleRegisterEquipment() {
	string registerID, registerName;
	cout << "[Register Equipment]\n\n";

	cout << "Equipment ID: ";
	cin >> registerID;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Equipment Name: ";
	getline(cin, registerName);

	if (!system.registerEquipment(registerID, registerName)) {
		cout << "Unable to register the equipment. Please check the ID and name.\n";
		return;
	}
	cout << "Equipment registered.\n";
}

bool EquipmentMenu::convertMenuNumberToStatus(int menuNumber, EquipmentStatus& status) const {
	switch (menuNumber) {
	case 1:
		status = EquipmentStatus::STOPPED;
		return true;
	case 2:
		status = EquipmentStatus::RUNNING;
		return true;
	case 3:
		status = EquipmentStatus::PAUSED;
		return true;
	case 4:
		status = EquipmentStatus::ERROR;
		return true;
	default:
		return false;
	}
}

void EquipmentMenu::handleChangeEquipmentStatus() {
	string id;
	int statusNumberToChange;

	cout << "[Change Equipment Status]\n\n";

	cout << "Equipment ID: ";
	cin >> id;

	const Equipment* equipment = system.findEquipment(id);
	if (equipment == nullptr) {
		cout << "Equipment not found.";
		return;
	}

	cout << "\n\n";

	cout << "Current Status: " << equipmentStatusToString(equipment->getStatus()) << "\n\n";

	cout << "1. STOPPED\n";
	cout << "2. RUNNING\n";
	cout << "3. PAUSED\n";
	cout << "4. ERROR\n\n";

	cout << "Status to change: ";
	cin >> statusNumberToChange;

	EquipmentStatus nextStatus;

	if (!convertMenuNumberToStatus(statusNumberToChange, nextStatus)) {
		cout << "Invalid status number\n";
		return;
	}

	if (!system.changeEquipmentStatus(id, nextStatus)) {
		cout << "Invalid status transition!\n";
		return;
	}

	cout << "Equipment status changed to " << equipmentStatusToString(nextStatus) << "\n";
}

