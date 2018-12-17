#include "DataBase.h"
#include <stdio.h>
#include "platform/CCFileUtils.h"

USING_NS_CC;

DataBase::QueryBack * DataBase::m_queryBack = nullptr;

std::mutex DataBase::sm_mutex;

DataBase & DataBase::instance( void )
{
    static DataBase * sm_dataBase = nullptr;

    if( sm_dataBase == nullptr )
    {
        sm_dataBase = new DataBase();

        if( !sm_dataBase->init() )
        {
            printf( "DataBase Init Final" );
        }
    }

    return *sm_dataBase;
}

int DataBase::callback( void *NotUsed, int argc, char **argv, char **azColName )
{

    std::map< std::string, std::string > t_row;
    int i;
    for( i=0; i<argc; i++ )
    {
        t_row[ azColName[i] ] = argv[i] ? argv[i] : "NULL";
    }
    m_queryBack->push_back( t_row );

    printf("\n");
    return 0;
}

bool DataBase::exec( const std::string & p_sql ) const
{
    char * t_ErrMsg = nullptr;
    // sm_mutex.lock();
    int t_res = sqlite3_exec( m_dataBase, p_sql.c_str(), nullptr, 0, &t_ErrMsg);
    // sm_mutex.unlock();
    if( t_res != SQLITE_OK )
    {
        printf( "SQL error: %s\n", t_ErrMsg);
        sqlite3_free( t_ErrMsg );
        return false;
    }

    return true;
}

DataBase::QueryBack DataBase::query( const std::string & p_sql )
{
    QueryBack t_result;
    m_queryBack = &t_result;
    char * t_ErrMsg = nullptr;
    int t_res = sqlite3_exec( m_dataBase, p_sql.c_str(), callback, 0, &t_ErrMsg);
    if( t_res != SQLITE_OK )
    {
        printf( "SQL error: %s\n", t_ErrMsg);
        sqlite3_free( t_ErrMsg );
    }

    return t_result;
}

bool DataBase::init( void )
{
    m_dataBase = nullptr;

    auto t_writePath = FileUtils::getInstance()->getWritablePath();
    
    int t_openRes = sqlite3_open( ( t_writePath + "goofypapa_db" ).c_str(), &m_dataBase );
    if( t_openRes )
    {
        printf( "sqlite open final\n" );
        return false;
    }

    return true;
}

DataBase::~DataBase( void )
{
    if( m_dataBase != nullptr )
    {
        sqlite3_close( m_dataBase );
    }
}
