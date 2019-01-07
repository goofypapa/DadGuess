//
//  DataTableCardBatch.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/14.
//

#include "DataTableCardBatch.h"
#include <sstream>

std::string batchIdList[] = { "animal", "ABC", "astronomy", "earth", "worldHistory", "historyChronology", "ceshi", "astrEnglish", "animalEn" };
const std::vector< std::string > DataCardBatchInfo::s_batchIdList( batchIdList, batchIdList + sizeof( batchIdList ) / sizeof( std::string ) );

DataCardBatchInfo::DataCardBatchInfo() : DataCardBatchInfo( "", "" )
{
    
}

DataCardBatchInfo::DataCardBatchInfo( const std::string & p_id, const std::string & p_synopsis, const bool p_activation ) : id( p_id ), synopsis( p_synopsis ), activation( p_activation )
{
    
}

std::string DataCardBatchInfo::toJson( void ) const
{
    std::stringstream t_sstr;
    
    t_sstr << "{ ";
    
    t_sstr << "\"id\": \"" << id << "\", ";
    t_sstr << "\"synopsis\": \"" << synopsis << "\", ";
    t_sstr << "\"activation\": " << ( activation ? "true" : "false" );
    
    t_sstr << " }";
    
    return t_sstr.str();
}


DataTableCardBatch & DataTableCardBatch::instance( void )
{
    static DataTableCardBatch * sm_instance = nullptr;
    if( !sm_instance )
    {
        sm_instance = new DataTableCardBatch();
        
        if( !sm_instance->init() )
        {
            printf( "DataTableCardBatch init faild! \n" );
        }
    }
    
    return *sm_instance;
}

bool DataTableCardBatch::insert( const DataCardBatchInfo & p_cardBatchInfo ) const
{
    
    std::stringstream t_ssql;
    
    t_ssql << "INSERT INTO " << DataTableCardBatchName << "( id, synopsis, activation ) VALUES( "
    << "\"" << p_cardBatchInfo.id << "\", "
    << "\"" << p_cardBatchInfo.synopsis << "\", "
    << 0
    << ");";
    std::string t_sql = t_ssql.str();
    if( !DataBase::instance().exec( t_sql ) )
    {
        return false;
    }
    return true;
}

std::vector< DataCardBatchInfo > DataTableCardBatch::list( void ) const
{
    std::vector< DataCardBatchInfo > t_result;
    
    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( std::string( "SELECT * FROM " DataTableCardBatchName ) );
    
    for( auto t_row : t_list )
    {
        t_result.push_back( dataRowToDataCardBatchInfo( t_row ) );
    }
    DataBase::sm_mutex.unlock();

    return t_result;
}

DataCardBatchInfo DataTableCardBatch::find( const std::string & p_id ) const
{
    DataCardBatchInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableCardBatchName <<  " WHERE id= \"" << p_id << "\"";
    std::string t_sql = t_ssql.str();
    
    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_sql );
    
    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataCardBatchInfo( *t_list.begin() );
    }
    DataBase::sm_mutex.unlock();
    
    return t_result;
}


bool DataTableCardBatch::update( const DataCardBatchInfo & p_cardBatchInfo ) const
{
    bool t_needUpdate = false;
    
    DataCardBatchInfo t_oldInfo = find( p_cardBatchInfo.id );
    if( t_oldInfo.id.length() <= 0 )
    {
        return false;
    }
    
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardBatchName << " SET ";
    
    if( p_cardBatchInfo.synopsis != t_oldInfo.synopsis )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "synopsis = \"" << p_cardBatchInfo.synopsis << "\"";
    }
    
    if( !t_needUpdate )
    {
        return true;
    }
    
    t_ssql << " WHERE id=\"" << p_cardBatchInfo.id << "\"";
    
    return DataBase::instance().exec( t_ssql.str() );
    
}

bool DataTableCardBatch::remove( const std::string & p_id ) const
{
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM " << DataTableCardBatchName << " WHERE id=\"" << p_id << "\"";
    std::string t_sql = t_ssql.str();
    
    return DataBase::instance().exec( t_sql );
}


bool DataTableCardBatch::drop( void ) const
{
    return DataBase::instance().exec( DataTableCardBatchDrapSql );
}

bool DataTableCardBatch::activation( const DataCardBatchInfo & p_cardBatchInfo ) const
{
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardBatchName << " SET activation = 1 WHERE id = \"" << p_cardBatchInfo.id << "\"";
    std::string t_sql = t_ssql.str();
    
    return DataBase::instance().exec( t_sql );
}


bool DataTableCardBatch::init( void ) const
{
    if( !DataBase::instance().exec( DataTableCardBatchCreateSql ) )
    {
        return false;
    }
    return true;
}

DataCardBatchInfo DataTableCardBatch::dataRowToDataCardBatchInfo( std::map<std::string, std::string> & p_dataRow ) const
{
    return DataCardBatchInfo( p_dataRow["id"], p_dataRow["synopsis"], atoi( p_dataRow["activation"].c_str() ) == 1 );
}
