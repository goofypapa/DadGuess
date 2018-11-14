//
//  DataTableCardBatch.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/14.
//

#ifndef __DATA_TABLE_CARD_BATCH_H__
#define __DATA_TABLE_CARD_BATCH_H__

#include "DataBase.h"

#define DataTableCardBatchName "table_card_batch"

#define DataTableCardCreateSql "CREATE TABLE IF NOT EXISTS " DataTableCardBatchName "( "\
"id TEXT PRIMARY KEY NOT NULL, "\
"name TEXT NOT NULL, "\
"synopsis TEXT NOT NULL, "\
"cover TEXT NOT NULL, "\
"coverMd5 TEXT NOT NULL, "\
"isUsable BOOLEAN NOT NULL "\
")"

#define DataTableCardDrapSql "DROP TABLE " DataTableCardBatchName

class DataCardBatchInfo : public DataBaseInfo
{
public:
    virtual std::string toJson( void ) override;
};

#endif //__DATA_TABLE_CARD_BATCH_H__
