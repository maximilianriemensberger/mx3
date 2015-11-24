//
//  MX3ObjcWebSocket.h
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#pragma once
#include "gen/MX3WebSocket.h"

#import "3rdParty/SocketRocket/SRWebSocket.h"

@interface MX3ObjcWebSocket : NSObject<MX3WebSocket, SRWebSocketDelegate>

- (void)connect:(nonnull NSString *)url
       listener:(nullable MX3WebSocketListener *)listener;

- (void)send:(nonnull NSString *)message;

- (BOOL)isConnected;

- (void)close;

@end
