package org.cocos2dx.lib;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;

/**
 * 自定义检查手机网络状态是否切换的广播接受器
 *
 * @author cj
 *
 */
public class NetBroadcastReceiver extends BroadcastReceiver {

    public static native void netStateChange(int netState);

    @Override
    public void onReceive(Context context, Intent intent) {

        //NotReachable: 0
        //ReachableViaWWAN: 1
        //ReachableViaWiFi: 2
        int t_netState = 0;
        if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.LOLLIPOP) {

            //获得ConnectivityManager对象
            ConnectivityManager connMgr = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);

            //获取ConnectivityManager对象对应的NetworkInfo对象
            //获取WIFI连接的信息
            NetworkInfo wifiNetworkInfo = connMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
            //获取移动数据连接的信息
            NetworkInfo dataNetworkInfo = connMgr.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
            if (wifiNetworkInfo.isConnected() && dataNetworkInfo.isConnected()) {
                t_netState = 2;
            } else if (wifiNetworkInfo.isConnected() && !dataNetworkInfo.isConnected()) {
                t_netState = 2;
            } else if (!wifiNetworkInfo.isConnected() && dataNetworkInfo.isConnected()) {
                t_netState = 1;
            } else {
                t_netState = 0;
            }
        //API大于23时使用下面的方式进行网络监听
        }else {

            System.out.println("API level 大于23");
            //获得ConnectivityManager对象
            ConnectivityManager connMgr = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);

            //获取所有网络连接的信息
            Network[] networks = connMgr.getAllNetworks();
            //通过循环将网络信息逐个取出来
            for (int i = 0; i < networks.length; i++) {
                //获取ConnectivityManager对象对应的NetworkInfo对象
                NetworkInfo networkInfo = connMgr.getNetworkInfo(networks[i]);
                if( !networkInfo.isConnected() )
                {
                    continue;
                }

                if( networkInfo.getTypeName().compareTo( "WIFI" ) == 0 && t_netState < 2 )
                {
                    t_netState = 2;
                }

                if( networkInfo.getTypeName().compareTo( "MOBILE" ) == 0 && t_netState < 1 )
                {
                    t_netState = 1;
                }
            }

        }

        netStateChange( t_netState );
    }
}
