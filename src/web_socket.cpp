#include <deque>
#include "web_socket.hpp"

using mx3::WebSocket;
using mx3::BroadcastingWebSocketListener;
using mx3::LoggingWebSocketListener;
using mx3::ForwardingWebSocketListener;

using ListenerKey = WebSocket::ListenerKey;

WebSocket::WebSocket(shared_ptr<mx3_gen::WebSocket> web_socket) :
    web_socket_ {move(web_socket)},
    broadcaster_ {make_shared<BroadcastingWebSocketListener>()}
{
}

ListenerKey
WebSocket::add_listener(unique_ptr<mx3_gen::WebSocketListener> listener)
{
    return broadcaster_->add_listener(move(listener));
}

bool
WebSocket::del_listener(ListenerKey key)
{
    return broadcaster_->del_listener(key);
}

void
WebSocket::connect(const string & url)
{
    web_socket_->connect(url, broadcaster_);
}

bool WebSocket::is_connected()
{
    return web_socket_->is_connected();
}

void WebSocket::close()
{
    web_socket_->close();
}

void WebSocket::send(const string & message)
{
    web_socket_->send(message);
}


BroadcastingWebSocketListener::BroadcastingWebSocketListener() :
    listeners_ {}
{
}

ListenerKey
BroadcastingWebSocketListener::
add_listener(unique_ptr<mx3_gen::WebSocketListener> listener)
{
    ListenerKey key = (ListenerKey) listener.get();
    listeners_.insert(std::make_pair(key, move(listener)));
    return key;
}

bool
BroadcastingWebSocketListener::
del_listener(ListenerKey key)
{
    return listeners_.erase(key) > 0;
}

void
BroadcastingWebSocketListener::
on_open(int16_t code, const string & message)
{
    for (const auto & listener : listeners_)
        listener.second->on_open(code, message);
}

void
BroadcastingWebSocketListener::
on_close(int16_t code, const string & message, bool remote)
{
    for (const auto & listener : listeners_)
        listener.second->on_close(code, message, remote);
}

void
BroadcastingWebSocketListener::
on_error(int16_t code, const string & message)
{
    for (const auto & listener : listeners_)
        listener.second->on_error(code, message);
}

void
BroadcastingWebSocketListener::
on_message(const string & message)
{
    for (const auto & listener : listeners_)
        listener.second->on_message(message);
}

ForwardingWebSocketListener::ForwardingWebSocketListener(
    shared_ptr<mx3_gen::EventListener> listener
) :
    listener_ {move(listener)}
{
}

void
ForwardingWebSocketListener::
on_open(int16_t /*code*/, const string & /*message*/)
{
}

void
ForwardingWebSocketListener::
on_close(int16_t /*code*/, const string & /*message*/, bool /*remote*/)
{
}

void
ForwardingWebSocketListener::
on_error(int16_t /*code*/, const string & /*message*/)
{
}

void
ForwardingWebSocketListener::
on_message(const string & message)
{
    listener_->on_websocket_message(message);
}

LoggingWebSocketListener::LoggingWebSocketListener(
    shared_ptr<mx3_gen::Logger> logger
) :
    logger_ {move(logger)}
{
}

void
LoggingWebSocketListener::
on_open(int16_t code, const string & message)
{
    logger_->log(mx3_gen::Logger::LOG_LEVEL_INFO, "WebSocket",
            "OPEN Code: " + to_string(code) + " Message: " + message);
}

void
LoggingWebSocketListener::
on_close(int16_t code, const string & message, bool /*remote*/)
{
    logger_->log(mx3_gen::Logger::LOG_LEVEL_INFO, "WebSocket",
            "CLOSE Code: " + to_string(code) + " Message: " + message);
}

void
LoggingWebSocketListener::
on_error(int16_t code, const string & message)
{
    logger_->log(mx3_gen::Logger::LOG_LEVEL_ERROR, "WebSocket",
            "ERROR Code: " + to_string(code) + " Message: " + message);
}

void
LoggingWebSocketListener::
on_message(const string & message)
{
    logger_->log(mx3_gen::Logger::LOG_LEVEL_INFO, "WebSocket",
            "MESSAGE: " + message);
}

/* vim: set et ts=4 sts=4 sw=4 tw=80 : */
