#pragma once
#include "FactorySystem.h"
#include "EquipmentMenu.h"
#include "WorkOrderMenu.h"

class FactoryFlowMenu {
private :
	FactorySystem& system;
	EquipmentMenu equipmentMenu;
	WorkOrderMenu workOrderMenu;
	int readInteger ( const std::string& message ) const;
	void printMenu ( ) const;

public :
	FactoryFlowMenu(FactorySystem& system);
	void run();
};