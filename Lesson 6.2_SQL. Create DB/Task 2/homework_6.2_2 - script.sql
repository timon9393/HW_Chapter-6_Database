CREATE TABLE IF NOT EXISTS Employee (
	id SERIAL PRIMARY KEY,
	emp_name VARCHAR(30) NOT NULL,
	department VARCHAR(50) NOT NULL,
	manager_id INTEGER,
	FOREIGN KEY (manager_id) REFERENCES Employee(id)
);