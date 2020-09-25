/*
 * clientmanager.cpp
 *
 *  Created on: Apr 28, 2020
 *      Author: jamil.zaman
 */
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <fcntl.h>   // open
#include <unistd.h>  // read, write, close
#include <cstdio>    // BUFSIZ
#include <zip.h>
#include <fstream>
#include <future>

#include "clientmanager.h"
#include "sqlite_handler.h"
#include "pg_handler.h"


client_handler::client_handler(socket_ptr _socket, std::string &_sq_database)
: sq_database_(_sq_database),
  pg_(new pg_handler),
  socket_(_socket),
  c_time_(time_wrapper::get_timer()),
  archive_file_{""}
{}

client_handler::~client_handler() {
    // TODO Auto-generated destructor stub
    // TODO: remove the auto generated files
    std::cout << c_time_  << " FAIRWELL CLIENT: "<< providerid_ << " !!"<< std::endl;
}

void client_handler::copy_file(const std::string& src, const std::string& dst) {
    char buf[BUFSIZ];
    size_t size;

    int source = open(src.c_str(), O_RDONLY, 0);
    int dest = open(dst.c_str(), O_WRONLY | O_CREAT /*| O_TRUNC*/, 0644);

    while ((size = read(source, buf, BUFSIZ)) > 0) {
        write(dest, buf, size);
    }
    close(source);
    close(dest);
}

bool client_handler::zip_file(const std::string& src, const std::string& dst) {
    int error = 0;
    //Open zip archive, if not present create first
    std::cout << c_time_ << "[" << providerid_ << "] begin compression ..." << std::endl;
    zip *archive = zip_open(dst.c_str(), ZIP_CREATE, &error);

    if(error) {
        std::cerr << "could not open or create archive" << std::endl;
        return false;
    }

    //source the zip content from file
    zip_source *source = zip_source_file(archive, src.c_str(), 0, 0);
    if(source == NULL) {
        std::cerr << "failed to create source buffer. " << zip_strerror(archive) << std::endl;
        return false;
    }

    //add the source to zip archive by name 'file.txt', any other name could have been chosen
    int index = zip_add(archive, src.c_str(), source);

#ifdef DEBUG
    std::cout << "Check if file creates at index: " << index <<std::endl;
#endif
    zip_close(archive);
    std::cout << c_time_ << "[" << providerid_ << "] end compression ..." << std::endl;
    return true;
}

void client_handler::parse_header(const char data[], size_t length) {

    const char *msg_end {data+length}, *head{data}, *tail {data};

    while (tail != msg_end && *tail != ' ') ++tail; //request type e.g. GET/POST
    request_details_["request_type"] = std::string(head, tail); //Omitting the space

    while (tail != msg_end && *tail == ' ') ++tail; //pass the space
    head = tail; //for next substring
    while (tail != msg_end && *tail != ' ') ++tail; //request path
    request_details_["path"] = std::string(head, tail ); //Omitting the space
    size_t idx = request_details_["path"].find_first_not_of("0123456789",1);
    request_details_["providerid"] = std::string(request_details_["path"],1,idx-1);
    //from '/' to first non-numeric character
    //TODO: extract the password field as well

    while (tail != msg_end && *tail == ' ') ++tail; //pass the space
    head = tail; //for next substring
    while (tail != msg_end && (*tail != ' ' || *tail != '\r')) ++tail; //request type e.g. GET/POST
    request_details_["version"] = std::string(head, tail); //Omitting the space
}

bool client_handler::prepare_db(
        const std::string& pg_database,
        const std::string& providerid) {

    //prepare the client copy of sqlite3 db in the current folder

    dst_sq_database_ = providerid + ".sqlite3";
    archive_file_ = providerid + ".zip";

    try {
        copy_file(sq_database_, dst_sq_database_); //copy the template database
        //clock the entire operation
        clock_t t, final = clock();
        t_keyed_data all_data;
        std::vector<std::string> column_names, result_values;
        std::map<std::string, std::future<int>> result_status;
        query_builder queries;
        int i = 0;
        std::string table_name = "";
        //create the sqlitehandler first
        std::shared_ptr<sqlite_handler> sq {std::make_shared<sqlite_handler>(dst_sq_database_)};
        for(auto it:queries.get_query_map()) { //browse each table of query map
            t = clock();
            table_name = it.first;
            all_data[table_name] = std::make_pair(std::vector<std::string>(), std::vector<std::string>());
            //extract data from postgresql
            std::cout << c_time_ <<" Querying TABLE: " << table_name << std::endl;
            //pg_->select_data(pg_database, table_name, providerid, column_names, result_values);

            /*pg_->select_data(pg_database, table_name, providerid,
                   all_data[table_name].first column names,
                    all_data[table_name].secondresult values);
             */

            result_status[table_name] = std::async(
                    std::launch::async,
                    &pg_handler::select_data, pg_, //the function and the object reference to call
                    pg_database,
                    table_name,
                    providerid,
                    std::ref(all_data[table_name].first),// column names,
                    std::ref(all_data[table_name].second)//result values
            );

            /*result_status[table_name] = std::async(
                                std::launch::async,
                                &pg_handler::get_providerid, pg_, //the function and the object reference to call
                                providerid, std::stoi(providerid), std::ref(column_names)
                        );*/

            //write it back to the sqlite database
            //std::cout << c_time_ <<" Writing TABLE '" << table_name << std::endl;

            /*sq->insert_data(
                    //dst_sq_database_,
                    table_name,
                    all_data[table_name].first,
                    all_data[table_name].second
                    );*/

            t = clock() - t;
            std::cout << c_time_ <<" TABLE '" << table_name << "' took " << t <<" ticks and " \
                    << (((float)t)/CLOCKS_PER_SEC) << " seconds" << std::endl;

            //clear the holding vectors after each round
            //result_values.clear();
            //column_names.clear();
            //if(i++> 3) break;
        }

        for(auto it:queries.get_query_map()) { //browse each table of query map
            if(result_status[it.first].get() == 0) { //result ready
                std::cout << c_time_ <<" [CM] RESULT READY FOR TABLE '" << it.first << " START INSERTION" << std::endl;
                sq->insert_data(
                        it.first,
                        all_data[it.first].first,
                        all_data[it.first].second
                );
            } else {
                std::cout << c_time_ <<" [CM] RESULT NOT READY FOR TABLE '" << it.first << " START INSERTION" << std::endl;
            }
        }

        //dump in_memory dbto actual file
        sq->dump_db(dst_sq_database_);
        //zip the file
        zip_file(dst_sq_database_, archive_file_);
        final = clock() - final;
        std::cout << c_time_ <<" ["<< providerid_<<"] TOTAL LOADING TIME: " << final <<" TICKS AND " \
                << (((float)final)/CLOCKS_PER_SEC) << " SECONDS" << std::endl;
    } catch (std::exception& e) {
        std::cerr << "[CM - PrepareDB] - " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "[CM - PrepareDB] - UNKNOWN ERROR" << std::endl;
        return false;
    }

    return true;
}

//void client_wrapper::session(socket_ptr _socket) {
int client_handler::session() {

    std::string response_header {};
    std::string response_tail ("\r\r");

    try
    {
        for (;;)
        {
            char data[1024];

            boost::system::error_code error{};
            std::cout << c_time_ << "[CM] -> NEW client connected ..." <<std::endl;
            size_t length = socket_->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            //parse the data to get the client details
            parse_header(data, length);
            providerid_ = request_details_["providerid"]; //unnecessary though
            //TODO: Verify providerid
            providers_db_ = pg_->get_providers_db(providerid_);

            std::cout << c_time_ << "User -> " << providerid_ << " db -> " << providers_db_ <<std::endl;
            //TODO: Need authentication code to check for valid request
            //boost::asio::write(*socket_, boost::asio::buffer(data, length));

            std::string total_response {};
            if (providers_db_.compare("ERROR") && //0 if true - if provider does not exist
                prepare_db(providers_db_, providerid_)) { //SUCCESS
                std::ifstream ifs(archive_file_);
                std::string str(std::istreambuf_iterator<char>{ifs}, {});
                response_header = std::string(
                        "HTTP/1.1 200 OK\r\n\
                         Connection: Closed \
                         content-type: application/octet-stream \
                         content-disposition: attachment; filename="+archive_file_+
                         " content-length: " + std::to_string(str.length()) +
                         "\r\n\r\n");

                total_response = response_header + str + response_tail;
            } else { //FAIL
                total_response = std::string (
                        "HTTP/1.1 400 ERROR\r\n\
                        Connection: Closed content-type: text/html \r\n\r\n\
                        <h2>BAD REQUEST CHECK PARAMETERS AGAIN</h2>");
            }

            boost::asio::write(*socket_, boost::asio::buffer(total_response.c_str(), total_response.length()));
            clean_client();
            break; //end the loop
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "[CM] Exception in thread: " << e.what() << "\n";
    }
    return 0;
}

bool client_handler::is_alive() {
    //check if the socket is disconnected
    return socket_->is_open();
}

void client_handler::clean_client() {

    boost::system::error_code error{};
    socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    socket_->close(error);

    int archive_err = std::remove(archive_file_.c_str());
    int db_err = std::remove(dst_sq_database_.c_str());
    if (error || archive_err || db_err) {
        std::cerr << "Client "<<providerid_<<" did not close cleanly... " << std::endl;
    }
}