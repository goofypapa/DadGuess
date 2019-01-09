package org.cocos2dx.cpp;

import android.util.Log;

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
}
