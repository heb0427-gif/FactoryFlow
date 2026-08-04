#pragma once
#include "FactorySystem.h"

class WorkOrderMenu {
private :
	FactorySystem& system;
	void printMenu() const;
	void printWorkOrder(const WorkOrder& workOrder) const;
	void handleCreateWorkOrder();
	void handleDisplayWorkOrder() const;
	void handleDisplayAllWorkOrders() const;
	int readInteger(const std::string& message) const;
	bool convertMenuNumberToPriority(int menuNumber, WorkOrderPriority& priority) const;

	void handleAssignEquipment();
	void handleStartWorkOrder();
	void handlePauseWorkOrder();
	void handleResumeWorkOrder();
	void handleCompleteWorkOrder();
	void handleCancelWorkOrder();

public:
	WorkOrderMenu(FactorySystem& system); // 생성자
	void run();
};