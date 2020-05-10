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
class client_handler;

class network_manager {
public:
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
    network_manager(short _port, std::string &_sq_database);
    virtual ~network_manager();
    void server_loop();

private:
    short port_;
    std::string &sq_database_;
    boost::asio::io_service io_service;
    std::vector<std::shared_ptr<client_handler>> client_list;
};

#endif /* NETWORKMANAGER_H_ */
