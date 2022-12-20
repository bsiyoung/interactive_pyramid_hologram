package kr.ac.kumoh.s20200886.bluetooth_2;

import static android.content.ContentValues.TAG;

import static java.lang.Thread.sleep;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    TextView mTvBluetoothStatus;
    TextView mTvReceiveData;
    TextView mTvSendData;
    Button mBtnBluetoothOn;
    Button mBtnBluetoothOff;
    Button mBtnConnect;
    Button mBtnSendData;

    BluetoothAdapter mBluetoothAdapter;
    Set<BluetoothDevice> mPairedDevices;
    List<String> mListPairedDevices;

    Handler mBluetoothHandler;
    ConnectedBluetoothThread mThreadConnectedBluetooth;
    BluetoothDevice mBluetoothDevice;
    BluetoothSocket mBluetoothSocket;

    final static int BT_REQUEST_ENABLE = 1;
    final static int BT_MESSAGE_READ = 2;
    final static int BT_CONNECTING_STATUS = 3;
    // 스마트폰 - 스마트폰간 데이터 전송
   // 00001101-0000-1000-8000-00805F9B34FB

    final static UUID BT_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");

    //Using the Accelometer & Gyroscoper
    private SensorManager mSensorManager = null;

    //Using the Gyroscope
    private SensorEventListener mGyroLis;
    private Sensor mGgyroSensor = null;

    //Roll and Pitch
    private double pitch;
    private double roll;
    private double yaw;

    //timestamp and dt
    private double timestamp;
    private double dt;

    // for radian -> dgree
    private double RAD2DGR = 180 / Math.PI;
    private static final float NS2S = 1.0f/1000000000.0f;
    private int pitch_x;
    private int count=0;
    // 안드로이드 생명주기
    @SuppressLint({"MissingInflatedId", "HandlerLeak"})
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTvBluetoothStatus = (TextView) findViewById(R.id.tvBluetoothStatus);
        mTvReceiveData = (TextView) findViewById(R.id.tvReceiveData);
//        mTvSendData = (EditText) findViewById(R.id.c);
        mTvSendData = (TextView) findViewById(R.id.tvSendData);
        mBtnBluetoothOn = (Button) findViewById(R.id.btnBluetoothOn);
        mBtnBluetoothOff = (Button) findViewById(R.id.btnBluetoothOff);
        mBtnConnect = (Button) findViewById(R.id.btnConnect);
        mBtnSendData = (Button) findViewById(R.id.btnSendData);

        // 해당 장치가 블루투스 기능을 지원하는지 알아오는 메소드
        // bluetoothOn() 메소드에서 사용
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        //Using the Gyroscope & Accelometer
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        //Using the Accelometer


        //Using the Accelometer
        mGgyroSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        mGyroLis = new MainActivity.GyroscopeListener();
        mSensorManager.registerListener(mGyroLis, mGgyroSensor, SensorManager.SENSOR_DELAY_UI);
        //mTvSendData.setText((int) pitch);
        mBtnBluetoothOn.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                bluetoothOn();
            }
        });

        // -------------------------------------------------
        // 버튼들이 클릭되었을 때 발생하는 이벤트를 구현한 리스너
        mBtnBluetoothOff.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                bluetoothOff();
            }
        });
        mBtnConnect.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                listPairedDevices();
            }
        });


        mBtnSendData.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mBtnSendData.setPressed(true);
                    if (mThreadConnectedBluetooth != null) {
                        mThreadConnectedBluetooth.write((String.format("%.1f", pitch*RAD2DGR))+"A" +
                        (String.format("%.1f", roll*RAD2DGR))+"B"
                    +(String.format("%.1f", yaw*RAD2DGR))+"C ");

                    }
            }
        });
        // -------------------------------------------------

        // 블루투스 핸들러로 블루투스 연결뒤 수신된 데이터를 읽어와
        // ReceiveData 텍스트 뷰에 표시해주는 부분
        mBluetoothHandler = new Handler() {
            public void handleMessage(android.os.Message msg) {
                if (msg.what == BT_MESSAGE_READ) {
                    String readMessage = null;
                    try {
                        readMessage = new String((byte[]) msg.obj, "UTF-8");
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    mTvReceiveData.setText(readMessage);
                }
            }
        };
    }
    private class GyroscopeListener implements SensorEventListener {

        @Override
        public void onSensorChanged(SensorEvent event) {


            /* 각 축의 각속도 성분을 받는다. */
            double gyroX = event.values[0];
            double gyroY = event.values[1];
            double gyroZ = event.values[2];

            /* 각속도를 적분하여 회전각을 추출하기 위해 적분 간격(dt)을 구한다.
             * dt : 센서가 현재 상태를 감지하는 시간 간격
             * NS2S : nano second -> second */
            dt = (event.timestamp - timestamp) * NS2S;
            timestamp = event.timestamp;

            /* 맨 센서 인식을 활성화 하여 처음 timestamp가 0일때는 dt값이 올바르지 않으므로 넘어간다. */

            if (dt - timestamp*NS2S != 0) {
                /* 각속도 성분을 적분 -> 회전각(pitch, roll)으로 변환.
                 * 여기까지의 pitch, roll의 단위는 '라디안'이다.
                 * SO 아래 로그 출력부분에서 멤버변수 'RAD2DGR'를 곱해주어 degree로 변환해줌.  */
                pitch = pitch + gyroY*dt;
                roll = roll + gyroX*dt;
                yaw = yaw + gyroZ*dt;

//                roll = Double.parseDouble(String.format("%.1f", pitch*RAD2DGR));
//                yaw= Double.parseDouble(String.format("%.1f", yaw*RAD2DGR));
                System.out.println("Pitch:" +Double.parseDouble(String.format("%.1f", pitch*RAD2DGR))+"    Roll: " +
                        Double.parseDouble(String.format("%.1f", roll*RAD2DGR))+"     Yaw" +
                        Double.parseDouble(String.format("%.1f", yaw*RAD2DGR)));


            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {

        }
    }

    // 블루투스 ON 버튼을 누르면 동작하는 메소드, 최초에 "mBluetoothAdapter == null"
    // 을 통하여 블루투스를 지원하는 기기인지, 아닌지 판별
    void bluetoothOn() {
        if (mBluetoothAdapter == null) {
            Toast.makeText(getApplicationContext(), "블루투스를 지원하지 않는 기기입니다.", Toast.LENGTH_LONG).show();
        } else {
            // isEnabled() -- 블루투스 활성화 되있을때,
            if (mBluetoothAdapter.isEnabled()) {
                Toast.makeText(getApplicationContext(), "블루투스가 이미 활성화 되어 있습니다.", Toast.LENGTH_LONG).show();
                mTvBluetoothStatus.setText("활성화");
            } else {
                Toast.makeText(getApplicationContext(), "블루투스가 활성화 되어 있지 않습니다.", Toast.LENGTH_LONG).show();
                Intent intentBluetoothEnable = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    startActivityForResult(intentBluetoothEnable, BT_REQUEST_ENABLE);
                    return;
                }
            }
        }
    }

    // 블루투스 비활성화 메소드, disable()메소드 -> 비활성화
    void bluetoothOff() {
        if (mBluetoothAdapter.isEnabled()) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                mBluetoothAdapter.disable();
                Toast.makeText(getApplicationContext(), "블루투스가 비활성화 되었습니다.", Toast.LENGTH_SHORT).show();
                mTvBluetoothStatus.setText("비활성화");
                return;
            }
        } else {
            Toast.makeText(getApplicationContext(), "블루투스가 이미 비활성화 되어 있습니다.", Toast.LENGTH_SHORT).show();
        }
    }
    @Override
    public void onPause(){
        super.onPause();
        Log.e("LOG", "onPause()");
        mSensorManager.unregisterListener(mGyroLis);
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        Log.e("LOG", "onDestroy()");
        mSensorManager.unregisterListener(mGyroLis);
    }

    // 블루투스 활성화 결과를 위한 메소드
    // 블루투스 ON 메소드에서 Intent로 받은 결과를 처리하는 메소드
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case BT_REQUEST_ENABLE:
                if (resultCode == RESULT_OK) { // 블루투스 활성화를 확인을 클릭하였다면
                    Toast.makeText(getApplicationContext(), "블루투스 활성화", Toast.LENGTH_LONG).show();
                    mTvBluetoothStatus.setText("활성화");
                } else if (resultCode == RESULT_CANCELED) { // 블루투스 활성화를 취소를 클릭하였다면
                    Toast.makeText(getApplicationContext(), "취소", Toast.LENGTH_LONG).show();
                    mTvBluetoothStatus.setText("비활성화");
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    // 블루투스 페어링 장치목록 가져오는 메소드
    void listPairedDevices() {
        // 먼저 블루투스가 활성화 상태인지 확인
        if (mBluetoothAdapter.isEnabled()) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                mPairedDevices = mBluetoothAdapter.getBondedDevices();

                // 페어링된 장치가 존재할때
                if (mPairedDevices.size() > 0) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("장치 선택");
                    mListPairedDevices = new ArrayList<String>();
                    for (BluetoothDevice device : mPairedDevices) {
                        mListPairedDevices.add(device.getName());
                        //mListPairedDevices.add(device.getName() + "\n" + device.getAddress());
                    }

                    // 페어링된 장치수를 얻어와서 각 장치를 누르면 장치명을 매개변수로 사용하여
                    // connectSelectedDevice 메소드로 전달해주는 클릭 이벤트 추가
                    final CharSequence[] items = mListPairedDevices.toArray(new CharSequence[mListPairedDevices.size()]);
                    mListPairedDevices.toArray(new CharSequence[mListPairedDevices.size()]);

                    builder.setItems(items, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int item) {
                            connectSelectedDevice(items[item].toString());
                        }
                    });

                    // 위에서 리스트로 추가된 알람창을 실제로 띄워줌.
                    AlertDialog alert = builder.create();
                    alert.show();

                    // 173줄, 176줄에 대응하여 페어링된 장치가 없는 조건과 블루투스가 비활성화된
                    // 조건에 대해 메세지를 띄어주는 코드
                } else {
                    Toast.makeText(getApplicationContext(), "페어링된 장치가 없습니다.", Toast.LENGTH_LONG).show();
                }
                return;
            }

        } else {
            Toast.makeText(getApplicationContext(), "블루투스가 비활성화 되어 있습니다.", Toast.LENGTH_SHORT).show();
        }
    }

    // 블루투스 연결하는 메소드(실제 블루투스 장치와 연결)
    void connectSelectedDevice(String selectedDeviceName) {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            for (BluetoothDevice tempDevice : mPairedDevices) {
                if (selectedDeviceName.equals(tempDevice.getName())) {
                    mBluetoothDevice = tempDevice;
                    break;
                }
            }
            try {
                mBluetoothSocket = mBluetoothDevice.createRfcommSocketToServiceRecord(BT_UUID);
                mBluetoothSocket.connect();
                mThreadConnectedBluetooth = new ConnectedBluetoothThread(mBluetoothSocket);
                mThreadConnectedBluetooth.start();
                mBluetoothHandler.obtainMessage(BT_CONNECTING_STATUS, 1, -1).sendToTarget();
                Toast.makeText(getApplicationContext(), "블루투스 연결완료.", Toast.LENGTH_LONG).show();
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "블루투스 연결 중 오류가 발생했습니다.", Toast.LENGTH_LONG).show();
            }
            return;
        }
    }

    // 쓰레드 시작 - 쓰레드에서 사용할 전역 객체들을 선언
    private class ConnectedBluetoothThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        // 쓰레드 초기화 과정
        // 데이터 전송 및 수신하는 길을 만들어주는 작업
        public ConnectedBluetoothThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "소켓 연결 중 오류가 발생했습니다.", Toast.LENGTH_LONG).show();
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        // 수신받은 데이터는 언제들어올 지 모르니 항상 확인
        public void run() {
            byte[] buffer = new byte[1024];
            int bytes;

            // 처리된 데이터가 존재하면 데이터를 읽어오는 작업
            while (true) {
                try {
                    bytes = mmInStream.available();
                    if (bytes != 0) {
                        SystemClock.sleep(100);
                        bytes = mmInStream.available();
                        bytes = mmInStream.read(buffer, 0, bytes);
                        mBluetoothHandler.obtainMessage(BT_MESSAGE_READ, bytes, -1, buffer).sendToTarget();
                    }
                } catch (IOException e) {
                    break;
                }
            }
        }

        // 데이터 전송을 위한 메소드
        public void write(String str) {
            str += "\n";
            byte[] bytes = str.getBytes();
            try {
                mmOutStream.write(bytes);
                mmOutStream.flush();
                //mTvSendData.append();
               // Toast.makeText(getApplicationContext(), "데이터 전송 완료", Toast.LENGTH_LONG).show();
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "데이터 전송 중 오류가 발생했습니다.", Toast.LENGTH_LONG).show();
            }
        }

        // 블루투스 소켓을 닫는 메소드
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "소켓 해제 중 오류가 발생했습니다.", Toast.LENGTH_LONG).show();
            }
        }
    }
}
