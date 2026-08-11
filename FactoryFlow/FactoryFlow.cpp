#include <iostream>
#include <string>
#include <vector>
#include "FactorySystem.h"

using namespace std;

int passedCount = 0;
int failedCount = 0;


// ================================================
// bool 결과 검사
// ================================================

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


// ================================================
// int 결과 검사
// ================================================

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


// ================================================
// vector<string> 결과 검사
// ================================================

void checkVector(
	const string& description,
	const vector<string>& actual,
	const vector<string>& expected) {

	bool passed = (actual == expected);

	cout << description << "\n";

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
	cout << "Result  : " << (passed ? "PASS" : "FAIL") << "\n\n";

	if (passed)
		passedCount++;
	else
		failedCount++;
}


void printSection(const string& title) {
	cout << "\n==================================================\n";
	cout << title << "\n";
	cout << "==================================================\n\n";
}


int main() {

	cout << boolalpha;

	printSection("FactoryFlow v0.4.0 - LOT Traceability Integration Test");

	FactorySystem system;


	// ==================================================
	// 1. 설비 등록
	//
	// M01
	// M02
	// ==================================================

	checkBool(
		1,
		"Register equipment M01",
		system.registerEquipment("M01", "Machine 01"),
		true
	);

	checkBool(
		2,
		"Register equipment M02",
		system.registerEquipment("M02", "Machine 02"),
		true
	);


	// ==================================================
	// 2. 작업지시 등록
	//
	// WO001 -> M01
	// WO002 -> M02
	//
	// WO003은 설비 미배정 테스트용
	// ==================================================

	checkBool(
		3,
		"Create work order WO001",
		system.createWorkOrder(
			"WO001",
			"SEMI-PRODUCT",
			100,
			WorkOrderPriority::NORMAL
		),
		true
	);

	checkBool(
		4,
		"Create work order WO002",
		system.createWorkOrder(
			"WO002",
			"FIN-PRODUCT",
			100,
			WorkOrderPriority::NORMAL
		),
		true
	);

	checkBool(
		5,
		"Create work order WO003 without equipment",
		system.createWorkOrder(
			"WO003",
			"TEST-PRODUCT",
			100,
			WorkOrderPriority::NORMAL
		),
		true
	);


	// ==================================================
	// 3. 설비 배정
	// ==================================================

	checkBool(
		6,
		"Assign M01 to WO001",
		system.assignEquipmentToWorkOrder("WO001", "M01"),
		true
	);

	checkBool(
		7,
		"Assign M02 to WO002",
		system.assignEquipmentToWorkOrder("WO002", "M02"),
		true
	);


	// ==================================================
	// 4. LOT 등록
	//
	// RAW-001
	// RAW-002
	// RAW-003
	// SEMI-001
	// FIN-001
	// ==================================================

	checkBool(
		8,
		"Register RAW-001",
		system.registerLot(
			"RAW-001",
			"STEEL-A",
			LotType::RAW_MATERIAL,
			100
		),
		true
	);

	checkBool(
		9,
		"Register RAW-002",
		system.registerLot(
			"RAW-002",
			"STEEL-B",
			LotType::RAW_MATERIAL,
			100
		),
		true
	);

	checkBool(
		10,
		"Register RAW-003",
		system.registerLot(
			"RAW-003",
			"RESIN-A",
			LotType::RAW_MATERIAL,
			100
		),
		true
	);

	checkBool(
		11,
		"Register SEMI-001",
		system.registerLot(
			"SEMI-001",
			"SEMI-A",
			LotType::SEMI_FINISHED,
			50
		),
		true
	);

	checkBool(
		12,
		"Register FIN-001",
		system.registerLot(
			"FIN-001",
			"FIN-A",
			LotType::FINISHED,
			50
		),
		true
	);


	// ==================================================
	// 5. LOT 관계 생성
	//
	// RAW-001 ─┐
	//          ├── WO001 / M01 ─→ SEMI-001
	// RAW-002 ─┘
	//
	// SEMI-001 ─┐
	//           ├── WO002 / M02 ─→ FIN-001
	// RAW-003  ─┘
	// ==================================================

	printSection("LOT Relation Creation");

	checkBool(
		13,
		"RAW-001 -> SEMI-001 using WO001 / M01",
		system.addLotRelation(
			"RAW-001",
			"SEMI-001",
			"WO001",
			30
		),
		true
	);

	checkBool(
		14,
		"RAW-002 -> SEMI-001 using WO001 / M01",
		system.addLotRelation(
			"RAW-002",
			"SEMI-001",
			"WO001",
			20
		),
		true
	);

	checkBool(
		15,
		"SEMI-001 -> FIN-001 using WO002 / M02",
		system.addLotRelation(
			"SEMI-001",
			"FIN-001",
			"WO002",
			40
		),
		true
	);

	checkBool(
		16,
		"RAW-003 -> FIN-001 using WO002 / M02",
		system.addLotRelation(
			"RAW-003",
			"FIN-001",
			"WO002",
			10
		),
		true
	);


	// 관계 총 4건 확인
	checkInt(
		"LOT relation count",
		static_cast<int>(system.getLotRelations().size()),
		4
	);


	// ==================================================
	// 6. 역추적
	//
	// FIN-001
	//   ↓
	// SEMI-001
	//   ↓
	// RAW-001
	// RAW-002
	//
	// FIN-001
	//   ↓
	// RAW-003
	//
	// 최종:
	// SEMI-001
	// RAW-001
	// RAW-002
	// RAW-003
	// ==================================================

	printSection("Backward Trace Tests");

	vector<string> backwardFin =
		system.traceBackward("FIN-001");

	vector<string> backwardFinExpected = {
		"SEMI-001",
		"RAW-001",
		"RAW-002",
		"RAW-003"
	};

	checkVector(
		"traceBackward(\"FIN-001\")",
		backwardFin,
		backwardFinExpected
	);


	// ==================================================
	// 7. RAW-001 정방향 추적
	//
	// RAW-001
	//   ↓
	// SEMI-001
	//   ↓
	// FIN-001
	// ==================================================

	printSection("Forward Trace Tests");

	vector<string> forwardRaw001 =
		system.traceForward("RAW-001");

	vector<string> forwardRaw001Expected = {
		"SEMI-001",
		"FIN-001"
	};

	checkVector(
		"traceForward(\"RAW-001\")",
		forwardRaw001,
		forwardRaw001Expected
	);


	// ==================================================
	// 8. RAW-003 정방향 추적
	//
	// RAW-003
	//   ↓
	// FIN-001
	// ==================================================

	vector<string> forwardRaw003 =
		system.traceForward("RAW-003");

	vector<string> forwardRaw003Expected = {
		"FIN-001"
	};

	checkVector(
		"traceForward(\"RAW-003\")",
		forwardRaw003,
		forwardRaw003Expected
	);


	// ==================================================
	// 거부 테스트
	// ==================================================

	printSection("LOT Registration Rejection Tests");


	// 9. 중복 LOT
	checkBool(
		17,
		"Duplicate LOT ID",
		system.registerLot(
			"RAW-001",
			"OTHER",
			LotType::RAW_MATERIAL,
			100
		),
		false
	);


	// 10. 빈 LOT ID
	checkBool(
		18,
		"Empty LOT ID",
		system.registerLot(
			"",
			"ITEM-A",
			LotType::RAW_MATERIAL,
			100
		),
		false
	);


	// 11. 빈 품목 코드
	checkBool(
		19,
		"Empty item code",
		system.registerLot(
			"RAW-999",
			"",
			LotType::RAW_MATERIAL,
			100
		),
		false
	);


	// 12. 수량 0
	checkBool(
		20,
		"LOT quantity 0",
		system.registerLot(
			"RAW-998",
			"ITEM-X",
			LotType::RAW_MATERIAL,
			0
		),
		false
	);


	// ==================================================
	// 관계 생성 거부 테스트
	// ==================================================

	printSection("LOT Relation Rejection Tests");


	// 13. 존재하지 않는 input LOT
	checkBool(
		21,
		"Nonexistent input LOT",
		system.addLotRelation(
			"NOT-FOUND",
			"FIN-001",
			"WO002",
			10
		),
		false
	);


	// 14. 존재하지 않는 output LOT
	checkBool(
		22,
		"Nonexistent output LOT",
		system.addLotRelation(
			"RAW-001",
			"NOT-FOUND",
			"WO001",
			10
		),
		false
	);


	// 15. 존재하지 않는 WorkOrder
	checkBool(
		23,
		"Nonexistent WorkOrder",
		system.addLotRelation(
			"RAW-001",
			"FIN-001",
			"WO999",
			10
		),
		false
	);


	// 16. input / output 동일
	checkBool(
		24,
		"Same input and output LOT",
		system.addLotRelation(
			"RAW-001",
			"RAW-001",
			"WO001",
			10
		),
		false
	);


	// 17. 사용 수량 0
	checkBool(
		25,
		"Used quantity 0",
		system.addLotRelation(
			"RAW-001",
			"FIN-001",
			"WO001",
			0
		),
		false
	);


	// 18. 설비가 없는 WO003
	checkBool(
		26,
		"WorkOrder without assigned equipment",
		system.addLotRelation(
			"RAW-001",
			"FIN-001",
			"WO003",
			10
		),
		false
	);


	// ==================================================
	// 없는 LOT 추적 테스트
	// ==================================================

	printSection("Invalid LOT Trace Tests");


	vector<string> nonexistentBackward =
		system.traceBackward("LOT-NOT-FOUND");

	vector<string> emptyExpected;

	checkVector(
		"Backward trace nonexistent LOT",
		nonexistentBackward,
		emptyExpected
	);


	vector<string> nonexistentForward =
		system.traceForward("LOT-NOT-FOUND");

	checkVector(
		"Forward trace nonexistent LOT",
		nonexistentForward,
		emptyExpected
	);


	// ==================================================
	// 추가 정상 결과
	//
	// RAW 원재료는 이전 LOT가 없으므로
	// 역추적 결과가 빈 벡터여야 함.
	// ==================================================

	vector<string> rawBackward =
		system.traceBackward("RAW-001");

	checkVector(
		"Backward trace RAW-001 - no previous LOT",
		rawBackward,
		emptyExpected
	);


	// ==================================================
	// 관계 정보 추가 검증
	// ==================================================

	printSection("LOT Relation Data Verification");

	const vector<LotRelation>& relations =
		system.getLotRelations();

	if (relations.size() >= 4) {

		checkInt(
			"First relation ID",
			relations[0].getRelationId(),
			1
		);

		checkBool(
			27,
			"First relation equipment must be M01",
			relations[0].getEquipmentId() == "M01",
			true
		);

		checkBool(
			28,
			"Third relation equipment must be M02",
			relations[2].getEquipmentId() == "M02",
			true
		);
	}
	else {
		cout << "Relation data verification cannot run.\n";
		cout << "Expected at least 4 relations.\n\n";

		failedCount += 3;
	}


	// ==================================================
	// 최종 결과
	// ==================================================

	printSection("Test Summary");

	cout << "Passed checks: " << passedCount << "\n";
	cout << "Failed checks: " << failedCount << "\n";

	if (failedCount == 0) {
		cout << "Overall result: ALL TESTS PASSED\n";
	}
	else {
		cout << "Overall result: TEST FAILED\n";
	}

	cout << "\n";

	return failedCount == 0 ? 0 : 1;
}