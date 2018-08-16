#ifndef __DATA_TABLE_USER_H__
#define __DATA_TABLE_USER_H__

#include <string>
#include <vector>
#include <map>

#define DataTableUserName "table_user"

#define DataTableUserCreateSql "CREATE TABLE IF NOT EXISTS table_user("\
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
#define DataTableUserDrapSql "DROP TABLE table_user"

class DataUser
{
public:
    
    DataUser();
    DataUser( const DataUser & p_dataUser );
    
    std::string toJson( void );

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

    bool insert( const DataUser & p_userInfo );
    bool insert( const std::string & p_userId, const std::string & p_userName, const std::string & p_loginName, const unsigned int p_userSex, const std::string & p_userBirthday, const std::string & p_headImg, const std::string & p_token, DataUser::LoginType p_loginType, const bool p_activation );

    std::vector< DataUser > list( void );
    DataUser find( const std::string & p_userId );
    
    DataUser getActivation( void );

    bool update( const DataUser & p_userInfo );
    bool remove( const std::string & p_userId );
    
    bool logout( void );
    
    bool drop( void );

protected:
    bool init( void );

private:
    DataUser dataRowToDataUser( std::map<std::string, std::string> & p_dataRow );
};

#endif //__DATA_TABLE_USER_H__
