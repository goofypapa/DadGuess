#include "DataTableUser.h"
#include "DataBase.h"
#include <stdio.h>
#include <sstream>

DataTableUser & DataTableUser::instance( void )
{
    static DataTableUser * sm_dataTableUser = nullptr;

    if( sm_dataTableUser == nullptr )
    {
        sm_dataTableUser = new DataTableUser();
        if( !sm_dataTableUser->init() )
        {
            printf( "DataTableUset init final" );
        }
    }

    return *sm_dataTableUser;
}

bool DataTableUser::init( void )
{

    DataBase::instance().exec( DataTableUserDrapSql );

    if( !DataBase::instance().exec( DataTableUserCreateSql ) )
    {
        return false;
    }

    return true;
}

bool DataTableUser::insert( const std::string & p_phone, const std::string & p_name, const std::string & p_token, const bool p_activation )
{
    std::stringstream t_ssql;

    t_ssql << "INSERT INTO " << DataTableUserName << "( phone, name, token, activation ) VALUES( "
                << "\"" << p_phone << "\", "
                << "\"" << p_name << "\", "
                << "\"" << p_token << "\", "
                << ( p_activation ? 1 : 0 ) << " "
                << ");";
    std::string t_sql = t_ssql.str();
    if( !DataBase::instance().exec( t_sql.c_str() ) )
    {
        return false;
    }

    return true;
}

std::vector< std::map<std::string, std::string> > DataTableUser::list( void )
{
    return DataBase::instance().query( "SELECT * FROM table_user" );
}
