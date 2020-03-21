#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string>

#include "pg_handler.h"

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);

   fprintf(stdout, "QUERY RESULT:\n");
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   std::string sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("rhis_fwc.sqlite3", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   //sql = "select sql from sqlite_master where name = 'member';";
   sql = "SELECT unionid, count(*) as member FROM MEMBER group by unionid;";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);
   /* Create SQL statement */
      sql = "CREATE TABLE COMPANY("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "NAME           TEXT    NOT NULL," \
         "AGE            INT     NOT NULL," \
         "ADDRESS        CHAR(50)," \
         "SALARY         REAL );";

      /* Execute SQL statement */
      //rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);

   //handle pg
   //create_table (argc, argv);
   //insert data
   //insert_data(argc, argv);
   std::vector<std::string> column_name, result_values;
   select_data(column_name, result_values);
   return 0;
}
