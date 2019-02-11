//
//  DataTableCard.h
//  DadGuess-mobile
//
//  Created by 吴思 on 2018/10/30.
//

#ifndef __DATA_TABLE_CARD_H__
#define __DATA_TABLE_CARD_H__

#include "DataBase.h"

#define DataTableCardName "table_card"

#define DataTableCardCreateSql "CREATE TABLE IF NOT EXISTS " DataTableCardName "( "\
"id TEXT PRIMARY KEY NOT NULL, "\
"batchId TEXT NOT NULL, "\
"rfid INTEGER NOT NULL, "\
"coverFileUrl TEXT NOT NULL, "\
"coverFileMd5 TEXT NOT NULL, "\
"activation INTEGER NOT NULL "\
")"
#define DataTableCardDrapSql "DROP TABLE " DataTableCardName


class DataCardInfo : public DataBaseInfo
{
public:
    DataCardInfo();
    
    DataCardInfo( const std::string & p_id, const std::string & p_batchId, const int p_rfid, const std::string & p_coverFileUrl, const std::string & p_coverFileMd5 );
    
    virtual std::string toJson( void ) const override;
    
    std::string id, batchId, coverFileUrl, coverFileMd5;
    int rfid;
    bool activation;
};

class DataTableCard
{
public:
    static DataTableCard & instance( void );
    
    bool insert( const DataCardInfo & p_cardInfo ) const;
    
    std::vector< DataCardInfo > list( const std::string & p_batchId = "" ) const;
    
    DataCardInfo find( const std::string & p_id ) const;
    DataCardInfo find( const int p_rfid ) const;
    
    bool update( const DataCardInfo & p_cardInfo ) const;
    bool remove( const std::string & p_id ) const;
    
    bool activation( const DataCardInfo & p_cardInfo ) const;
    static bool activation( const std::string & p_batchId );
    
    bool drop( void ) const;
    
protected:
    bool init( void ) const;
private:
    
    DataCardInfo dataRowToDataCardInfo( std::map<std::string, std::string> & p_dataRow ) const;
};

#endif //__DATA_TABLE_CARD_H__
