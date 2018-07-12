#ifndef __DATA_TABLE_USER_H__
#define __DATA_TABLE_USER_H__

#include <string>

#define DataTableUserName "table_user"

#define DataTableUserCreateSql "CREATE TABLE IF NOT EXISTS table_user("\
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "\
                "phone TEXT NOT NULL, "\
                "name TEXT NOT NULL, "\
                "token TEXT NOT NULL, "\
                "activation INTEGER NOT NULL"\
                ")"
#define DataTableUserDrapSql "DROP TABLE table_user"

class DataTableUser
{
public:

    static DataTableUser & instance( void );

    bool insert( const std::string & p_phone, const std::string & p_name, const std::string & p_token, const bool p_activation );

    std::vector< std::map<std::string, std::string> > list( void );

protected:
    bool init( void );
};

#endif //__DATA_TABLE_USER_H__