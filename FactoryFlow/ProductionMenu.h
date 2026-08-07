#pragma once
#include "ProductionEvent.h"
#include "FactorySystem.h"
#include <string>

class ProductionMenu {
private:
	FactorySystem& system;

	void printMenu() const;
	void handleRegisterProductionEvent() const;
	void handleDisplayPendingEventCount() const;
	void handleProcessNextEvent();
	void handleProcessAllEvents();
	void printProductionEvent(const ProductionEvent& event) const;
	void handleDisplayProcessedEvents() const;
	void handleDisplayRejectedEvents() const;
	void handleDisplayFactoryProductionSummary() const;
	int readInteger(const std::string& message) const;

public:
	ProductionMenu(FactorySystem& system);
	void run();
};