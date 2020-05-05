/*
 * networkmanager.h
 *
 *  Created on: Apr 28, 2020
 *      Author: jamil.zaman
 */

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_
#include <memory> //std::shared_ptr
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
class client_manager;
class network_manager {
public:
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
    network_manager(short _port);
    virtual ~network_manager();
    void server_loop();

private:
    boost::asio::io_service io_service;
    socket_ptr socket;
    short port;
    std::vector<std::shared_ptr<client_manager>> client_list;
};

#endif /* NETWORKMANAGER_H_ */
