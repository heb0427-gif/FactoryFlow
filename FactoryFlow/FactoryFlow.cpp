#include <iostream>
#include "EquipmentMenu.h"
#include "FactorySystem.h"
#include "FactoryFlowMenu.h"
#include "WorkOrder.h"
using namespace std;

void printWorkOrderAndEquipmentStatus(
	const FactorySystem& system,
	const string& workOrderId,
	const string& equipmentId
) {
	const WorkOrder* workOrder = system.findWorkOrder(workOrderId);
	const Equipment* equipment = system.findEquipment(equipmentId);

	if (workOrder != nullptr) {
		cout << "WorkOrder status: "
			<< workOrderStatusToString(workOrder->getStatus())
			<< '\n';
	}

	if (equipment != nullptr) {
		cout << "Equipment status: "
			<< equipmentStatusToString(equipment->getStatus())
			<< '\n';
	}
}


int main() {

		cout << boolalpha;

		// ==================================================
		// 성공 테스트 1
		// READY 작업 시작
		// WorkOrder: READY -> RUNNING
		// Equipment: STOPPED -> RUNNING
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Success Test 1] Start READY WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.registerEquipment("M01", "Equipment 01");
			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			system.assignEquipmentToWorkOrder("WO001", "M01");

			bool result = system.startWorkOrder("WO001");

			cout << "Expected result: true\n";
			cout << "Actual result  : " << result << '\n';

			printWorkOrderAndEquipmentStatus(
				system,
				"WO001",
				"M01"
			);

			cout << "Expected WorkOrder status: RUNNING\n";
			cout << "Expected Equipment status: RUNNING\n\n";
		}

		// ==================================================
		// 성공 테스트 2
		// RUNNING 작업 일시정지
		// WorkOrder: RUNNING -> PAUSED
		// Equipment: RUNNING -> PAUSED
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Success Test 2] Pause RUNNING WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.registerEquipment("M01", "Equipment 01");
			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			system.assignEquipmentToWorkOrder("WO001", "M01");
			system.startWorkOrder("WO001");

			bool result = system.pauseWorkOrder("WO001");

			cout << "Expected result: true\n";
			cout << "Actual result  : " << result << '\n';

			printWorkOrderAndEquipmentStatus(
				system,
				"WO001",
				"M01"
			);

			cout << "Expected WorkOrder status: PAUSED\n";
			cout << "Expected Equipment status: PAUSED\n\n";
		}

		// ==================================================
		// 성공 테스트 3
		// PAUSED 작업 재개
		// WorkOrder: PAUSED -> RUNNING
		// Equipment: PAUSED -> RUNNING
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Success Test 3] Resume PAUSED WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.registerEquipment("M01", "Equipment 01");
			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			system.assignEquipmentToWorkOrder("WO001", "M01");
			system.startWorkOrder("WO001");
			system.pauseWorkOrder("WO001");

			bool result = system.resumeWorkOrder("WO001");

			cout << "Expected result: true\n";
			cout << "Actual result  : " << result << '\n';

			printWorkOrderAndEquipmentStatus(
				system,
				"WO001",
				"M01"
			);

			cout << "Expected WorkOrder status: RUNNING\n";
			cout << "Expected Equipment status: RUNNING\n\n";
		}

		// ==================================================
		// 성공 테스트 4
		// RUNNING 작업 완료
		// WorkOrder: RUNNING -> COMPLETED
		// Equipment: RUNNING -> STOPPED
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Success Test 4] Complete RUNNING WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.registerEquipment("M01", "Equipment 01");
			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			system.assignEquipmentToWorkOrder("WO001", "M01");
			system.startWorkOrder("WO001");

			bool result = system.completeWorkOrder("WO001");

			cout << "Expected result: true\n";
			cout << "Actual result  : " << result << '\n';

			printWorkOrderAndEquipmentStatus(
				system,
				"WO001",
				"M01"
			);

			cout << "Expected WorkOrder status: COMPLETED\n";
			cout << "Expected Equipment status: STOPPED\n\n";
		}

		// ==================================================
		// 실패 테스트 1
		// WAITING 작업 시작
		// 설비가 배정되지 않았으므로 실패
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Failure Test 1] Start WAITING WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			bool result = system.startWorkOrder("WO001");

			cout << "Expected result: false\n";
			cout << "Actual result  : " << result << '\n';

			const WorkOrder* workOrder =
				system.findWorkOrder("WO001");

			if (workOrder != nullptr) {
				cout << "WorkOrder status: "
					<< workOrderStatusToString(
						workOrder->getStatus()
					)
					<< '\n';
			}

			cout << "Expected WorkOrder status: WAITING\n\n";
		}

		// ==================================================
		// 실패 테스트 2
		// 완료된 작업 다시 시작
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Failure Test 2] Restart COMPLETED WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.registerEquipment("M01", "Equipment 01");
			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			system.assignEquipmentToWorkOrder("WO001", "M01");
			system.startWorkOrder("WO001");
			system.completeWorkOrder("WO001");

			bool result = system.startWorkOrder("WO001");

			cout << "Expected result: false\n";
			cout << "Actual result  : " << result << '\n';

			printWorkOrderAndEquipmentStatus(
				system,
				"WO001",
				"M01"
			);

			cout << "Expected WorkOrder status: COMPLETED\n";
			cout << "Expected Equipment status: STOPPED\n\n";
		}

		// ==================================================
		// 실패 테스트 3
		// 설비 상태가 STOPPED가 아닌 상태에서 작업 시작
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Failure Test 3] Start With Non-STOPPED Equipment\n";
			cout << "================================\n";

			FactorySystem system;

			system.registerEquipment("M01", "Equipment 01");
			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			// 배정 시점에는 설비가 STOPPED여야 함
			system.assignEquipmentToWorkOrder("WO001", "M01");

			// 배정 후 설비 상태만 RUNNING으로 변경
			system.changeEquipmentStatus(
				"M01",
				EquipmentStatus::RUNNING
			);

			bool result = system.startWorkOrder("WO001");

			cout << "Expected result: false\n";
			cout << "Actual result  : " << result << '\n';

			printWorkOrderAndEquipmentStatus(
				system,
				"WO001",
				"M01"
			);

			cout << "Expected WorkOrder status: READY\n";
			cout << "Expected Equipment status: RUNNING\n\n";
		}

		// ==================================================
		// 실패 테스트 4
		// 배정되지 않은 작업 일시정지
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Failure Test 4] Pause Unassigned WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			bool result = system.pauseWorkOrder("WO001");

			cout << "Expected result: false\n";
			cout << "Actual result  : " << result << '\n';

			const WorkOrder* workOrder =
				system.findWorkOrder("WO001");

			if (workOrder != nullptr) {
				cout << "WorkOrder status: "
					<< workOrderStatusToString(
						workOrder->getStatus()
					)
					<< '\n';
			}

			cout << "Expected WorkOrder status: WAITING\n\n";
		}

		// ==================================================
		// 실패 테스트 5
		// 완료된 작업 취소
		// ==================================================
		{
			cout << "================================\n";
			cout << "[Failure Test 5] Cancel COMPLETED WorkOrder\n";
			cout << "================================\n";

			FactorySystem system;

			system.registerEquipment("M01", "Equipment 01");
			system.createWorkOrder(
				"WO001",
				"PART-A",
				100,
				WorkOrderPriority::HIGH
			);

			system.assignEquipmentToWorkOrder("WO001", "M01");
			system.startWorkOrder("WO001");
			system.completeWorkOrder("WO001");

			bool result = system.cancelWorkOrder("WO001");

			cout << "Expected result: false\n";
			cout << "Actual result  : " << result << '\n';

			printWorkOrderAndEquipmentStatus(
				system,
				"WO001",
				"M01"
			);

			cout << "Expected WorkOrder status: COMPLETED\n";
			cout << "Expected Equipment status: STOPPED\n\n";
		}

		return 0;
}