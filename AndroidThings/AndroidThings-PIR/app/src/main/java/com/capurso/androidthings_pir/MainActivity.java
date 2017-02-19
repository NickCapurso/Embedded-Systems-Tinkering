package com.capurso.androidthings_pir;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.capurso.androidthings_pir.pir.Pir;
import com.capurso.androidthings_pir.pir.PirMotionListener;
import com.google.android.things.pio.PeripheralManagerService;

public class MainActivity extends Activity implements PirMotionListener {
    private static final String TAG = MainActivity.class.getName();

    private Pir pir;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, new PeripheralManagerService().getGpioList().toString());
        pir = new Pir("BCM6", this);
    }

    @Override
    protected void onDestroy() {
        if (pir != null) {
            pir.close();
        }
    }

    @Override
    public void onMotionDetected() {
        Log.d(TAG, "PIR motion detected");
    }

    @Override
    public void onMotionStopped() {
        Log.d(TAG, "PIR motion stopped");
    }
}
