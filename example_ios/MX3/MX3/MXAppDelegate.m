#import "MXAppDelegate.h"
#import "MX3HttpObjc.h"
#import "MX3EventLoopObjc.h"
#import "MX3ThreadLauncherObjc.h"
#import "MX3ObjcEventListener.h"
#import "MX3ObjcLogger.h"
#import "MX3ObjcWebSocket.h"
#import "MX3ObjcMulticastSocket.h"

@implementation MXAppDelegate

- (MX3BgcClient *) makeBgcClient {
    
    // give mx3 a root folder to work in
    // todo, make sure that the path exists before passing it to mx3 c++
    NSString *documentsFolder = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    NSString *file = [documentsFolder stringByAppendingPathComponent:@"mx3"];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:file]) {
        [fileManager createDirectoryAtPath:file withIntermediateDirectories:NO attributes:nil error:nil];
    }
    
    id <MX3Http> httpImpl = [[MX3HttpObjc alloc] init];
    id <MX3EventLoop> uiThread= [[MX3EventLoopObjc alloc] init];
    id <MX3ThreadLauncher> launcher = [[MX3ThreadLauncherObjc alloc] init];
    
    id <MX3EventListener> eventListener = [[MX3ObjcEventListener alloc] init];
    _logger = [[MX3ObjcLogger alloc] init];
    id <MX3WebSocket> webSocket = [[MX3ObjcWebSocket alloc] init];
    id <MX3MulticastSocket> multicastSocket = [[MX3ObjcMulticastSocket alloc] init];
    
    MX3BgcClient *bgcClient = [MX3BgcClient createBgcClient:file uiThread:uiThread launcher:launcher listener:eventListener logger:_logger httpImpl:httpImpl webSocketImpl:webSocket multicastSocketImpl:multicastSocket];
    
    return bgcClient;
}


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    self.bgcClient = [self makeBgcClient];
    return YES;
}

@end
