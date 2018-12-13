#include "DataTableWebServiceDataCache.h"

#include <stdlib.h>
#include <sstream>

DataWebServiceDataCacheInfo::DataWebServiceDataCacheInfo( void ) : DataWebServiceDataCacheInfo( "", "", "", 0 )
{

}

DataWebServiceDataCacheInfo::DataWebServiceDataCacheInfo( const std::string & p_id, const std::string & p_url, const std::string & p_res, const long p_date ):
id( p_id ), url( p_url ), res( p_res ), date( p_date )
{

}

std::string DataWebServiceDataCacheInfo::toJson( void ) const
{
    std::stringstream t_sstr;
    
    t_sstr << "{";
    
    t_sstr << "\"id\": \"" << id << "\", ";
    t_sstr << "\"url\": \"" << url << "\", ";
    t_sstr << "\"res\": \"" << res << "\", ";
    t_sstr << "\"date\": " << date ;

    t_sstr << "}";
    
    return t_sstr.str();
}

DataTableWebServiceDataCache & DataTableWebServiceDataCache::instance( void )
{
    static DataTableWebServiceDataCache * s_instance = nullptr;

    if( !s_instance )
    {
        s_instance = new DataTableWebServiceDataCache();
        if( !s_instance->init() )
        {
            printf( "DataTableWebServiceDataCache init fial \n" );
        }
    }
    return *s_instance;
}

bool DataTableWebServiceDataCache::init( void )
{

    //    drop();

    if( !DataBase::instance().exec( DataTableWebServiceDataCacheCreateSql ) )
    {
        return false;
    }

    return true;
}

bool DataTableWebServiceDataCache::insert( const DataWebServiceDataCacheInfo & p_webServiceDataCacheInfo ) const
{
    std::stringstream t_ssql;
    
    t_ssql << "INSERT INTO " << DataTableWebServiceDataCacheName << "( id, url, res, date ) VALUES( "
    << "\"" << p_webServiceDataCacheInfo.id << "\", "
    << "\"" << p_webServiceDataCacheInfo.url << "\", "
    << "\"" << p_webServiceDataCacheInfo.res << "\", "
    << p_webServiceDataCacheInfo.date
    << ");";
    std::string t_sql = t_ssql.str();
    if( !DataBase::instance().exec( t_sql ) )
    {
        return false;
    }
    
    return true;
}


bool DataTableWebServiceDataCache::update( const DataWebServiceDataCacheInfo & p_webServiceDataCacheInfo ) const
{
     bool t_needUpdate = false;
    
    DataWebServiceDataCacheInfo t_oldInfo = find( p_webServiceDataCacheInfo.url );
    if( t_oldInfo.id.empty() )
    {
        return false;
    }

    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableWebServiceDataCacheName << " SET ";

    if( t_oldInfo.res.compare( p_webServiceDataCacheInfo.res ) != 0 )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "res = \"" << p_webServiceDataCacheInfo.res << "\"";
    }

    if( t_oldInfo.date != p_webServiceDataCacheInfo.date )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "date = " << p_webServiceDataCacheInfo.date;
    }

    if( !t_needUpdate )
    {
        return true;
    }

    t_ssql << " WHERE id=\"" << t_oldInfo.id << "\"";

    std::string t_sql = t_ssql.str();
    if( !DataBase::instance().exec( t_sql ) )
    {
        return false;
    }
    
    return true;
}

bool DataTableWebServiceDataCache::remove( const std::string & p_url ) const
{
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM " << DataTableWebServiceDataCacheName << " WHERE url=\"" << p_url << "\"";
    std::string t_sql = t_ssql.str();

    return DataBase::instance().exec( t_sql );
}

DataWebServiceDataCacheInfo DataTableWebServiceDataCache::find( const std::string & p_url ) const
{
    DataWebServiceDataCacheInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableWebServiceDataCacheName <<  " WHERE url= \"" << p_url << "\"";
    std::string t_sql = t_ssql.str();

    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_sql );

    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataWebServiceDataCacheInfo( *t_list.begin() );
    }
    DataBase::sm_mutex.unlock();

    return t_result;
}

bool DataTableWebServiceDataCache::drop( void ) const
{
    return DataBase::instance().exec( DataTableWebServiceDataCacheDrapSql );
}

DataWebServiceDataCacheInfo DataTableWebServiceDataCache::dataRowToDataWebServiceDataCacheInfo( std::map<std::string, std::string> & p_dataRow ) const
{
    DataWebServiceDataCacheInfo t_result;

    t_result.id = p_dataRow["id"];
    t_result.url = p_dataRow["url"];
    t_result.res = p_dataRow["res"];

    t_result.date = atol( p_dataRow["date"].c_str() );

    return t_result;
}
