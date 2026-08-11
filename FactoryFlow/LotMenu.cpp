#include "LotMenu.h"
#include "Lot.h"
#include "FactorySystem.h"
#include <iostream>
#include <limits>
using namespace std;

LotMenu::LotMenu(FactorySystem& system) : system(system) {}

void LotMenu::printMenu() const {
	cout << "[LOT Traceability]\n\n";
	cout << "1. Register LOT\n";
	cout << "2. View All LOTs\n";
	cout << "3. View LOT Details\n";
	cout << "4. Add LOT Relation\n";
	cout << "5. Trace Backward\n";
	cout << "6. Trace Forward\n";
	cout << "7. View LOT Relations\n";
	cout << "0. Back\n\n";
}

void LotMenu::printLot(const Lot& lot) const {
	cout << "LOT ID: " << lot.getId() << "\n";
	cout << "Item Code: " << lot.getItemCode() << "\n";
	cout << "Type: " << lot.lotTypeToString(lot.getType()) << "\n";
	cout << "Quantity: " << lot.getQuantity() << "\n\n";
}

void LotMenu::handleRegisterLot() {
	string lotId, itemCode;
	LotType lotType;

	cout << "[Register Lot]\n\n";

	cout << "LOT ID: ";
	cin >> lotId;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Item Code: ";
	getline(cin, itemCode);

	int typeNumber = readInteger(
		"1. RAW_MATERIAL\n"
		"2. SEMI_FINISHED\n"
		"3. FINISHED\n\n"
		"LOT Type: "
	);

	if (!convertMenuNumberToLotType(typeNumber, lotType)) {
		cout << "Invalid LOT type.\n\n";
		return;
	}

	int quantity = readInteger("Quantity: ");

	if (!system.registerLot(lotId, itemCode, lotType, quantity)) {
		cout << "Failed to register LOT.\n\n";
		return;
	}
	cout << "LOT registered.\n\n";
}

bool LotMenu::convertMenuNumberToLotType(int menuNumber, LotType& type) const {

	switch (menuNumber) {
	case 1:
		type = LotType::RAW_MATERIAL;
		return true;
	case 2:
		type = LotType::SEMI_FINISHED;
		return true;
	case 3:
		type = LotType::FINISHED;
		return true;
	default:
		return false;
	}
}

void LotMenu::handleDisplayAllLots() const {
	// getLots()를 이용해 모든 LOT를 출력

	const vector<Lot>& lots = system.getLots(); 

	if (lots.empty()) {
		cout << "There're no registered lots.\n\n";
		return;
	}

	for (size_t i = 0; i < lots.size(); i++)
		printLot(lots[i]);
}

void LotMenu::handleDisplayLot() const { // LOT 한개 출력
	string id;
	cout << "Enter LOT ID: ";
	cin >> id;

	const Lot* lotPtr = system.findLot(id);

	if (lotPtr == nullptr) {
		cout << "LOT not found.\n\n";
		return;
	}

	printLot(*lotPtr); // 주소가 가리키는 실제 객체를 얻음(역참조)
}

void LotMenu::handleAddLotRelation() const {
	string inputLotId, outputLotId, workOrderId;

	cout << "Input LOT ID: ";
	cin >> inputLotId;

	cout << "Output LOT ID: ";
	cin >> outputLotId;

	cout << "Work Order ID: ";
	cin >> workOrderId;

	int usedQuantity = readInteger("Used Quantity: ");

	if (!system.addLotRelation(
		inputLotId,
		outputLotId,
		workOrderId,
		usedQuantity)) {

		cout << "Failed to add LOT relation.\n\n";
		return;
	}

	cout << "LOT relation added.\n\n";
}

void LotMenu::handleTraceBackward() const {
	string lotId;

	cout << "Backward Trace: ";
	cin >> lotId;

	vector<string> result = system.traceBackward(lotId);

	cout << "\n";

	if (result.empty()) {
		cout << "No previous LOTs found.\n\n";
		return;
	}

	for (size_t i = 0; i < result.size(); i++) {
		cout << result[i] << "\n";
	}

	cout << "\n";
}

void LotMenu::handleTraceForward() const {
	string lotId;

	cout << "Forward Trace: ";
	cin >> lotId;
	vector<string> result = system.traceForward(lotId);

	cout << "\n";

	if (result.empty()) {
		cout << "No affected LOTs found.\n\n";
		return;
	}

	for (size_t i = 0; i < result.size(); i++) {
		cout << result[i] << "\n";
	}

	cout << "\n";
}

void LotMenu::handleDisplayRelations() const {

	const vector<LotRelation>& lotRelations = system.getLotRelations();

	if (lotRelations.empty()) {
		cout << "There're no LOT relations.\n\n";
		return;
	}

	for (size_t i = 0; i < lotRelations.size(); i++) {
		cout << "Relation ID: " << lotRelations[i].getRelationId() << "\n";
		cout << "Input LOT: " << lotRelations[i].getInputLotId() << "\n";
		cout << "Output LOT: " << lotRelations[i].getOutputLotId() << "\n";
		cout << "Work Order: " << lotRelations[i].getWorkOrderId() << "\n";
		cout << "Equipment: " << lotRelations[i].getEquipmentId() << "\n";
		cout << "Used Quantity: " << lotRelations[i].getUsedQuantity() << "\n\n";
	}
}

int LotMenu::readInteger(const string& message) const {
	int number;

	while (true) {
		cout << message;

		if (cin >> number) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return number;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Error! Enter an Integer.\n\n";
	}
}

void LotMenu::run() {
	while (true) {
		printMenu();

		int menuNumber = readInteger("Select menu: ");

		switch (menuNumber) {
		case 1:
			handleRegisterLot();
			break;

		case 2:
			handleDisplayAllLots();
			break;

		case 3:
			handleDisplayLot();
			break;

		case 4:
			handleAddLotRelation();
			break;

		case 5:
			handleTraceBackward();
			break;

		case 6:
			handleTraceForward();
			break;

		case 7:
			handleDisplayRelations();
			break;

		case 0:
			return;

		default:
			cout << "Invalid menu number! Enter a number from 0 to 7.\n\n";
		}
	}
}