//
//  BlueDeviceListener.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/27.
//

#include "BlueDeviceListener.h"

std::list< BlueDeviceListener * > BlueDeviceListener::sm_blueDeviceListenerList;
bool BlueDeviceListener::m_listened = false;


void BlueDeviceListener::_onConnectStateChanged( bool p_state )
{
    for( auto t_item : sm_blueDeviceListenerList )
    {
        if( t_item->m_onConnectStateChanged )
        {
            t_item->m_onConnectStateChanged( p_state );
        }
    }
}

void BlueDeviceListener::_onRecvedData( const std::vector< unsigned char > & p_data )
{
    for( auto t_item : sm_blueDeviceListenerList )
    {
        if( t_item->m_onRecvedData )
        {
            t_item->m_onRecvedData( p_data );
        }
    }
}

BlueDeviceListener::BlueDeviceListener( const FuncOnConnectStateChanged & p_onConnectChanged, const FuncOnRecvedData & p_onRecvedData )
{
    m_onConnectStateChanged = p_onConnectChanged;
    m_onRecvedData = p_onRecvedData;
    
    sm_blueDeviceListenerList.push_back( this );
    
}

BlueDeviceListener::~BlueDeviceListener()
{
    auto t_it = std::find( sm_blueDeviceListenerList.begin(), sm_blueDeviceListenerList.end(), this );
    if( t_it != sm_blueDeviceListenerList.end() )
    {
        sm_blueDeviceListenerList.erase( t_it );
    }
}
