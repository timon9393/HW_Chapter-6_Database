#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>

//#pragma execution_character_set( "utf-8" )     // �������� ��� �������� ��� ������

int main()
{
	//setlocale(LC_ALL, "ru");    // ��� ��� ������ �� �����
	SetConsoleCP(CP_UTF8);    // ����� �������� 1251 �� ��������.
	SetConsoleOutputCP(CP_UTF8);  // ����� �������� 1251 �� ��������.
	// setvbuf(stdout, nullptr, _IOFBF, 1000);    // ��������, ��� �������� ��� ������

	try
	{
		std::cout << "Hello!" << std::endl;

		pqxx::connection conn(
			"host=127.0.0.1 "
			"port=5432 "
			"dbname=Lesson_6.5 "
			"user=postgres "
			"password=215041993"
		);

		std::cout << "Connected!!!" << std::endl;

		pqxx::transaction tr{ conn };   // pqxx::transaction ���������� pqxx::work  // ������� ����������
		
		//   ********
		//   ������ � SELECT-��������� ����� �++
		//   ********

		auto result = tr.query <int, std::string, std::string>("SELECT * FROM book;");  //������ �� ��������� ������ �� �� // ���������� �������� // �� ���� �������� ������
		// � "" ������� SQL ������. ����� � "" ������ ���� ������� �������, ����.: SELECT title, author FROM book WHERE id > 0 ORDER BY author LIMIT 2

		std::tuple <int, std::string, double> t = { 10, "hello", 1.5 };    // ������
		std::cout << std::get<0>(t) << std::endl;   // ���������� � ��������� ������� ����� ����� std::get<>()

		for (const std::tuple<int, std::string, std::string>& record : result)  // ����� �������� ���:  for (auto [title, author] : result) � ������ ��� get<> ����������� title � author, �� ��� ������ �� ������. ��� ���������� ���������� �������
		{
			std::cout << "ID: " << std::get<0>(record);                     //
			std::cout << " with Title: " << std::get<1>(record);            //   ��������� ������ �� ������� � ��
			std::cout << " and Author: " << std::get<2>(record);            //
			std::cout << std::endl;                                         //
		}

		// ���� ��� ����� ������ ���� ����, ��� ��������, ����� ������������ std::query_value<>(), � �� ����� �������� ����� ���:
		std::string titleOfId3 = tr.query_value<std::string>("SELECT title FROM book WHERE id=3 LIMIT 1;");
		std::cout << "Title of book id_3 is: " << titleOfId3 << std::endl;

		//   ********
		//   ������ � INSERT � UPDATE-��������� ����� �++
		//   ********

		//    �������� �������������� ����� ����������. 3 ����� (�� ������� INSERT-�������):
		// 1. pqxx::work tx { c };                  ������ ������ ����������. ����� ����� std::work, ����� ����� std::transaction
		// 2. tx.exec ("INSERT INTO book.....");    ��������� ������ ����� .exec
		// 3. tx.commit();                          ��������� ������ � ���������� �����������. ���� ��� �� �������, ������ � �� �� ��������.

		tr.exec("INSERT INTO book(title, author) VALUES('Tom Soyer', 'Mark Twain');");
		std::cout << "Transaction in progress..." << std::endl;

		tr.commit();
		std::cout << "Transaction complited!" << std::endl;


		//   ********
		//   ������ �� ����� SQL Injection
		//   ********

		// ��� �������� ���� ���� ���� �������� ������������ ���� ��� ���������� ������������� �����.
		std::string newTitle, newAuthor;
		int a;
		std::cout << "������� �������� ����� � ������: " << std::endl;
		std::cin >> newTitle >> newAuthor >> a;     // ������������ ����� ��� �������� ������ � ��
		pqxx::work tx{ conn };
		/*tx.exec("INSERT INTO book(title, author) "
			"VALUES('" + newTitle + "', '" + newAuthor + "')");
		tx.commit();*/

		// ������ ������ ���������� �� ����� - ������������� ������
		tx.exec("INSERT INTO book(title, author, id) "
			"VALUES('" + tx.esc(newTitle) + "', '" + newAuthor + "', " + std::to_string(a) + ");");  // ����������� ���������� � tx.esc()
		tx.commit();
		
		// ������ ������ - ����������� ������ ������� SQL (Prepared statement), ����� ����� ���������� � ���� ������ ������
		//conn.prepare("insert_book", "INSERT INTO book(title, author) VALUES($1, $2"); // � ������ "" ��������� �������� �������. ������ - ����� �������, ��� ������ $1 � $2 ����� ������������� ����������.
		// ����� ������� �������������� ����� tx.exec_prepared()
		//tx.exec_prepared("insert_book", newTitle, newAuthor); // � ������ ������ $1 � $2 ����������� ���������� newTitle � newAuthor
		// tx.commit();


	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	// �����!!! ���������� ����� ���� ������ ����. ���� ����� � ���� �������� � ����������� ������������, �� �� ����� ����� � ��������� {} (����� � ������ ����� try{})

		

	
}