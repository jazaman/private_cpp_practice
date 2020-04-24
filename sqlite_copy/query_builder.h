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

    typedef std::map<const std::string, std::string&>::iterator table_iterator;
    const std::string get_query(const std::string& table_name);
    const std::map<const std::string, std::string&>& get_query_map() { return query_map;};
private:
    std::map<const std::string, std::string&> query_map;
    std::string general_query = "select services.* " +
            std::string("from ( with pd as (select zillaid, upazilaid, unionid from providerdb ") +
            std::string("where providerid =  $1 and active = 1) ") + //1 - providerid
            std::string("select distinct healthid from <table_name> where providerid = $1 UNION " ) + //2 table_name 3 providerid
            std::string("select generatedid as healthid from clientmap ") +
            std::string("inner join pd using (zillaid, upazilaid, unionid)) sq ") +
            std::string("inner join <table_name> as services using (healthid) ") +
            std::string("inner join clientmap on sq.healthid = generatedid; ");
            //std::string("order by healthid, serviceid;");

    std::string service_query =
            std::string("with pd as ( ") +
            std::string("select providerid, zillaid, upazilaid, unionid, active ") +
            std::string("from providerdb inner join ( ") +
            std::string("select zillaid, upazilaid, unionid from providerdb where providerid = $1 ") +
            std::string(") pd using (zillaid, upazilaid, unionid) ) ") +
            std::string("select services.* from <table_name> as services ") +
            std::string("inner join (select distinct healthid ") +
            std::string("from <table_name> inner join pd using (providerid) ") +
            std::string("union select distinct generatedid as healthid ") +
            std::string("from clientmap inner join pd using (zillaid, upazilaid, unionid) ") +
            std::string(") service_all using (healthid) ");

    std::string clientmap_query =
            std::string("with pd as ( ") +
            std::string("select providerid, zillaid, upazilaid, unionid, active ") +
            std::string("from providerdb inner join ( ") +
            std::string("select zillaid, upazilaid, unionid from providerdb where providerid = $1 ") +
            std::string(") pdb using (zillaid, upazilaid, unionid) ") +
            std::string(") ") +
            std::string("select cm.* from clientmap cm inner join ( ") +
            std::string("select distinct healthid from pregwomen inner join pd using (providerid) union ") +
            std::string("select distinct healthid from ancservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from delivery inner join pd using (providerid) union ") +
            std::string("select distinct healthid from pncservicemother inner join pd using (providerid) union ") +
            std::string("select distinct healthid from pncservicechild inner join pd using (providerid) union ") +
            std::string("select distinct healthid from pacservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from fpinfo inner join pd using (providerid) union ") +
            std::string("select distinct healthid from pillcondomservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from womaninjectable inner join pd using (providerid) union ") +
            std::string("select distinct healthid from iudservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from iudfollowupservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from implantservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from implantfollowupservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from gpservice inner join pd using (providerid) union ") +
            std::string("select distinct healthid from death inner join pd using (providerid) union ") +
            std::string("select distinct generatedid as healthid from clientmap inner join pd using (zillaid, upazilaid, unionid) ") +
            std::string(") all_id on cm.generatedid = all_id.healthid ");

    std::string member_query =
                std::string("with pd as ( ") +
                std::string("select providerid, zillaid, upazilaid, unionid, active ") +
                std::string("from providerdb inner join ( ") +
                std::string("select zillaid, upazilaid, unionid from providerdb where providerid = $1 ") +
                std::string(") pdb using (zillaid, upazilaid, unionid) ") +
                std::string(") ") +
                std::string("select distinct m.* from (select * from member where extype = 0) m ") +
                std::string("inner join clientmap cm using(healthid) inner join ( ") + //assuming all member has clientmap reference
                std::string("select distinct healthid from pregwomen inner join pd using (providerid) union ") +
                std::string("select distinct healthid from ancservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from delivery inner join pd using (providerid) union ") +
                std::string("select distinct healthid from pncservicemother inner join pd using (providerid) union ") +
                std::string("select distinct healthid from pncservicechild inner join pd using (providerid) union ") +
                std::string("select distinct healthid from pacservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from fpinfo inner join pd using (providerid) union ") +
                std::string("select distinct healthid from pillcondomservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from womaninjectable inner join pd using (providerid) union ") +
                std::string("select distinct healthid from iudservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from iudfollowupservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from implantservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from implantfollowupservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from gpservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from death inner join pd using (providerid) union ") +
                std::string("select distinct generatedid as healthid from clientmap inner join pd using (zillaid, upazilaid, unionid) ") +
                std::string(") all_id on cm.generatedid = all_id.healthid ");

    std::string pregwomen_query =
                std::string("with pd as ( ") +
                std::string("select providerid, zillaid, upazilaid, unionid, active ") +
                std::string("from providerdb inner join ( ") +
                std::string("select zillaid, upazilaid, unionid from providerdb where providerid = $1 ") +
                std::string(") pdb using (zillaid, upazilaid, unionid) ") +
                std::string(") ") +
                std::string("select pw.* from pregwomen pw inner join ( ") +
                std::string("select distinct healthid from pregwomen inner join pd using (providerid) union ") +
                std::string("select distinct healthid from ancservice inner join pd using (providerid) union ") +
                std::string("select distinct healthid from delivery inner join pd using (providerid) union ") +
                std::string("select distinct healthid from pncservicemother inner join pd using (providerid) union ") +
                std::string("select distinct healthid from pncservicechild inner join pd using (providerid) union ") +
                std::string("select distinct healthid from pacservice inner join pd using (providerid) union ") +
                std::string("select distinct generatedid as healthid from clientmap inner join pd using (zillaid, upazilaid, unionid) ") +
                std::string(") all_id using(healthid) ");

    std::string search_replace_all(const std::string& str, const std::string& search_str, const std::string& replace_str);
};



#endif /* SQLITE_COPY_QUERY_BUILDER_H_ */
