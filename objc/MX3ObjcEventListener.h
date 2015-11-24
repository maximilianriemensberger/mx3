//
//  MX3ObjcEventListener.h
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#pragma once
#include "gen/MX3EventListener.h"

@interface MX3ObjcEventListener : NSObject<MX3EventListener>

- (void)onWebsocketMessage:(nonnull NSString *)message;

@end
