/*
 * pg_handler.h
 *
 *  Created on: Mar 20, 2020
 *      Author: jamil.zaman
 */

#ifndef SQLITE_COPY_PG_HANDLER_H_
#define SQLITE_COPY_PG_HANDLER_H_

int create_table (int argc, char* argv[]);
int insert_data(int argc, char* argv[]);
int select_data(int argc, char* argv[]);



#endif /* SQLITE_COPY_PG_HANDLER_H_ */
