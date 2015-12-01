#import <UIKit/UIKit.h>
#import "gen/MX3Api.h"
#import "gen/MX3BgcClient.h"

@interface MXAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) MX3BgcClient *bgcClient;
@property (strong, nonatomic) id<MX3Logger> logger;

@end
