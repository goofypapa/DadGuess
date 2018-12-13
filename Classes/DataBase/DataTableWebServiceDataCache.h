#ifndef __DATA_TABLE_WEB_SERVICE_DATA_CACHE_H__
#define __DATA_TABLE_WEB_SERVICE_DATA_CACHE_H__

#include <string>
#include <vector>
#include <map>
#include "DataBase.h"

#define DataTableWebServiceDataCacheName "table_web_service_data_cache"

#define DataTableWebServiceDataCacheCreateSql "CREATE TABLE IF NOT EXISTS " DataTableWebServiceDataCacheName "("\
                "id TEXT PRIMARY KEY NOT NULL, "\
                "url TEXT NOT NULL, "\
                "res TEXT NOT NULL, "\
                "date INTEGER NOT NULL "\
                ")"
#define DataTableWebServiceDataCacheDrapSql "DROP TABLE " DataTableWebServiceDataCacheName

class DataWebServiceDataCacheInfo: public DataBaseInfo
{
public:
    DataWebServiceDataCacheInfo( void );

    DataWebServiceDataCacheInfo( const std::string & p_id, const std::string & p_url, const std::string & p_res, const long p_date );

    virtual std::string toJson( void ) const override;

    std::string id, url, res;
    long date;
};


class DataTableWebServiceDataCache
{
public:
    static DataTableWebServiceDataCache & instance( void );

    bool insert( const DataWebServiceDataCacheInfo & p_webServiceDataCacheInfo ) const;

    bool update( const DataWebServiceDataCacheInfo & p_webServiceDataCacheInfo ) const;

    bool remove( const std::string & p_url ) const;

    DataWebServiceDataCacheInfo find( const std::string & p_url ) const;

    bool drop( void ) const;

protected:
    virtual bool init( void );
private:
    DataWebServiceDataCacheInfo dataRowToDataWebServiceDataCacheInfo( std::map<std::string, std::string> & p_dataRow ) const;
};

#endif //__DATA_TABLE_WEB_SERVICE_DATA_CACHE_H__
