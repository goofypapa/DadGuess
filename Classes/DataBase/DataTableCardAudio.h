//
//  DataTableCardAudio.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/15.
//

#ifndef __DATA_TABLE_CARD_AUDIO_H__
#define __DATA_TABLE_CARD_AUDIO_H__

#include "DataBase.h"

#define DataTableCardAudioName "table_card_audio"

#define DataTableCardAudioCreateSql "CREATE TABLE IF NOT EXISTS " DataTableCardAudioName "( "\
"id TEXT PRIMARY KEY NOT NULL, "\
"cardId TEXT NOT NULL, "\
"fileId TEXT NOT NULL, "\
"audioType INTEGER NOT NULL "\
")"

#define DataTableCardAudioDrapSql "DROP TABLE " DataTableCardAudioName

class DataCardAudioInfo : public DataBaseInfo
{
public:
    
    enum AudioType{ commentary = 0, hue };
    
    DataCardAudioInfo();
    DataCardAudioInfo( const std::string & p_id, const std::string & p_cardId, const std::string & p_fileId, AudioType p_audioType );
    virtual std::string toJson( void ) const override;
    
    std::string id, cardId, fileId;
    AudioType audioType;
};

class DataTableCardAudio
{
public:
    static DataTableCardAudio & instance( void );
    
    bool insert( const DataCardAudioInfo & p_cardAudioInfo ) const;
    
    std::vector< DataCardAudioInfo > list( const std::string & p_cardId ) const;
    
    DataCardAudioInfo find( const std::string & p_id ) const;
    
    bool update( const DataCardAudioInfo & p_cardAudioInfo ) const;
    
    bool remove( const std::string & p_id ) const;
    bool removeWithCardId( const std::string & p_cardId ) const;
    
    bool drop( void ) const;
private:
    bool init( void ) const;
    
    
    DataCardAudioInfo dataRowToDataCardAudioInfo( std::map<std::string, std::string> & p_dataRow ) const;
};

#endif //__DATA_TABLE_CARD_AUDIO_H__
