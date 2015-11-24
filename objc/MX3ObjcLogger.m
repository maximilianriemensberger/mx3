//
//  MX3ObjcLogger.m
//  mx3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#import "MX3ObjcLogger.h"

@implementation MX3ObjcLogger

- (void)log:(int32_t)level
        tag:(nonnull NSString *)tag
    message:(nonnull NSString *)message {
    [self log:level tag:tag message:message];
}

- (void)logException:(int32_t)level
                 tag:(nonnull NSString *)tag
             message:(nonnull NSString *)message
                what:(nonnull NSString *)what {
    [self log:level tag:tag message:[NSString stringWithFormat:@"%@ [Exception: %@ ]", message, what]];
}

- (void)logLevel:(int32_t)level tag:(nonnull NSString *)tag message:(nonnull NSString *)message {
    NSString* prefix;
    
    // switch case statements do not work here
    if (level ==  MX3LoggerLOGLEVELERROR) {
        prefix = @"ERROR: ";
    } else if (level ==  MX3LoggerLOGLEVELWARN) {
        prefix = @"WARN: ";
    } else if (level ==  MX3LoggerLOGLEVELINFO) {
        prefix = @"INFO: ";
    } else if (level ==  MX3LoggerLOGLEVELDEBUG) {
        prefix = @"DEBUG: ";
    } else if (level ==  MX3LoggerLOGLEVELVERBOSE) {
        prefix = @"VERBOSE: ";
    } else if (level ==  MX3LoggerLOGLEVELFINEST) {
        prefix = @"FINEST: ";
    } else {
        prefix = @"LOG: ";
    }
    
    NSLog(@"%@ %@: %@", prefix, tag, message);
}

@end
