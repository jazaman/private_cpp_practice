/*
 * pg_handler.cpp
 *
 *  Created on: Mar 20, 2020
 *      Author: jamil.zaman
 */
#include "pg_handler.h"
#include <iostream>
#include <string>
#include <pqxx/pqxx>

#include <typeinfo>

#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#include <sstream>
#include <iterator>
#include "configmanager.h"

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
//using namespace pqxx;
//template <typename T> std::string type_name();

pg_handler::pg_handler(): cm(config_manager::instance("")) {}

int pg_handler::create_table (int argc, char* argv[]) {
    std::string sql;

    try {
        pqxx::connection C("dbname = test_db user = postgres password = postgres \
	      hostaddr = 127.0.0.1 port = 5432");
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
        pqxx::work W(C);

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

int pg_handler::insert_data(int argc, char* argv[]) {
    std::string sql;

    try {
        pqxx::connection conn("dbname = test_db user = postgres password = postgres \
	   	      hostaddr = 127.0.0.1 port = 5432");
        if (conn.is_open()) {
            cout << "Opened database successfully: " << conn.dbname() << endl;
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
        pqxx::work W(conn);
        /* Execute SQL query */
        W.exec( sql.c_str() );
        W.commit();
        cout << "Records created successfully" << endl;
        conn.disconnect ();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int pg_handler::select_data(
        const std::string& db_name,
        const std::string& table_name,
        const std::string& provider_id,
        std::vector<std::string>& column_names,
        std::vector<std::string>& result_values,
        table_status& query_status) {
    std::string sql;

    try {
        pqxx::connection conn("dbname = "+ db_name +" user = "+cm.user()+" password = "
                + cm.password()+" hostaddr = " + cm.address() + " port = " + std::to_string(cm.db_port()));
        if (conn.is_open()) {
            cout << "Opened database successfully: " << conn.dbname()<< " for table: " << table_name << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        /* Get SQL statement */
        sql = query.get_query(table_name);

#ifdef DEBUG
        std::cout << "SELECT QUERY:\n" << sql << std::endl;
#endif
        /* Create a non-transactional object. */
        //pqxx::nontransaction N(conn);
        pqxx::work W(conn);
        conn.prepare("query", sql.c_str());
        /* Execute SQL query */
        //pqxx::result R( N.exec( sql ));
        pqxx::result R = W.prepared("query")(provider_id).exec();

        for(size_t i = 0; i < R.columns(); i++) {
            //std::cout << "Column: \'" << R.column_name(i) <<"\' "<<std::endl;
            column_names.push_back(R.column_name(i));
        }

        const char* const delim = ", ";

        std::ostringstream imploded;
        std::copy(column_names.begin(), column_names.end(),
                   std::ostream_iterator<std::string>(imploded, delim));

        std::string columns = imploded.str();
        columns.erase(columns.find_last_of(","));
#ifdef DEBUG_2
        std::cout << "COLUMNS:: " << columns << std::endl;
#endif
        std::string result_holder = "";

        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            for(auto name:column_names) {
#ifdef DEBUG_2
                cout << name<<": " << c[name] << " db type: " << c[name].type() \
                     << " cpp type: "<< demangle(typeid(c[name]).name())<< endl;
#endif
                result_holder.append("'"+ std::string(c[name].c_str())+ "', ");
            }
            result_holder.erase(result_holder.find_last_of(","));
#ifdef DEBUG_2
            std::cout << "imploded result: " << result_holder << std::endl;
#endif
            result_values.push_back(result_holder);
            result_holder = "";
        }

#ifdef DEBUG_2
        for(auto values:result_values) {
            std::cout << "-> " << values <<std::endl;
        }
#endif

        /*std::ostringstream imploded_values;
        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            std::copy(c.begin(), c.end, std::ostream_iterator<const char*>(imploded_values, delim));
        }*/

        cout << "QUERY COMPLETED, FETCHED "<< result_values.size()<< " RECORDS FOR " << table_name << endl;
        query_status = table_status::READY; //indicating query ended
        conn.disconnect ();
    } catch (const std::exception &e) {
        cerr <<"ERROR OCCURRED FOR SQL:\n" << sql << endl;
        cerr << e.what() << std::endl;
        query_status = table_status::ERRORED; //indicating query ended
        return 1;
    }

    return 0;
}

const std::string pg_handler:: get_providers_db(const std::string _providerid) {
    std::string selected_db {}, sql {};
    try {
        //TODO - Need zillaid from client side to identify which district database to connect to
        /*pqxx::connection conn("dbname = RHIS_36 user = postgres password = postgres \
          hostaddr = 127.0.0.1 port = 9432");*/
        pqxx::connection conn("dbname = RHIS_36 user = " + cm.user() + " password = "
                        + cm.password() + " hostaddr = " + cm.address() + " port = " + std::to_string(cm.db_port()));
        if (conn.is_open()) {
            cout << "Opened database successfully: " << conn.dbname()<< "for provider: [" <<_providerid<<"]"<< endl;
        } else {
            cout << "Can't open database" << endl;
            return selected_db;
        }

        /* Get SQL statement */
        sql = query.get_db_selection_query(_providerid);

#ifdef DEBUG
        std::cout << "SELECT QUERY:\n" << sql << std::endl;
#endif
        /* Create a non-transactional object. */
        //pqxx::nontransaction N(conn);
        pqxx::work W(conn);
        conn.prepare("query", sql.c_str());
        /* Execute SQL query */
        //pqxx::result R( N.exec( sql ));
        pqxx::result R { W.prepared("query")(_providerid).exec()};

        auto it = R.begin();

        if(it != R.end()) {
            selected_db = it["db"].c_str();
        } else {
            std::cout << "ERROR: provider do not exist" << std::endl;
            selected_db = "ERROR";
        }

    } catch (const std::exception &e) {
        cerr <<"ERROR OCCURRED FOR SQL:\n" << sql << endl;
        cerr << e.what() << std::endl;
    }
    return selected_db;
}
