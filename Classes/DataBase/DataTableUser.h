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
    
    enum LoginType{
        phone = 0,
        wechat,
        sina
    };
    
    DataUserInfo();
    DataUserInfo( const std::string & p_userId, const std::string & p_userName, const std::string & p_loginName, const unsigned int p_userSex, const std::string & p_userBirthday, const std::string & p_headImg, const std::string & p_token, LoginType p_loginType, const bool p_activation );
    
    virtual std::string toJson( void ) const override;
    
    int userSex;
    std::string userId, userName, loginName, userBirthday, headImg, token;
    LoginType loginType;
    bool activation;
};

class DataTableUser
{
public:

    static DataTableUser & instance( void );

    bool insert( const DataUserInfo & p_userInfo ) const;

    std::vector< DataUserInfo > list( void ) const;
    DataUserInfo find( const std::string & p_userId ) const;
    
    DataUserInfo getActivation( void ) const;

    bool update( const DataUserInfo & p_userInfo ) const;
    bool remove( const std::string & p_userId ) const;
    
    bool logout( void ) const;
    
    bool drop( void ) const;

protected:
    bool init( void ) const;

private:
    DataUserInfo dataRowToDataUserInfo( std::map<std::string, std::string> & p_dataRow ) const;
};

#endif //__DATA_TABLE_USER_H__
