package com.mx3;

public class AndroidMulticastSocket extends MulticastSocket {

    @Override
    public void open(SocketAddress multicastSocketAddress, MulticastSocketListener listener) {

    }

    @Override
    public void close() {

    }

    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }
}
