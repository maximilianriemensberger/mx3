//
//  MX3ObjcWebSocket.m
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#import "MX3ObjcWebSocket.h"
#import "MX3ObjcLogger.h"
#import "MX3WebSocketListener.h"

@implementation MX3ObjcWebSocket
{
    SRWebSocket* _webSocket;
    MX3WebSocketListener* _listener;
    MX3ObjcLogger* _logger;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        _logger = [[MX3ObjcLogger alloc] init];
    }
    return self;
}

- (void)connect:(nonnull NSString *)url
       listener:(nullable MX3WebSocketListener *)listener {
    [_logger log:MX3LoggerLOGLEVELINFO tag:@"MX3ObjcWebSocket" message:@"prepare connect ..."];
    
    if (_webSocket != nil) {
        [_webSocket close];
        _webSocket = nil;
    }
    
    NSURL* nsurl = [NSURL URLWithString:url];
    if (nsurl == nil) {
        return;
    }
    
    _listener = listener;
    
    [_logger log:MX3LoggerLOGLEVELINFO tag:@"MX3ObjcWebSocket" message:@"Connecting ..."];

    _webSocket = [[SRWebSocket alloc] initWithURL:nsurl];
    _webSocket.delegate = self;
    [_webSocket open];
}

- (void)send:(nonnull NSString *)message {
    [_webSocket send:message];
}

- (BOOL)isConnected {
    if (_webSocket == nil) {
        return NO;
    } else {
        return _webSocket.readyState == SR_OPEN;
    }
}

- (void)close {
    [_webSocket close];
    _webSocket = nil;
}


#pragma mark - SRWebSocketDelegate

// message will either be an NSString if the server is using text
// or NSData if the server is using binary.
- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessage:(id)message {
    if (webSocket == _webSocket) {
        
        if ([message isKindOfClass:[NSString class]]) {
            [_listener onMessage:message];
        } else if ([message isKindOfClass:[NSData class]]) {
            [_listener onError:0 message:@"Binary messages are not supported."];
        }
    }
}

- (void)webSocketDidOpen:(SRWebSocket *)webSocket {
    if (webSocket == _webSocket) {
        
        [_logger log:MX3LoggerLOGLEVELINFO tag:@"MX3ObjcWebSocket" message:@"Connected ..."];

        
        CFIndex statusCodeCFIndex = CFHTTPMessageGetResponseStatusCode(webSocket.receivedHTTPHeaders);
        int16_t statusCode = (int16_t) statusCodeCFIndex;
        
        CFStringRef statusLineCFStringRef = CFHTTPMessageCopyResponseStatusLine(webSocket.receivedHTTPHeaders);
        NSString *statusLine = (__bridge NSString *)statusLineCFStringRef;
        
        [_listener onOpen:statusCode message:statusLine];
    }
}
- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error {
    if (webSocket == _webSocket) {
        
        [_logger log:MX3LoggerLOGLEVELINFO tag:@"MX3ObjcWebSocket" message:@"Did fail with error ..."];
        
        [_listener onError:(int16_t)0 message:error.localizedDescription];
    }
}


- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean {
    if (webSocket == _webSocket) {
        BOOL remote = !wasClean;
        [_listener onClose:(int16_t)code message:reason remote:remote];
    }
}

@end
