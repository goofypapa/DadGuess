package org.cocos2dx.lib;

import android.bluetooth.BluetoothA2dp;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.util.UUID;

import android.os.Message;
import android.util.Log;

public abstract class BluePackage {
    static String sm_blueName = "爸爸猜猜";
    private static final UUID SPP_UUID =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    static BluetoothA2dp m_bluetoothA2dp;
    static private BluetoothSocket m_socket = null;
    static private InputStream m_inStream = null;

    static private boolean sm_scanDevice = false, sm_connectIng = false;

    static BluetoothAdapter m_blueToolThAdapter;
    static BroadcastReceiver m_broadcastReceiver;
    static Map< String,BluetoothDevice> m_bluetoothDeviceList = new HashMap<>();
    static BluetoothDevice m_device;

    static String sm_data;

    public static native void scanedDevice( String p_deviceAddess, String p_deviceName );
    public static native void recvData( String p_data );
    public static native void connectDeviceStateChange( int p_connectState );

    static boolean s_deviceConnected = false;

    public static void listen()
    {
        m_broadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String t_action = intent.getAction();

                //开始扫描设备
                if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(t_action)) {
                    Log.d("BroadcastReceiver", "ACTION_DISCOVERY_STARTED");
                    Log.d("DEBUG", "开始扫描设备");
                }

                //找到设备
                if (BluetoothDevice.ACTION_FOUND.equals(t_action)) {
                    Log.d("BroadcastReceiver", "ACTION_FOUND");
                    BluetoothDevice t_device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    // 已经配对的则跳过

                    if( t_device == null ) return;

                    String t_name = t_device.getName();

                    if( t_name == null ) return;

                    try {
                        if ( t_device.getBondState() != BluetoothDevice.BOND_BONDED && t_device.getName().equals(sm_blueName)) {
                            if( m_bluetoothDeviceList.containsKey( t_device.getAddress() ) )
                            {
                                return;
                            }
                            m_bluetoothDeviceList.put( t_device.getAddress(), t_device );
                            scanedDevice( t_device.getAddress(), t_device.getName() );
                            Log.d("DEBUG", "找到未配对蓝牙：" + t_device.getName() + "--" + t_device.getAddress());
                        }
                    }catch (Exception e){
                        Log.d("DEBUG", "检测到异常" + e.toString());
                    }

                }


                //配对状态
                if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.equals(t_action)) {
                    Log.d("BroadcastReceiver", "ACTION_BOND_STATE_CHANGED");

                    BluetoothDevice t_device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    switch (t_device.getBondState()) {
                        case BluetoothDevice.BOND_BONDING://正在配对
                            Log.d("DEBUG",  "正在配对......" );

                            try {
                                t_device.setPairingConfirmation( true );
                            }catch (Exception e)
                            {

                            }
                            break;
                        case BluetoothDevice.BOND_BONDED://配对结束
                            Log.d("DEBUG",  "完成配对" );
                            connect( t_device.getAddress() );
                            break;
                        case BluetoothDevice.BOND_NONE://取消配对/未配对
                            Log.d("DEBUG", "取消配对" );
                        default:
                            break;
                    }
                }

                //扫描结束
                if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(t_action)) {
                    Log.d("BroadcastReceiver", "ACTION_DISCOVERY_FINISHED");
                    Log.d("DEBUG", "扫描设备结束");

//                    if( m_bluetoothDevice == null ){
//                        Log.d("DEBUG", "没有找到设备");
//                    }

                    if( sm_scanDevice ) {
                        scan();
                    }
                }

                //蓝牙状态
                if (BluetoothAdapter.ACTION_STATE_CHANGED.equals(t_action)) {
                    Log.d("BroadcastReceiver", "ACTION_STATE_CHANGED");
                    int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1);
                    switch (state) {
                        case BluetoothAdapter.STATE_TURNING_ON:
                            Log.d("DEBUG", "正在打开蓝牙");
                            break;
                        case BluetoothAdapter.STATE_ON:
                            Log.d("DEBUG", "蓝牙已打开");
                            scan();
                            break;
                        case BluetoothAdapter.STATE_TURNING_OFF:
                            Log.d("DEBUG", "正在关闭蓝牙");
                            cancelPair(m_device);
                            break;
                        case BluetoothAdapter.STATE_OFF:
                            Log.d("DEBUG", "蓝牙已关闭");
                            sm_scanDevice = false;
                            break;
                    }
                }
            }
        };

        //开启蓝牙服务线程
        IntentFilter t_intent = new IntentFilter();
        t_intent.addAction(BluetoothDevice.ACTION_FOUND);
        t_intent.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);//状态改变
        t_intent.addAction(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED);//行动扫描模式改变了
        t_intent.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);//动作状态发生了变化

        t_intent.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);//扫描开始
        t_intent.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);//扫描结束

        Cocos2dxActivity.getContext().registerReceiver(m_broadcastReceiver, t_intent);


        m_blueToolThAdapter = BluetoothAdapter.getDefaultAdapter();

    }

    public static void openDevice()
    {

        Cocos2dxActivity.openLocationAccess();

        if(m_blueToolThAdapter.isEnabled())
        {
            scan();
            return;
        }

        Cocos2dxActivity.openBlue();

    }

    public static void scan()
    {

        sm_scanDevice = true;

        m_bluetoothDeviceList.clear();
        Set<BluetoothDevice> pairedDevices = m_blueToolThAdapter.getBondedDevices();
        //
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                if (device.getName().equals(sm_blueName)) {
                    if( m_bluetoothDeviceList.containsKey( device.getAddress() ) )
                    {
                        continue;
                    }

                    if( checkConnected( device ) ) {
//                        m_bluetoothDeviceList.put(device.getAddress(), device);
//                        scanedDevice(device.getAddress(), device.getName());

//                        _connectA2dp( device );
//                        closeA2DP();
//                        cancelPair( device );
                        Log.d("DEBUG", "已配对设备:" + device.getName() + ", " + device.getAddress());
                    }else{
                        cancelPair( device );
                    }
                }
            }
        }

        if( m_blueToolThAdapter.isDiscovering() )
        {
            return;
        }
        m_blueToolThAdapter.startDiscovery();
    }

    public static void stopScan()
    {
        sm_scanDevice = false;
        if( m_blueToolThAdapter.isDiscovering() )
        {
            m_blueToolThAdapter.cancelDiscovery();
        }
    }

    public static void connect( String p_deviceAddess )
    {
        Log.d( "------->", "connect" + p_deviceAddess );

        if( m_bluetoothDeviceList.containsKey( p_deviceAddess ) )
        {
            BluetoothDevice t_device = m_bluetoothDeviceList.get( p_deviceAddess );
            if( t_device.getBondState() != BluetoothDevice.BOND_BONDED )
            {
                pair( t_device );
                return;
            }

            if( sm_connectIng )
            {
                return;
            }
            sm_connectIng = true;

            listenSocket( t_device );
        }
    }

    private static void pair( BluetoothDevice p_device )
    {
        if( p_device.getBondState() == BluetoothDevice.BOND_BONDING )
        {
            return;
        }

        p_device.createBond();
    }

    public static void cancelPair( BluetoothDevice p_device )
    {
        if( p_device == null ) return;
        try{
            Method removeBondMethod = BluetoothDevice.class.getMethod("removeBond");
            removeBondMethod.invoke( p_device );
        }catch (Exception e){

        }
    }

    private static void listenSocket( BluetoothDevice p_device ) {
        m_device = p_device;


        new Thread(new Runnable() {
            @Override
            public void run() {

                try {
                    m_socket = m_device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
                } catch (Exception e) {
                    Log.d("DEBUG", "创建Socket失败" + e.toString());
                    ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            connectDeviceStateChange( 1 );
                            sm_connectIng = false;
                        }
                    });

                    return;
                }

                new Thread(new Runnable() {

                    @Override
                    public void run() {

                        try {
                            m_socket.connect();
                        } catch (IOException e) {
                            Log.d("DEBUG", "连接Socket失败" + e.toString());

                            try {
                                Method t_m = m_device.getClass().getMethod("createRfcommSocket", new Class[] {int.class});
                                m_socket = (BluetoothSocket) t_m.invoke(m_device, 1);
                                m_socket.connect();
                            } catch (Exception ae) {
                                Log.e("BLUE",ae.toString());
                                try{
                                    m_socket.close();
                                }catch (IOException ie){ }

                                ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        connectDeviceStateChange( 1 );
                                        sm_connectIng = false;
                                    }
                                });
                                return;
                            }
                        }
                        try {
                            m_inStream = m_socket.getInputStream();
                        } catch (IOException e) {
                            Log.d("DEBUG", "获取流异常" + e.toString());

                            ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    connectDeviceStateChange( 2 );
                                    sm_connectIng = false;
                                }
                            });
                            return;
                        }

                        Log.d("DEBUG", "监听成功");
                        connectA2DP(m_device);

                        byte[] t_buffer = new byte[256];
                        boolean t_socketConnected = true;
                        while (m_socket != null) {
                            try {
                                int t_size = m_inStream.read(t_buffer);

                                sm_data = byteArrayToHexStr(t_buffer, t_size);
                                ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        recvData( sm_data );
                                    }
                                });

                                Log.d("DEBUG", "卡号：" + sm_data );
                            } catch (IOException e) {
                                t_socketConnected = false;

                                Log.e("ERROR", "读取信息失败" + e.toString() + "---" + m_socket.isConnected());
                            }

                            if (t_socketConnected) {
                                continue;
                            }

                            try {
                                m_socket.close();
                                m_socket = null;
                            } catch (IOException e) {
                            }
                        }

                        Log.d("DEBUG", "断开连接");
                        closeA2DP();
                        ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                connectDeviceStateChange( 3 );
                                sm_connectIng = false;
                            }
                        });
                    }
                }).start();
            }
        }).start();
    }

    private static void connectA2DP( BluetoothDevice p_device ) {

        if( checkConnected( p_device ) )
        {
            ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    connectDeviceStateChange( 0 );
                    sm_connectIng = false;
                }
            });
            //已经连接
            return;
        }

        m_device = p_device;
        if(m_blueToolThAdapter.getProfileConnectionState(BluetoothProfile.A2DP)!=BluetoothProfile.STATE_CONNECTED){
            //在listener中完成A2DP服务的调用
            m_blueToolThAdapter.getProfileProxy( Cocos2dxActivity.getContext(), new BluetoothProfile.ServiceListener() {
                @Override
                public void onServiceDisconnected(int profile) {
                    if(profile == BluetoothProfile.A2DP){
                        m_bluetoothA2dp = null;
                    }
                }
                @Override
                public void onServiceConnected(int profile, BluetoothProfile proxy) {
                    if(profile == BluetoothProfile.A2DP){
                        m_bluetoothA2dp = (BluetoothA2dp) proxy; //转换
                        _connectA2dp( m_device );
                    }
                }
            }, BluetoothProfile.A2DP);
        }
    }

    private static void _connectA2dp( BluetoothDevice p_device ){
        setPriority( p_device, 100 ); //设置priority
        try {
            //通过反射获取BluetoothA2dp中connect方法（hide的），进行连接。
            Method connectMethod =BluetoothA2dp.class.getMethod("connect",
                    BluetoothDevice.class);
            connectMethod.invoke( m_bluetoothA2dp, p_device );

            ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Log.d("DEBUG", "连接a2dp成功");
                    connectDeviceStateChange( 0 );
                    sm_connectIng = false;
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
            Log.d("DEBUG", "连接a2dp失败" + e.toString());
            ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    connectDeviceStateChange( 4 );
                    sm_connectIng = false;
                }
            });
        }
    }

    public static void setPriority(BluetoothDevice device, int priority) {
        if (m_bluetoothA2dp == null) return;
        try {//通过反射获取BluetoothA2dp中setPriority方法（hide的），设置优先级
            Method connectMethod =BluetoothA2dp.class.getMethod("setPriority",
                    BluetoothDevice.class,int.class);
            connectMethod.invoke(m_bluetoothA2dp, device, priority);
        } catch (Exception e) {
            e.printStackTrace();
            Log.d("DEBUG", "设置优先级失败" + e.toString());
        }
    }

    private static void closeA2DP()
    {
        if( m_bluetoothA2dp == null ) return;
        //关闭ProfileProxy，也就是断开service连接
        m_blueToolThAdapter.closeProfileProxy(BluetoothProfile.A2DP,m_bluetoothA2dp);
    }

    public static void closeDevice()
    {
        if(m_blueToolThAdapter.isEnabled()){
            boolean ret = m_blueToolThAdapter.disable();
        }
    }

    public static String byteArrayToHexStr(byte[] byteArray, int length) {
        if (byteArray == null){
            return null;
        }
        char[] hexArray = "0123456789ABCDEF".toCharArray();
        char[] hexChars = new char[length * 2];
        for (int j = 0; j < length; j++) {
            int v = byteArray[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    private static boolean checkConnected( BluetoothDevice p_device )
    {
        try {//得到连接状态的方法
            Method method = m_blueToolThAdapter.getClass().getDeclaredMethod("getConnectionState", (Class[]) null);
            //打开权限
            method.setAccessible(true);
            int state = (int) method.invoke(m_blueToolThAdapter, (Object[]) null);

            if(state == BluetoothAdapter.STATE_CONNECTED){
                Set<BluetoothDevice> devices = m_blueToolThAdapter.getBondedDevices();

                for(BluetoothDevice device : devices){
                    Method isConnectedMethod = BluetoothDevice.class.getDeclaredMethod("isConnected", (Class[]) null);
                    method.setAccessible(true);
                    boolean isConnected = (boolean) isConnectedMethod.invoke(device, (Object[]) null);
                    if(isConnected && device.getName().equals(p_device.getName() ) && device.getAddress().equals( p_device.getAddress() ) ){
                        Log.d("DEBUG", "connected:"+device.getName());
                        return true;
                    }
                }
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

        return false;
    }
}
