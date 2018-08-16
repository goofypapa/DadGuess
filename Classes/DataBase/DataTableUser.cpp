#include "DataTableUser.h"
#include "DataBase.h"
#include <stdio.h>
#include <sstream>

DataUser::DataUser() : userId(""), userName(""), userBirthday(""), userSex(2), loginName(""), headImg(""), token(""), loginType(LoginType::phone), activation(0)
{
    
}

std::string DataUser::toJson( void )
{
    std::stringstream t_sstr;
    
    t_sstr << "{";
    
    t_sstr << "\"userId\": \"" << userId << "\", ";
    t_sstr << "\"userName\": \"" << userName << "\", ";
    t_sstr << "\"userBirthday\": \"" << userBirthday << "\", ";
    t_sstr << "\"userSex\": " << userSex << ", ";
    t_sstr << "\"loginName\": \"" << loginName << "\", ";
    t_sstr << "\"headImg\": \"" << headImg << "\", ";
    t_sstr << "\"token\": \"" << token << "\", ";
    t_sstr << "\"loginType\": " << loginType << ", ";
    t_sstr << "\"activation\": " << activation ;
    
    t_sstr << "}";
    
    return t_sstr.str();
}

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

//    drop();

    if( !DataBase::instance().exec( DataTableUserCreateSql ) )
    {
        return false;
    }

    return true;
}

bool DataTableUser::insert( const DataUser & p_userInfo )
{
    return insert( p_userInfo.userId, p_userInfo.userName, p_userInfo.loginName, p_userInfo.userSex, p_userInfo.userBirthday, p_userInfo.headImg, p_userInfo.token, p_userInfo.loginType, p_userInfo.activation );
}

bool DataTableUser::insert( const std::string & p_userId, const std::string & p_userName, const std::string & p_loginName, const unsigned int p_userSex, const std::string & p_userBirthday, const std::string & p_headImg, const std::string & p_token, DataUser::LoginType p_loginType, const bool p_activation )
{
    std::stringstream t_ssql;

    t_ssql << "INSERT INTO " << DataTableUserName << "( userId, userName, loginName, userSex, userBirthday, headImg, token, loginType, activation ) VALUES( "
                << "\"" << p_userId << "\", "
                << "\"" << p_userName << "\", "
                << "\"" << p_loginName << "\", "
                << p_userSex << ", "
                << "\"" << p_userBirthday << "\", "
                << "\"" << p_headImg << "\", "
                << "\"" << p_token << "\", "
                << p_loginType << ", "
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
        t_result.push_back( dataRowToDataUser( t_row ) );
    }

    return t_result;
}

DataUser DataTableUser::find( const std::string & p_userId )
{
    DataUser t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM table_user WHERE userId= \"" << p_userId << "\"";
    std::string t_sql = t_ssql.str();

    auto t_list = DataBase::instance().query( t_sql.c_str() );

    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataUser( *t_list.begin() );
    }

    return t_result;
}

DataUser DataTableUser::getActivation( void )
{
    DataUser t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM table_user WHERE activation= 1";
    std::string t_sql = t_ssql.str();
    
    auto t_list = DataBase::instance().query( t_sql.c_str() );
    
    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataUser( *t_list.begin() );

    }else if( t_list.size() )
    {
        logout();
    }
    
    return t_result;
}


bool DataTableUser::update( const DataUser & p_userInfo )
{
    bool t_needUpdate = false;

    DataUser t_oldInfo = find( p_userInfo.userId );
    if( t_oldInfo.userId.length() <= 0 )
    {
        return false;
    }

    std::stringstream t_ssql;
    t_ssql << "UPDATE table_user SET ";

    if( p_userInfo.userName != t_oldInfo.userName )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "userName = \"" << p_userInfo.userName << "\"";
    }

    if( p_userInfo.loginName != t_oldInfo.loginName )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }

        t_ssql << "loginName = \"" << p_userInfo.loginName << "\"";
    }
    
    if( p_userInfo.userSex != t_oldInfo.userSex )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "userSex = " << p_userInfo.userSex;
    }
    
    if( p_userInfo.userBirthday != t_oldInfo.userBirthday )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "userBirthday = \"" << p_userInfo.userBirthday << "\"";
    }
    
    if( p_userInfo.headImg != t_oldInfo.headImg )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "headImg = \"" << p_userInfo.headImg << "\"";
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
    
    if( p_userInfo.loginType != t_oldInfo.loginType )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "loginType = " << p_userInfo.loginType;
    }
    
    if( p_userInfo.activation != t_oldInfo.activation )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }

        t_ssql << "activation = " << ( p_userInfo.activation ? 1 : 0 );
    }

    if( !t_needUpdate )
    {
        return true;
    }

    t_ssql << " WHERE userId=\"" << p_userInfo.userId << "\"";

    std::string t_sql = t_ssql.str();

    printf( "sql: %s \n", t_sql.c_str() );

    return DataBase::instance().exec( t_sql.c_str() );
}


bool DataTableUser::remove( const std::string & p_userId )
{
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM table_user WHERE userId=\"" << p_userId << "\"";
    std::string t_sql = t_ssql.str();

    return DataBase::instance().exec( t_sql.c_str() );
}

bool DataTableUser::logout( void )
{
    return DataBase::instance().exec( "UPDATE table_user SET activation=0 WHERE activation=1" );
}

bool DataTableUser::drop( void )
{
    return DataBase::instance().exec( DataTableUserDrapSql );
}

DataUser DataTableUser::dataRowToDataUser( std::map<std::string, std::string> & p_dataRow )
{
    DataUser t_result;

    t_result.userId = p_dataRow["userId"].c_str();
    t_result.userName = p_dataRow["userName"];
    t_result.loginName = p_dataRow["loginName"];
    t_result.userSex = atoi( p_dataRow["userSex"].c_str() );
    t_result.userBirthday = p_dataRow["userBirthday"];
    t_result.headImg = p_dataRow["headImg"];
    t_result.token = p_dataRow["token"];
    t_result.loginType = (DataUser::LoginType)atoi( p_dataRow["loginType"].c_str() );
    t_result.activation = atoi( p_dataRow["activation"].c_str() ) == 1;

    return t_result;
}
