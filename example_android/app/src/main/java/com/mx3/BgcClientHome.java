package com.mx3;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.net.InetSocketAddress;

public class BgcClientHome extends Activity {
    static {
        System.loadLibrary("mx3_android");
    }

    private static final String TEST_HELLO_MESSAGE =
            "{\"CMD\":0,\"ARG\":[{\"userid\":\"testuser-a40uvsv0SG\",\"useragent\":\"testuser\"}]}";


    private BgcClient bgcClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_bgc_client_home);

        final TextView textView = (TextView) findViewById(R.id.textView);
        EventListener eventListener = new EventListener() {
            @Override
            public void onWebsocketMessage(final String message) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        textView.setText(message);
                    }
                });
            }
        };

        bgcClient = BgcClient.createBgcClient(
                this.getFilesDir().getAbsolutePath(),
                new AndroidEventLoop(),
                new AndroidThreadLauncher(),
                eventListener,
                new AndroidLogger(),
                new AndroidHttp(),
                new AndroidWebSocket(),
                new AndroidMulticastSocket(this));

        bgcClient.setServerUri("ws://192.168.1.1:4080");

        InetSocketAddress socketAddress = new InetSocketAddress("226.1.1.1", 15708);
        SocketAddress multicastAddress = new SocketAddress(socketAddress.getAddress().getAddress(),
                (short)socketAddress.getPort());
        bgcClient.setMulticastAddress(multicastAddress);

        bgcClient.connect();

        final Button buttonSendHello = (Button)findViewById(R.id.button);
        View.OnClickListener onClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bgcClient.send(TEST_HELLO_MESSAGE);
            }
        };
        buttonSendHello.setOnClickListener(onClickListener);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_bgc_client_home, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
