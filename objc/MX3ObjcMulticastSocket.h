//
//  MX3ObjcMulticastSocket.h
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#pragma once
#include "gen/MX3MulticastSocket.h"

@interface MX3ObjcMulticastSocket : NSObject<MX3MulticastSocket>

- (void)open:(nonnull MX3SocketAddress *)multicastSocketAddress
    listener:(nullable MX3MulticastSocketListener *)listener;

- (void)close;

@end
