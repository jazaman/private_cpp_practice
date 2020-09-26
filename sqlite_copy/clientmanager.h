/*
 * clientmanager.h
 *
 *  Created on: Apr 28, 2020
 *      Author: jamil.zaman
 */

#ifndef CLIENTWRAPPER_H_
#define CLIENTWRAPPER_H_

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "time_wrapper.h"

class pg_handler;
class sqlite_handler;


class client_handler {
public:
    //using boost::asio::ip::tcp;
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
    typedef std::map<std::string, std::pair<std::vector<std::string>,std::vector<std::string>>> t_keyed_data;
    client_handler(socket_ptr _socket, std::string &_sq_database);
    virtual ~client_handler();
    //void session(socket_ptr _socket);
    int session();
    bool is_alive();
    enum HTTP_HEADER {REQUEST_TYPE, REQUEST_PATH, VERSION, HOST }; //TODO: Not sure if I need this
private:
    std::string &sq_database_;
    std::shared_ptr<pg_handler> pg_;
    std::shared_ptr<sqlite_handler> sq_;
    socket_ptr socket_;
    time_wrapper& c_time_;
    std::string dst_sq_database_;
    std::map<std::string, std::string> request_details_;
    std::string providers_db_;
    std::string providerid_;
    std::string archive_file_;


    void copy_file(const std::string& src, const std::string& dst);
    bool zip_file(const std::string& src, const std::string& dst);
    void clean_client();
    bool prepare_db(const std::string& pg_database, const std::string& providerid);
    void parse_header(const char data[], size_t length);
};

#endif /* CLIENTWRAPPER_H_ */
