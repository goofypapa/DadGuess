//
//  DataTableFile.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/20.
//

#include "DataTableFile.h"
#include "DataBase.h"
#include <sstream>
#include "Common.h"

DataFile::DataFile() : fileId( "" ), sourceUrl( "" ), fileName( "" ), fileMd5( "" )
{

}

std::string DataFile::toJson( void )
{
    std::stringstream t_sstr;
    
    t_sstr << "{ ";
    
    t_sstr << "\"fileId\": \"" << fileId << "\", ";
    t_sstr << "\"sourceUrl\": \"" << sourceUrl << "\", ";
    t_sstr << "\"fileName\": \"" << fileName << "\", ";
    t_sstr << "\"fileMd5\": \"" << fileMd5 << "\"";
    
    t_sstr << " }";
    
    return t_sstr.str();
}

DataTableFile & DataTableFile::instance( void )
{
    static DataTableFile * sm_instance = nullptr;
    if( !sm_instance )
    {
        sm_instance = new DataTableFile();

        if( !sm_instance->init() )
        {
            printf( "DataTableFile init faild! \n" );
        }
    }
    
    return *sm_instance;
}


bool DataTableFile::insert( const DataFile & p_fileInfo )
{
    return insert( p_fileInfo.fileId, p_fileInfo.sourceUrl, p_fileInfo.fileName, p_fileInfo.fileMd5 );
}

bool DataTableFile::insert( const std::string & p_fileId, const std::string & p_sourceUrl, const std::string & p_fileName, const std::string & p_fileMd5 )
{
    std::stringstream t_ssql;

    t_ssql << "INSERT INTO " << DataTableFileName << "( fileId, sourceUrl, fileName, fileMd5 ) VALUES( "
                << "\"" << p_fileId << "\", "
                << "\"" << p_sourceUrl << "\", "
                << "\"" << p_fileName << "\", "
                << "\"" << p_fileMd5 << "\""
                << ");";
    std::string t_sql = t_ssql.str();
    if( !DataBase::instance().exec( t_sql ) )
    {
        return false;
    }

    return true;
}

std::vector< DataFile > DataTableFile::list( void )
{
    std::vector< DataFile > t_result;
    auto t_list = DataBase::instance().query( std::string( "SELECT * FROM " ) + DataTableFileName );

    for( auto t_row : t_list )
    {
        t_result.push_back( dataRowToDataUser( t_row ) );
    }

    return t_result;
}


DataFile DataTableFile::find( const std::string & p_fileId )
{
    DataFile t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableFileName <<  " WHERE fileId= \"" << p_fileId << "\"";

    auto t_list = DataBase::instance().query( t_ssql.str() );

    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataUser( *t_list.begin() );
    }

    return t_result;
}

DataFile DataTableFile::findBySourceUrl( const std::string & p_sourceUrl )
{
    DataFile t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableFileName <<  " WHERE sourceUrl = \"" << p_sourceUrl << "\"";

    auto t_list = DataBase::instance().query( t_ssql.str() );

    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataUser( *t_list.begin() );
    }

    return t_result;
}


bool DataTableFile::update( const DataFile & p_fileInfo )
{
    bool t_needUpdate = false;

    DataFile t_oldInfo = find( p_fileInfo.fileId );
    if( t_oldInfo.fileId.length() <= 0 )
    {
        return false;
    }

    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableFileName << " SET ";

    if( p_fileInfo.sourceUrl != t_oldInfo.sourceUrl )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "sourceUrl = \"" << p_fileInfo.sourceUrl << "\"";
    }

    if( p_fileInfo.fileName != t_oldInfo.fileName )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "fileName = \"" << p_fileInfo.fileName << "\"";
    }

    if( p_fileInfo.fileMd5 != t_oldInfo.fileMd5 )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "fileMd5 = \"" << p_fileInfo.fileMd5 << "\"";
    }

    if( !t_needUpdate )
    {
        return true;
    }

    t_ssql << " WHERE fileId=\"" << p_fileInfo.fileId << "\"";

    std::string t_sql = t_ssql.str();

    return DataBase::instance().exec( t_sql );

}

bool DataTableFile::remove( const std::string & p_fileId )
{
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM " << DataTableFileName << " WHERE fileId=\"" << p_fileId << "\"";
    return DataBase::instance().exec( t_ssql.str() );
}

bool DataTableFile::drop( void )
{
    return DataBase::instance().exec( DataTableFileDrapSql );
}


bool DataTableFile::init( void )
{
    // drop();

    if( !DataBase::instance().exec( DataTableFileCreateSql ) )
    {
        return false;
    }

    return true;
}


DataFile DataTableFile::dataRowToDataUser( std::map<std::string, std::string> & p_dataRow )
{
    DataFile t_result;

    t_result.fileId = p_dataRow["fileId"];
    t_result.sourceUrl = p_dataRow["sourceUrl"];
    t_result.fileName = p_dataRow["fileName"];
    t_result.fileMd5 = p_dataRow["fileMd5"];

    return t_result;
}
