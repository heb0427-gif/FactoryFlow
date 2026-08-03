#include <iostream>
#include "EquipmentMenu.h"
#include "FactorySystem.h"
#include "FactoryFlowMenu.h"
#include "WorkOrder.h"
using namespace std;

#include <iostream>
#include "FactorySystem.h"

using namespace std;

int main() {
	cout << boolalpha;

	// 1. 존재하는 WO001 + 존재하는 M01 -> 성공
	{
		FactorySystem system;

		system.registerEquipment("M01", "Equipment 01");
		system.createWorkOrder(
			"WO001",
			"PART-A",
			100,
			WorkOrderPriority::HIGH
		);

		bool result =
			system.assignEquipmentToWorkOrder("WO001", "M01");

		cout << "[Test 1] Existing WO001 + Existing M01\n";
		cout << "Expected: true\n";
		cout << "Actual  : " << result << "\n\n";
	}

	// 2. 존재하지 않는 WO999 + 존재하는 M01 -> 실패
	{
		FactorySystem system;

		system.registerEquipment("M01", "Equipment 01");

		bool result =
			system.assignEquipmentToWorkOrder("WO999", "M01");

		cout << "[Test 2] Missing WO999 + Existing M01\n";
		cout << "Expected: false\n";
		cout << "Actual  : " << result << "\n\n";
	}

	// 3. 존재하는 WO001 + 존재하지 않는 M99 -> 실패
	{
		FactorySystem system;

		system.createWorkOrder(
			"WO001",
			"PART-A",
			100,
			WorkOrderPriority::HIGH
		);

		bool result =
			system.assignEquipmentToWorkOrder("WO001", "M99");

		cout << "[Test 3] Existing WO001 + Missing M99\n";
		cout << "Expected: false\n";
		cout << "Actual  : " << result << "\n\n";
	}

	// 4. ERROR 상태 설비 배정 -> 실패
	{
		FactorySystem system;

		system.registerEquipment("M01", "Equipment 01");
		system.createWorkOrder(
			"WO001",
			"PART-A",
			100,
			WorkOrderPriority::HIGH
		);

		// STOPPED -> ERROR는 허용되지 않으므로
		// STOPPED -> RUNNING -> ERROR 순서로 변경
		system.changeEquipmentStatus(
			"M01",
			EquipmentStatus::RUNNING
		);

		system.changeEquipmentStatus(
			"M01",
			EquipmentStatus::ERROR
		);

		bool result =
			system.assignEquipmentToWorkOrder("WO001", "M01");

		cout << "[Test 4] Assign ERROR Equipment M01\n";
		cout << "Expected: false\n";
		cout << "Actual  : " << result << "\n\n";
	}

	// 5. 이미 다른 활성 작업지시에 배정된 설비 -> 실패
	{
		FactorySystem system;

		system.registerEquipment("M01", "Equipment 01");

		system.createWorkOrder(
			"WO001",
			"PART-A",
			100,
			WorkOrderPriority::HIGH
		);

		system.createWorkOrder(
			"WO002",
			"PART-B",
			200,
			WorkOrderPriority::NORMAL
		);

		bool firstAssignment =
			system.assignEquipmentToWorkOrder("WO001", "M01");

		bool secondAssignment =
			system.assignEquipmentToWorkOrder("WO002", "M01");

		cout << "[Test 5] Assign M01 To Two Active Work Orders\n";
		cout << "First assignment expected: true\n";
		cout << "First assignment actual  : "
			<< firstAssignment << "\n";

		cout << "Second assignment expected: false\n";
		cout << "Second assignment actual  : "
			<< secondAssignment << "\n\n";
	}

	// 6. 이미 READY 상태인 작업지시에 재배정 -> 실패
	{
		FactorySystem system;

		system.registerEquipment("M01", "Equipment 01");
		system.registerEquipment("M02", "Equipment 02");

		system.createWorkOrder(
			"WO001",
			"PART-A",
			100,
			WorkOrderPriority::HIGH
		);

		bool firstAssignment =
			system.assignEquipmentToWorkOrder("WO001", "M01");

		bool reassignment =
			system.assignEquipmentToWorkOrder("WO001", "M02");

		cout << "[Test 6] Reassign READY Work Order WO001\n";
		cout << "First assignment expected: true\n";
		cout << "First assignment actual  : "
			<< firstAssignment << "\n";

		cout << "Reassignment expected: false\n";
		cout << "Reassignment actual  : "
			<< reassignment << "\n\n";
	}

	return 0;
}