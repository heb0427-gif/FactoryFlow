#pragma once
#include <string>
#include "FactorySystem.h"

class LotMenu {
private:
	FactorySystem& system;

	void printMenu() const;
	void printLot(const Lot& lot) const;
	void handleRegisterLot();
	bool convertMenuNumberToLotType(int menuNumber, LotType& type) const;
	void handleDisplayAllLots() const;
	void handleDisplayLot() const;
	void handleAddLotRelation() const;
	void handleTraceBackward() const;
	void handleTraceForward() const;
	void handleDisplayRelations() const;
	int readInteger(const std::string& message) const;

public:
	LotMenu(FactorySystem& system);
	void run();
};