#include <iostream>
#include <string>

#include "FactorySystem.h"

using namespace std;

int passedCount = 0;
int failedCount = 0;

void printTestResult(
	int testNumber,
	const string& description,
	bool actualResult,
	bool expectedResult
) {
	bool passed = actualResult == expectedResult;

	cout << "[" << testNumber << "] "
		<< description << "\n";

	cout << "Expected: " << boolalpha << expectedResult << "\n";
	cout << "Actual  : " << boolalpha << actualResult << "\n";
	cout << "Result  : " << (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

void printWorkOrderStatusResult(
	const FactorySystem& system,
	const string& workOrderId,
	WorkOrderStatus expectedStatus
) {
	const WorkOrder* workOrder = system.findWorkOrder(workOrderId);

	if (workOrder == nullptr) {
		cout << "WorkOrder " << workOrderId
			<< " was not found.\n";
		cout << "Status check: FAIL\n\n";

		failedCount++;
		return;
	}

	WorkOrderStatus actualStatus = workOrder->getStatus();
	bool passed = actualStatus == expectedStatus;

	cout << "WorkOrder " << workOrderId << " status\n";
	cout << "Expected: "
		<< workOrderStatusToString(expectedStatus) << "\n";
	cout << "Actual  : "
		<< workOrderStatusToString(actualStatus) << "\n";
	cout << "Result  : "
		<< (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

void printEquipmentStatusResult(
	const FactorySystem& system,
	const string& equipmentId,
	EquipmentStatus expectedStatus
) {
	const Equipment* equipment = system.findEquipment(equipmentId);

	if (equipment == nullptr) {
		cout << "Equipment " << equipmentId
			<< " was not found.\n";
		cout << "Status check: FAIL\n\n";

		failedCount++;
		return;
	}

	EquipmentStatus actualStatus = equipment->getStatus();
	bool passed = actualStatus == expectedStatus;

	cout << "Equipment " << equipmentId << " status\n";
	cout << "Expected: "
		<< equipmentStatusToString(expectedStatus) << "\n";
	cout << "Actual  : "
		<< equipmentStatusToString(actualStatus) << "\n";
	cout << "Result  : "
		<< (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}

void printSection(const string& title) {
	cout << "==================================================\n";
	cout << title << "\n";
	cout << "==================================================\n\n";
}

int main() {
	cout << boolalpha;

	printSection("FactoryFlow v0.2.0 Integration Test");

	FactorySystem system;

	// --------------------------------------------------
	// 기본 통합 테스트 1
	// M01 설비 등록
	// --------------------------------------------------
	bool test1 = system.registerEquipment(
		"M01",
		"Equipment 01"
	);

	printTestResult(
		1,
		"Register equipment M01",
		test1,
		true
	);

	// --------------------------------------------------
	// 기본 통합 테스트 2
	// WO001 생성 -> WAITING
	// --------------------------------------------------
	bool test2 = system.createWorkOrder(
		"WO001",
		"PART-A",
		100,
		WorkOrderPriority::HIGH
	);

	printTestResult(
		2,
		"Create work order WO001",
		test2,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO001",
		WorkOrderStatus::WAITING
	);

	// --------------------------------------------------
	// 기본 통합 테스트 3
	// WO001에 M01 배정 -> READY
	// --------------------------------------------------
	bool test3 = system.assignEquipmentToWorkOrder(
		"WO001",
		"M01"
	);

	printTestResult(
		3,
		"Assign M01 to WO001",
		test3,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO001",
		WorkOrderStatus::READY
	);

	printEquipmentStatusResult(
		system,
		"M01",
		EquipmentStatus::STOPPED
	);

	// --------------------------------------------------
	// 기본 통합 테스트 4
	// WO001 시작 -> 둘 다 RUNNING
	// --------------------------------------------------
	bool test4 = system.startWorkOrder("WO001");

	printTestResult(
		4,
		"Start WO001",
		test4,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO001",
		WorkOrderStatus::RUNNING
	);

	printEquipmentStatusResult(
		system,
		"M01",
		EquipmentStatus::RUNNING
	);

	// --------------------------------------------------
	// 기본 통합 테스트 5
	// WO001 일시정지 -> 둘 다 PAUSED
	// --------------------------------------------------
	bool test5 = system.pauseWorkOrder("WO001");

	printTestResult(
		5,
		"Pause WO001",
		test5,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO001",
		WorkOrderStatus::PAUSED
	);

	printEquipmentStatusResult(
		system,
		"M01",
		EquipmentStatus::PAUSED
	);

	// --------------------------------------------------
	// 기본 통합 테스트 6
	// WO001 재개 -> 둘 다 RUNNING
	// --------------------------------------------------
	bool test6 = system.resumeWorkOrder("WO001");

	printTestResult(
		6,
		"Resume WO001",
		test6,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO001",
		WorkOrderStatus::RUNNING
	);

	printEquipmentStatusResult(
		system,
		"M01",
		EquipmentStatus::RUNNING
	);

	// --------------------------------------------------
	// 기본 통합 테스트 7
	// WO001 완료 -> WorkOrder COMPLETED, M01 STOPPED
	// --------------------------------------------------
	bool test7 = system.completeWorkOrder("WO001");

	printTestResult(
		7,
		"Complete WO001",
		test7,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO001",
		WorkOrderStatus::COMPLETED
	);

	printEquipmentStatusResult(
		system,
		"M01",
		EquipmentStatus::STOPPED
	);

	// --------------------------------------------------
	// 기본 통합 테스트 8
	// 완료 작업 재시작 -> 실패
	// --------------------------------------------------
	bool test8 = system.startWorkOrder("WO001");

	printTestResult(
		8,
		"Restart completed WO001",
		test8,
		false
	);

	// --------------------------------------------------
	// 기본 통합 테스트 9
	// WO002에 M01 배정 -> 성공
	// --------------------------------------------------
	system.createWorkOrder(
		"WO002",
		"PART-B",
		200,
		WorkOrderPriority::NORMAL
	);

	bool test9 = system.assignEquipmentToWorkOrder(
		"WO002",
		"M01"
	);

	printTestResult(
		9,
		"Assign released M01 to WO002",
		test9,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO002",
		WorkOrderStatus::READY
	);

	// --------------------------------------------------
	// 기본 통합 테스트 10
	// WO003에도 M01 배정 -> 실패
	// --------------------------------------------------
	system.createWorkOrder(
		"WO003",
		"PART-C",
		300,
		WorkOrderPriority::LOW
	);

	bool test10 = system.assignEquipmentToWorkOrder(
		"WO003",
		"M01"
	);

	printTestResult(
		10,
		"Assign active M01 to WO003",
		test10,
		false
	);

	printWorkOrderStatusResult(
		system,
		"WO003",
		WorkOrderStatus::WAITING
	);

	// --------------------------------------------------
	// 기본 통합 테스트 11
	// WO002 취소 -> CANCELLED, M01 STOPPED
	// --------------------------------------------------
	bool test11 = system.cancelWorkOrder("WO002");

	printTestResult(
		11,
		"Cancel WO002",
		test11,
		true
	);

	printWorkOrderStatusResult(
		system,
		"WO002",
		WorkOrderStatus::CANCELLED
	);

	printEquipmentStatusResult(
		system,
		"M01",
		EquipmentStatus::STOPPED
	);

	// --------------------------------------------------
	// 기본 통합 테스트 12
	// 존재하지 않는 설비 배정 -> 실패
	// --------------------------------------------------
	bool test12 = system.assignEquipmentToWorkOrder(
		"WO003",
		"M99"
	);

	printTestResult(
		12,
		"Assign nonexistent M99 to WO003",
		test12,
		false
	);

	// --------------------------------------------------
	// 기본 통합 테스트 13
	// 존재하지 않는 작업 시작 -> 실패
	// --------------------------------------------------
	bool test13 = system.startWorkOrder("WO999");

	printTestResult(
		13,
		"Start nonexistent WO999",
		test13,
		false
	);

	// --------------------------------------------------
	// 기본 통합 테스트 14
	// 목표 수량 0으로 생성 -> 실패
	// --------------------------------------------------
	bool test14 = system.createWorkOrder(
		"WO004",
		"PART-D",
		0,
		WorkOrderPriority::NORMAL
	);

	printTestResult(
		14,
		"Create WO004 with target quantity 0",
		test14,
		false
	);

	// --------------------------------------------------
	// 기본 통합 테스트 15
	// 중복 작업지시 ID 생성 -> 실패
	// --------------------------------------------------
	bool test15 = system.createWorkOrder(
		"WO001",
		"PART-X",
		500,
		WorkOrderPriority::HIGH
	);

	printTestResult(
		15,
		"Create duplicate work order WO001",
		test15,
		false
	);

	// ==================================================
	// 추가 상태 테스트
	// 서로 영향을 주지 않도록 별도 FactorySystem 사용
	// ==================================================
	printSection("Additional State Tests");

	// --------------------------------------------------
	// 추가 테스트 16
	// ERROR 상태 설비 배정 -> 실패
	// --------------------------------------------------
	{
		FactorySystem errorSystem;

		errorSystem.registerEquipment(
			"M10",
			"Error Equipment"
		);

		errorSystem.createWorkOrder(
			"WO010",
			"PART-ERROR",
			100,
			WorkOrderPriority::HIGH
		);

		// STOPPED -> ERROR는 금지되어 있으므로
		// STOPPED -> RUNNING -> ERROR 경로 사용
		errorSystem.changeEquipmentStatus(
			"M10",
			EquipmentStatus::RUNNING
		);

		errorSystem.changeEquipmentStatus(
			"M10",
			EquipmentStatus::ERROR
		);

		bool test16 =
			errorSystem.assignEquipmentToWorkOrder(
				"WO010",
				"M10"
			);

		printTestResult(
			16,
			"Assign ERROR equipment M10",
			test16,
			false
		);

		printEquipmentStatusResult(
			errorSystem,
			"M10",
			EquipmentStatus::ERROR
		);
	}

	// --------------------------------------------------
	// 추가 테스트 17
	// PAUSED 상태 설비 신규 배정 -> 실패
	// --------------------------------------------------
	{
		FactorySystem pausedSystem;

		pausedSystem.registerEquipment(
			"M20",
			"Paused Equipment"
		);

		pausedSystem.createWorkOrder(
			"WO020",
			"PART-PAUSED",
			100,
			WorkOrderPriority::NORMAL
		);

		// STOPPED -> PAUSED는 금지이므로
		// STOPPED -> RUNNING -> PAUSED
		pausedSystem.changeEquipmentStatus(
			"M20",
			EquipmentStatus::RUNNING
		);

		pausedSystem.changeEquipmentStatus(
			"M20",
			EquipmentStatus::PAUSED
		);

		bool test17 =
			pausedSystem.assignEquipmentToWorkOrder(
				"WO020",
				"M20"
			);

		printTestResult(
			17,
			"Assign PAUSED equipment M20",
			test17,
			false
		);
	}

	// --------------------------------------------------
	// 추가 테스트 18
	// WAITING 상태에서 완료 -> 실패
	// --------------------------------------------------
	{
		FactorySystem waitingSystem;

		waitingSystem.createWorkOrder(
			"WO030",
			"PART-WAITING",
			100,
			WorkOrderPriority::LOW
		);

		bool test18 =
			waitingSystem.completeWorkOrder("WO030");

		printTestResult(
			18,
			"Complete WAITING work order WO030",
			test18,
			false
		);

		printWorkOrderStatusResult(
			waitingSystem,
			"WO030",
			WorkOrderStatus::WAITING
		);
	}

	// --------------------------------------------------
	// 추가 테스트 19
	// READY 상태에서 일시정지 -> 실패
	// --------------------------------------------------
	{
		FactorySystem readySystem;

		readySystem.registerEquipment(
			"M40",
			"Ready Equipment"
		);

		readySystem.createWorkOrder(
			"WO040",
			"PART-READY",
			100,
			WorkOrderPriority::NORMAL
		);

		readySystem.assignEquipmentToWorkOrder(
			"WO040",
			"M40"
		);

		bool test19 =
			readySystem.pauseWorkOrder("WO040");

		printTestResult(
			19,
			"Pause READY work order WO040",
			test19,
			false
		);

		printWorkOrderStatusResult(
			readySystem,
			"WO040",
			WorkOrderStatus::READY
		);

		printEquipmentStatusResult(
			readySystem,
			"M40",
			EquipmentStatus::STOPPED
		);
	}

	// --------------------------------------------------
	// 추가 테스트 20
	// CANCELLED 상태 작업지시에 재배정 -> 실패
	// --------------------------------------------------
	{
		FactorySystem cancelledSystem;

		cancelledSystem.registerEquipment(
			"M50",
			"Equipment 50"
		);

		cancelledSystem.registerEquipment(
			"M51",
			"Equipment 51"
		);

		cancelledSystem.createWorkOrder(
			"WO050",
			"PART-CANCELLED",
			100,
			WorkOrderPriority::HIGH
		);

		cancelledSystem.assignEquipmentToWorkOrder(
			"WO050",
			"M50"
		);

		cancelledSystem.cancelWorkOrder("WO050");

		bool test20 =
			cancelledSystem.assignEquipmentToWorkOrder(
				"WO050",
				"M51"
			);

		printTestResult(
			20,
			"Reassign equipment to CANCELLED WO050",
			test20,
			false
		);

		printWorkOrderStatusResult(
			cancelledSystem,
			"WO050",
			WorkOrderStatus::CANCELLED
		);
	}

	// ==================================================
	// 최종 결과
	// ==================================================
	printSection("Test Summary");

	cout << "Passed checks: " << passedCount << "\n";
	cout << "Failed checks: " << failedCount << "\n";
	cout << "Overall result: "
		<< (failedCount == 0 ? "ALL TESTS PASSED" : "TEST FAILED")
		<< "\n";

	return failedCount == 0 ? 0 : 1;
}