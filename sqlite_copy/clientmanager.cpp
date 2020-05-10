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

#include "clientmanager.h"
#include "sqlite_handler.h"
#include "pg_handler.h"


client_handler::client_handler(socket_ptr _socket, std::string &_sq_database)
: pg_(new pg_handler),
  sq_(new sqlite_handler),
  socket_(_socket),
  sq_database_(_sq_database)
{}

client_handler::~client_handler() {
    // TODO Auto-generated destructor stub
    //TODO: remove the auto generated files
    std::cout << "Bye bye client ..." << std::endl;
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
    zip *archive = zip_open(dst.c_str(), ZIP_CREATE, &error);

    if(error) {
        std::cout << "could not open or create archive" << std::endl;
        return false;
    }

    //source the zip content from file
    zip_source *source = zip_source_file(archive, src.c_str(), 0, 0);
    if(source == NULL) {
        std::cout << "failed to create source buffer. " << zip_strerror(archive) << std::endl;
        return false;
    }

    //add the source to zip archive by name 'file.txt', any other name could have been chosen
    int index = zip_add(archive, src.c_str(), source);

#ifdef DEBUG
    std::cout << "Check if file creates at index: " << index <<std::endl;
#endif
    zip_close(archive);
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

bool client_handler::prepare_db(const std::string& pg_database, const std::string& providerid, std::string& return_archive) {

    //prepare the client copy of sqlite3 db in the current folder

    dst_sq_database_ = providerid + ".sqlite3";
    return_archive = providerid + ".zip";

    try {
        copy_file(sq_database_, dst_sq_database_);
        //clock the entire operation
        clock_t t, final = clock();
        std::vector<std::string> column_names, result_values;
        query_builder queries;
        int i = 0;
        std::string table_name = "";
        for(auto it:queries.get_query_map()) {
            t = clock();
            table_name = it.first;
            pg_->select_data(pg_database, table_name, providerid, column_names, result_values);
            //sqlite_handler::create_table(sq_database);
            sq_->insert_data(dst_sq_database_, table_name, column_names, result_values);
            t = clock() - t;
            std::cout <<"\nTABLE '" << table_name << "' took " << t <<" ticks and " \
                    << (((float)t)/CLOCKS_PER_SEC) << " seconds" << std::endl;

            result_values.clear();
            column_names.clear();
            //if(i++> 3) break;
        }

        //zip the file
        zip_file(dst_sq_database_, return_archive);
        final = clock() - final;
        std::cout <<"\nTHE DATA LOADING TOOK " << final <<" ticks and " \
                << (((float)final)/CLOCKS_PER_SEC) << " seconds" << std::endl;
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
void client_handler::session() {

    std::string response_header {};
    std::string response_tail ("\r\r");

    try
    {
        for (;;)
        {
            char data[1024];

            boost::system::error_code error;
            std::cout << " [CM] -> Waiting for client data ..." <<std::endl;
            size_t length = socket_->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            //parse the data to get the client details
            parse_header(data, length);
            providerid_ = request_details_["providerid"]; //unnecessary though
            providers_db_ = pg_->get_providers_db(providerid_);

            std::cout << "User -> " << providerid_ << " db -> " << providers_db_ <<std::endl;
            //TODO: Need authentication code to check for valid request
            //boost::asio::write(*socket_, boost::asio::buffer(data, length));
            std::string archive_file {};
            std::string total_response {};
            if (prepare_db(providers_db_, providerid_, archive_file)) { //SUCCESS
                std::ifstream ifs(archive_file);
                std::string str(std::istreambuf_iterator<char>{ifs}, {});
                response_header = std::string(
                        "HTTP/1.1 200 OK\r\n\
                         Connection: Closed \
                         content-type: application/octet-stream \
                         content-disposition: attachment; filename="+archive_file+
                         " content-length: " + std::to_string(str.length()) +
                         "\r\n\r\n");

                total_response = response_header + str + response_tail;
            } else { //FAIL
                total_response = std::string (
                        "HTTP/1.1 500 ERROR\r\n\
                        Connection: Closed content-type: text/html \r\n\r\n\
                        <h2>INTERNAL SERVER ISSUE CHECK PARAMETERS AGAIN</h2>");
            }

            boost::asio::write(*socket_, boost::asio::buffer(total_response.c_str(), total_response.length()));
            socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
            socket_->close(error);
            if (error) {
                std::cerr << "Connection did not close cleanly... " << std::endl;
            }
            break;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "[CM] Exception in thread: " << e.what() << "\n";
    }
}


