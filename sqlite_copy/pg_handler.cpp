/*
 * pg_handler.cpp
 *
 *  Created on: Mar 20, 2020
 *      Author: jamil.zaman
 */

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <vector>
#include <typeinfo>

#include <cstdlib>
#include <memory>
#include <cxxabi.h>

std::string demangle(const char* name) {

    int status = -4; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : name ;
}

using namespace std;
using namespace pqxx;
//template <typename T> std::string type_name();

int create_table (int argc, char* argv[]) {
	std::string sql;

	   try {
	      connection C("dbname = test_db user = postgres password = postgres \
	      hostaddr = 127.0.0.1 port = 9432");
	      if (C.is_open()) {
	         cout << "Opened database successfully: " << C.dbname() << endl;
	      } else {
	         cout << "Can't open database" << endl;
	         return 1;
	      }

	      /* Create SQL statement */
	      sql = "CREATE TABLE COMPANY("  \
	      "ID INT PRIMARY KEY     NOT NULL," \
	      "NAME           TEXT    NOT NULL," \
	      "AGE            INT     NOT NULL," \
	      "ADDRESS        CHAR(50)," \
	      "SALARY         REAL );";

	      /* Create a transactional object. */
	      work W(C);

	      /* Execute SQL query */
	      W.exec( sql.c_str() );
	      W.commit();
	      cout << "Table created successfully" << endl;
	      C.disconnect ();
	   } catch (const std::exception &e) {
	      cerr << e.what() << std::endl;
	      return 1;
	   }
	return 0;
}

int insert_data(int argc, char* argv[]) {
   std::string sql;

   try {
	   connection C("dbname = test_db user = postgres password = postgres \
	   	      hostaddr = 127.0.0.1 port = 9432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      /* Create SQL statement */
      sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

      /* Create a transactional object. */
      work W(C);

      /* Execute SQL query */
      W.exec( sql.c_str() );
      W.commit();
      cout << "Records created successfully" << endl;
      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}

int select_data(int argc, char* argv[]) {
   std::string sql;

   try {
      connection C("dbname = test_db user = postgres password = postgres \
      hostaddr = 127.0.0.1 port = 9432");
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }

      /* Create SQL statement */
      sql = "SELECT * from COMPANY";

      /* Create a non-transactional object. */
      nontransaction N(C);

      /* Execute SQL query */
      result R( N.exec( sql ));

      std::vector<std::string> column_names;

      for(size_t i = 0; i < R.columns(); i++) {
    	 //std::cout << "Column: \'" << R.column_name(i) <<"\' "<<std::endl;
    	  column_names.push_back(R.column_name(i));
      }

      for(auto name:column_names) {
    	  std::cout << "Column: \'" << name <<"\' "<<std::endl;
      }

      /* List down all the records */
      /*for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
         cout << "ID = " << c[0].as<int>() << endl;
         cout << "Name = " << c[1].as<string>() << endl;
         cout << "Age = " << c[2].as<int>() << endl;
         cout << "Address = " << c[3].as<string>() << endl;
         cout << "Salary = " << c[4].as<float>() << endl;
      }*/

      for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    	  for(auto name:column_names) {

    		  cout << name<<": " << c[name] << " db type: " << c[name].type() \
    				  << " cpp type: "<< demangle(typeid(c[name]).name())<< endl;
    	  }
      }

      cout << "Operation done successfully" << endl;
      C.disconnect ();
   } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}
