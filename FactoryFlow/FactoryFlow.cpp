#include <iostream>
#include "Equipment.h"
#include "FactorySystem.h"
using namespace std;

int main() {
	FactorySystem system;  // 벡터 멤버변수가 있음
	cout << system.registerEquipment("M01", "equipment1") << endl;
	cout << system.registerEquipment("M02", "equipment2") << endl;
	cout << system.registerEquipment("M01", "equipment3") << endl;
	return 0;
}