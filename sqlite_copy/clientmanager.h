/*
 * clientmanager.h
 *
 *  Created on: Apr 28, 2020
 *      Author: jamil.zaman
 */

#ifndef CLIENTMANAGER_H_
#define CLIENTMANAGER_H_

#include <boost/asio.hpp>

class pg_handler;
class sqlite_handler;


class client_manager {
public:
    client_manager();
    virtual ~client_manager();
};

#endif /* CLIENTMANAGER_H_ */
