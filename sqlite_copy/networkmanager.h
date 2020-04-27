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

class network_manager {
public:
    network_manager();
    virtual ~network_manager();
    void session();
    void server_loop();
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
private:
    socket_ptr socket;
};

#endif /* NETWORKMANAGER_H_ */
