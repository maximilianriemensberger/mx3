//
//  MX3ObjcEventListener.m
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#import "MX3ObjcEventListener.h"

@implementation MX3ObjcEventListener

- (void)onWebsocketMessage:(nonnull NSString *)message {
    [[NSNotificationCenter defaultCenter] postNotificationName:@"MX3ObjcEventListener-OnWebSocketMessage" object:self userInfo:@{@"message" : message}];
}

@end
