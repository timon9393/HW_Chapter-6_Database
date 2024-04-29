#include <iostream>
#include <Windows.h>
#include <pqxx/pqxx>
#include <iomanip>


class ManageDB
{
public:
	ManageDB(pqxx::connection& conn) : conn(conn) {};

	bool isExists_id(int id = 0)
	{
		pqxx::transaction tr{ conn };
		auto result = tr.exec("SELECT id FROM clients WHERE id =  " + std::to_string(id) + " LIMIT 1;");
		if (result.empty())
			return false;
		else
			return true;
	}

	int whose_tel(int id = 0)
	{
		pqxx::transaction tr{ conn };
		auto result = tr.exec("SELECT id_client FROM tel_numbers WHERE tel =  '" + tr.esc(tel) + "' AND id_client = " + std::to_string(id) + " LIMIT 1;");
		if (result.empty())
		{
			result = tr.exec("SELECT id_client FROM tel_numbers WHERE tel =  '" + tr.esc(tel) + "' LIMIT 1;");
			if (result.empty())
				return 0;
			else
				return result.at(0)[0].as<int>();
		}
		else
			return result.at(0)[0].as<int>();
	}

	void createDB()
	{
		pqxx::transaction tr{ conn };
		try
		{
			tr.exec("CREATE TABLE public.clients"
				" (id SERIAL PRIMARY KEY NOT NULL,"
				" name VARCHAR(15) NOT NULL,"
				" last_name VARCHAR(15) NOT NULL,"
				" email VARCHAR(30) NOT NULL);"
			);
			tr.exec("CREATE TABLE public.tel_numbers"
				" (id SERIAL PRIMARY KEY NOT NULL,"
				" id_client INTEGER NOT NULL,"
				" FOREIGN KEY(id_client) REFERENCES clients(id),"
				" tel VARCHAR(15));"
			);
			tr.commit();
			std::cout << "The structure of the \"clients\" database has been successfully created!" << std::endl;
		}
		catch(...){}
	}

	void add_client()
	{
		name = last_name = email = tel = "";
		pqxx::transaction tr{ conn };
		std::cout << "\nEnter name:\t";
		std::cin >> name;
		std::cout << "Enter last name:\t";
		std::cin >> last_name;
		std::cout << "Enter e-mail:\t";
		std::cin >> email;
		std::cout << "Enter phone number:\t";
		std::cin >> tel;

		tr.exec("INSERT INTO clients(name, last_name, email) VALUES"
			" ('" + tr.esc(name) + "',"
			" '" + tr.esc(last_name) + "',"
			" '" + tr.esc(email) + "');"
		);

		if (!tel.empty())
		{
			do
			{
				tr.exec("INSERT INTO tel_numbers (id_client, tel)"
					" SELECT id, '" + tr.esc(tel) + "'"
					" FROM clients WHERE id = (SELECT MAX(id) FROM clients);"
				);
				std::cout << "Enter additional phone number or 'n' to complete:\t";
				std::cin >> tel;
			} 
			while (tel != "n");
		}
		tr.commit();
	}

	void add_tel()
	{
		int id = 0;
		std::cout << "\nEnter client ID whose you want to add a phone number:\t";
		std::cin >> id;
		
		if (!isExists_id(id))
		{
			std::cout << "Client with ID: " << id << " does not exists." << std::endl;
			return;
		}
		else
		{			
			std::cout << "Enter additional phone number for client: " << client_full_name(id) << " with ID = " << id << ": " << std::endl;
			std::cin >> tel;
			pqxx::transaction tr{ conn };
			tr.exec("INSERT INTO tel_numbers (id_client, tel) VALUES"
			" (" + std::to_string(id) + ", '" + tr.esc(tel) + "');"
			);
			tr.commit();
			std::cout << "Phone number has been added successfully!" << std::endl;
		}
	}

	void change_data()
	{
		int id = 0;
		std::cout << "\nEnter client ID whose data you want to edit:\t";
		std::cin >> id;

		if (!isExists_id(id))
		{
			std::cout << "Client with ID: " << id << " does not exists." << std::endl;
			return;
		}
		else
		{
			int index;
			std::cout << "Founded client is: " << client_full_name(id) << std::endl;

			do
			{
				std::cout << "\nWhat type of data you want to change? Type 1, 2, 3, 4 or 0 to exit edit menu" << std::endl;
				std::cout << "1.\tName \n2.\tLast name \n3.\tE-mail \n4.\tPhone number \n\n0.\tExit edit menu" << std::endl;
				std::cin >> index;
				pqxx::transaction tr{ conn };

				switch (index)
				{
				case 1:
					std::cout << "Enter new name:\t";
					std::cin >> name;
					tr.exec("UPDATE clients SET name = '" + tr.esc(name) + "' WHERE id = " + std::to_string(id) + ";");
					tr.commit();
					std::cout << "Name has been updated successfully! Client with ID: " << id << " is " << client_full_name(id) << std::endl;
					break;
				case 2:
					std::cout << "Enter new last name:\t";
					std::cin >> last_name;
					tr.exec("UPDATE clients SET last_name = '" + tr.esc(last_name) + "' WHERE id = " + std::to_string(id) + ";");
					tr.commit();
					std::cout << "Last name has been updated successfully! Client with ID: " << id << " is " << client_full_name(id) << std::endl;
					break;
				case 3:
					std::cout << "Enter new e-mail:\t";
					std::cin >> email;
					tr.exec("UPDATE clients SET email = '" + tr.esc(email) + "' WHERE id = " + std::to_string(id) + ";");
					tr.commit();
					std::cout << "E-mail has been updated successfully! New e-mail for " << client_full_name(id) << " is: " << email << std::endl;
					break;
				case 4:
					tr.commit();
					edit_tel(id);
					break;
				case 0:
					break;
				default:
					std::cout << "Unknown command: " << index << "\tType 1, 2, 3, 4 or 0 to exit edit menu" << std::endl;
					break;
				}

			} while (index != 0);
		}
	}

	void edit_tel(int& id)
	{
		std::cout << "Enter phone number to edit (old number): ";
		std::cin >> tel;
		int tel_owner = whose_tel(id);
		if (tel_owner == 0)
		{
			std::cout << "Client with phone number: " << tel << " does not exists." << std::endl;
			return;
		}
		else if (tel_owner == id)
		{
			pqxx::transaction tr{ conn };
			std::string new_tel;
			std::cout << "Enter new phone number:\t";
			std::cin >> new_tel;
			tr.exec("UPDATE tel_numbers SET tel = '" + tr.esc(new_tel) + "' WHERE tel = '" + tr.esc(tel) + "' AND id_client = " + std::to_string(id) + ";");
			tr.commit();
			std::cout << "Phone number: " << tel << " for client " << client_full_name(id) << " has been successfully changed to : " << new_tel << std::endl;
			tel = new_tel;
		}
		else
		{
			std::cout << "Phone number: " << tel << " does not belong to client: " << client_full_name(id) << ". It is owned by the client: " << client_full_name(tel_owner) << " with ID: " << tel_owner << std::endl;
		}
	}

	void delete_phone()
	{
		int id = 0;
		std::cout << "\nEnter client ID whose phone number you want to delete:\t";
		std::cin >> id;

		if (!isExists_id(id))
		{
			std::cout << "Client with ID: " << id << " does not exists." << std::endl;
			return;
		}
		else
		{
			bool isOne;
			std::cout << "Founded client is: " << client_full_name(id) << std::endl;
			std::cout << "Do you want to delete all the phone numbers of this client or just one? Enter 1 or 0" << std::endl;
			std::cout << "1.\tOne phone number\n0.\tAll phone numbers" << std::endl;
			std::cin >> isOne;
			
			if (isOne)
			{
				std::cout << "Enter phone number you want to delete: ";
				std::cin >> tel;
				int tel_owner = whose_tel(id);
				if (tel_owner == 0)
				{
					std::cout << "Client with phone number: " << tel << " does not exists." << std::endl;
					return;
				}
				else if (tel_owner == id)
				{
					pqxx::transaction tr{ conn };
					tr.exec("DELETE FROM tel_numbers WHERE tel = '" + tr.esc(tel) + "' AND id_client = " + std::to_string(id) + ";");
					tr.commit();
					std::cout << "Phone number: " << tel << " has been successfully deleted for client " << client_full_name(id) << std::endl;
				}
				else
				{
					std::cout << "Phone number: " << tel << " does not belong to client: " << client_full_name(id) << ". It is owned by the client: " << client_full_name(tel_owner) << " with ID: " << tel_owner << std::endl;
				}
			}
			else
			{
				pqxx::transaction tr{ conn };
				tr.exec("DELETE FROM tel_numbers WHERE id_client = " + std::to_string(id) + ";");
				tr.commit();
				std::cout << "All phone numbers for client " << client_full_name(id) << " with ID: " << id << " have been successfully deleted!" << std::endl;
			}
		}
	}

	void delete_client()
	{
		int id = 0;
		std::cout << "\nEnter client ID you want to delete:\t";
		std::cin >> id;

		if (!isExists_id(id))
		{
			std::cout << "Client with ID: " << id << " does not exists." << std::endl;
			return;
		}
		else
		{
			pqxx::transaction tr{ conn };
			if (!(tr.exec("SELECT id_client FROM tel_numbers WHERE id_client =  " + std::to_string(id) + " LIMIT 1;").empty()))
			{
				tr.exec("DELETE FROM tel_numbers WHERE id_client = " + std::to_string(id) + ";");
			}
			tr.exec("DELETE FROM clients WHERE id = " + std::to_string(id) + ";");
			tr.commit();
			std::cout << "Client " << client_full_name(id) << " with ID: " << id << " has been successfully deleted!" << std::endl;
		}
	}

	void find_client()
	{
		int index;
		std::cout << "\nWhat do you know about client? Enter: 1, 2, 3 or 4" << std::endl;
		std::cout << "1.\tName\n2.\tLast name\n3.\tE-mail\n4.\tPhone number" << std::endl;
		std::cin >> index;
		pqxx::transaction tr{ conn };

		switch (index)
		{
		case 1:
			std::cout << "Enter name: ";
			std::cin >> name;
			if (!(tr.exec("SELECT id FROM clients WHERE name = '" + tr.esc(name) + "' LIMIT 1;").empty()))
			{
				std::cout << "Founded clients with name \"" << name << "\":" << std::endl;
				auto result = tr.query <int, std::string, std::string, std::string, std::string>("SELECT c.id, c.name, c.last_name, c.email, COALESCE(t.tel, '-') FROM clients c"
																								" LEFT JOIN tel_numbers t ON c.id = t.id_client"
																								" WHERE c.name = '" + tr.esc(name) + "';");
				std::cout << std::left << std::setw(10) << "ID" << std::left << std::setw(15) << "Name" << std::left << std::setw(15) << "Last name" << std::left << std::setw(30) << "E-mail" << std::left << std::setw(15) << "Phone number" << std::endl;
				for (const std::tuple<int, std::string, std::string, std::string, std::string>& record : result)
				{
					std::cout << std::left << std::setw(10) << std::get<0>(record) << std::left << std::setw(15) << std::get<1>(record) << std::left << std::setw(15) << std::get<2>(record) << std::left << std::setw(30) << std::get<3>(record) << std::left << std::setw(15) << std::get<4>(record) << std::endl;
				}
			}
			else
			{
				std::cout << "Client with name \"" << name << "\" is not found." << std::endl;
			}
			break;
		case 2:
			std::cout << "Enter last name: ";
			std::cin >> last_name;
			if (!(tr.exec("SELECT id FROM clients WHERE last_name =  '" + tr.esc(last_name) + "' LIMIT 1;").empty()))
			{
				std::cout << "Founded clients with last name \"" << last_name << "\":" << std::endl;
				auto result = tr.query <int, std::string, std::string, std::string, std::string>("SELECT c.id, c.name, c.last_name, c.email, COALESCE(t.tel, '-') FROM clients c"
																								" LEFT JOIN tel_numbers t ON c.id = t.id_client"
																								" WHERE c.last_name = '" + tr.esc(last_name) + "';");
				std::cout << std::left << std::setw(10) << "ID" << std::left << std::setw(15) << "Name" << std::left << std::setw(15) << "Last name" << std::left << std::setw(30) << "E-mail" << std::left << std::setw(15) << "Phone number" << std::endl;
				for (const std::tuple<int, std::string, std::string, std::string, std::string>& record : result)
				{
					std::cout << std::left << std::setw(10) << std::get<0>(record) << std::left << std::setw(15) << std::get<1>(record) << std::left << std::setw(15) << std::get<2>(record) << std::left << std::setw(30) << std::get<3>(record) << std::left << std::setw(15) << std::get<4>(record) << std::endl;
				}
			}
			else
			{
				std::cout << "Client with last name \"" << last_name << "\" is not found." << std::endl;
			}
			break;
		case 3:
			std::cout << "Enter e-mail: ";
			std::cin >> email;
			if (!(tr.exec("SELECT id FROM clients WHERE email =  '" + tr.esc(email) + "' LIMIT 1;").empty()))
			{
				std::cout << "Founded clients with e-mail \"" << email << "\":" << std::endl;
				auto result = tr.query <int, std::string, std::string, std::string, std::string>("SELECT c.id, c.name, c.last_name, c.email, COALESCE(t.tel, '-') FROM clients c"
																								" LEFT JOIN tel_numbers t ON c.id = t.id_client"
																								" WHERE c.email = '" + tr.esc(email) + "';");
				std::cout << std::left << std::setw(10) << "ID" << std::left << std::setw(15) << "Name" << std::left << std::setw(15) << "Last name" << std::left << std::setw(30) << "E-mail" << std::left << std::setw(15) << "Phone number" << std::endl;
				for (const std::tuple<int, std::string, std::string, std::string, std::string>& record : result)
				{
					std::cout << std::left << std::setw(10) << std::get<0>(record) << std::left << std::setw(15) << std::get<1>(record) << std::left << std::setw(15) << std::get<2>(record) << std::left << std::setw(30) << std::get<3>(record) << std::left << std::setw(15) << std::get<4>(record) << std::endl;
				}
			}
			else
			{
				std::cout << "Client with e-mail \"" << email << "\" is not found." << std::endl;
			}
			break;
		case 4:
			std::cout << "Enter phone number: ";
			std::cin >> tel;
			if (!(tr.exec("SELECT id FROM tel_numbers WHERE tel =  '" + tr.esc(tel) + "' LIMIT 1;").empty()))
			{
				std::cout << "Founded clients with phone number \"" << tel << "\":" << std::endl;
				auto result = tr.query <int, std::string, std::string, std::string, std::string>("SELECT c.id, c.name, c.last_name, c.email, COALESCE(t.tel, '-') FROM clients c"
																								" LEFT JOIN tel_numbers t ON c.id = t.id_client"
																								" WHERE t.tel = '" + tr.esc(tel) + "';");
				std::cout << std::left << std::setw(10) << "ID" << std::left << std::setw(15) << "Name" << std::left << std::setw(15) << "Last name" << std::left << std::setw(30) << "E-mail" << std::left << std::setw(15) << "Phone number" << std::endl;
				for (const std::tuple<int, std::string, std::string, std::string, std::string>& record : result)
				{
					std::cout << std::left << std::setw(10) << std::get<0>(record) << std::left << std::setw(15) << std::get<1>(record) << std::left << std::setw(15) << std::get<2>(record) << std::left << std::setw(30) << std::get<3>(record) << std::left << std::setw(15) << std::get<4>(record) << std::endl;
				}
			}
			else
			{
				std::cout << "Client with phone number \"" << tel << "\" is not found." << std::endl;
			}
			break;
		default:
			std::cout << "Unknown command." << std::endl;
			break;
		}
	}

	void printDB()
	{
		pqxx::transaction tr{ conn };
		std::cout << "\nFull database:\n" << std::endl;
		std::cout << "\033[1m" << std::left << std::setw(10) << "ID" << std::left << std::setw(15) << "Name" << std::left << std::setw(15) << "Last name" << std::left << std::setw(30) << "E-mail" << std::left << std::setw(15) << "Phone number" << "\033[0m" << std::endl << std::endl;
		auto result = tr.query <int, std::string, std::string, std::string, std::string>("SELECT c.id, c.name, c.last_name, c.email, COALESCE(t.tel, '-') FROM clients c"
																						" LEFT JOIN tel_numbers t ON c.id = t.id_client;");
		for (const std::tuple<int, std::string, std::string, std::string, std::string>& record : result)
		{
			std::cout << std::left << std::setw(10) << std::get<0>(record) << std::left << std::setw(15) << std::get<1>(record) << std::left << std::setw(15) << std::get<2>(record) << std::left << std::setw(30) << std::get<3>(record) << std::left << std::setw(15) << std::get<4>(record) << std::endl;
		}
	}

	std::string client_full_name(int& id)
	{
		pqxx::transaction tr{ conn };
		std::string full_name;
		auto res = tr.query<std::string, std::string>("SELECT name, last_name FROM clients WHERE id = " + std::to_string(id) + " LIMIT 1;");
		for (auto [name, last_name] : res)
		{
			full_name = name + " " + last_name;
		}
		return full_name;
	}

private:
	std::string name, last_name, email, tel;
	pqxx::connection& conn;
};


int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	try
	{
		pqxx::connection conn(
			"host=127.0.0.1 "
			"port=5432 "
			"dbname=homework_6.5 "
			"user=postgres "
			"password=215041993"
		);

		ManageDB client(conn);
		
		int command;
		std::cout << "Hello! Type '1' to create database, or '0' to exit" << std::endl;
		std::cout << "1. Create a database or connect to an already existing one\n0. Exit" << std::endl;
		std::cin >> command;

		if(command != 1){}
		else
		{
			client.createDB();
			do
			{
				std::cout << "\n\"CLIENTS\" DATABASE MENU. Choose any action:" << std::endl;
				std::cout << "1. Add new client\n2. Add phone number for an existing client\n3. Edit client data\n4. Delete phone number for an existing client\n5. Delete existing client from database\n6. Find client\n7. Print full database\n\n0. Exit" << std::endl;
				std::cin >> command;

				switch (command)
				{
				case 1:
					client.add_client();
					break;
				case 2:
					client.add_tel();
					break;
				case 3:
					client.change_data();
					break;
				case 4:
					client.delete_phone();
					break;
				case 5:
					client.delete_client();
					break;
				case 6:
					client.find_client();
					break;
				case 7:
					client.printDB();
					break;
				case 0:
					std::cout << "Good bye!" << std::endl;
					break;
				default:
					std::cout << "Unknown command!" << std::endl;
					break;
				}

			} while (command != 0);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}