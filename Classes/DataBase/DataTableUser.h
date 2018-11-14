#ifndef __DATA_TABLE_USER_H__
#define __DATA_TABLE_USER_H__

#include <string>
#include <vector>
#include <map>
#include "DataBase.h"

#define DataTableUserName "table_user"

#define DataTableUserCreateSql "CREATE TABLE IF NOT EXISTS " DataTableUserName "("\
                "userId TEXT PRIMARY KEY NOT NULL, "\
                "userName TEXT NOT NULL, "\
                "loginName TEXT NOT NULL, "\
                "userSex INTEGER NOT NULL, "\
                "userBirthday TEXT NOT NULL, "\
                "headImg TEXT NOT NULL, "\
                "token TEXT NOT NULL, "\
                "loginType INTEGER NOT NULL, "\
                "activation INTEGER NOT NULL"\
                ")"
#define DataTableUserDrapSql "DROP TABLE " DataTableUserName

class DataUserInfo : public DataBaseInfo
{
public:
    
    DataUserInfo();
    
    virtual std::string toJson( void ) override;

    enum LoginType{
        phone = 0,
        wechat,
        sina
    };
    
    int userSex;
    std::string userId, userName, loginName, userBirthday, headImg, token;
    LoginType loginType;
    bool activation;
};

class DataTableUser
{
public:

    static DataTableUser & instance( void );

    bool insert( const DataUserInfo & p_userInfo );
    bool insert( const std::string & p_userId, const std::string & p_userName, const std::string & p_loginName, const unsigned int p_userSex, const std::string & p_userBirthday, const std::string & p_headImg, const std::string & p_token, DataUserInfo::LoginType p_loginType, const bool p_activation );

    std::vector< DataUserInfo > list( void );
    DataUserInfo find( const std::string & p_userId );
    
    DataUserInfo getActivation( void );

    bool update( const DataUserInfo & p_userInfo );
    bool remove( const std::string & p_userId );
    
    bool logout( void );
    
    bool drop( void );

protected:
    bool init( void );

private:
    DataUserInfo dataRowToDataUserInfo( std::map<std::string, std::string> & p_dataRow );
};

#endif //__DATA_TABLE_USER_H__
