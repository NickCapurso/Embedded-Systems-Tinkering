package com.capurso.androidthingsutils;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.android.things.pio.Gpio;
import com.google.android.things.pio.GpioCallback;
import com.google.android.things.pio.PeripheralManagerService;

import java.io.IOException;

public class GpioUtils {
    private static final String TAG = GpioUtils.class.getName();

    private static final PeripheralManagerService peripheralManagerService = new PeripheralManagerService();

    @Nullable
    public static Gpio openGpioForOutput(String pin) {
        if (!pinExists(pin)) {
            Log.e(TAG, "Could not find pin " + pin);
            return null;
        }

        try {
            Gpio gpio = peripheralManagerService.openGpio(pin);
            gpio.setDirection(Gpio.DIRECTION_OUT_INITIALLY_LOW);
            return gpio;
        } catch (IOException e) {
            Log.e(TAG, e.getMessage());
        }
        return null;
    }

    @Nullable
    public static Gpio openGpioForInput(String pin, @Nullable GpioCallback callback) {
        if (!pinExists(pin)) {
            Log.e(TAG, "Could not find pin " + pin);
            return null;
        }

        try {
            Gpio gpio = peripheralManagerService.openGpio(pin);
            gpio.setDirection(Gpio.DIRECTION_IN);
            gpio.setEdgeTriggerType(Gpio.EDGE_BOTH);

            if(callback != null) {
                gpio.registerGpioCallback(callback);
            }
            return gpio;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static boolean pinExists(String pin) {
        return peripheralManagerService.getGpioList().contains(pin);
    }
}
