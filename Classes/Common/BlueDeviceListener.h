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
typedef std::function< void( const std::vector< unsigned char > & ) > FuncOnRecvedData;;


class BlueDeviceListener
{
public:
    static void listen( void );
    BlueDeviceListener( const FuncOnConnectStateChanged &, const FuncOnRecvedData & );
    
    static void _onConnectStateChanged( bool p_state );
    static void _onRecvedData( const std::vector< unsigned char > & );
    
    ~BlueDeviceListener();
private:
    static std::list< BlueDeviceListener * > sm_blueDeviceListenerList;
    
    FuncOnConnectStateChanged m_onConnectStateChanged;
    FuncOnRecvedData m_onRecvedData;
    
    bool m_connected;
    static bool m_listened;
    
};

#endif //__BLUE_DEVICE_LISTENER_H__
