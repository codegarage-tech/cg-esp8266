package cn.com.startai.esptouchsender.customer;

import cn.com.startai.esptouchsender.IEsptouchListener;

/**
 * Created by Robin on 2018/7/14.
 * qq: 419109715 彬影
 */

public interface MyEsptouchListener extends IEsptouchListener {

    void onEspTouchResultFailed(String errorCode, String errorMsg);
}
