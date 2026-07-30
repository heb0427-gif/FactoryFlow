#pragma once
#include "FactorySystem.h"
#include "EquipmentMenu.h"

class FactoryFlowMenu {
private :
	FactorySystem& system;
	EquipmentMenu equipmentMenu;
	int readInteger ( const std::string& message ) const;
	void printMenu ( ) const;

public :
	FactoryFlowMenu(FactorySystem& system);
	void run();
};