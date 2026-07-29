#include <iostream>
#include "EquipmentMenu.h"
#include "FactorySystem.h"
using namespace std;

int main() {
	FactorySystem system;
	EquipmentMenu menu(system);
	menu.run();

	return 0;
}