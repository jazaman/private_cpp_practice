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

#include "clientmanager.h"
#include "sqlite_handler.h"
#include "pg_handler.h"


client_manager::client_manager(socket_ptr _socket)
    : pg_(new pg_handler),
      sq_(new sqlite_handler),
      socket_(_socket)
{}

client_manager::~client_manager() {
    // TODO Auto-generated destructor stub
    std::cout << "Bye bye client ..." << std::endl;
}

void client_manager::copy_file(const std::string& src, const std::string& dst) {
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

bool client_manager::zip_file(const std::string& src, const std::string& dst) {
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

//void client_manager::session(socket_ptr _socket) {
void client_manager::session() {
    try
    {
        for (;;)
        {
            char data[1024];

            boost::system::error_code error;
            size_t length = socket_->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            boost::asio::write(*socket_, boost::asio::buffer(data, length));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "[CM] Exception in thread: " << e.what() << "\n";
    }
}


