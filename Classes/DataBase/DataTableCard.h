//
//  DataTableCard.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/30.
//

#ifndef __DATA_TABLE_CARD_H__
#define __DATA_TABLE_CARD_H__

#include "DataBase.h"

#define DataTableCardName "table_card"

#define DataTableCardCreateSql "CREATE TABLE IF NOT EXISTS table_card( "\
"fileId TEXT PRIMARY KEY NOT NULL, "\
"sourceUrl TEXT NOT NULL, "\
"fileName TEXT NOT NULL, "\
"fileMd5 TEXT NOT NULL "\
")"
#define DataTableCardDrapSql "DROP TABLE table_card"

class DataTableCard
{
public:
    static DataTableCard & instance( void );
};

#endif //__DATA_TABLE_CARD_H__
