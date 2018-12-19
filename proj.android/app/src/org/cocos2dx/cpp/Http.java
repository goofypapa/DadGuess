package org.cocos2dx.cpp;

import android.content.Context;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.Charset;

public class Http {
    String m_url, m_data, m_token, m_requestId, m_result;
    boolean m_requesting;

    public static native void HttpResponse( boolean p_state, String p_requestId, String p_res );

    public Http( String p_url, String p_data, String p_token, String p_requestId )
    {
        m_url = p_url;
        m_data = p_data;
        m_token = p_token;
        m_requestId = p_requestId;
        m_requesting = false;
        m_result = null;
    }

    public void post( )
    {
        if( m_requesting ) return;
        m_requesting = true;
        new Thread(new Runnable() {
            @Override
            public void run() {

                try {
                    URL t_url = new URL(m_url);
                    HttpURLConnection t_httpURLConnection = (HttpURLConnection) t_url.openConnection();
                    t_httpURLConnection.setConnectTimeout(0);            //设置连接超时时间
                    t_httpURLConnection.setDoInput(true);                  //打开输入流，以便从服务器获取数据
                    t_httpURLConnection.setDoOutput(true);                 //打开输出流，以便向服务器提交数据
                    t_httpURLConnection.setRequestMethod("POST");           //设置以Post方式提交数据
                    t_httpURLConnection.setUseCaches(false);               //使用Post方式不能使用缓存

                    byte[] t_data = m_data.getBytes();

                    //设置请求体的类型是文本类型
                    t_httpURLConnection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
                    //设置请求体的长度
                    t_httpURLConnection.setRequestProperty("Content-Length", String.valueOf(t_data.length));
                    //token
                    t_httpURLConnection.setRequestProperty("Authorization", m_token);
                    //获得输出流，向服务器写入数据
                    OutputStream t_outputStream = t_httpURLConnection.getOutputStream();
                    t_outputStream.write(t_data);

                    int t_response = t_httpURLConnection.getResponseCode();            //获得服务器的响应码
                    if(t_response == HttpURLConnection.HTTP_OK) {
                        InputStream t_inptStream = t_httpURLConnection.getInputStream();

                        BufferedReader t_reader = new BufferedReader(new InputStreamReader(t_inptStream, Charset.forName("UTF-8")));

                        String t_line = null;

                        while( (t_line = t_reader.readLine()) != null )
                        {
                            if( m_result == null )
                            {
                                m_result = t_line;
                                continue;
                            }
                            m_result += t_line;
                        }

                        Log.d( "-------->", m_result );

                        ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                HttpResponse( true, m_requestId, m_result );
                            }
                        });
                    }
                }catch (IOException e) {
                    m_result = e.getMessage().toString();
                    ((Cocos2dxActivity)Cocos2dxActivity.getContext()).runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            HttpResponse( false, m_requestId, m_result );
                        }
                    });
                }
            }
        } ).start();
    }
}
