#include "bgc_client.hpp"

using mx3::BgcClient;
using mx3::WebSocket;
using mx3::LoggingWebSocketListener;

shared_ptr<mx3_gen::BgcClient>
mx3_gen::BgcClient::create_bgc_client(
    const string & root_path,
    const shared_ptr<EventLoop> & ui_thread_impl,
    const shared_ptr<ThreadLauncher> & launcher_impl,
    const shared_ptr<mx3_gen::EventListener> & listener_impl,
    const shared_ptr<Logger> & logger_impl,
    const shared_ptr<Http> & http_impl,
    const shared_ptr<WebSocket> & web_socket_impl,
    const shared_ptr<MulticastSocket> & multicast_socket_impl
) {
    return make_shared<mx3::BgcClient>(
            root_path,
            make_unique<mx3::EventLoopRef>(ui_thread_impl),
            make_unique<mx3::EventLoopCpp>(launcher_impl),
            listener_impl,
            make_unique<mx3::WebSocket>(web_socket_impl),
            logger_impl,
            http_impl,
            multicast_socket_impl);
}

BgcClient::BgcClient(
    const string & root_path,
    unique_ptr<SingleThreadTaskRunner> ui_runner,
    unique_ptr<SingleThreadTaskRunner> bg_runner,
    shared_ptr<mx3_gen::EventListener> listener,
    unique_ptr<mx3::WebSocket> web_socket,
    shared_ptr<mx3_gen::Logger> logger,
    shared_ptr<mx3_gen::Http> http_client,
    shared_ptr<mx3_gen::MulticastSocket> multicast_socket
) :
    root_path_ {root_path},
    ui_thread_ {move(ui_runner)},
    bg_thread_ {move(bg_runner)},
    listener_ {move(listener)},
    web_socket_ {move(web_socket)},
    logger_ {move(logger)},
    http_ {move(http_client)},
    multicast_socket_ {move(multicast_socket)},
    server_uri_ {},
    multicast_address_ {{}, 0}
{
    web_socket_->add_listener(make_unique<LoggingWebSocketListener>(logger_));
    web_socket_->add_listener(make_unique<ForwardingWebSocketListener>(listener_));
}

void
BgcClient::set_server_uri(const string & uri)
{
    server_uri_ = uri;
    logger_->log(mx3_gen::Logger::LOG_LEVEL_INFO, "BgcClient",
            "BgcClient::set_server_uri: " + uri);
}

void
BgcClient::set_multicast_address(const mx3_gen::SocketAddress & address)
{
    multicast_address_ = address;
    logger_->log(mx3_gen::Logger::LOG_LEVEL_INFO, "BgcClient",
            "BgcClient::set_multicast_address");
}

void
BgcClient::connect()
{
    web_socket_->connect(server_uri_);
    //multicast_socket_->open(multicast_address_, /* listener? */);
}

void
BgcClient::disconnect()
{
    //multicast_socket_->close();
    web_socket_->close();
}

string
BgcClient::get_clip_mpd_uri(const string & clip_id)
{
    return clip_id;
}

string
BgcClient::get_clip_m3u8_uri(const string & clip_id)
{
    return clip_id;
}

/** Test only interface function */
void
BgcClient::send(const string & message)
{
    web_socket_->send(message);
}

/* vim: set et ts=4 sts=4 sw=4 tw=80 : */
