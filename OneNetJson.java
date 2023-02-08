package com.example.smartcarcontrol;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class OneNetJson
{
    //获取数据流字符串
    //json {"errno":0,"data":{"devices":[{"title":"BC26","id":"1005551888","datastreams":[{"at":"2022-11-04 18:03:46","id":"T","value":"T27H55HT0AC0AN0GV323BV176BPS94P123.456789123.456789"}]}]},"error":"succ"}
    public String get_data_stream( String json)
    {
        String data=null;
        try
        {
            //创建初始json对象
            JSONObject jsonObject_original = new JSONObject(json);
            //创建data部分json对象
            JSONObject jsonObject_data=(JSONObject)jsonObject_original.get("data");
            JSONArray jsonArray = jsonObject_data.getJSONArray("devices");
            for(int i=0;i<jsonArray.length();i++)
            {
                JSONObject jsonObject_temp=(JSONObject) jsonArray.get(i);
                JSONArray jsonArray_datastreams=jsonObject_temp.getJSONArray("datastreams");
                JSONObject result=(JSONObject) jsonArray_datastreams.get(0);
                data=result.getString("value");
                break;
            }
        }
        catch ( Exception e)
        {
            e.printStackTrace();
        }
        return data;
    }
    public String get_device_state(String json)
    {
        String device_state=null;
        try
        {
            //创建初始json对象
            JSONObject jsonObject_original = new JSONObject(json);
            //创建data部分json对象
            JSONObject jsonObject_data=(JSONObject)jsonObject_original.get("data");
            //创建devices部分json对象
            JSONArray jsonArray = jsonObject_data.getJSONArray("devices");
            JSONObject jsonObject_temp=(JSONObject) jsonArray.get(0);
            device_state=jsonObject_temp.getString("online");
            return device_state;
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

return device_state;
    }
    /*
    //获取历史数据
    public String [] get_history_data(String json)
    {
        String []result=null;
        try {
            //创建初始json对象
            JSONObject jsonObject_original=new JSONObject(json);
            //创建data部分json对象
            JSONObject jsonObject_data=(JSONObject)jsonObject_original.get("data");
            //获取历史数据条数
            int history_data_length=((int)jsonObject_data.get("count"))*2;
            //创建历史数据字符串数组
            String []history_data_arrays=new String[history_data_length];
            JSONArray jsonArray=jsonObject_data.getJSONArray("datastreams");
            //获取datapoints部分的json对象
            JSONObject jsonObject_datapoints=(JSONObject) jsonArray.get(0);
            JSONArray jsonArray_datapoints=jsonObject_datapoints.getJSONArray("datapoints");
            JSONObject jsonObject_temp_datapoint=null;
            for(int i=0;i<jsonArray_datapoints.length();i+=2)
            {
                jsonObject_temp_datapoint=(JSONObject) jsonArray_datapoints.get(i);
                history_data_arrays[i]=jsonObject_temp_datapoint.get("at").toString();
                history_data_arrays[i+1]=jsonObject_temp_datapoint.get("value").toString();
            }
            result=history_data_arrays;
        } catch (JSONException e)
        {
            e.printStackTrace();
        }
        return result;
    }
     */
}
