package org.cocos2dx.cpp;

import android.content.Context;
import android.os.Message;
import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.Charset;

public class Http {
    String m_url, m_data, m_token, m_requestId, m_result, m_requestType;
    boolean m_requesting;

    String m_filePath;

    public static native void HttpResponse( boolean p_state, String p_requestId, String p_res );

    public static native void HttpDownloadStart( String p_taskId, int p_fileSize );
    public static native void HttpDownloadEnd( String p_taskId );
    public static native void HttpDownloadFinal( String p_taskId, String p_msg );
    public static native void HttpDownloadRate( String p_taskId, float p_rate );


    public Http( String p_url, String p_data, String p_token, String p_requestId )
    {
        m_url = p_url.replace( "http://", "https://" );
        m_data = p_data;
        m_token = p_token;
        m_requestId = p_requestId;
        m_requesting = false;
        m_result = null;
    }

    public void post( )
    {
        m_requestType = "POST";
        request();
    }

    public void get()
    {
        m_requestType = "GET";
        request();
    }

    public void downloadFile( String p_filePath )
    {
        m_filePath = p_filePath;
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    URL t_url = new URL(m_url);
                    HttpURLConnection t_conn = (HttpURLConnection)t_url.openConnection();

                    InputStream t_is = t_conn.getInputStream();
                    while( t_conn.getResponseCode() == 302 ){
                        String newUrl = t_conn.getHeaderField("Location");
                        if( t_url.equals(newUrl) )return;
                        t_url = new URL( newUrl );
                        t_conn = (HttpURLConnection)t_url.openConnection();
                        t_is.close();
                        t_is = t_conn.getInputStream();
                    }

                    int t_downloadFileSie = t_conn.getContentLength();
                    int t_currDownloadSize = 0;

                    HttpDownloadStart( m_requestId, t_downloadFileSie );

                    byte[] t_bs = new byte[1024];

                    int t_readLen;

                    OutputStream t_os = new FileOutputStream( m_filePath );

                    while((t_readLen = t_is.read(t_bs)) > 0){

                        t_currDownloadSize += t_readLen;
                        HttpDownloadRate( m_requestId, (float)t_currDownloadSize / t_downloadFileSie );

                        t_os.write(t_bs, 0, t_readLen);
                    }

                    t_os.close();
                    t_is.close();

                    HttpDownloadEnd( m_requestId );

                }catch (Exception e){
                    HttpDownloadFinal( m_requestId, e.toString() );
                }
            }
        }).start();
    }

    private void request()
    {
        if( m_requesting ) return;
        m_requesting = true;
        new Thread(new Runnable() {
            @Override
            public void run() {

                try {
                    URL t_url = new URL(m_url);
                    HttpURLConnection t_httpURLConnection = (HttpURLConnection) t_url.openConnection();
                    t_httpURLConnection.setConnectTimeout(0);              //设置连接超时时间
                    t_httpURLConnection.setDoInput(true);                  //打开输入流，以便从服务器获取数据

                    t_httpURLConnection.setRequestMethod(m_requestType);   //设置提交数据方式
                    //token
                    t_httpURLConnection.setRequestProperty("Authorization", m_token);

                    if( m_requestType.equals("POST") )
                    {
                        byte[] t_data = m_data.getBytes("UTF-8");

                        t_httpURLConnection.setDoOutput(true);                 //打开输出流，以便向服务器提交数据
                        t_httpURLConnection.setUseCaches(false);               //使用Post方式不能使用缓存
                        //设置请求体的类型是文本类型
                        t_httpURLConnection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
                        //设置请求体的长度
                        t_httpURLConnection.setRequestProperty("Content-Length", String.valueOf(t_data.length));
                        //获得输出流，向服务器写入数据
                        OutputStream t_outputStream = t_httpURLConnection.getOutputStream();
                        t_outputStream.write(t_data);
                    }

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
