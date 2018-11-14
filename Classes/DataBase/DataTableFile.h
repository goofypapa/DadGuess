//
//  DataTableFile.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/20.
//

#ifndef __DATA_TABLE_FILE_H__
#define __DATA_TABLE_FILE_H__

#include <string>
#include <map>
#include "DataBase.h"


#define DataTableFileName "table_file"

#define DataTableFileCreateSql "CREATE TABLE IF NOT EXISTS " DataTableFileName "( "\
"fileId TEXT PRIMARY KEY NOT NULL, "\
"sourceUrl TEXT NOT NULL, "\
"fileName TEXT NOT NULL, "\
"fileMd5 TEXT NOT NULL "\
")"
#define DataTableFileDrapSql "DROP TABLE " DataTableFileName

class DataFileInfo : public DataBaseInfo
{
public:
    DataFileInfo();

    virtual std::string toJson( void ) const override;
    
    std::string fileId, sourceUrl, fileName, fileMd5;
};


class DataTableFile
{
public:
    static DataTableFile & instance( void );

    bool insert( const DataFileInfo & p_fileInfo );
    bool insert( const std::string & p_fileId, const std::string & p_sourceUrl, const std::string & p_fileName, const std::string & p_fileMd5 );

    std::vector< DataFileInfo > list( void );

    DataFileInfo find( const std::string & p_fileId );
    DataFileInfo findBySourceUrl( const std::string & p_sourceUrl );

    bool update( const DataFileInfo & p_fileInfo );
    bool remove( const std::string & p_fileId );

    bool drop( void );

protected:
    bool init( void );

private:
    DataFileInfo dataRowToDataUser( std::map<std::string, std::string> & p_dataRow );
};


#endif //__DATA_TABLE_FILE_H__
