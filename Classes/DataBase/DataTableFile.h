//
//  DataTableFile.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/20.
//

#ifndef __DATA_TABLE_FILE_H__
#define __DATA_TABLE_FILE_H__

#include <string>


#define DataTableFileName "table_file"

#define DataTableFileCreateSql "CREATE TABLE IF NOT EXISTS table_file( "\
"fileId TEXT PRIMARY KEY NOT NULL, "\
"sourceUrl TEXT NOT NULL, "\
"fileName TEXT NOT NULL, "\
"fileMd5 TEXT NOT NULL "\
")"
#define DataTableFileDrapSql "DROP TABLE table_file"

class DataFile
{
public:
    DataFile();

    std::string toJson( void );
    
    std::string fileId, sourceUrl, fileName, fileMd5;
};


class DataTableFile
{
public:
    static DataTableFile & instance( void );

    bool insert( const DataFile & p_fileInfo );
    bool insert( const std::string & p_fileId, const std::string & p_sourceUrl, const std::string & p_fileName, const std::string & p_fileMd5 );

    std::vector< DataFile > list( void );

    DataFile find( const std::string & p_fileId );
    DataFile findBySourceUrl( const std::string & p_sourceUrl );

    bool update( const DataFile & p_fileInfo );
    bool remove( const std::string & p_fileId );

    bool drop( void );

protected:
    bool init( void );

private:
    DataFile dataRowToDataUser( std::map<std::string, std::string> & p_dataRow );
};


#endif //__DATA_TABLE_FILE_H__
