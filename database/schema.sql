CREATE TABLE equipment (
    equipment_id VARCHAR(50) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    status VARCHAR(20) NOT NULL,
    total_produced INTEGER NOT NULL DEFAULT 0,
    total_defect INTEGER NOT NULL DEFAULT 0,
    updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE work_order (
	work_order_id VARCHAR(50) PRIMARY KEY,
	product_code VARCHAR(50) NOT NULL,
	target_quantity INTEGER NOT NULL,
	produced_quantity INTEGER NOT NULL DEFAULT 0,
	defect_quantity INTEGER NOT NULL DEFAULT 0,
	status VARCHAR(20) NOT NULL,
	equipment_id VARCHAR(50),
		FOREIGN KEY (equipment_id)
			REFERENCES equipment(equipment_id)
);

CREATE TABLE lot (
	 lot_id VARCHAR(50) PRIMARY KEY,
	item_code VARCHAR(50) NOT NULL,
	type VARCHAR(20) NOT NULL,
	quantity INTEGER NOT NULL
);

CREATE TABLE telemetry (
	id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
	equipment_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (equipment_id)
			REFERENCES equipment(equipment_id),
	work_order_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (work_order_id)
			REFERENCES work_order(work_order_id),
	timestamp TIMESTAMP NOT NULL,
	temperature DOUBLE PRECISION NOT NULL,
	vibration DOUBLE PRECISION NOT NULL,
	production_count INTEGER NOT NULL,
	defect_count INTEGER NOT NULL,
	lot_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (lot_id)
			REFERENCES lot(lot_id)
);


CREATE TABLE lot_relation (
	id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
	input_lot_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (input_lot_id)
			REFERENCES lot(lot_id),
	output_lot_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (output_lot_id)
			REFERENCES lot(lot_id),
	work_order_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (work_order_id)
			REFERENCES work_order(work_order_id),
	equipment_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (equipment_id)
    			REFERENCES equipment(equipment_id),
	used_quantity INTEGER NOT NULL
);
	
CREATE TABLE alarm (
	id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
	equipment_id VARCHAR(50) NOT NULL,
		FOREIGN KEY (equipment_id)
    			REFERENCES equipment(equipment_id),
	temperature DOUBLE PRECISION NOT NULL,
	message VARCHAR(200) NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);