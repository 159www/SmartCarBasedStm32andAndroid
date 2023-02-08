package com.example.smartcarcontrol;

import android.util.Log;

import androidx.annotation.NonNull;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class OneNetRequest
{
    private String api_key="iL7lj=9v0OLJ8eiRsFvXcNCWmYg=";
    private String device_id="1042075544";//设备id
    private  String  get_data_url="http://api.heclouds.com/devices/datapoints?devIds="+device_id;
    private  String  send_command_url="http://api.heclouds.com/cmds?device_id="+device_id;
    private  String  get_device_state="http://api.heclouds.com/devices/status?devIds="+device_id;
    private String   get_history_data_url="http://api.heclouds.com/devices/"+device_id+"/datapoints?datastream_id=";
    //查询最新数据
    public String get_data() throws IOException {
        OkHttpClient okHttpClient=new OkHttpClient();
        Request request=new Request.Builder()
                .url(get_data_url)
                .addHeader("api-key",api_key)
                .addHeader("Content_Type","application/json").build();
        Response response=okHttpClient.newCall(request).execute();
        String response_string=response.body().string();
        Log.d("last_message",response_string);
        return  response_string;

    }
    //命令下发
    public void SendCommand(String Command) throws IOException {
        OkHttpClient okHttpClient=new OkHttpClient();
        RequestBody requestBody=RequestBody.create(MediaType.parse("application/json;charset=utf-8"),Command);
        Request request=new Request.Builder()
                .url(send_command_url)
                .addHeader("api-key",api_key)
                .addHeader("Content_Type","application/json")
                .post(requestBody)
                .build();
        Response response=okHttpClient.newCall(request).execute();
        Log.d("last_command",response.body().string());

    }
    //查询设备是否在线
    public String get_device_state() throws IOException {
        OkHttpClient okHttpClient=new OkHttpClient();
        Request request=new Request.Builder()
                .url(get_device_state)
                .addHeader("api-key",api_key)
                .addHeader("Content_Type","application/json").build();
        Response response=okHttpClient.newCall(request).execute();
        String response_string=response.body().string();
        Log.d("device_online",response_string);
        return  response_string;
    }
    /*
    //查询历史数据
    //数据流id T    开始时间 2022-01-01T00:00:00    结束时间  2022-01-01T00:00:00
    public String  get_history_data(String stream_id,String start_time,String end_time) throws IOException {
        // String temp_url=get_history_data_url+stream_id+"&start="+start_time+"&end="+end_time;
        String temp_url=get_history_data_url+stream_id+"&start="+start_time+"&end"+end_time;
        OkHttpClient okHttpClient=new OkHttpClient();
        Request request=new Request.Builder()
                .url(temp_url)
                .addHeader("api-key",api_key)
                .addHeader("Content_Type","application/json").build();
        Response response=okHttpClient.newCall(request).execute();
        String response_string=response.body().string();
        return  response_string;
    }
     */
}
