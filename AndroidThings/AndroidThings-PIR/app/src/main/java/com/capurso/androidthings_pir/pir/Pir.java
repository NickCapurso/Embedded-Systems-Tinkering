package com.capurso.androidthings_pir.pir;

import android.support.annotation.Nullable;

import com.capurso.androidthingsutils.GpioUtils;
import com.google.android.things.pio.Gpio;
import com.google.android.things.pio.GpioCallback;

import java.io.IOException;

public class Pir {
    private static final String TAG = Pir.class.getName();

    private enum MotionState {
      MOTION, NO_MOTION
    }

    private Gpio gpio;

    private MotionState motionState = MotionState.NO_MOTION;

    private PirMotionListener listener;

    public Pir(String pin, @Nullable PirMotionListener listener) {
        this.listener = listener;
        this.gpio = GpioUtils.openGpioForInput(pin, gpioCallback);

        if(gpio == null) {
            throw new RuntimeException("Could not set up input GPIO for PIR on pin " + pin);
        }
    }

    @Override
    public boolean equals(Object other) {
        return other instanceof Pir && ((Pir) other).gpio.equals(gpio);
    }

    public void close() {
        try {
            gpio.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private GpioCallback gpioCallback = new GpioCallback() {
        @Override
        public boolean onGpioEdge(Gpio gpio) {
            try {
                if(gpio.getValue() && motionState == MotionState.NO_MOTION) {
                    motionState = MotionState.MOTION;
                    if (listener != null) {
                        listener.onMotionDetected();
                    }
                } else if (!gpio.getValue() && motionState == MotionState.MOTION) {
                    motionState = MotionState.NO_MOTION;
                    if (listener != null) {
                        listener.onMotionStopped();
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            return true;
        }
    };
}
