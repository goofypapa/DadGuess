//
//  DataTableCardBatch.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/14.
//

#ifndef __DATA_TABLE_CARD_BATCH_H__
#define __DATA_TABLE_CARD_BATCH_H__

#include "DataBase.h"
#include <vector>

#define DataTableCardBatchName "table_card_batch"

#define DataTableCardBatchCreateSql "CREATE TABLE IF NOT EXISTS " DataTableCardBatchName "( "\
"id TEXT PRIMARY KEY NOT NULL, "\
"name TEXT NOT NULL, "\
"synopsis TEXT NOT NULL, "\
"coverFileId TEXT NOT NULL, "\
"activation INTEGER NOT NULL "\
")"

#define DataTableCardBatchDrapSql "DROP TABLE " DataTableCardBatchName

class DataCardBatchInfo : public DataBaseInfo
{
public:
    DataCardBatchInfo();
    DataCardBatchInfo( const std::string & p_id, const std::string & p_name, const std::string & p_synopsis, const std::string & p_coverFileId, const bool p_activation = false );
    
    virtual std::string toJson( void ) const override;
    
    std::string id, name, synopsis, coverFileId;
    bool activation;
    
    static const std::vector< std::string > s_batchIdList;
};

class DataTableCardBatch
{
public:
    static DataTableCardBatch & instance( void );
    
    bool insert( const DataCardBatchInfo & p_cardBatchInfo ) const;
    
    std::vector< DataCardBatchInfo > list( void ) const;
    
    DataCardBatchInfo find( const std::string & p_id ) const;
    
    bool update( const DataCardBatchInfo & p_cardBatchInfo ) const;
    bool remove( const std::string & p_id ) const;
    
    bool drop( void ) const;
private:
    bool init( void ) const;
    
    
    DataCardBatchInfo dataRowToDataCardBatchInfo( std::map<std::string, std::string> & p_dataRow ) const;
};

#endif //__DATA_TABLE_CARD_BATCH_H__
