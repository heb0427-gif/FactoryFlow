#pragma once
#include "FactorySystem.h"
#include "EquipmentMenu.h"
#include "WorkOrderMenu.h"
#include "ProductionMenu.h"

class FactoryFlowMenu {
private :
	FactorySystem& system;

	EquipmentMenu equipmentMenu;
	WorkOrderMenu workOrderMenu;
	ProductionMenu productionMenu;

	int readInteger ( const std::string& message ) const;
	void printMenu ( ) const;

public :
	FactoryFlowMenu(FactorySystem& system);
	void run();
};