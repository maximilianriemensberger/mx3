//
//  MX3ObjcLogger.h
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#pragma once
#include "gen/MX3Logger.h"

@interface MX3ObjcLogger : NSObject <MX3Logger>

- (void)log:(int32_t)level
        tag:(nonnull NSString *)tag
    message:(nonnull NSString *)message;

- (void)logException:(int32_t)level
                 tag:(nonnull NSString *)tag
             message:(nonnull NSString *)message
                what:(nonnull NSString *)what;

@end
