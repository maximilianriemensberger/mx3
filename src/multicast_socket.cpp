#include "multicast_socket.hpp"

using mx3::MulticastSocket;
using mx3::BroadcastingMulticastSocketListener;
using mx3::LoggingMulticastSocketListener;

using ListenerKey = MulticastSocket::ListenerKey;

MulticastSocket::MulticastSocket(
        shared_ptr<mx3_gen::MulticastSocket> multicast_socket
) :
    multicast_socket_ {move(multicast_socket)},
    broadcaster_ {make_shared<BroadcastingMulticastSocketListener>()}
{
}

ListenerKey
MulticastSocket::add_listener(
        unique_ptr<mx3_gen::MulticastSocketListener> listener
) {
    return broadcaster_->add_listener(move(listener));
}

bool
MulticastSocket::del_listener(ListenerKey key)
{
    return broadcaster_->del_listener(key);
}

void
MulticastSocket::open(const mx3_gen::SocketAddress & multicast_socket_address)
{
    multicast_socket_->open(multicast_socket_address, broadcaster_);
}

void MulticastSocket::close()
{
    multicast_socket_->close();
}

BroadcastingMulticastSocketListener::BroadcastingMulticastSocketListener() :
    listeners_ {}
{
}

MulticastSocket::ListenerKey
BroadcastingMulticastSocketListener::add_listener(
        unique_ptr<mx3_gen::MulticastSocketListener> listener
) {
    ListenerKey key = (ListenerKey) listener.get();
    listeners_.insert(std::make_pair(key, move(listener)));
    return key;
}

bool
BroadcastingMulticastSocketListener::del_listener(
        MulticastSocket::ListenerKey key
) {
    return listeners_.erase(key) > 0;
}

void
BroadcastingMulticastSocketListener::on_datagram(const mx3_gen::Datagram & datagram)
{
    for (const auto & listener : listeners_)
        listener.second->on_datagram(datagram);
}

LoggingMulticastSocketListener::LoggingMulticastSocketListener(
    shared_ptr<mx3_gen::Logger> logger
) :
    logger_ {move(logger)}
{
}

void
LoggingMulticastSocketListener::
on_datagram(const mx3_gen::Datagram & datagram)
{
    logger_->log(mx3_gen::Logger::LOG_LEVEL_INFO, "MulticastSocket",
            "DATAGRAM: length = " + datagram.data.size());
}


/* vim: set et ts=4 sts=4 sw=4 tw=80 : */
