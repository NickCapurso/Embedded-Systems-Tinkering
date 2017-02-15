package com.capurso.androidthings_pushbuttonled;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

import com.google.android.things.pio.Gpio;
import com.google.android.things.pio.GpioCallback;
import com.google.android.things.pio.PeripheralManagerService;

import java.io.IOException;

public class HomeActivity extends Activity {
    private static final String TAG = "HomeActivity";

    private static final String INPUT_PIN_NAME = "BCM17";

    private static final String OUTPUT_PIN_NAME = "BCM27";

    private Gpio inputPin;

    private Gpio outputPin;

    private Handler handler = new Handler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        PeripheralManagerService service = new PeripheralManagerService();
        Log.d(TAG, "Available GPIO: " + service.getGpioList());

        try {
            inputPin = service.openGpio(INPUT_PIN_NAME);
            inputPin.setDirection(Gpio.DIRECTION_IN);
            inputPin.setEdgeTriggerType(Gpio.EDGE_BOTH);
            inputPin.registerGpioCallback(inputCallback);

            outputPin = service.openGpio(OUTPUT_PIN_NAME);
            outputPin.setDirection(Gpio.DIRECTION_OUT_INITIALLY_LOW);
        } catch (IOException e) {
            Log.e(TAG, "Error on PeripheralIO API", e);
            e.printStackTrace();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (inputPin != null) {
            inputPin.unregisterGpioCallback(inputCallback);
        }
        closePin(inputPin);
        closePin(outputPin);
    }

    private void closePin(Gpio gpio) {
        if (gpio != null) {
            try {
                gpio.close();
            } catch (IOException e) {
                Log.e(TAG, "Error on PeripheralIO API", e);
            }
        }
    }

    private GpioCallback inputCallback = new GpioCallback() {
        @Override
        public boolean onGpioEdge(Gpio gpio) {
            Log.d(TAG, "onGpioEdge");
            try {
                Log.d(TAG, "Input is high: " + gpio.getValue());
               outputPin.setValue(gpio.getValue());
            } catch (IOException e) {
                e.printStackTrace();
            }
            return true;
        }
    };
}
