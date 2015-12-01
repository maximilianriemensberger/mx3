//
//  MX3ObjcMulticastSocket.m
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#import "MX3ObjcMulticastSocket.h"
#import "MX3ObjcLogger.h"
#import "MX3MulticastSocketListener.h"
#import "MX3Datagram.h"

@implementation MX3ObjcMulticastSocket
{
    GCDAsyncUdpSocket *_udpSocket;
    MX3MulticastSocketListener *_listener;
    MX3SocketAddress *_address;
    MX3ObjcLogger  *_logger;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        _logger = [[MX3ObjcLogger alloc] init];
    }
    return self;
}


- (void)open:(nonnull MX3SocketAddress *)multicastSocketAddress
    listener:(nullable MX3MulticastSocketListener *)listener {
    [_logger log:MX3LoggerLOGLEVELINFO tag:@"MX3ObjcMulticastSocket" message:@"Opening."];
    
    // Close old socket if exists
    if (_udpSocket != nil) {
        [_udpSocket close];
        _udpSocket = nil;
    }
    
    _listener = listener;
    _address = multicastSocketAddress;
    
    // Create new socket
    _udpSocket = [[GCDAsyncUdpSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];

    // bind to port
    NSError *error = nil;
    [_udpSocket bindToPort:multicastSocketAddress.port error:&error];
    if (error != nil) {
        [_logger logException:MX3LoggerLOGLEVELERROR tag:@"MX3ObjcMulticastSocket" message:@"Bind to port failed" what:error.localizedDescription];
        return;
    }
    
//    // join multicast group
    NSString *ipv4adress = [self ipv4StringFromData:_address.address];
    if (ipv4adress == nil) {
        [_logger log:MX3LoggerLOGLEVELERROR tag:@"MX3ObjcMulticastSocket" message:@"Convert adress to NSString failed"];
        return;
    }
    
    
    error = nil;
    [_udpSocket joinMulticastGroup:ipv4adress error:&error];
    if (error != nil) {
        [_logger logException:MX3LoggerLOGLEVELERROR tag:@"MX3ObjcMulticastSocket" message:@"Join Multicast group failed" what:error.localizedDescription];
        return;
    }
    
    // begin receiving
    error = nil;
    [_udpSocket beginReceiving:&error];
    if (error != nil) {
        [_logger logException:MX3LoggerLOGLEVELERROR tag:@"MX3ObjcMulticastSocket" message:@"Begin receiving failed failed" what:error.localizedDescription];
        return;
    }
}

- (void)close {
    // Close old socket if exists
    if (_udpSocket != nil) {
        [_udpSocket close];
        _udpSocket = nil;
    }
    
    _listener = nil;
    _address = nil;
}

-(NSString *)ipv4StringFromData:(NSData *)data {

    if (data.length == 4) {
        unsigned char a;
        [data getBytes:&a range:NSMakeRange(0, 1)];
        unsigned char b;
        [data getBytes:&b range:NSMakeRange(1, 1)];
        unsigned char c;
        [data getBytes:&c range:NSMakeRange(2, 1)];
        unsigned char d;
        [data getBytes:&d range:NSMakeRange(3, 1)];
        
        NSString *ipv4adress = [NSString stringWithFormat:@"%i.%i.%i.%i", (int)a, (int)b, (int)c, (int)d];
        return ipv4adress;
    }
    
    return nil;
}

/**
 * By design, UDP is a connectionless protocol, and connecting is not needed.
 * However, you may optionally choose to connect to a particular host for reasons
 * outlined in the documentation for the various connect methods listed above.
 *
 * This method is called if one of the connect methods are invoked, and the connection is successful.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didConnectToAddress:(NSData *)address {

}

/**
 * By design, UDP is a connectionless protocol, and connecting is not needed.
 * However, you may optionally choose to connect to a particular host for reasons
 * outlined in the documentation for the various connect methods listed above.
 *
 * This method is called if one of the connect methods are invoked, and the connection fails.
 * This may happen, for example, if a domain name is given for the host and the domain name is unable to be resolved.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didNotConnect:(NSError *)error {
    
}

/**
 * Called when the datagram with the given tag has been sent.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didSendDataWithTag:(long)tag {
    
}

/**
 * Called if an error occurs while trying to send a datagram.
 * This could be due to a timeout, or something more serious such as the data being too large to fit in a sigle packet.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didNotSendDataWithTag:(long)tag dueToError:(NSError *)error {
    
}

/**
 * Called when the socket has received the requested datagram.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didReceiveData:(NSData *)data
      fromAddress:(NSData *)address
withFilterContext:(id)filterContext {
    
    // TODO: Source port?
    MX3SocketAddress *source = [[MX3SocketAddress alloc] initWithAddress:address port:0];
    MX3SocketAddress *destination = [[MX3SocketAddress alloc] initWithAddress:_address.address port:_address.port];
    
    MX3Datagram *datagram = [[MX3Datagram alloc] initWithSrc:source dst:destination data:data];
    [_listener onDatagram:datagram];
}

/**
 * Called when the socket is closed.
 **/
- (void)udpSocketDidClose:(GCDAsyncUdpSocket *)sock withError:(NSError *)error {
    
}

@end
