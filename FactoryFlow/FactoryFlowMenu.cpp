#include <iostream>
#include "FactoryFlowMenu.h"
#include "EquipmentMenu.h"
#include "WorkOrderMenu.h"
#include "ProductionMenu.h"
#include "LotMenu.h"
#include <limits>
using namespace std;

FactoryFlowMenu::FactoryFlowMenu(FactorySystem& system)
	: system(system), equipmentMenu(system), workOrderMenu(system), 
	productionMenu(system), lotMenu(system) {}
 
void FactoryFlowMenu::printMenu( ) const {
	cout << "================================\n";
	cout << "	FactoryFlow v0.4\n";
	cout << "================================\n\n";

	cout << "1. Equipment Management\n";
	cout << "2. Work Order Management\n";
	cout << "3. Production Management\n";
	cout << "4. LOT Traceability\n";
	cout << "0. Exit\n\n";
}

int FactoryFlowMenu::readInteger ( const std::string& message ) const {
	int number;

	while ( true ) {
		cout << message;

		if ( cin >> number ) {
			cin.ignore ( numeric_limits<streamsize>::max ( ) , '\n' );
			return number;
		}
		cin.clear ( );
		cin.ignore ( numeric_limits<streamsize>::max ( ) , '\n' );
		cout << "Error! Enter an Integer.\n\n";
	}
}

void FactoryFlowMenu::run( ) {

	while ( true ) {
		FactoryFlowMenu::printMenu ( );
		int number = readInteger("Select menu number: ");

		switch ( number ) {
		case 1 :
			equipmentMenu.run(); break;
		case 2 :
			workOrderMenu.run(); break;
		case 3 :
			productionMenu.run(); break;
		case 4:
			lotMenu.run(); break;
		case 0 :
			return;
		default :
			cout << "Entered wrong number. Enter number from 0 to 4.\n\n";
		}
	}
}