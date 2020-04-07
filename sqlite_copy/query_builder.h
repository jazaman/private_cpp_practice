/*
 * query_builder.h
 *
 *  Created on: Apr 6, 2020
 *      Author: jamil.zaman
 */

#ifndef SQLITE_COPY_QUERY_BUILDER_H_
#define SQLITE_COPY_QUERY_BUILDER_H_

#include <string>
#include <map>

class query_builder {
public:
    query_builder();
    ~query_builder();

    const std::string get_query(const std::string& table_name);
private:
    std::map<std::string, std::string> query_map;
    const std::string general_query = "select services.* " +
            std::string("from ( with pd as (select zillaid, upazilaid, unionid from providerdb ") +
            std::string("where providerid = $1 and active = 1) ") + //1 - providerid
            std::string("select distinct healthid from $2 where providerid = $3 UNION " ) + //2 table_name 3 providerid
            std::string("select generatedid as healthid from clientmap ") +
            std::string("inner join pd using (zillaid, upazilaid, unionid)) sq ") +
            std::string("inner join ancservice as services using (healthid) ") +
            std::string("inner join clientmap on sq.healthid = generatedid; ") +
            std::string("order by healthid, serviceid;");
};



#endif /* SQLITE_COPY_QUERY_BUILDER_H_ */
