package org.cocos2dx.lib;


import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.telephony.PhoneStateListener;
import android.telephony.ServiceState;
import android.telephony.TelephonyManager;
import android.util.Log;

public class PhoneBroadcastReceiver extends PhoneStateListener {

    public static native void phoneStateChange(int netState);

    @Override
    public void onServiceStateChanged(ServiceState serviceState) {
        super.onServiceStateChanged(serviceState);
    }

    @Override
    public void onCallStateChanged(int state, String incomingNumber) {
//        Log.d(PhoneListenService.TAG, "CustomPhoneStateListener state: "
//                + state + " incomingNumber: " + incomingNumber);
        switch (state) {
            case TelephonyManager.CALL_STATE_IDLE:      // 电话挂断
                phoneStateChange( 0 );
                break;
            case TelephonyManager.CALL_STATE_RINGING:   // 电话响铃
//                Log.d(PhoneListenService.TAG, "CustomPhoneStateListener onCallStateChanged endCall");
//                HangUpTelephonyUtil.endCall(mContext);
                phoneStateChange( 1 );
                break;
            case TelephonyManager.CALL_STATE_OFFHOOK:   // 来电接通 或者 去电，去电接通  但是没法区分
                phoneStateChange( 2 );
                break;
        }
    }


//    @Override
//    public void onReceive(Context context, Intent intent) {
//        if (intent.getAction().equals(Intent.ACTION_NEW_OUTGOING_CALL)) {// 如果是拨打电话
//            String phoneNumber = intent.getStringExtra(Intent.EXTRA_PHONE_NUMBER);
//            Log.e( "------------->", "call " + phoneNumber );
//
//        } else {
//            TelephonyManager tManager = (TelephonyManager)
//                    context.getSystemService(Service.TELEPHONY_SERVICE);
//            switch (tManager.getCallState()) {
//                case TelephonyManager.CALL_STATE_RINGING:  //来电状态
//                    Log.e( "------------->", "CALL_STATE_RINGING" );
//                    break;
//                case TelephonyManager.CALL_STATE_OFFHOOK: //摘机状态(接听)
//                    Log.e( "------------->", "CALL_STATE_OFFHOOK" );
//                    break;
//                case TelephonyManager.CALL_STATE_RINGING:
//                    Log.e( "------------->", "CALL_STATE_RINGING" );
//                    break;
//                case TelephonyManager.CALL_STATE_IDLE:// 空闲状态，没有任何活动。(挂断)
//                    Log.e( "------------->", "CALL_STATE_IDLE" );
//                    break;
//            }
//        }
//    }
}