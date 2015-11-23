package com.mx3;

import android.util.Log;

public class AndroidLogger extends Logger {
    @Override
    public void log(int level, String tag, String message) {
        logImpl(level, tag, message);
    }

    @Override
    public void logException(int level, String tag, String message, String what) {
        logImpl(level, tag, message + "[Exception: " + what + "]");
    }

    private void logImpl(int level, String tag, String message) {
        switch (level) {
            // TODO: Add WTF log level which might lead to immediate application destruction? (Really?)
            case LOG_LEVEL_ERROR:
                Log.e(tag, message);
                break;
            case LOG_LEVEL_WARN:
                Log.w(tag, message);
                break;
            case LOG_LEVEL_INFO:
                Log.i(tag, message);
                break;
            case LOG_LEVEL_DEBUG:
                Log.d(tag, message);
                break;
            case LOG_LEVEL_VERBOSE:
                Log.v(tag, message);
                break;
            case LOG_LEVEL_FINEST:
                Log.v(tag, message);
                break;
        }
    }
}
