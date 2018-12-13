#include "DataTableKeyValue.h"
#include <sstream>
#include <stdlib.h>

DataKeyValueInfo::DataKeyValueInfo() : key( "" ), value( "" ), date( 0 )
{

}

DataKeyValueInfo::DataKeyValueInfo( const std::string & p_key, const std::string & p_value, const long p_date ) : key( p_key ), value(p_value), date( p_date )
{

}

DataKeyValueInfo::DataKeyValueInfo( const std::string & p_key, const bool p_value, const long p_date ) : key( p_key ), date( p_date )
{
    std::stringstream t_sstr;

    t_sstr << ( p_value ? "true" : "false" );

    value = t_sstr.str();
}

std::string DataKeyValueInfo::toJson( void ) const
{
    std::stringstream t_sstr;

    t_sstr << "{";
    
    t_sstr << "\"key\": \"" << key << "\", ";
    t_sstr << "\"value\": \"" << value << "\", ";
    t_sstr << "\"date\": " << date;
    
    t_sstr << "}";
    
    return t_sstr.str();
}

bool DataKeyValueInfo::getBooleanValue( void )
{
    return value.compare( "true" ) == 0;
}
int DataKeyValueInfo::getIntValue( void )
{
    return atoi( value.c_str() );
}
long DataKeyValueInfo::getLongValue( void )
{
    return atol( value.c_str() );
}
float DataKeyValueInfo::getFloatValue( void )
{
    return getDoubleValue();
}
double DataKeyValueInfo::getDoubleValue( void )
{
    return atof( value.c_str() );
}


DataTableKeyValue & DataTableKeyValue::instance( void )
{
    static DataTableKeyValue * s_instance = nullptr;
    if( !s_instance )
    {
        s_instance = new DataTableKeyValue();
        if( !s_instance->init() )
        {
            printf( "create DataTableKeyValue fial \n" );
        }
    }

    return *s_instance;
}

bool DataTableKeyValue::set( const DataKeyValueInfo & p_keyValueInfo ) const
{
    time_t t_curTime;
    time(&t_curTime);

    auto t_keyValueInfo = get( p_keyValueInfo.key );

    std::stringstream t_ssql;
    
    if( t_keyValueInfo.key.empty() )
    {
        t_ssql << "INSERT INTO " << DataTableKeyValueName << "( _key, _value, date ) VALUES( "
        << "\"" << p_keyValueInfo.key << "\", "
        << "\"" << p_keyValueInfo.value << "\", "
        << t_curTime
        << ");";
    
    }else{
        t_ssql << "UPDATE " << DataTableKeyValueName << " SET "
        << "_value = \"" << p_keyValueInfo.value << "\", "
        << "date = " << t_curTime
        << " WHERE _key = \"" << p_keyValueInfo.key << "\";";
    }

    std::string t_sql = t_ssql.str();
    if( !DataBase::instance().exec( t_sql ) )
    {
        return false;
    } 

    return true;
}

 DataKeyValueInfo DataTableKeyValue::get( const std::string & p_key ) const
 {
    DataKeyValueInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableKeyValueName <<  " WHERE _key= \"" << p_key << "\"";

    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_ssql.str() );

    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataKeyValue( *t_list.begin() );
    }
    DataBase::sm_mutex.unlock();

    return t_result;
 }

bool DataTableKeyValue::init( void )
{
    if( !DataBase::instance().exec( DataTableKeyValueCreateSql ) )
    {
        return false;
    }
    return true;
}

DataKeyValueInfo DataTableKeyValue::dataRowToDataKeyValue( std::map<std::string, std::string> & p_dataRow ) const
{
    return DataKeyValueInfo( p_dataRow["_key"], p_dataRow["_value"], atoi( p_dataRow["date"].c_str() ) );
}

