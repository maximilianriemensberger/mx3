#pragma once
#include "interface/event_listener.hpp"
#include "interface/logger.hpp"
#include "interface/web_socket.hpp"
#include "interface/web_socket_listener.hpp"
#include "stl.hpp"

namespace mx3 {

class BroadcastingWebSocketListener;

/**
 * WebSocket
 *
 * Wraps the communication with the device framework interfaces WebSocket and
 * WebSocketListener.
 */
class WebSocket {
public:
    using ListenerKey = uintptr_t;

    WebSocket(shared_ptr<mx3_gen::WebSocket> web_socket);

    // Listener management
    ListenerKey add_listener(unique_ptr<mx3_gen::WebSocketListener> listener);
    bool del_listener(ListenerKey key);

    // Connection management
    void connect(const string & url);
    bool is_connected();
    void close();

    // Message transmission
    void send(const string & message);

private:
    // Handle to the WebSocket interface
    const shared_ptr<mx3_gen::WebSocket> web_socket_;
    // WebSocketListener interface handle that is passed to web_socket_
    const shared_ptr<mx3::BroadcastingWebSocketListener> broadcaster_;
};

class BroadcastingWebSocketListener : public mx3_gen::WebSocketListener {
public:
    using ListenerKey = WebSocket::ListenerKey;

    BroadcastingWebSocketListener();

    virtual void on_open(int16_t code, const string & message) override;
    virtual void on_close(int16_t code, const string & message,
                          bool remote) override;
    virtual void on_error(int16_t code, const string & message) override;
    virtual void on_message(const string & message) override;

    ListenerKey add_listener(unique_ptr<mx3_gen::WebSocketListener> listener);
    bool del_listener(ListenerKey key);

private:
    std::map<ListenerKey, unique_ptr<mx3_gen::WebSocketListener>> listeners_;
};

class ForwardingWebSocketListener : public mx3_gen::WebSocketListener {
public:
    ForwardingWebSocketListener(shared_ptr<mx3_gen::EventListener> Listener);

    virtual void on_open(int16_t code, const string & message) override;
    virtual void on_close(int16_t code, const string & message,
                          bool remote) override;
    virtual void on_error(int16_t code, const string & message) override;
    virtual void on_message(const string & message) override;

private:
    const shared_ptr<mx3_gen::EventListener> listener_;
};

class LoggingWebSocketListener : public mx3_gen::WebSocketListener {
public:
    LoggingWebSocketListener(shared_ptr<mx3_gen::Logger> logger);

    virtual void on_open(int16_t code, const string & message) override;
    virtual void on_close(int16_t code, const string & message,
                          bool remote) override;
    virtual void on_error(int16_t code, const string & message) override;
    virtual void on_message(const string & message) override;

private:
    const shared_ptr<mx3_gen::Logger> logger_;
};
}

/* vim: set et ts=4 sts=4 sw=4 tw=80 : */
