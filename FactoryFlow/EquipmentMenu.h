#pragma once
#include "FactorySystem.h"
#include <string>

class EquipmentMenu {
private:
	/* EquipmentMenu가 FactorySystem을 복사해
	별도 시스템을 만드는 것이 아니라, main()에서 생성한
	실제 시스템을 함께 사용하기 위해 참조로 받는다. */
	FactorySystem& system;

	void printMenu() const;
	void handleRegisterEquipment();
	void handleDisplayAllEquipments() const;
	void handleDisplayEquipment() const;
	void handleChangeEquipmentStatus();
	void printEquipment(const Equipment& equipment) const;

	int readInteger(const std::string& message) const;
	bool convertMenuNumberToStatus(int menuNumber, EquipmentStatus& status) const;

public:
	EquipmentMenu(FactorySystem& system);
	void run();
};