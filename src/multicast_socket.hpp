#pragma once
#include "stl.hpp"
#include "interface/logger.hpp"
#include "interface/event_listener.hpp"
#include "interface/socket_address.hpp"
#include "interface/datagram.hpp"
#include "interface/multicast_socket.hpp"
#include "interface/multicast_socket_listener.hpp"

namespace mx3 {

class BroadcastingMulticastSocketListener;

class MulticastSocket {
  public:
    MulticastSocket(shared_ptr<mx3_gen::MulticastSocket> multicast_socket);

    // Listener management
    using ListenerKey = uintptr_t;
    ListenerKey add_listener(unique_ptr<mx3_gen::MulticastSocketListener> listener);
    bool del_listener(ListenerKey key);

    // Connection management
    void open(const mx3_gen::SocketAddress & multicast_socket_address);
    void close();

  private:
    const shared_ptr<mx3_gen::MulticastSocket> multicast_socket_;
    const shared_ptr<mx3::BroadcastingMulticastSocketListener> broadcaster_;
};

class BroadcastingMulticastSocketListener : public mx3_gen::MulticastSocketListener {
  public:
    BroadcastingMulticastSocketListener();

    virtual void on_datagram(const mx3_gen::Datagram & packet) override;

    MulticastSocket::ListenerKey
        add_listener(unique_ptr<mx3_gen::MulticastSocketListener> listener);
    bool del_listener(MulticastSocket::ListenerKey key);

  private:
    std::map<MulticastSocket::ListenerKey, unique_ptr<mx3_gen::MulticastSocketListener>>
        listeners_;
};

class LoggingMulticastSocketListener : public mx3_gen::MulticastSocketListener {
  public:
    LoggingMulticastSocketListener(shared_ptr<mx3_gen::Logger> logger);

    virtual void on_datagram(const mx3_gen::Datagram & packet) override;

  private:
    const shared_ptr<mx3_gen::Logger> logger_;
};

}

/* vim: set et ts=4 sts=4 sw=4 tw=80 : */
