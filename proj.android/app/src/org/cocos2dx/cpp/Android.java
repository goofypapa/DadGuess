package org.cocos2dx.cpp;

import android.content.Intent;
import android.net.Uri;
import android.nfc.NfcAdapter;
import android.provider.Settings;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.NfcUtils;

public class Android {

    public static String createUUID()
    {
        return java.util.UUID.randomUUID().toString();
    }

    public static void httpPost( String p_url, String p_data, String p_token, String p_requestId )
    {
        new Http( p_url, p_data, p_token, p_requestId ).post();
    }

    public static void httpGet( String p_url, String p_token, String p_requestId )
    {
        new Http( p_url, "", p_token, p_requestId ).get();
    }

    public static void httpDownload( String p_taskId, String p_url, String p_filePath )
    {
        new Http( p_url, "", "", p_taskId ).downloadFile( p_filePath );
    }

    public static boolean whetherSupportNFC()
    {
        return NfcAdapter.getDefaultAdapter( (Cocos2dxActivity)Cocos2dxActivity.getContext() ) != null;
    }

    public static boolean whetherOpenNFC()
    {
        return NfcUtils.NfcCheck( (Cocos2dxActivity)Cocos2dxActivity.getContext() ) != null;
    }

    public static void openNFC()
    {
        ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Intent setNfc = new Intent(Settings.ACTION_NFC_SETTINGS);
                ((Cocos2dxActivity)Cocos2dxActivity.getContext()).startActivity(setNfc);
            }
        });
    }

    public static void goSystemBlue()
    {

    }

    public static void goChrome( String p_url )
    {
        Intent intent = new Intent();
        intent.setAction("android.intent.action.VIEW");
        Uri content_url = Uri.parse( p_url );
        intent.setData(content_url);
        intent.setClassName("com.android.browser","com.android.browser.BrowserActivity");
        ((Cocos2dxActivity)Cocos2dxActivity.getContext()).startActivity(intent);
    }
}
