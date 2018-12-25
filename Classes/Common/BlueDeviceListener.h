//
//  BlueDeviceManage.h
//  dadpat
//
//  Created by 吴思 on 2018/11/26.
//

#ifndef __BLUE_DEVICE_LISTENER_H__
#define __BLUE_DEVICE_LISTENER_H__


#include <functional>
#include <list>
#include <string>
#include <vector>

typedef std::function< void( bool ) > FuncOnConnectStateChanged;
typedef std::function< void( const std::vector< unsigned char > & ) > FuncOnRecvedData;
typedef std::function< void( const std::string &, const std::string ) > FuncOnScanDevice;


class BlueDeviceListener
{
public:

    static void listen( void );

    static void scan( void );
    static void cancelScan( void );
    static void connect( const std::string & p_deviceId );
    static void deconnect( void );

    BlueDeviceListener( const FuncOnConnectStateChanged &, const FuncOnRecvedData & p_onRecvData = nullptr, const FuncOnScanDevice & p_onScanDevice = nullptr );
    
    static void _onConnectStateChanged( bool p_state );
    static void _onRecvedData( const std::vector< unsigned char > & );
    static void _onScanDevice( const std::string & p_deviceId, const std::string & p_deviceName );
    
    ~BlueDeviceListener();
private:
    static std::list< BlueDeviceListener * > sm_blueDeviceListenerList;
    
    FuncOnConnectStateChanged m_onConnectStateChanged;
    FuncOnRecvedData m_onRecvedData;
    FuncOnScanDevice m_onScanDevice;
    
    bool m_connected;
    static bool m_listened;
    
};

#endif //__BLUE_DEVICE_LISTENER_H__
