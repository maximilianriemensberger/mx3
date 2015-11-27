package com.mx3;

import android.util.Log;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft_17;
import org.java_websocket.framing.Framedata;
import org.java_websocket.framing.FramedataImpl1;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;
import java.nio.ByteBuffer;
import java.util.concurrent.atomic.AtomicReference;

public class AndroidWebSocket extends WebSocket {
    /**
     * Reference to WebSocketClient. Must be atomic since multiple threads can arbitrarily access
     * this class both from the native and the java world.
     */
    private final AtomicReference<WebSocketClient> clientReference;

    public AndroidWebSocket() {
        this.clientReference = new AtomicReference<WebSocketClient>();
    }

    @Override
    public void connect(String url, final WebSocketListener listener) {
        Log.i("AndroidWebSocket", "Connecting ...");
        WebSocketClient client = clientReference.getAndSet(null);
        if (client != null)
            client.close();

        URI theUri;
        try {
            theUri = new URI(url);
        } catch (Exception e) {
            Log.e("AndroidWebSocket", "Error converting the uri.", e);
            return;
        }

        final WebSocketClient newClient = new WebSocketClient(theUri, new Draft_17()) {
            @Override
            public void onOpen(ServerHandshake handshakedata) {
                listener.onOpen(handshakedata.getHttpStatus(), handshakedata.getHttpStatusMessage());
            }

            @Override
            public void onClose(int code, String reason, boolean remote) {
                listener.onClose((short)code, reason, remote);
            }

            @Override
            public void onError(Exception ex) {
                listener.onError((short)0, ex.getClass().getCanonicalName() + ": " + ex.getMessage());
            }

            @Override
            public void onMessage(String message) {
                listener.onMessage(message);
            }

            @Override
            public void onMessage(ByteBuffer bytes) {
                listener.onError((short)0, "Binary messages are not supported.");
            }

            @Override
            public void onWebsocketPing(org.java_websocket.WebSocket conn, Framedata f) {
                super.onWebsocketPing(conn, f);
            }

            @Override
            public void onWebsocketPong(org.java_websocket.WebSocket conn, Framedata f) {
                super.onWebsocketPong(conn, f);
            }
        };

        new AndroidThreadLauncher().startThread("WebSocketConnector", new AsyncTask() {
            @Override
            public void execute() {
                try {
                    newClient.connectBlocking();
                    if (!clientReference.compareAndSet(null, newClient))
                        newClient.close();
                    else
                        Log.i("AndroidWebSocket", "Connected.");
                } catch (Exception e) {
                    //TODO
                }
            }
        });
    }

    @Override
    public boolean isConnected() {
        WebSocketClient client = clientReference.get();
        return client != null && client.getConnection().isOpen();
    }

    @Override
    public void close() {
        WebSocketClient client = clientReference.getAndSet(null);
        if (client != null) {
            client.close();
            Log.i("AndroidWebSocket", "Closed.");
        }
    }

    @Override
    public void send(String message) {
        WebSocketClient client = clientReference.get();
        if (client != null)
            client.send(message);
    }

    private void ping() {
        WebSocketClient client = clientReference.get();
        if (client != null)
            client.getConnection().sendFrame(new FramedataImpl1(Framedata.Opcode.PING));
    }

    private void pong() {
        WebSocketClient client = clientReference.get();
        if (client != null)
            client.getConnection().sendFrame(new FramedataImpl1(Framedata.Opcode.PONG));
    }

    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }

}
