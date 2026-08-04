#include <iostream>
#include "WorkOrderMenu.h"
#include <limits>
using namespace std;

// 생성자
WorkOrderMenu::WorkOrderMenu(FactorySystem& system) : system(system) { }

void WorkOrderMenu::printMenu() const {
	cout << "================================\n";
	cout << "	FactoryFlow v0.2\n";
	cout << "================================\n";
	cout << "1. Create Work Order\n";
	cout << "2. View All Work Orders\n";
	cout << "3. View Work Order Details\n";
	cout << "4. Assign Equipment\n";
	cout << "5. Start Work Order\n";
	cout << "6. Pause Work Order\n";
	cout << "7. Resume Work Order\n";
	cout << "8. Complete Work Order\n";
	cout << "9. Cancel Work Order\n";
	cout << "0. Back\n\n";
}

void WorkOrderMenu::printWorkOrder(const WorkOrder& workOrder) const {
	// 이미 찾은 workOrder 객체를 화면에 출력만 하는 함수

	cout << "Work Order ID: " << workOrder.getId() << "\n";
	cout << "Product Code: " << workOrder.getProductCode() << "\n";
	cout << "Target Quantity: " << workOrder.getTargetQuantity() << "\n";
	cout << "Priority: " << workOrderPriorityToString(workOrder.getPriority()) << "\n";
	
	if (!workOrder.hasAssignedEquipment())
		cout << "Assigned Equipment: NONE" << "\n";
	else
		cout << "Assigned Equipment: " << workOrder.getAssignedEquipmentId() << "\n";

	cout << "Status: " << workOrderStatusToString(workOrder.getStatus()) << "\n\n";
}

void WorkOrderMenu::handleCreateWorkOrder() {
	string workOrderId, productCode;

	cout << "Work Order ID: ";
	getline(cin, workOrderId);

	cout << "Product Code: ";
	getline(cin, productCode);

	int targetQuantity = readInteger("Target Quantity: ");

	cout << "\n\n";

	WorkOrderPriority priority;

	while (true) {

		int priorityNumber = readInteger("1. LOW\n2. NORMAL\n3. HIGH\n\nPriority: ");

		if (!convertMenuNumberToPriority(priorityNumber, priority)) {
			cout << "Invalid menu number.\n\n";
			continue;
		}

		break;
	}

	if (!system.createWorkOrder(workOrderId, productCode, targetQuantity,
		priority)) {
		cout << "Unable to register the Work Order. Please check the Id, product code, etc.\n\n";
		return;
	}

	cout << "Work Order created.\n\n";
}

void WorkOrderMenu::handleDisplayWorkOrder() const {

	string workOrderId;
	cout << "Enter Work Order ID: ";
	cin >> workOrderId;

	const WorkOrder* workOrder = system.findWorkOrder(workOrderId);
	// 객체를 얻음

	if (workOrder == nullptr) {
		cout << "Work Order not found.\n\n";
		return;
	}

	printWorkOrder(*workOrder);
}

void WorkOrderMenu::handleDisplayAllWorkOrders() const {
	const vector<WorkOrder>& workOrders = system.getWorkOrders();

	if (workOrders.empty()) {
		cout << "There're no registered Work Orders.\n\n";
		return;
	}

	for (size_t i = 0; i < workOrders.size(); i++)
		printWorkOrder(workOrders[i]);
}

int WorkOrderMenu::readInteger(const std::string& message) const {
	int integer;

	while (true) {
		cout << message;

		if (cin >> integer) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return integer;
		}

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Error! Enter an Integer.\n\n";
	}
}

bool WorkOrderMenu::convertMenuNumberToPriority(int menuNumber, 
	WorkOrderPriority& priority) const {

	switch (menuNumber) {
	case 1:
		priority = WorkOrderPriority::LOW;
		return true;
	case 2:
		priority = WorkOrderPriority::NORMAL;
		return true;
	case 3:
		priority = WorkOrderPriority::HIGH;
		return true;
	default:
		return false;
	}
}

void WorkOrderMenu::run() {
	while (true) {
		printMenu();

		int  menuNumber = readInteger("Select menu: ");

		switch (menuNumber) {
		case 1:
			handleCreateWorkOrder(); break;
		case 2:
			handleDisplayAllWorkOrders(); break;
		case 3:
			handleDisplayWorkOrder(); break;
		case 0:
			return;
		default:
			cout << "Invalid menu number! Enter a number from 0 to 3.\n\n";
		}
	}
}

void WorkOrderMenu::handleAssignEquipment() {
	string workOrderId, equipmentId;

	cout << "Enter Work Order id: ";
	getline(cin, workOrderId);

	cout << "Enter Equipment id: ";
	getline(cin, equipmentId);

	if (!system.assignEquipmentToWorkOrder(workOrderId, equipmentId)) {
		cout << "Failed to assign Equipment to Work Order.\n\n";
		return;
	}

	cout << "Equipment assigned.\n\n";

}

void WorkOrderMenu::handleStartWorkOrder() {
	string workOrderId;

	cout << "Enter Work Order id: ";
	getline(cin, workOrderId);

	if (!system.startWorkOrder(workOrderId)) {
		cout << "Failed to start Work Order.\n\n";
		return;
	}

	cout << "Work order started.\n\n";
}

void WorkOrderMenu::handlePauseWorkOrder() {
	string workOrderId;

	cout << "Enter Work Order id: ";
	getline(cin, workOrderId);

	if (!system.pauseWorkOrder(workOrderId)) {
		cout << "Failed to pause Work Order.\n\n";
		return;
	}

	cout << "Work order paused.\n\n";
}

void WorkOrderMenu::handleResumeWorkOrder() {
	string workOrderId;

	cout << "Enter Work Order id: ";
	getline(cin, workOrderId);

	if (!system.resumeWorkOrder(workOrderId)) {
		cout << "Failed to resume Work Order.\n\n";
		return;
	}

	cout << "Work order resumed.\n\n";
}

void WorkOrderMenu::handleCompleteWorkOrder() {
	string workOrderId;

	cout << "Enter Work Order id: ";
	getline(cin, workOrderId);

	if (!system.completeWorkOrder(workOrderId)) {
		cout << "Failed to complete Work Order.\n\n";
		return;
	}

	cout << "Work order completed.\n\n";
}

void WorkOrderMenu::handleCancelWorkOrder() {
	string workOrderId;

	cout << "Enter Work Order id: ";
	getline(cin, workOrderId);

	if (!system.cancelWorkOrder(workOrderId)) {
		cout << "Failed to cancel Work Order.\n\n";
		return;
	}

	cout << "Work order cancelled.\n\n";
}

