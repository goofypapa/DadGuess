//
//  DataTableCard.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/30.
//

#include "DataTableCard.h"
#include <sstream>


DataCardInfo::DataCardInfo() : DataCardInfo( "", "", -1, "", "" )
{
    
}

DataCardInfo::DataCardInfo( const std::string & p_id, const std::string & p_batchId, const int p_rfid, const std::string & p_coverFileId, const std::string & p_simpleDrawingFileId ) : id( p_id ), batchId( p_batchId ), rfid( p_rfid ), coverFileId( p_coverFileId ), simpleDrawingFileId( p_simpleDrawingFileId ), activation( false )
{
    
}

std::string DataCardInfo::toJson( void ) const
{
    std::stringstream t_sstr;
    
    t_sstr << "{ ";
    
    t_sstr << "\"id\": \"" << id << "\", ";
    t_sstr << "\"batchId\": \"" << batchId << "\", ";
    t_sstr << "\"rfid\": " << rfid << ", ";
    t_sstr << "\"coverFileId\": \"" << coverFileId << "\", ";
    t_sstr << "\"simpleDrawingFileId\": \"" << simpleDrawingFileId << "\", ";
    t_sstr << "\"activation\": " << ( activation == 1 ? "true" : "false" ) << " ";
    
    t_sstr << " }";
    
    return t_sstr.str();
}


DataTableCard & DataTableCard::instance( void )
{
    static DataTableCard * sm_instance = nullptr;
    if( !sm_instance )
    {
        sm_instance = new DataTableCard();
        
        if( !sm_instance->init() )
        {
            printf( "DataTableCard init faild! \n" );
        }
    }
    
    return *sm_instance;
}

bool DataTableCard::insert( const DataCardInfo & p_cardInfo ) const
{
    std::stringstream t_ssql;
    
    t_ssql << "INSERT INTO " << DataTableCardName << "( id, batchId, rfid, coverFileId, simpleDrawingFileId, activation ) VALUES( "
    << "\"" << p_cardInfo.id << "\", "
    << "\"" << p_cardInfo.batchId << "\", "
    << "\"" << p_cardInfo.rfid << "\", "
    << "\"" << p_cardInfo.coverFileId << "\", "
    << "\"" << p_cardInfo.simpleDrawingFileId << "\", "
    << ( p_cardInfo.activation ? 1 : 0 ) << " "
    << ");";
    std::string t_sql = t_ssql.str();
    
    return DataBase::instance().exec( t_sql );
}

std::vector< DataCardInfo > DataTableCard::list( const std::string & p_batchId ) const
{
    std::vector< DataCardInfo > t_result;
    
    std::stringstream t_ssql;
    
    t_ssql << "SELECT * FROM " << DataTableCardName;
    if( !p_batchId.empty() )
    {
        t_ssql << " WHERE batchId = \"" << p_batchId << "\"";
    }
    
    auto t_list = DataBase::instance().query( t_ssql.str() );
    
    for( auto t_row : t_list )
    {
        t_result.push_back( dataRowToDataCardInfo( t_row ) );
    }
    
    return t_result;
}

DataCardInfo DataTableCard::find( const std::string & p_id ) const
{
    DataCardInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableCardName <<  " WHERE id= \"" << p_id << "\"";
    std::string t_sql = t_ssql.str();
    
    auto t_list = DataBase::instance().query( t_sql );
    
    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataCardInfo( *t_list.begin() );
    }
    
    return t_result;
}

bool DataTableCard::update( const DataCardInfo & p_cardInfo ) const
{
    bool t_needUpdate = false;
    
    DataCardInfo t_oldInfo = find( p_cardInfo.id );
    if( t_oldInfo.id.length() <= 0 )
    {
        return false;
    }
    
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardName << " SET ";
    
    if( p_cardInfo.batchId != t_oldInfo.batchId )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "batchId = \"" << p_cardInfo.batchId << "\"";
    }
    
    if( p_cardInfo.rfid != t_oldInfo.rfid )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "rfid = " << p_cardInfo.rfid;
    }
    
    if( p_cardInfo.coverFileId != t_oldInfo.coverFileId )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "coverFileId = \"" << p_cardInfo.coverFileId << "\"";
    }
    
    if( p_cardInfo.simpleDrawingFileId != t_oldInfo.simpleDrawingFileId )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "simpleDrawingFileId = \"" << p_cardInfo.simpleDrawingFileId << "\"";
    }
    
    if( !t_needUpdate )
    {
        return true;
    }
    
    t_ssql << " WHERE id=\"" << p_cardInfo.id << "\"";
    
    return DataBase::instance().exec( t_ssql.str() );
}

bool DataTableCard::remove( const std::string & p_id ) const
{
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM " << DataTableCardName << " WHERE id=\"" << p_id << "\"";
    std::string t_sql = t_ssql.str();
    
    return DataBase::instance().exec( t_sql );
}

bool DataTableCard::activation( const DataCardInfo & p_cardInfo ) const
{
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardName << " SET activation = 1 WHERE id=\"" << p_cardInfo.id << "\"";
    return DataBase::instance().exec( t_ssql.str() );
}

bool DataTableCard::activation( const std::string & p_batchId ) const
{
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardName << " SET activation = 1 WHERE batchId=\"" << p_batchId << "\"";
    return DataBase::instance().exec( t_ssql.str() );
}

bool DataTableCard::drop( void ) const
{
    return DataBase::instance().exec(  DataTableCardDrapSql );
}

bool DataTableCard::init( void ) const
{
    if( !DataBase::instance().exec( DataTableCardCreateSql ) )
    {
        return false;
    }
    return true;
}

DataCardInfo DataTableCard::dataRowToDataCardInfo( std::map<std::string, std::string> & p_dataRow ) const
{
    auto t_result = DataCardInfo( p_dataRow["id"], p_dataRow["batchId"], atoi( p_dataRow["rfid"].c_str() ), p_dataRow["coverFileId"], p_dataRow["simpleDrawingFileId"] );
    t_result.activation = atoi( p_dataRow["activation"].c_str() ) == 1;
    return t_result;
}
