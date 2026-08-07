#include "ProductionMenu.h"
#include "ProductionEvent.h"
#include "FactorySystem.h"
#include <iostream>
#include <limits>
using namespace std;

ProductionMenu::ProductionMenu(FactorySystem& system) : system(system) {}

void ProductionMenu::printMenu() const {
	cout << "================================\n";
	cout << "	FactoryFlow v0.3\n";
	cout << "================================\n";
	cout << "1. Register Production Event\n";
	cout << "2. View Pending Event Count\n";
	cout << "3. Process Next Event\n";
	cout << "4. Process All Events\n";
	cout << "5. View Processed Events\n";
	cout << "6. View Rejected Events\n";
	cout << "7. View Factory Production Summary\n";
	cout << "0. Back\n\n";
}

void ProductionMenu::handleRegisterProductionEvent() const {
	string workOrderId, equipmentId;

	cout << "[Register Production Event]\n\n";

	cout << "Work Order ID: ";
	cin >> workOrderId;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Equipment ID: ";
	getline(cin, equipmentId);

	int produced = readInteger("Produced Quantity: ");
	int defects = readInteger("Defect Quantity: ");


	if (!system.enqueueProductionEvent(workOrderId, equipmentId, produced, defects)) {
		cout << "Unable to register the Production Event.\n\n";
		return;
	}
	cout << "Production Event registered.\n\n";
}

void ProductionMenu::handleDisplayPendingEventCount() const {
	cout << "Pending event count : " << system.getPendingProductionEventCount() << "\n\n";
}

void ProductionMenu::handleProcessNextEvent() {
	if (!system.processNextProductionEvent()) {
		cout << "Failed to process production event.\n\n";
		return;
	}
	cout << "Production event processed.\n\n";
}

void ProductionMenu::handleProcessAllEvents() {
	// 대기 중인 이벤트가 몇 개 있는지 카운트
	int pendingCountBefore = system.getPendingProductionEventCount();

	int rejectedCountBefore = system.getRejectedProductionEvents().size();

	int processedCount = system.processAllProductionEvents(); // 처리 완료된 건수 리턴

	int rejectedCountAfter = system.getRejectedProductionEvents().size();

	int pendingCountAfter = system.getPendingProductionEventCount();

	cout << "Processed event count : " << processedCount << "\n";
	cout << "Rejected event count : " << rejectedCountAfter - rejectedCountBefore << "\n";
	cout << "Pending event count : " << pendingCountAfter << "\n";
}

void ProductionMenu::printProductionEvent(const ProductionEvent& event) const {
	cout << "Event ID: " << event.getEventId() << "\n";
	cout << "Work Order ID: " << event.getWorkOrderId() << "\n";
	cout << "Equipment ID: " << event.getEquipmentId() << "\n";
	cout << "Produced: " << event.getProducedQuantity() << "\n";
	cout << "Pass: " << event.getPassQuantity() << "\n";
	cout << "Defect: " << event.getDefectQuantity() << "\n\n";
}

void ProductionMenu::handleDisplayProcessedEvents() const {

	const vector<ProductionEvent>& processedProductionEvents = system.getProcessedProductionEvents();

	if (processedProductionEvents.empty()) {
		cout << "There's no processed production events.\n\n";
		return;
	}

	for (int i = 0; i < processedProductionEvents.size(); i++) {
		printProductionEvent(processedProductionEvents[i]);
	}
}

void ProductionMenu::handleDisplayRejectedEvents() const {
	const vector<ProductionEvent>& rejectedProductionEvents = system.getRejectedProductionEvents();

	if (rejectedProductionEvents.empty()) {
		cout << "There's no rejected production events.\n\n";
		return;
	}

	for (int i = 0; i < rejectedProductionEvents.size(); i++) {
		printProductionEvent(rejectedProductionEvents[i]);
	}
}

void ProductionMenu::handleDisplayFactoryProductionSummary() const {
	cout << "[Factory Production Summary]\n\n";

	cout << "Total Produced: "
		<< system.getTotalProducedQuantity() << "\n";

	cout << "Total Pass: "
		<< system.getTotalPassQuantity() << "\n";

	cout << "Total Defect: "
		<< system.getTotalDefectQuantity() << "\n";

	cout << "Defect Rate: "
		<< system.getOverallDefectRate() << "%\n";

	cout << "Pending Events: "
		<< system.getPendingProductionEventCount() << "\n";

	cout << "Processed Events: "
		<< system.getProcessedProductionEvents().size() << "\n";

	cout << "Rejected Events: "
		<< system.getRejectedProductionEvents().size() << "\n\n";
}

int ProductionMenu::readInteger(const string& message) const {
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

void ProductionMenu::run() {
	while (true) {
		printMenu();

		int  menuNumber = readInteger("Select menu: ");

		switch (menuNumber) {
		case 1:
			handleRegisterProductionEvent(); break;
		case 2:
			handleDisplayPendingEventCount(); break;
		case 3:
			handleProcessNextEvent(); break;
		case 4:
			handleProcessAllEvents(); break;
		case 5:
			handleDisplayProcessedEvents(); break;
		case 6:
			handleDisplayRejectedEvents(); break;
		case 7:
			handleDisplayFactoryProductionSummary(); break;
		case 0:
			return;
		default:
			cout << "Invalid menu number! Enter a number from 0 to 7.\n\n";
		}
	}
}