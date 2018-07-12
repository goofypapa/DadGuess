#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "sqlite3.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

class DataBase
{
public:

    typedef std::vector< std::map<std::string, std::string> > QueryBack;

    static DataBase & instance( void );

    // static int callback( void *NotUsed, int argc, char **argv, char **azColName );

    static int callback( void *NotUsed, int argc, char **argv, char **azColName );

    bool exec( const char * p_sql );
    QueryBack query( const char * p_sql );

protected:
    bool init( void );
    ~DataBase( void );
private:
    sqlite3 * m_dataBase;
    static QueryBack * m_queryBack;
};

#endif //__DATABASE_H__