/*
 * networkmanager.cpp
 *
 *  Created on: Apr 28, 2020
 *      Author: jamil.zaman
 */

#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
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
    //std::vector<std::thread> workers;
    std::map<client_handler*, std::future<int>> results;
    for (;;) //forever, server loop may need to install a signal to stop it gracefully
    {
        socket_ptr sock(new tcp::socket(io_service));
        auto cm (std::make_shared<client_handler>(sock, sq_database_));
        client_list.push_back(cm);

        a.accept(*sock);
        //TODO:Welcome client
        //boost::asio::write(*sock, boost::asio::buffer(std::string("Welcome TO SQLITE COPY\r\n")));
        //workers.push_back(std::thread (std::bind(&client_handler::session, cm)));

        //t.join();
        //check for closed connections and remove them
        //results.push_back(std::async(std::launch::async, &client_handler::session, cm));
        results[cm.get()] = std::async(std::launch::async, &client_handler::session, cm);
        std::cout << "TOTAL CONNECTED CLIENT: " << client_list.size() << std::endl;
        remove_dead_clients(results);
    }
}

void network_manager::remove_dead_clients(std::map<client_handler*, std::future<int>>& _results) {
    //remove inactive clients
    for(auto clients = _results.begin(); clients != _results.end();) {
        if (clients->first->get_status() == client_handler::DONE ) {
            clients->second.get(); //finish the async task
            _results.erase(clients++); //post iteration is important,
        } else if (clients->first->get_status() == client_handler::TIMEOUT) {
                clients->first->get_socket()->close(); //finish the async task
                _results.erase(clients++); //post iteration is important,
        } else {
            ++clients;
        }
    }

    client_list.erase(
        std::remove_if(
            client_list.begin(), client_list.end(),
            [](decltype (*client_list.begin()) &x) { //lamda - wish I could use auto
                return !x->is_alive();
            }
        ),
        client_list.end()
    );
}

