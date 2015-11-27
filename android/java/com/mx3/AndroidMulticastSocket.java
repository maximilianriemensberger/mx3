package com.mx3;

import android.content.Context;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.MulticastLock;
import android.net.wifi.WifiManager.WifiLock;
import android.util.Log;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicReference;

public class AndroidMulticastSocket extends MulticastSocket {

    private static final int SOCKET_TIMEOUT_MILLIS = 500;
    private static final int CLOSE_TIMEOUT_MILLIS = 500 + SOCKET_TIMEOUT_MILLIS;
    private static final int DATAGRAM_BUFFER_SIZE = 8192;

    private class MulticastClient implements Runnable {
        private final InetAddress address;
        private final int port;
        private final MulticastSocketListener listener;

        private java.net.MulticastSocket socket;
        private MulticastLock multicastLock;
        private WifiLock fullHighPerfLock;
        private Thread thread;

        public MulticastClient(SocketAddress socketAddress,
                               MulticastSocketListener multicastSocketListener) throws IOException
        {
            listener = multicastSocketListener;
            address = InetAddress.getByAddress(socketAddress.getAddress());
            port = socketAddress.getPort() & 0xFFFF;
            try {
                socket = new java.net.MulticastSocket(port);
                socket.setReuseAddress(true);
                socket.setSoTimeout(SOCKET_TIMEOUT_MILLIS);
                WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
                multicastLock = wifiManager.createMulticastLock("MulticastLock");
                fullHighPerfLock = wifiManager.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, "FullHighPerfLock");
                multicastLock.acquire();
                fullHighPerfLock.acquire();
                if (address.isMulticastAddress())
                    socket.joinGroup(address);
                thread = new Thread(this, "MulticastSocketReceiver");
                thread.start();
            } catch (Exception e) {
                close();
                throw (e instanceof IOException ? (IOException)e : new IOException(e));
            }
        }

        public void close() {
            try {
                if (thread != null && ! thread.equals(Thread.currentThread())) {
                    thread.interrupt();
                    try {
                        thread.join(CLOSE_TIMEOUT_MILLIS);
                    } catch (InterruptedException ignored) {}
                }
                if (socket != null && address.isMulticastAddress())
                    socket.leaveGroup(address);
                if (fullHighPerfLock != null)
                    fullHighPerfLock.release();
                if (multicastLock != null)
                    multicastLock.release();
                if (socket != null)
                    socket.close();
            } catch (Exception ignored) {}
        }

        @Override
        public void run() {
            SocketAddress destination = new SocketAddress(address.getAddress(), (short)(port & 0xFFFF));
            DatagramPacket packet = new DatagramPacket(new byte[DATAGRAM_BUFFER_SIZE], DATAGRAM_BUFFER_SIZE);
            while (true) {
                try {
                    socket.receive(packet);
                    SocketAddress source = new SocketAddress(packet.getAddress().getAddress(),
                            (short)(packet.getPort() & 0xFFFF));
                    Datagram datagram = new Datagram(source, destination,
                            Arrays.copyOf(packet.getData(), packet.getLength()));
                    listener.onDatagram(datagram);
                } catch (InterruptedIOException ignored) {
                } catch (Exception e) {
                    close();
                    break;
                }
                if (Thread.currentThread().isInterrupted()) {
                    close();
                    break;
                }
            }
        }
    }

    private final Context context;
    private final AtomicReference<MulticastClient> clientReference;

    public AndroidMulticastSocket(Context context) {
        this.context = context;
        this.clientReference = new AtomicReference<MulticastClient>(null);
    }

    @Override
    public void open(SocketAddress address, MulticastSocketListener listener) {
        try {
            Log.i("AndroidMulticastSocket", "Opening.");
            MulticastClient client = clientReference.getAndSet(null);
            if (client != null)
                client.close();
            client = new MulticastClient(address, listener);
            if (!clientReference.compareAndSet(null, client))
                client.close();
            else
                Log.i("AndroidMulticastSocket", "Open.");
        } catch (IOException e) {
            Log.e("AndroidMulticastSocket", "Error opening socket.", e);
        }
    }

    @Override
    public void close() {
        MulticastClient client = clientReference.getAndSet(null);
        if (client != null)
            client.close();
    }

    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }

}
