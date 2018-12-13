#ifndef __DATA_TABLE_KEY_VALUE_H__
#define __DATA_TABLE_KEY_VALUE_H__


#include <string>
#include <map>
#include "DataBase.h"


#define DataTableKeyValueName "table_key_value"

#define DataTableKeyValueCreateSql "CREATE TABLE IF NOT EXISTS " DataTableKeyValueName "( "\
"_key TEXT PRIMARY KEY NOT NULL, "\
"_value TEXT NOT NULL, "\
"date INTEGER NOT NULL "\
")"
#define DataTableKeyValueDrapSql "DROP TABLE " DataTableKeyValueName

class DataKeyValueInfo : public DataBaseInfo
{
public:
    DataKeyValueInfo();

    
    DataKeyValueInfo( const std::string & p_key, const std::string & p_value, const long p_date = 0 );
    DataKeyValueInfo( const std::string & p_key, const bool p_value, const long p_date = 0 );

    bool getBooleanValue( void );
    int getIntValue( void );
    long getLongValue( void );
    float getFloatValue( void );
    double getDoubleValue( void );

    virtual std::string toJson( void ) const;
    
    std::string key, value;
    long date;
};

class DataTableKeyValue
{
public:
    static DataTableKeyValue & instance( void );

    bool set( const DataKeyValueInfo & p_keyValueInfo ) const;
    DataKeyValueInfo get( const std::string & p_key ) const;
protected:
    virtual bool init( void );
private:
    DataKeyValueInfo dataRowToDataKeyValue( std::map<std::string, std::string> & p_dataRow ) const;
};

#endif //__DATA_TABLE_KEY_VALUE_H__
