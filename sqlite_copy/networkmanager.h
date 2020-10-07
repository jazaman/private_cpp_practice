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
namespace std {
template <class T>
class future;
}
class network_manager {
public:
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
    network_manager(short _port, std::string &_sq_database);
    virtual ~network_manager();
    void server_loop();
    void remove_dead_clients(std::map<client_handler*, std::future<int>>& _results);
    //void set_port(short _port) {port_ = _port;};
    const unsigned short& get_port() const {return port_;};

private:
    unsigned short port_;
    std::string &sq_database_;
    boost::asio::io_service io_service;
    std::vector<std::shared_ptr<client_handler>> client_list;
};

#endif /* NETWORKMANAGER_H_ */
