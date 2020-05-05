/*
 * clientmanager.h
 *
 *  Created on: Apr 28, 2020
 *      Author: jamil.zaman
 */

#ifndef CLIENTMANAGER_H_
#define CLIENTMANAGER_H_

#include <boost/asio.hpp>
#include <memory>
#include <string>

class pg_handler;
class sqlite_handler;


class client_manager {
public:
    //using boost::asio::ip::tcp;
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
    client_manager(socket_ptr _socket);
    virtual ~client_manager();
    //void session(socket_ptr _socket);
    void session();
private:
    std::shared_ptr<pg_handler> pg_;
    std::shared_ptr<sqlite_handler> sq_;
    socket_ptr socket_;

    void copy_file(const std::string& src, const std::string& dst);
    bool zip_file(const std::string& src, const std::string& dst);
};

#endif /* CLIENTMANAGER_H_ */
