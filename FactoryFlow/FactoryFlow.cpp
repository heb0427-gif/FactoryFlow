#include <iostream>
#include "EquipmentMenu.h"
#include "FactorySystem.h"
#include "FactoryFlowMenu.h"
#include "WorkOrder.h"
using namespace std;

int main() {
	WorkOrder workOrder ( "WO001" , "PART-A" , 100 , WorkOrderPriority::HIGH );
	cout << workOrder.hasAssignedEquipment ( );
	cout << workOrderStatusToString ( workOrder.getStatus ( ) );
	return 0;
}