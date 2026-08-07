#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "FactorySystem.h"

using namespace std;

int passedCount = 0;
int failedCount = 0;

void printSection(const string& title) {
	cout << "\n==================================================\n";
	cout << title << "\n";
	cout << "==================================================\n\n";
}

void checkBool(
	int number,
	const string& description,
	bool actual,
	bool expected) {

	bool passed = (actual == expected);

	cout << "[" << number << "] " << description << "\n";
	cout << "Expected: " << boolalpha << expected << "\n";
	cout << "Actual  : " << boolalpha << actual << "\n";
	cout << "Result  : " << (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

void checkInt(
	const string& description,
	int actual,
	int expected) {

	bool passed = (actual == expected);

	cout << description << "\n";
	cout << "Expected: " << expected << "\n";
	cout << "Actual  : " << actual << "\n";
	cout << "Result  : " << (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

void checkFloat(
	const string& description,
	float actual,
	float expected,
	float epsilon = 0.01f) {

	bool passed = fabs(actual - expected) < epsilon;

	cout << description << "\n";
	cout << "Expected: " << expected << "\n";
	cout << "Actual  : " << actual << "\n";
	cout << "Result  : " << (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

void checkWorkOrderStatus(
	const FactorySystem& system,
	const string& workOrderId,
	WorkOrderStatus expected) {

	const WorkOrder* workOrder = system.findWorkOrder(workOrderId);

	if (workOrder == nullptr) {
		cout << "WorkOrder " << workOrderId << " not found.\n";
		cout << "Result  : FAIL\n\n";

		failedCount++;
		return;
	}

	WorkOrderStatus actual = workOrder->getStatus();
	bool passed = (actual == expected);

	cout << "WorkOrder " << workOrderId << " status\n";
	cout << "Expected: " << workOrderStatusToString(expected) << "\n";
	cout << "Actual  : " << workOrderStatusToString(actual) << "\n";
	cout << "Result  : " << (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

void checkEquipmentStatus(
	const FactorySystem& system,
	const string& equipmentId,
	EquipmentStatus expected) {

	const Equipment* equipment = system.findEquipment(equipmentId);

	if (equipment == nullptr) {
		cout << "Equipment " << equipmentId << " not found.\n";
		cout << "Result  : FAIL\n\n";

		failedCount++;
		return;
	}

	EquipmentStatus actual = equipment->getStatus();
	bool passed = (actual == expected);

	cout << "Equipment " << equipmentId << " status\n";
	cout << "Expected: " << equipmentStatusToString(expected) << "\n";
	cout << "Actual  : " << equipmentStatusToString(actual) << "\n";
	cout << "Result  : " << (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

int main() {
	cout << boolalpha;

	// ==================================================
	// 기본 통합 테스트
	// ==================================================

	printSection("FactoryFlow v0.3.0 - Basic Integration Test");

	FactorySystem system;

	// 1. M01 등록 -> STOPPED
	checkBool(
		1,
		"Register M01",
		system.registerEquipment("M01", "Equipment 01"),
		true
	);

	checkEquipmentStatus(
		system,
		"M01",
		EquipmentStatus::STOPPED
	);

	// 2. WO001 목표 20 생성 -> WAITING
	checkBool(
		2,
		"Create WO001, target quantity 20",
		system.createWorkOrder(
			"WO001",
			"PART-A",
			20,
			WorkOrderPriority::HIGH
		),
		true
	);

	checkWorkOrderStatus(
		system,
		"WO001",
		WorkOrderStatus::WAITING
	);

	// 3. M01 배정 -> READY
	checkBool(
		3,
		"Assign M01 to WO001",
		system.assignEquipmentToWorkOrder("WO001", "M01"),
		true
	);

	checkWorkOrderStatus(
		system,
		"WO001",
		WorkOrderStatus::READY
	);

	// 4. 작업 시작 -> 둘 다 RUNNING
	checkBool(
		4,
		"Start WO001",
		system.startWorkOrder("WO001"),
		true
	);

	checkWorkOrderStatus(
		system,
		"WO001",
		WorkOrderStatus::RUNNING
	);

	checkEquipmentStatus(
		system,
		"M01",
		EquipmentStatus::RUNNING
	);

	// 5. 생산 10 / 불량 2 등록 -> pending 1
	checkBool(
		5,
		"Enqueue production 10, defect 2",
		system.enqueueProductionEvent("WO001", "M01", 10, 2),
		true
	);

	checkInt(
		"Pending event count",
		system.getPendingProductionEventCount(),
		1
	);

	// 6. 생산 5 / 불량 1 등록 -> pending 2
	checkBool(
		6,
		"Enqueue production 5, defect 1",
		system.enqueueProductionEvent("WO001", "M01", 5, 1),
		true
	);

	checkInt(
		"Pending event count",
		system.getPendingProductionEventCount(),
		2
	);

	// 7. 다음 이벤트 처리 -> 생산 10 / 불량 2
	checkBool(
		7,
		"Process next production event",
		system.processNextProductionEvent(),
		true
	);

	const WorkOrder* workOrder = system.findWorkOrder("WO001");

	if (workOrder != nullptr) {
		checkInt(
			"WO001 produced quantity after first event",
			workOrder->getProducedQuantity(),
			10
		);

		checkInt(
			"WO001 defect quantity after first event",
			workOrder->getDefectQuantity(),
			2
		);
	}
	else {
		cout << "WO001 not found.\n";
		failedCount += 2;
	}

	// 8. 나머지 이벤트 모두 처리 -> 생산 15 / 불량 3
	int processedCount = system.processAllProductionEvents();

	checkInt(
		"Processed count by processAllProductionEvents",
		processedCount,
		1
	);

	workOrder = system.findWorkOrder("WO001");

	if (workOrder != nullptr) {
		checkInt(
			"WO001 total produced quantity",
			workOrder->getProducedQuantity(),
			15
		);

		checkInt(
			"WO001 total defect quantity",
			workOrder->getDefectQuantity(),
			3
		);
	}

	// 9. 생산 5 / 불량 0 등록 후 처리 -> 목표 20 달성
	checkBool(
		9,
		"Enqueue final production 5, defect 0",
		system.enqueueProductionEvent("WO001", "M01", 5, 0),
		true
	);

	checkBool(
		9,
		"Process final production event",
		system.processNextProductionEvent(),
		true
	);

	// 10. 목표 달성 -> COMPLETED
	checkWorkOrderStatus(
		system,
		"WO001",
		WorkOrderStatus::COMPLETED
	);

	// 11. 설비 -> STOPPED
	checkEquipmentStatus(
		system,
		"M01",
		EquipmentStatus::STOPPED
	);

	workOrder = system.findWorkOrder("WO001");

	// 12. 진행률 100%
	if (workOrder != nullptr) {
		checkFloat(
			"WO001 progress rate",
			workOrder->getProgressRate(),
			100.0f
		);

		// 13. 불량률 3 / 20 = 15%
		checkFloat(
			"WO001 defect rate",
			workOrder->getDefectRate(),
			15.0f
		);
	}

	// 14. 설비 누적 실적 -> 생산 20 / 불량 3
	const Equipment* equipment = system.findEquipment("M01");

	if (equipment != nullptr) {
		checkInt(
			"M01 total produced quantity",
			equipment->getTotalProducedQuantity(),
			20
		);

		checkInt(
			"M01 total defect quantity",
			equipment->getTotalDefectQuantity(),
			3
		);

		checkInt(
			"M01 total pass quantity",
			equipment->getTotalPassQuantity(),
			17
		);

		checkFloat(
			"M01 defect rate",
			equipment->getDefectRate(),
			15.0f
		);
	}

	// 공장 전체 생산 현황도 추가 확인
	checkInt(
		"Factory total produced quantity",
		system.getTotalProducedQuantity(),
		20
	);

	checkInt(
		"Factory total defect quantity",
		system.getTotalDefectQuantity(),
		3
	);

	checkInt(
		"Factory total pass quantity",
		system.getTotalPassQuantity(),
		17
	);

	checkFloat(
		"Factory overall defect rate",
		system.getOverallDefectRate(),
		15.0f
	);

	// ==================================================
	// 등록 거부 테스트
	// ==================================================

	printSection("Production Event Registration Rejection Tests");

	// 존재하지 않는 작업지시
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M10", "Equipment 10");
		testSystem.changeEquipmentStatus(
			"M10",
			EquipmentStatus::RUNNING
		);

		checkBool(
			15,
			"Nonexistent work order",
			testSystem.enqueueProductionEvent(
				"WO999",
				"M10",
				5,
				0
			),
			false
		);
	}

	// 존재하지 않는 설비
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M20", "Equipment 20");
		testSystem.createWorkOrder(
			"WO020",
			"PART-B",
			20,
			WorkOrderPriority::NORMAL
		);

		testSystem.assignEquipmentToWorkOrder("WO020", "M20");
		testSystem.startWorkOrder("WO020");

		checkBool(
			16,
			"Nonexistent equipment",
			testSystem.enqueueProductionEvent(
				"WO020",
				"M999",
				5,
				0
			),
			false
		);
	}

	// 배정된 설비와 다른 설비
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M30", "Equipment 30");
		testSystem.registerEquipment("M31", "Equipment 31");

		testSystem.createWorkOrder(
			"WO030",
			"PART-C",
			20,
			WorkOrderPriority::NORMAL
		);

		testSystem.assignEquipmentToWorkOrder("WO030", "M30");
		testSystem.startWorkOrder("WO030");

		// M31도 RUNNING으로 만들어 설비 상태 때문이 아니라
		// 배정 불일치 때문에 실패하는지 확인
		testSystem.changeEquipmentStatus(
			"M31",
			EquipmentStatus::RUNNING
		);

		checkBool(
			17,
			"Different equipment from assigned equipment",
			testSystem.enqueueProductionEvent(
				"WO030",
				"M31",
				5,
				0
			),
			false
		);
	}

	// WAITING 작업
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M40", "Equipment 40");

		testSystem.createWorkOrder(
			"WO040",
			"PART-D",
			20,
			WorkOrderPriority::LOW
		);

		checkBool(
			18,
			"Production event on WAITING work order",
			testSystem.enqueueProductionEvent(
				"WO040",
				"M40",
				5,
				0
			),
			false
		);
	}

	// READY 작업
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M50", "Equipment 50");

		testSystem.createWorkOrder(
			"WO050",
			"PART-E",
			20,
			WorkOrderPriority::HIGH
		);

		testSystem.assignEquipmentToWorkOrder("WO050", "M50");

		checkBool(
			19,
			"Production event on READY work order",
			testSystem.enqueueProductionEvent(
				"WO050",
				"M50",
				5,
				0
			),
			false
		);
	}

	// PAUSED 작업
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M60", "Equipment 60");

		testSystem.createWorkOrder(
			"WO060",
			"PART-F",
			20,
			WorkOrderPriority::HIGH
		);

		testSystem.assignEquipmentToWorkOrder("WO060", "M60");
		testSystem.startWorkOrder("WO060");
		testSystem.pauseWorkOrder("WO060");

		checkBool(
			20,
			"Production event on PAUSED work order",
			testSystem.enqueueProductionEvent(
				"WO060",
				"M60",
				5,
				0
			),
			false
		);
	}

	// STOPPED 설비
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M70", "Equipment 70");

		testSystem.createWorkOrder(
			"WO070",
			"PART-G",
			20,
			WorkOrderPriority::NORMAL
		);

		testSystem.assignEquipmentToWorkOrder("WO070", "M70");
		testSystem.startWorkOrder("WO070");

		// WorkOrder는 RUNNING으로 둔 채 설비만 STOPPED
		testSystem.changeEquipmentStatus(
			"M70",
			EquipmentStatus::STOPPED
		);

		checkBool(
			21,
			"Production event on STOPPED equipment",
			testSystem.enqueueProductionEvent(
				"WO070",
				"M70",
				5,
				0
			),
			false
		);
	}

	// 잘못된 생산량 / 불량량
	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M80", "Equipment 80");

		testSystem.createWorkOrder(
			"WO080",
			"PART-H",
			20,
			WorkOrderPriority::HIGH
		);

		testSystem.assignEquipmentToWorkOrder("WO080", "M80");
		testSystem.startWorkOrder("WO080");

		checkBool(
			22,
			"Produced quantity 0",
			testSystem.enqueueProductionEvent(
				"WO080",
				"M80",
				0,
				0
			),
			false
		);

		checkBool(
			23,
			"Negative produced quantity",
			testSystem.enqueueProductionEvent(
				"WO080",
				"M80",
				-1,
				0
			),
			false
		);

		checkBool(
			24,
			"Negative defect quantity",
			testSystem.enqueueProductionEvent(
				"WO080",
				"M80",
				5,
				-1
			),
			false
		);

		checkBool(
			25,
			"Defect quantity greater than produced quantity",
			testSystem.enqueueProductionEvent(
				"WO080",
				"M80",
				5,
				6
			),
			false
		);

		checkBool(
			26,
			"Production quantity exceeds target",
			testSystem.enqueueProductionEvent(
				"WO080",
				"M80",
				21,
				0
			),
			false
		);
	}

	// ==================================================
	// 처리 시점 거부 테스트
	// 큐 등록 후 상태가 달라진 경우
	// ==================================================

	printSection("Processing-Time Rejection Test");

	{
		FactorySystem testSystem;

		testSystem.registerEquipment("M90", "Equipment 90");

		testSystem.createWorkOrder(
			"WO090",
			"PART-I",
			20,
			WorkOrderPriority::HIGH
		);

		testSystem.assignEquipmentToWorkOrder("WO090", "M90");
		testSystem.startWorkOrder("WO090");

		// 등록 시점에는 정상
		checkBool(
			27,
			"Enqueue valid event before cancellation",
			testSystem.enqueueProductionEvent(
				"WO090",
				"M90",
				5,
				0
			),
			true
		);

		checkInt(
			"Pending before cancellation",
			testSystem.getPendingProductionEventCount(),
			1
		);

		// 등록 후 작업 취소
		checkBool(
			28,
			"Cancel WO090 after enqueue",
			testSystem.cancelWorkOrder("WO090"),
			true
		);

		// 처리 시점에는 CANCELLED / STOPPED이므로 reject
		checkBool(
			29,
			"Process event after work order cancellation",
			testSystem.processNextProductionEvent(),
			false
		);

		checkInt(
			"Rejected event count",
			static_cast<int>(
				testSystem.getRejectedProductionEvents().size()
				),
			1
		);

		checkInt(
			"Pending event count after rejection",
			testSystem.getPendingProductionEventCount(),
			0
		);

		const WorkOrder* rejectedWorkOrder =
			testSystem.findWorkOrder("WO090");

		if (rejectedWorkOrder != nullptr) {
			checkInt(
				"Rejected event must not change work order production",
				rejectedWorkOrder->getProducedQuantity(),
				0
			);
		}
	}

	// 빈 큐 처리
	{
		FactorySystem emptySystem;

		checkBool(
			30,
			"Process event from empty queue",
			emptySystem.processNextProductionEvent(),
			false
		);
	}

	// ==================================================
	// FIFO 테스트
	// ==================================================

	printSection("FIFO Test");

	{
		FactorySystem fifoSystem;

		fifoSystem.registerEquipment(
			"MF01",
			"FIFO Equipment"
		);

		fifoSystem.createWorkOrder(
			"WOF01",
			"PART-FIFO",
			20,
			WorkOrderPriority::HIGH
		);

		fifoSystem.assignEquipmentToWorkOrder(
			"WOF01",
			"MF01"
		);

		fifoSystem.startWorkOrder("WOF01");

		// ID 1
		checkBool(
			31,
			"FIFO enqueue event 1 - produced 3",
			fifoSystem.enqueueProductionEvent(
				"WOF01",
				"MF01",
				3,
				0
			),
			true
		);

		// ID 2
		checkBool(
			32,
			"FIFO enqueue event 2 - produced 4",
			fifoSystem.enqueueProductionEvent(
				"WOF01",
				"MF01",
				4,
				0
			),
			true
		);

		// ID 3
		checkBool(
			33,
			"FIFO enqueue event 3 - produced 5",
			fifoSystem.enqueueProductionEvent(
				"WOF01",
				"MF01",
				5,
				0
			),
			true
		);

		checkInt(
			"FIFO pending count before processing",
			fifoSystem.getPendingProductionEventCount(),
			3
		);

		checkInt(
			"FIFO processed count",
			fifoSystem.processAllProductionEvents(),
			3
		);

		const vector<ProductionEvent>& history =
			fifoSystem.getProcessedProductionEvents();

		bool fifoPassed = true;

		if (history.size() != 3) {
			fifoPassed = false;
		}
		else {
			if (history[0].getEventId() != 1 ||
				history[0].getProducedQuantity() != 3) {
				fifoPassed = false;
			}

			if (history[1].getEventId() != 2 ||
				history[1].getProducedQuantity() != 4) {
				fifoPassed = false;
			}

			if (history[2].getEventId() != 3 ||
				history[2].getProducedQuantity() != 5) {
				fifoPassed = false;
			}
		}

		checkBool(
			34,
			"FIFO processed order must be 1 -> 2 -> 3",
			fifoPassed,
			true
		);

		if (history.size() == 3) {
			cout << "Actual processing order: "
				<< history[0].getEventId()
				<< " -> "
				<< history[1].getEventId()
				<< " -> "
				<< history[2].getEventId()
				<< "\n\n";
		}
	}

	// ==================================================
	// 최종 결과
	// ==================================================

	printSection("Test Summary");

	cout << "Passed checks: " << passedCount << "\n";
	cout << "Failed checks: " << failedCount << "\n";

	cout << "Overall result: "
		<< (failedCount == 0
			? "ALL TESTS PASSED"
			: "TEST FAILED")
		<< "\n\n";

	cout << "Manual menu tests remaining:\n";
	cout << "- Enter a character instead of a menu number.\n";
	cout << "- Enter a character instead of produced quantity.\n";

	return failedCount == 0 ? 0 : 1;
}