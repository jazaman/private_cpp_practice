/*
 * configmanager.cpp
 *
 *  Created on: Sep 25, 2020
 *      Author: jamil.zaman
 */

#include "configmanager.h"
#include <fstream>

using boost::property_tree::ptree;

void read(ptree const& pt, std::string& s) {
    s = pt.get_value(s);
}

void read(ptree const& pt, unsigned int& s) {
    s = pt.get_value(s);
}

//sample reading from property
void read(ptree const& pt, Object& object) {
    read(pt.get_child("port"), object.port);
    read(pt.get_child("address"), object.address);
    /*read(pt.get_child("property2"), object.property2);
    read(pt.get_child("property3"), object.property3);
    read(pt.get_child("property4"), object.property4);
    read(pt.get_child("property5"), object.property5);*/
}

template<typename T>
T read_as(ptree const& pt) {
    T v;
    read(pt, v);
    return v;
}


config_manager::config_manager(const std::string& _config_file)
    : address_{""}, port_{0}, thread_multiplier_{1}, password_{""}, db_type_{""} {
    //read the config file and load properties
    std::ifstream ifs(_config_file);
    std::istringstream iss(std::string(std::istreambuf_iterator<char>{ifs},{}));
    boost::property_tree::ptree config;
    read_ini(iss, config);

    std::cout << "Server " << config.get<std::string>("database.address") << std::endl;
    std::cout << "Port " <<   config.get<int>("database.port") << std::endl;
    address_ = config.get<std::string>("database.address");
    db_port_ = config.get<int>("database.port");
    password_ = config.get<std::string>("database.password");
    user_ = config.get<std::string>("database.user");
    db_type_ = config.get<std::string>("database.type");
    port_ = config.get<int>("application.port");
    thread_multiplier_ = config.get<int>("application.thread_multiplier");

    /*
    std::map<std::string, Object> parsed;
    for (auto& section : config) {
        parsed[section.first] = read_as<Object>(section.second);
    }

    for (auto& object : parsed) {
        std::cout << "Parsed object named " << object.first
                << " (e.g. port is '" << object.second.port << "')\n";
    }*/

}

config_manager::~config_manager() {
    // TODO Auto-generated destructor stub
}

