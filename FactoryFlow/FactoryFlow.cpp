#include <iostream>
#include <vector>
#include <string>
#include "FactorySystem.h"

using namespace std;

void printResult(
	const string& testName,
	const vector<string>& actual,
	const vector<string>& expected) {

	cout << "\n[" << testName << "]\n";

	cout << "Expected: ";

	if (expected.empty()) {
		cout << "(empty)";
	}
	else {
		for (size_t i = 0; i < expected.size(); i++) {
			cout << expected[i];

			if (i < expected.size() - 1)
				cout << " -> ";
		}
	}

	cout << "\n";

	cout << "Actual  : ";

	if (actual.empty()) {
		cout << "(empty)";
	}
	else {
		for (size_t i = 0; i < actual.size(); i++) {
			cout << actual[i];

			if (i < actual.size() - 1)
				cout << " -> ";
		}
	}

	cout << "\n";

	if (actual == expected)
		cout << "Result  : PASS\n";
	else
		cout << "Result  : FAIL\n";
}


int main() {

	cout << "==================================================\n";
	cout << "FactoryFlow v0.4.0 - LOT Trace Test\n";
	cout << "==================================================\n\n";


	FactorySystem system;


	// ==================================================
	// 1. 테스트에 필요한 설비 등록
	// ==================================================

	cout << "[Setup] Register Equipment M01\n";

	bool equipmentRegistered =
		system.registerEquipment("M01", "Machine 01");

	cout << "Result: "
		<< (equipmentRegistered ? "SUCCESS" : "FAIL")
		<< "\n\n";


	// ==================================================
	// 2. 테스트에 필요한 작업지시 생성
	// ==================================================

	cout << "[Setup] Create Work Order WO001\n";

	bool workOrderCreated =
		system.createWorkOrder(
			"WO001",
			"PRODUCT-A",
			100,
			WorkOrderPriority::NORMAL
		);

	cout << "Result: "
		<< (workOrderCreated ? "SUCCESS" : "FAIL")
		<< "\n\n";


	// ==================================================
	// 3. 작업지시에 설비 배정
	// addLotRelation()에서 배정된 설비가 필요함
	// ==================================================

	cout << "[Setup] Assign M01 to WO001\n";

	bool equipmentAssigned =
		system.assignEquipmentToWorkOrder(
			"WO001",
			"M01"
		);

	cout << "Result: "
		<< (equipmentAssigned ? "SUCCESS" : "FAIL")
		<< "\n\n";


	// ==================================================
	// 4. LOT 등록
	//
	// RAW-001 ─┐
	//          ├→ SEMI-001 ─→ FIN-001
	// RAW-002 ─┘
	//
	// ==================================================

	cout << "[Setup] Register LOTs\n";

	bool raw001Registered =
		system.registerLot(
			"RAW-001",
			"RAW-A",
			LotType::RAW_MATERIAL,
			100
		);

	bool raw002Registered =
		system.registerLot(
			"RAW-002",
			"RAW-B",
			LotType::RAW_MATERIAL,
			100
		);

	bool semi001Registered =
		system.registerLot(
			"SEMI-001",
			"SEMI-A",
			LotType::SEMI_FINISHED,
			100
		);

	bool fin001Registered =
		system.registerLot(
			"FIN-001",
			"FIN-A",
			LotType::FINISHED,
			100
		);

	cout << "RAW-001  : "
		<< (raw001Registered ? "SUCCESS" : "FAIL") << "\n";

	cout << "RAW-002  : "
		<< (raw002Registered ? "SUCCESS" : "FAIL") << "\n";

	cout << "SEMI-001 : "
		<< (semi001Registered ? "SUCCESS" : "FAIL") << "\n";

	cout << "FIN-001  : "
		<< (fin001Registered ? "SUCCESS" : "FAIL") << "\n\n";


	// ==================================================
	// 5. LOT 관계 생성
	//
	// RAW-001 ─┐
	//          ├→ SEMI-001 ─→ FIN-001
	// RAW-002 ─┘
	//
	// ==================================================

	cout << "[Setup] Create LOT Relations\n";

	bool relation1 =
		system.addLotRelation(
			"RAW-001",
			"SEMI-001",
			"WO001",
			30
		);

	bool relation2 =
		system.addLotRelation(
			"RAW-002",
			"SEMI-001",
			"WO001",
			20
		);

	bool relation3 =
		system.addLotRelation(
			"SEMI-001",
			"FIN-001",
			"WO001",
			50
		);

	cout << "RAW-001 -> SEMI-001 : "
		<< (relation1 ? "SUCCESS" : "FAIL") << "\n";

	cout << "RAW-002 -> SEMI-001 : "
		<< (relation2 ? "SUCCESS" : "FAIL") << "\n";

	cout << "SEMI-001 -> FIN-001 : "
		<< (relation3 ? "SUCCESS" : "FAIL") << "\n";


	// ==================================================
	// TEST 1
	// FIN-001 역추적
	//
	// 예상:
	// SEMI-001
	// RAW-001
	// RAW-002
	// ==================================================

	vector<string> backwardResult =
		system.traceBackward("FIN-001");

	vector<string> backwardExpected = {
		"SEMI-001",
		"RAW-001",
		"RAW-002"
	};

	printResult(
		"Backward Trace: FIN-001",
		backwardResult,
		backwardExpected
	);


	// ==================================================
	// TEST 2
	// RAW-001 정방향 추적
	//
	// 예상:
	// SEMI-001
	// FIN-001
	// ==================================================

	vector<string> forwardResult =
		system.traceForward("RAW-001");

	vector<string> forwardExpected = {
		"SEMI-001",
		"FIN-001"
	};

	printResult(
		"Forward Trace: RAW-001",
		forwardResult,
		forwardExpected
	);


	// ==================================================
	// TEST 3
	// RAW-001 역추적
	//
	// RAW-001보다 이전 LOT가 없으므로
	// 빈 벡터가 정상 결과
	// ==================================================

	vector<string> rawBackwardResult =
		system.traceBackward("RAW-001");

	vector<string> rawBackwardExpected;

	printResult(
		"Backward Trace: RAW-001",
		rawBackwardResult,
		rawBackwardExpected
	);


	cout << "\n==================================================\n";
	cout << "LOT Trace Test Finished\n";
	cout << "==================================================\n";

	return 0;
}