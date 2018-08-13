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

bool DataTableUser::insert( const DataUser & p_userInfo )
{
    return insert( p_userInfo.phone, p_userInfo.name, p_userInfo.sex, p_userInfo.birthday, p_userInfo.token, p_userInfo.actication );
}

bool DataTableUser::insert( const std::string & p_phone, const std::string & p_name, const unsigned int p_sex, const std::string & p_birthday, const std::string & p_token, const bool p_activation )
{
    std::stringstream t_ssql;

    t_ssql << "INSERT INTO " << DataTableUserName << "( phone, name, sex, birthday, token, activation ) VALUES( "
                << "\"" << p_phone << "\", "
                << "\"" << p_name << "\", "
                << p_sex << ", "
                << "\"" << p_birthday << "\", "
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

std::vector< DataUser > DataTableUser::list( void )
{
    std::vector< DataUser > t_result;
    auto t_list = DataBase::instance().query( "SELECT * FROM table_user" );

    for( auto t_row : t_list )
    {
        DataUser t_userInfo;
        t_userInfo.uid = atoi( t_row["id"].c_str() );
        t_userInfo.phone = t_row["phone"];
        t_userInfo.name = t_row["name"];
        t_userInfo.token = t_row["token"];
        t_userInfo.actication = atoi( t_row["actication"].c_str() ) == 1;

        t_result.push_back( t_userInfo );
    }

    return t_result;
}

DataUser DataTableUser::find( const int p_id )
{
    DataUser t_result;
    t_result.uid = -1;

    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM table_user WHERE id=" << p_id;
    std::string t_sql = t_ssql.str();

    auto t_list = DataBase::instance().query( t_sql.c_str() );

    if( t_list.size() == 1 )
    {
        auto t_dataInfo = *t_list.begin();
        t_result.uid = atoi( t_dataInfo["id"].c_str() );
        t_result.phone = t_dataInfo["phone"];
        t_result.name = t_dataInfo["name"];
        t_result.token = t_dataInfo["token"];
        t_result.actication = ( t_dataInfo["actication"] == "1" );
    }

    return t_result;
}


bool DataTableUser::update( const DataUser & p_userInfo )
{
    bool t_needUpdate = false;

    DataUser t_oldInfo = find( p_userInfo.uid );
    if( t_oldInfo.uid < 0 )
    {
        return false;
    }

    std::stringstream t_ssql;
    t_ssql << "UPDATE table_user SET ";

    if( p_userInfo.phone != t_oldInfo.phone )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "phone = \"" << p_userInfo.phone << "\"";
    }

    if( p_userInfo.name != t_oldInfo.name )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }

        t_ssql << "name = \"" << p_userInfo.name << "\"";
    }

    if( p_userInfo.token != t_oldInfo.token )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }

        t_ssql << "token = \"" << p_userInfo.token << "\"";
    }
    
    if( p_userInfo.actication != t_oldInfo.actication )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }

        t_ssql << "actication = " << ( p_userInfo.actication ? 1 : 0 );
    }

    if( !t_needUpdate )
    {
        return true;
    }

    t_ssql << " WHERE id=" << p_userInfo.uid;

    std::string t_sql = t_ssql.str();

    printf( "sql: %s \n", t_sql.c_str() );

    return DataBase::instance().exec( t_sql.c_str() );
}


bool DataTableUser::remove( const int p_id )
{
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM table_user WHERE id=" << p_id;
    std::string t_sql = t_ssql.str();

    return DataBase::instance().exec( t_sql.c_str() );
}
