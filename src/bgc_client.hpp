#pragma once
#include "stl.hpp"
#include "event_loop.hpp"
#include "http.hpp"
#include "single_thread_task_runner.hpp"
#include "interface/socket_address.hpp"
#include "interface/bgc_client.hpp"
#include "interface/logger.hpp"
#include "web_socket.hpp"

namespace mx3 {

/**
 * The "BgcClient" of how the UI is allowed to talk to the c++ code.
 *
 * It holds on to all modules that allow calling into the ui code (java/objc).
 * It makes direclty or indireclty sure the cooresponding interface live long
 * enough (via various RAII patterns).
 */
class BgcClient final : public mx3_gen::BgcClient {
  public:
    BgcClient(
            const string & root_path,
            unique_ptr<SingleThreadTaskRunner> ui_runner,
            unique_ptr<SingleThreadTaskRunner> bg_runner,
            shared_ptr<mx3_gen::EventListener> listener,
            unique_ptr<mx3::WebSocket> web_socket,
            shared_ptr<mx3_gen::Logger> logger,
            shared_ptr<mx3_gen::Http> http_client,
            shared_ptr<mx3_gen::MulticastSocket> multicast_socket
    );

    virtual void set_server_uri(const string & uri) override;
    //void set_server_uri(string && uri); //for move optimization

    /** Test only interface function (The server will provide this in the future) */
    virtual void set_multicast_address(const mx3_gen::SocketAddress & address);
    //void set_multicast_address(const mx3_gen::SocketAddress & address); //for move optimization

    virtual void connect() override;
    virtual void disconnect() override;

    virtual string get_clip_mpd_uri(const string& clip_id) override;
    virtual string get_clip_m3u8_uri(const string& clip_id) override;

    /** Test only interface function */
    virtual void send(const string& message) override;

  private:
    const string& root_path_;
    const unique_ptr<SingleThreadTaskRunner> ui_thread_;
    const unique_ptr<SingleThreadTaskRunner> bg_thread_;
    const shared_ptr<mx3_gen::EventListener> listener_;
    const unique_ptr<mx3::WebSocket> web_socket_;

    // TODO: replace by appropriate wrapper classes?
    const shared_ptr<mx3_gen::Logger> logger_;
    const shared_ptr<mx3_gen::Http> http_;
    const shared_ptr<mx3_gen::MulticastSocket> multicast_socket_;

    string server_uri_;
    mx3_gen::SocketAddress multicast_address_;
};

}

/* vim: set et ts=4 sts=4 sw=4 tw=80 : */
