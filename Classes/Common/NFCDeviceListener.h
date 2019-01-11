#ifndef __NFC_DEVICE_LISTENER_H__
#define __NFC_DEVICE_LISTENER_H__

#include <functional>
#include <list>

class NFCDeviceListener
{
public:
    typedef std::function< void( const bool p_state ) > OnNfcStateChangeFunc;
    typedef std::function< void( const int p_cardId ) > OnNfcScanCardFunc;
    NFCDeviceListener( OnNfcStateChangeFunc p_onNFCStateChange, OnNfcScanCardFunc p_onNFCScanCard = nullptr );
    ~NFCDeviceListener();

    static void NfcStateChange( const bool p_state );
    static void NfcScanCard( const int p_cardId );
private:
    static std::list< NFCDeviceListener * > sm_NfcStateListenList;
    OnNfcStateChangeFunc m_onNFCStateChange;
    OnNfcScanCardFunc m_onNFCScanCard;
};

#endif //__NFC_DEVICE_LISTENER_H__
