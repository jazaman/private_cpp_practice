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
    const std::map<const std::string, std::string&>& get_query_map() { return db_builder_queries_;};
    const std::string get_db_selection_query(const std::string _providerid) { return db_selection_query; };
private:
    std::map<const std::string, std::string&> db_builder_queries_;

    std::string provider_group =
        R"( with pd_geo as (
                select facilityid, providerid, zillaid, upazilaid, unionid, active from ( -- facilityid, providerid and geolocation of working area
                    select providerid from providerdb pd where providerid = $1 -- active facility
                    UNION
                    select associated_id as providerid from associated_provider_id ap 
                    where provider_id = $1 -- additional facility
                ) ownids
                inner join ( select facility_id as facilityid, providerid, is_active as active  
                    from facility_provider where is_active = 1) fp using (providerid)
                inner join facility_registry using (facilityid)
            ),
            fp_all as ( -- get the other providers who worked in the same facility
                select fp.*, zillaid, upazilaid,unionid from pd_geo inner join facility_provider fp on pd_geo.facilityid = fp.facility_id
            ),
            pd_all as (
                select pd.providerid, zillaid, upazilaid, unionid  -- providers who worked in the same area
                from providerdb pd inner join (select providerid, zillaid, upazilaid, unionid from pd_geo) sq using (zillaid, upazilaid, unionid)
                UNION
                select providerid, zillaid, upazilaid, unionid from fp_all -- providers who worked under same facility
            )

       )";

    std::string general_query = "select services.* " +
            std::string("from ( with pd as (select zillaid, upazilaid, unionid from providerdb ") +
            std::string("where providerid =  $1 and active = 1) ") + //1 - providerid
            std::string("select distinct healthid from <table_name> where providerid = $1 UNION " ) + //2 table_name 3 providerid
            std::string("select generatedid as healthid from clientmap ") +
            std::string("inner join pd using (zillaid, upazilaid, unionid)) sq ") +
            std::string("inner join <table_name> as services using (healthid) ") +
            std::string("inner join clientmap on sq.healthid = generatedid; ");
            //std::string("order by healthid, serviceid;");


    std::string service_query_old =
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

    std::string service_query = provider_group +
        R"(
        select services.* from <table_name> as services inner join (
            select distinct healthid 
            from <table_name> inner join pd_all using (providerid) 
            union 
            select distinct generatedid as healthid 
            from clientmap inner join pd_all using (zillaid, upazilaid, unionid) 
        ) service_all using (healthid) 
        )";

    std::string clientmap_query = provider_group +
        R"( 
        select cm.* from clientmap cm inner join ( 
            select distinct healthid from pregwomen inner join pd_all using (providerid) union 
            select distinct healthid from ancservice inner join pd_all using (providerid) union 
            select distinct healthid from delivery inner join pd_all using (providerid) union 
            select distinct healthid from pncservicemother inner join pd_all using (providerid) union 
            select distinct healthid from pncservicechild inner join pd_all using (providerid) union 
            select distinct healthid from pacservice inner join pd_all using (providerid) union 
            select distinct healthid from fpinfo inner join pd_all using (providerid) union 
            select distinct healthid from pillcondomservice inner join pd_all using (providerid) union 
            select distinct healthid from womaninjectable inner join pd_all using (providerid) union 
            select distinct healthid from iudservice inner join pd_all using (providerid) union 
            select distinct healthid from iudfollowupservice inner join pd_all using (providerid) union 
            select distinct healthid from implantservice inner join pd_all using (providerid) union 
            select distinct healthid from implantfollowupservice inner join pd_all using (providerid) union 
            select distinct healthid from gpservice inner join pd_all using (providerid) union 
            select distinct healthid from death inner join pd_all using (providerid) union 
            select distinct generatedid as healthid from clientmap inner join pd_all using (zillaid, upazilaid, unionid) 
        ) all_id on cm.generatedid = all_id.healthid

        )";

    std::string clientmap_query_old =
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

    std::string clientmap_extn_query = provider_group +
        R"(
        select cme.* from clientmap_extension cme inner join ( 
            select distinct healthid from pregwomen inner join pd_all using (providerid) union 
            select distinct healthid from ancservice inner join pd_all using (providerid) union 
            select distinct healthid from delivery inner join pd_all using (providerid) union 
            select distinct healthid from pncservicemother inner join pd_all using (providerid) union 
            select distinct healthid from pncservicechild inner join pd_all using (providerid) union 
            select distinct healthid from pacservice inner join pd_all using (providerid) union 
            select distinct healthid from fpinfo inner join pd_all using (providerid) union 
            select distinct healthid from pillcondomservice inner join pd_all using (providerid) union 
            select distinct healthid from womaninjectable inner join pd_all using (providerid) union 
            select distinct healthid from iudservice inner join pd_all using (providerid) union 
            select distinct healthid from iudfollowupservice inner join pd_all using (providerid) union 
            select distinct healthid from implantservice inner join pd_all using (providerid) union 
            select distinct healthid from implantfollowupservice inner join pd_all using (providerid) union 
            select distinct healthid from gpservice inner join pd_all using (providerid) union 
            select distinct healthid from death inner join pd_all using (providerid) union 
            select distinct generatedid as healthid from clientmap inner join pd_all using (zillaid, upazilaid, unionid) 
        ) all_id on cme.generated_id = all_id.healthid 

        )";

    std::string member_query = provider_group +

        R"(
        select distinct m.* from (select * from member where extype = 0) m inner join 
        clientmap cm using(healthid) inner join (  --assuming all member has clientmap reference
            select distinct healthid from pregwomen inner join pd_all using (providerid) union 
            select distinct healthid from ancservice inner join pd_all using (providerid) union 
            select distinct healthid from delivery inner join pd_all using (providerid) union 
            select distinct healthid from pncservicemother inner join pd_all using (providerid) union 
            select distinct healthid from pncservicechild inner join pd_all using (providerid) union 
            select distinct healthid from pacservice inner join pd_all using (providerid) union 
            select distinct healthid from fpinfo inner join pd_all using (providerid) union 
            select distinct healthid from pillcondomservice inner join pd_all using (providerid) union 
            select distinct healthid from womaninjectable inner join pd_all using (providerid) union 
            select distinct healthid from iudservice inner join pd_all using (providerid) union 
            select distinct healthid from iudfollowupservice inner join pd_all using (providerid) union 
            select distinct healthid from implantservice inner join pd_all using (providerid) union 
            select distinct healthid from implantfollowupservice inner join pd_all using (providerid) union 
            select distinct healthid from gpservice inner join pd_all using (providerid) union 
            select distinct healthid from death inner join pd_all using (providerid) union 
            select distinct generatedid as healthid from clientmap inner join pd_all using (zillaid, upazilaid, unionid) 
        ) all_id on cm.generatedid = all_id.healthid 
        )";

    std::string member_query_old =
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

    std::string pregwomen_query = provider_group +
        R"(
        select pw.* from pregwomen pw inner join ( 
        select distinct healthid from pregwomen inner join pd_all using (providerid) union 
        select distinct healthid from ancservice inner join pd_all using (providerid) union 
        select distinct healthid from delivery inner join pd_all using (providerid) union 
        select distinct healthid from pncservicemother inner join pd_all using (providerid) union 
        select distinct healthid from pncservicechild inner join pd_all using (providerid) union 
        select distinct healthid from pacservice inner join pd_all using (providerid) union 
        select distinct generatedid as healthid from clientmap inner join pd_all using (zillaid, upazilaid, unionid) 
        ) all_id using(healthid) 

        )";

    std::string pregwomen_query_old =
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

    std::string child_care_service_details =
        R"(
            select ccsd.* from (
                select ccs.* from (
                    with pd as (select zillaid, upazilaid, unionid from providerdb where providerid = $1 and active = 1)
                    select distinct healthid from child_care_service where providerid = $1
                    UNION
                    select generatedid as healthid from clientmap inner join pd using (zillaid, upazilaid, unionid)
                    order by healthid
                ) sq inner join child_care_service as ccs using (healthid)
            order by healthid, systementrydate
            ) as ccs
            inner join child_care_service_detail as ccsd on ccs.healthid = ccsd.healthid and ccs.systementrydate = ccsd.entrydate;
        )";

    std::string db_selection_query =
                std::string("select format('RHIS_%1s_%s', zillaid, LPAD(upazilaid::text, 2, '0')) ") +
                std::string("as db from providerdb where providerid = $1");

    std::string search_replace_all(const std::string& str, const std::string& search_str, const std::string& replace_str);
};



#endif /* SQLITE_COPY_QUERY_BUILDER_H_ */
