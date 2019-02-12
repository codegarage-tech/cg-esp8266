package cn.com.startai.esptouchsender;

import android.content.Context;
import android.text.TextUtils;

import java.util.List;

import cn.com.startai.esptouchsender.protocol.TouchData;
import cn.com.startai.esptouchsender.task.EsptouchTaskParameter;
import cn.com.startai.esptouchsender.task.__EsptouchTask;
import cn.com.startai.esptouchsender.util.EspAES;
import cn.com.startai.esptouchsender.util.EspNetUtil;

public class EsptouchTask implements IEsptouchTask {

    public __EsptouchTask _mEsptouchTask;
    private EsptouchTaskParameter _mParameter;

    /**
     * Constructor of EsptouchTask
     *
     * @param apSsid     the Ap's ssid
     * @param apBssid    the Ap's bssid
     * @param apPassword the Ap's password
     * @param context    the Context of the Application
     */
    public EsptouchTask(String apSsid, String apBssid, String apPassword,int timeout, Context context) {
        this(apSsid, apBssid, apPassword, null,timeout, context);
    }

    /**
     * Constructor of EsptouchTask
     *
     * @param apSsid     the Ap's ssid
     * @param apBssid    the Ap's bssid
     * @param apPassword the Ap's password
     * @param espAES     AES secret key and iv
     * @param context    the Context of the Application
     */
    public EsptouchTask(String apSsid, String apBssid, String apPassword, EspAES espAES, int timeout, Context context) {
        if (TextUtils.isEmpty(apSsid)) {
            throw new NullPointerException("SSID can't be empty");
        }
        if (TextUtils.isEmpty(apBssid)) {
            throw new NullPointerException("BSSID can't be empty");
        }
        if (apPassword == null) {
            apPassword = "";
        }
        TouchData ssid = new TouchData(apSsid);
        TouchData bssid = new TouchData(EspNetUtil.parseBssid2bytes(apBssid));
        TouchData password = new TouchData(apPassword);
        init(context, ssid, bssid, password, espAES, timeout);
    }

    public EsptouchTask(byte[] apSsid, byte[] apBssid, byte[] apPassword, EspAES espAES, int timeout, Context context) {
        if (apSsid == null || apSsid.length == 0) {
            throw new NullPointerException("SSID can't be empty");
        }
        if (apBssid == null || apBssid.length == 0) {
            throw new NullPointerException("BSSID can't be empty");
        }
        if (apPassword == null) {
            apPassword = new byte[0];
        }
        TouchData ssid = new TouchData(apSsid);
        TouchData bssid = new TouchData(apBssid);
        TouchData password = new TouchData(apPassword);
        init(context, ssid, bssid, password, espAES, timeout);
    }

    private void init(Context context, TouchData ssid, TouchData bssid, TouchData password, EspAES aes, int timeout) {
        _mParameter = new EsptouchTaskParameter();
        _mParameter.setWaitUdpTotalMillisecond(timeout);
        _mEsptouchTask = new __EsptouchTask(context, ssid, bssid, password, aes, _mParameter, true);
    }

    @Override
    public void interrupt() {
        _mEsptouchTask.interrupt();
    }

    @Override
    public IEsptouchResult executeForResult() throws RuntimeException {
        return _mEsptouchTask.executeForResult();
    }

    @Override
    public boolean isCancelled() {
        return _mEsptouchTask.isCancelled();
    }

    @Override
    public List<IEsptouchResult> executeForResults(int expectTaskResultCount)
            throws RuntimeException {
        if (expectTaskResultCount <= 0) {
            expectTaskResultCount = Integer.MAX_VALUE;
        }
        return _mEsptouchTask.executeForResults(expectTaskResultCount);
    }

    @Override
    public void setEsptouchListener(IEsptouchListener esptouchListener) {
        _mEsptouchTask.setEsptouchListener(esptouchListener);
    }
}
