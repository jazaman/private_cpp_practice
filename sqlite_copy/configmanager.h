/*
 * configmanager.h
 *
 *  Created on: Sep 25, 2020
 *      Author: jamil.zaman
 */

#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

#include <iostream>
#include <map>
#include <boost/property_tree/ini_parser.hpp>

static std::string const sample = R"(
[BLA]
properties1=val1, val2, val3
property2=Blah
property3=Blah
property4=Blah
property5=Bla bla bla

[BLO]
properties1=val1
property2=Blah
property3=Blah
property4=Blah
property5=Bla bla bla
)";

struct Object {
    std::string address;
    std::string password;
    unsigned int port;

};

class config_manager {
private:
    config_manager(const std::string& _conf); // Disallow instantiation outside of the class.
public:
    config_manager(const config_manager&) = delete;
    config_manager& operator=(const config_manager &) = delete;
    config_manager(config_manager &&) = delete;
    config_manager & operator=(config_manager &&) = delete;
    const unsigned int& port() {return port_;};
    const unsigned int& db_port() {return db_port_;};
    const unsigned int& thread_multiplier() {return thread_multiplier_;};
    const std::string& address() const {return address_;};
    const std::string& password() const {return password_;};
    const std::string& user() const {return user_;};

    static config_manager& instance(const std::string& _conf) {
        static config_manager config(_conf);
        return config;
    }
public:
    //config_manager();
    virtual ~config_manager();
    //static const config_manager&  get_config();
private:
    std::string address_;
    unsigned int port_;
    unsigned int db_port_;
    unsigned int thread_multiplier_;
    std::string password_;
    std::string user_;
    std::string db_type_;
};

#endif /* CONFIGMANAGER_H_ */
