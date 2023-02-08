package com.example.smartcarcontrol;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    private Button button_go=null;
    private Button button_left=null;
    private Button button_right=null;
    private Button button_stop=null;
    private Button button_back=null;
    private TextView device_state=null;
    private TextView distance_shower=null;
    private TextView angular_show=null;
    private TextView speed_show=null;
    private SeekBar  angular_setter=null;
    private SeekBar  speed_setter=null;
    private boolean check_device_state=false;
    private boolean control_state=false;
    private  String device;
    private  String distance=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        button_go=findViewById(R.id.go);
        button_back=findViewById(R.id.back);
        button_left=findViewById(R.id.left);
        button_right=findViewById(R.id.right);
        button_stop=findViewById(R.id.stop);
        device_state=findViewById(R.id.device_state);
        distance_shower=findViewById(R.id.distance_show);
        angular_show=findViewById(R.id.angular_shower);
        speed_show=findViewById(R.id.speed_shower);
        angular_setter=findViewById(R.id.angular_setter);
        speed_setter=findViewById(R.id.speed_setter);
        //新建子线程监听设备是否上线
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true)
                {
                    OneNetRequest oneNetRequest=new OneNetRequest();
                    OneNetJson oneNetJson=new OneNetJson();

                    String temp= null;
                    try {
                        //获取设备状态json
                        temp = oneNetRequest.get_device_state();
                        //解析json获取设备状态
                        device=oneNetJson.get_device_state(temp);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if(device=="true")
                                {
                                    Toast.makeText(getApplicationContext(),"设备在线",Toast.LENGTH_LONG).show();
                                    device_state.setText("在线");
                                    check_device_state=true;
                                }
                                else
                                {
                                    Toast.makeText(getApplicationContext(),"设备未就绪",Toast.LENGTH_SHORT).show();
                                }
                            }
                        });

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    Log.d("op",temp);
                    if(check_device_state)
                    {
                        check_device_state=false;
                        control_state=true;
                        break;
                    }
                }
            }
        }).start();
        //新建子线程实时显示测量距离
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true)
                {   String temp_original=null;
                    String distance_string=null;
                    //设备在线时更新UI显示的距离
                    if(device=="true")
                    {
                        OneNetRequest oneNetRequest=new OneNetRequest();
                        try {
                             temp_original=oneNetRequest.get_data();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        OneNetJson oneNetJson=new OneNetJson();
                        distance_string=oneNetJson.get_data_stream(temp_original);
                        distance=distance_string;
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                distance_shower.setText("距离："+distance);
                            }
                        });

                    }
                }

            }
        }).start();
        //测距角度控制
        angular_setter.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int angular= (int)(((progress-seekBar.getMin())/10.0)*180);
                angular_show.setText("旋转角度："+String.valueOf(angular)+"°");

            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                if(control_state==true) {
                    //发送旋转舵机命令
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            OneNetRequest oneNetRequest = new OneNetRequest();
                            try {
                                oneNetRequest.SendCommand("<" + "S"+String.valueOf(seekBar.getProgress()) + ">");
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                        }
                    }).start();
                }
                else
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(),"设备未上线，测距角度设置无效",Toast.LENGTH_LONG).show();
                        }
                    });

                }

            }
        });
        //速度控制
        speed_setter.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                speed_show.setText("挡位："+String.valueOf(progress/10)+"挡");
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                if(control_state==true) {
                    //发送调速命令
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            OneNetRequest oneNetRequest = new OneNetRequest();
                            try {
                                oneNetRequest.SendCommand("<" + "C"+String.valueOf(seekBar.getProgress() / 10)+"E" + ">");
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();
                }
                else
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(),"设备未上线，调速功能无效",Toast.LENGTH_LONG).show();
                        }
                    });

                }
            }
        });
        //前进按钮监听
        button_go.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(control_state==true) {
                    //发送前进命令
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            OneNetRequest oneNetRequest = new OneNetRequest();
                            try {
                                oneNetRequest.SendCommand("<" + "go" + ">");
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                        }
                    }).start();
                }
                else
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(),"设备未上线，运动控制无效",Toast.LENGTH_LONG).show();
                        }
                    });

                }
            }
        });
        //后退按钮监听
        button_back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(control_state==true) {
                    //发送后退命令
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            OneNetRequest oneNetRequest = new OneNetRequest();
                            try {
                                oneNetRequest.SendCommand("<" + "back" + ">");
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                        }
                    }).start();
                }
                else
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Log.d("back","后退未就绪");
                            Toast.makeText(getApplicationContext(),"设备未上线，运动控制无效",Toast.LENGTH_LONG).show();
                        }
                    });

                }
            }
        });
        //左转按钮监听
        button_left.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(control_state==true) {
                    //发送左转命令
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            OneNetRequest oneNetRequest = new OneNetRequest();
                            try {
                                oneNetRequest.SendCommand("<" + "left" + ">");
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                        }
                    }).start();
                }
                else
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(),"设备未上线，运动控制无效",Toast.LENGTH_LONG).show();
                        }
                    });

                }
            }
        });
        //右转按钮监听
        button_right.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(control_state==true) {
                    //发送右转命令
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            OneNetRequest oneNetRequest = new OneNetRequest();
                            try {
                                oneNetRequest.SendCommand("<" + "right" + ">");
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();
                }
                else
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(),"设备未上线，运动控制无效",Toast.LENGTH_LONG).show();
                        }
                    });

                }
            }
        });
        //停止按钮监听
        button_stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(control_state==true)
                {
                //发送停止命令
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        OneNetRequest oneNetRequest=new OneNetRequest();
                        try {
                            oneNetRequest.SendCommand("<"+"stop"+">");
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

                    }
                }).start();}
                else
                {
                    Toast.makeText(getApplicationContext(),"设备未上线，运动控制无效",Toast.LENGTH_LONG).show();
                }
            }
        });

    }
}