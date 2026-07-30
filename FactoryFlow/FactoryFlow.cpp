#include <iostream>
#include "EquipmentMenu.h"
#include "FactorySystem.h"
#include "FactoryFlowMenu.h"
using namespace std;

int main() {
	FactorySystem system;
	FactoryFlowMenu menu ( system );
	menu.run ( );

	return 0;
}