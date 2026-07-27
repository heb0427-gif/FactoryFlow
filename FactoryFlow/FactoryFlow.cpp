#include <iostream>
#include "EquipmentMenu.h"
#include "FactorySystem.h"
using namespace std;

int main() {
	FactorySystem system;  // 벡터 멤버변수가 있음
	EquipmentMenu menu(system);

	menu.run();

	return 0;
}