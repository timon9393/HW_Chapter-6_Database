#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>

//#pragma execution_character_set( "utf-8" )     // уточнить что означает эта запись

int main()
{
	//setlocale(LC_ALL, "ru");    // тут это вообше не нужно
	SetConsoleCP(CP_UTF8);    // здесь значение 1251 не подходит.
	SetConsoleOutputCP(CP_UTF8);  // здесь значение 1251 не подходит.
	// setvbuf(stdout, nullptr, _IOFBF, 1000);    // уточнить, что означает эта запись

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

		pqxx::transaction tr{ conn };   // pqxx::transaction аналогично pqxx::work  // создаем транзакцию
		
		//   ********
		//   Работа с SELECT-запросами через С++
		//   ********

		auto result = tr.query <int, std::string, std::string>("SELECT * FROM book;");  //запрос на получение данных из БД // возвращает итератор // по сути образует кортеж
		// в "" обычный SQL запрос. Можно в "" писать даже сложные запросы, напр.: SELECT title, author FROM book WHERE id > 0 ORDER BY author LIMIT 2

		std::tuple <int, std::string, double> t = { 10, "hello", 1.5 };    // кортеж
		std::cout << std::get<0>(t) << std::endl;   // обращаемся к элементам кортежа через метод std::get<>()

		for (const std::tuple<int, std::string, std::string>& record : result)  // можно записать как:  for (auto [title, author] : result) и дальше без get<> подставлять title и author, но так удобно не всегда. Это называется распаковка картежа
		{
			std::cout << "ID: " << std::get<0>(record);                     //
			std::cout << " with Title: " << std::get<1>(record);            //   Получение данных из таблицы в БД
			std::cout << " and Author: " << std::get<2>(record);            //
			std::cout << std::endl;                                         //
		}

		// если нам нужно только одно поле, без картежей, нужно использовать std::query_value<>(), и мы можем написать сразу так:
		std::string titleOfId3 = tr.query_value<std::string>("SELECT title FROM book WHERE id=3 LIMIT 1;");
		std::cout << "Title of book id_3 is: " << titleOfId3 << std::endl;

		//   ********
		//   Работа с INSERT и UPDATE-запросами через С++
		//   ********

		//    Действия осуществляются через транзакции. 3 этапа (на примере INSERT-запроса):
		// 1. pqxx::work tx { c };                  создаём объект транзакции. Можно через std::work, можно через std::transaction
		// 2. tx.exec ("INSERT INTO book.....");    выполняем запрос через .exec
		// 3. tx.commit();                          выполняем коммит и транзакция завершается. Пока его не сделаем, запись в БД не появится.

		tr.exec("INSERT INTO book(title, author) VALUES('Tom Soyer', 'Mark Twain');");
		std::cout << "Transaction in progress..." << std::endl;

		tr.commit();
		std::cout << "Transaction complited!" << std::endl;


		//   ********
		//   Защита от атаки SQL Injection
		//   ********

		// При раскладе ниже есть риск инъекции вредоносного кода при заполнении пользователем формы.
		std::string newTitle, newAuthor;
		int a;
		std::cout << "Введите название книги и автора: " << std::endl;
		std::cin >> newTitle >> newAuthor >> a;     // пользователь может сам добавить записи в БД
		pqxx::work tx{ conn };
		/*tx.exec("INSERT INTO book(title, author) "
			"VALUES('" + newTitle + "', '" + newAuthor + "')");
		tx.commit();*/

		// Первый способ защититься от этого - экранирование данных
		tx.exec("INSERT INTO book(title, author, id) "
			"VALUES('" + tx.esc(newTitle) + "', '" + newAuthor + "', " + std::to_string(a) + ");");  // оборачиваем переменные в tx.esc()
		tx.commit();
		
		// Второй способ - подготовить шаблон запроса SQL (Prepared statement), чтобы потом подставить в него нужные данные
		//conn.prepare("insert_book", "INSERT INTO book(title, author) VALUES($1, $2"); // В первых "" придумали название шаблона. Дальше - текст шаблона, где вместо $1 и $2 будут подставляться переменные.
		// Вызов шаблона осуществляется через tx.exec_prepared()
		//tx.exec_prepared("insert_book", newTitle, newAuthor); // В шаблон вместо $1 и $2 подставятся переменные newTitle и newAuthor
		// tx.commit();


	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	// ВАЖНО!!! Транзакция может быть только одна. Если нужно в коде работать с несколькими транзакциями, то их нужно брать в отдельные {} (можно в разные блоки try{})

		

	
}