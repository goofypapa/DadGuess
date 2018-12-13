#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "sqlite3/sqlite3.h"
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <mutex>

class DataBaseInfo
{
public:
    virtual std::string toJson( void ) const = 0;
};

class DataBase
{
public:

    typedef std::vector< std::map<std::string, std::string> > QueryBack;

    static DataBase & instance( void );

    static int callback( void *NotUsed, int argc, char **argv, char **azColName );

    bool exec( const std::string & p_sql ) const;
    QueryBack query( const std::string & p_sql );

    static std::mutex sm_mutex;
protected:
    bool init( void );
    ~DataBase( void );
    
private:
    sqlite3 * m_dataBase;
    static QueryBack * m_queryBack;
    
};

#endif //__DATABASE_H__
