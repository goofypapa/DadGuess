#include "NFCDeviceListener.h"

std::list< NFCDeviceListener * > NFCDeviceListener::sm_NfcStateListenList;

NFCDeviceListener::NFCDeviceListener( NFCDeviceListener::OnNfcStateChangeFunc p_onNFCStateChange, NFCDeviceListener::OnNfcScanCardFunc p_onNFCScanCard )
{
    m_onNFCStateChange = p_onNFCStateChange;
    m_onNFCScanCard = p_onNFCScanCard;

    sm_NfcStateListenList.push_back( this );
}

NFCDeviceListener::~NFCDeviceListener()
{
    sm_NfcStateListenList.remove( this );
}

void NFCDeviceListener::NfcStateChange( const bool p_state )
{
    for( auto item : sm_NfcStateListenList )
    {
        if( item->m_onNFCStateChange )
        {
            item->m_onNFCStateChange( p_state );
        }
    }
}

void NFCDeviceListener::NfcScanCard( const int p_cardId )
{
    for( auto item : sm_NfcStateListenList )
    {
        if( item->m_onNFCScanCard )
        {
            item->m_onNFCScanCard( p_cardId );
        }
    }
}
