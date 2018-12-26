//
//  DataTableFile.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/20.
//

#include "DataTableFile.h"
#include <sstream>
#include "Common.h"
#include "cocos2d.h"

USING_NS_CC;

DataFileInfo::DataFileInfo() : fileId( "" ), sourceUrl( "" ), fileName( "" ), fileMd5( "" )
{

}

std::string DataFileInfo::toJson( void ) const
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


const bool DataTableFile::insert( const DataFileInfo & p_fileInfo ) const
{
    std::stringstream t_ssql;
    
    t_ssql << "INSERT INTO " << DataTableFileName << "( fileId, sourceUrl, fileName, fileMd5 ) VALUES( "
    << "\"" << p_fileInfo.fileId << "\", "
    << "\"" << p_fileInfo.sourceUrl << "\", "
    << "\"" << p_fileInfo.fileName << "\", "
    << "\"" << p_fileInfo.fileMd5 << "\""
    << ");";
    std::string t_sql = t_ssql.str();
    if( !DataBase::instance().exec( t_sql ) )
    {
        return false;
    }
    
    return true;
}

const std::vector< DataFileInfo > DataTableFile::list( void ) const
{
    std::vector< DataFileInfo > t_result;
    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( std::string( "SELECT * FROM " ) + DataTableFileName );

    for( auto t_row : t_list )
    {
        t_result.push_back( dataRowToDataUser( t_row ) );
    }
    DataBase::sm_mutex.unlock();

    return t_result;
}


const DataFileInfo DataTableFile::find( const std::string & p_fileId ) const
{
    DataFileInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableFileName <<  " WHERE fileId= \"" << p_fileId << "\"";

    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_ssql.str() );

    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataUser( *t_list.begin() );
    }
    DataBase::sm_mutex.unlock();

    return t_result;
}

const DataFileInfo DataTableFile::findBySourceUrl( const std::string & p_sourceUrl ) const
{
    DataFileInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableFileName <<  " WHERE sourceUrl = \"" << p_sourceUrl << "\"";

    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_ssql.str() );

    if( t_list.size() >= 1 )
    {
        t_result = dataRowToDataUser( *t_list.begin() );
    }
    DataBase::sm_mutex.unlock();

    return t_result;
}


const bool DataTableFile::update( const DataFileInfo & p_fileInfo ) const
{
    bool t_needUpdate = false;

    DataFileInfo t_oldInfo = find( p_fileInfo.fileId );
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

const bool DataTableFile::remove( const std::string & p_fileId ) const
{
    
    auto t_fileInfo = find( p_fileId );
    
    if( t_fileInfo.fileId.compare( p_fileId ) != 0 )
    {
        return false;
    }
    
    return remove( t_fileInfo );
}

const bool DataTableFile::remove( const DataFileInfo & p_fileInfo ) const
{
    
    if( FileUtils::getInstance()->isFileExist( p_fileInfo.fileName ) )
    {
        FileUtils::getInstance()->removeFile( FileUtils::getInstance()->fullPathForFilename( p_fileInfo.fileName ) );
    }
    
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM " << DataTableFileName << " WHERE fileId=\"" << p_fileInfo.fileId << "\"";
    return DataBase::instance().exec( t_ssql.str() );
}

const bool DataTableFile::drop( void ) const
{
    return DataBase::instance().exec( DataTableFileDrapSql );
}


const bool DataTableFile::init( void ) const
{
    // drop();

    if( !DataBase::instance().exec( DataTableFileCreateSql ) )
    {
        return false;
    }

    return true;
}


const DataFileInfo DataTableFile::dataRowToDataUser( std::map<std::string, std::string> & p_dataRow ) const
{
    DataFileInfo t_result;

    t_result.fileId = p_dataRow["fileId"];
    t_result.sourceUrl = p_dataRow["sourceUrl"];
    t_result.fileName = p_dataRow["fileName"];
    t_result.fileMd5 = p_dataRow["fileMd5"];

    return t_result;
}
