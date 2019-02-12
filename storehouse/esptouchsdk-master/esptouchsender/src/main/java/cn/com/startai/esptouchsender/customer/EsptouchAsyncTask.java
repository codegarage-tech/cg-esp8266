package cn.com.startai.esptouchsender.customer;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

import java.util.List;

import cn.com.startai.esptouchsender.EsptouchTask;
import cn.com.startai.esptouchsender.IEsptouchResult;
import cn.com.startai.esptouchsender.IEsptouchTask;
import cn.com.startai.esptouchsender.util.EspAES;

public class EsptouchAsyncTask extends AsyncTask<byte[], Void, List<IEsptouchResult>> {
    private final MyEsptouchListener mListener;

    public static final boolean AES_ENABLE = false;
    public static final String AES_SECRET_KEY = "1234567890123456"; // TODO modify your own key


    private String TAG = EsptouchAsyncTask.class.getSimpleName();

    private Context mContext;
    // without the lock, if the user tap confirm and cancel quickly enough,
    // the bug will arise. the reason is follows:
    // 0. task is starting created, but not finished
    // 1. the task is cancel for the task hasn't been created, it do nothing
    // 2. task is created
    // 3. Oops, the task should be cancelled, but it is running
    private final Object mLock = new Object();
    private IEsptouchTask mEsptouchTask;

    private String bssid;
    private String ssid;
    private String password;
    private int deviceCount;
    private int timeout;



    public EsptouchAsyncTask(Context context, String bssid, String ssid, String password, int deviceCount, int timeout, MyEsptouchListener myListener) {
        mContext = context;
        mListener = myListener;
        this.bssid = bssid;
        this.ssid = ssid;
        this.password = password;
        this.deviceCount = deviceCount;
        this.timeout = timeout;


    }


    public void cancelEsptouch() {
        cancel(true);
        if (mEsptouchTask != null) {
            mEsptouchTask.interrupt();
        }
    }

    @Override
    protected void onPreExecute() {
    }

    @Override
    protected List<IEsptouchResult> doInBackground(byte[]... params) {

        int taskResultCount;
        synchronized (mLock) {

            if (AES_ENABLE) {
                byte[] secretKey = AES_SECRET_KEY.getBytes();
                EspAES aes = new EspAES(secretKey);
                mEsptouchTask = new EsptouchTask(ssid, bssid, password, aes, timeout, mContext);

            } else {
                mEsptouchTask = new EsptouchTask(ssid, bssid, password, null, timeout, mContext);
            }
            mEsptouchTask.setEsptouchListener(mListener);
        }
        return mEsptouchTask.executeForResults(deviceCount);
    }

    @Override
    protected void onPostExecute(List<IEsptouchResult> result) {


        if (result == null) {
            mListener.onEspTouchResultFailed("-1", "Create Esptouch task failed, the esptouch port could be used by other thread");
            return;
        }

        IEsptouchResult firstResult = result.get(0);
        // check whether the task is cancelled and no results received
        if (!firstResult.isCancelled()) {
            int count = 0;
            // max results to be displayed, if it is more than maxDisplayCount,
            // just show the count of redundant ones
            final int maxDisplayCount = 5;
            // the task received some results including cancelled while
            // executing before receiving enough results
            if (firstResult.isSuc()) {
                StringBuilder sb = new StringBuilder();
                for (IEsptouchResult resultInList : result) {
                    sb.append("Esptouch success, bssid = ")
                            .append(resultInList.getBssid())
                            .append(", InetAddress = ")
                            .append(resultInList.getInetAddress().getHostAddress())
                            .append("\n");
                    count++;
                    if (count >= maxDisplayCount) {
                        break;
                    }
                }
                if (count < result.size()) {
                    sb.append("\nthere's ")
                            .append(result.size() - count)
                            .append(" more result(s) without showing\n");
                }
                Log.d(TAG, sb.toString());


            } else {
                mListener.onEspTouchResultFailed("-5", "Esptouch failed ,timeout");
            }


        }

    }
}