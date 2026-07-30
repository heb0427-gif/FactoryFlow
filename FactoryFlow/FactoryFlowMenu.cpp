#include <iostream>
#include "FactoryFlowMenu.h"
#include "EquipmentMenu.h"
using namespace std;

FactoryFlowMenu::FactoryFlowMenu(FactorySystem& system)
	: system(system), equipmentMenu(system) {}

void FactoryFlowMenu::printMenu( ) const {
	cout << "================================\n";
	cout << "	FactoryFlow v0.2\n";
	cout << "================================\n\n";

	cout << "1. Equipment Management\n";
	cout << "2. Work Order Management\n";
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
			cout << "Work order management is not implemented yet.\n\n";
			break;
		case 0 :
			return;
		default :
			cout << "Entered wrong number. Enter number from 0 to 2.\n\n";
		}
	}
}