package cn.com.startai.testesptouch;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.Toast;


/**
 * Created by Robin on 2018/6/22.
 * qq: 419109715 彬影
 */

public class TAndL {

    private static Handler mHandler = new Handler(Looper.getMainLooper());

    public static final String TAG = "TAndL";

    public static void L(String text) {
        Log.i(TAG, text);
    }

    public static void T(final Context context, final String text) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context, text, Toast.LENGTH_SHORT).show();

            }
        });
    }

    public static void TL(Context context, String text) {

        L(text);
        T(context, text);

    }


}
