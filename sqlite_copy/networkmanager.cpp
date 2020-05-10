/*
 * networkmanager.cpp
 *
 *  Created on: Apr 28, 2020
 *      Author: jamil.zaman
 */

#include <Thread>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "networkmanager.h"
#include "clientmanager.h"

const int max_length = 1024;

network_manager::network_manager(short _port, std::string &_sq_database)
: port_(_port),
  sq_database_(_sq_database)
{
    // TODO Auto-generated constructor stub
}

network_manager::~network_manager() {
    // TODO Auto-generated destructor stub
}

void network_manager::server_loop()
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port_));
    for (;;) //forever, server loop may need to install a signal to stop it gracefully
    {
        socket_ptr sock(new tcp::socket(io_service));
        auto cm (std::make_shared<client_handler>(sock, sq_database_));
        client_list.push_back(cm);
        a.accept(*sock);
        //TODO:Welcome client
        //boost::asio::write(*sock, boost::asio::buffer(std::string("Welcome TO SQLITE COPY\r\n")));
        boost::thread t(std::bind(&client_handler::session, cm));
    }
}

