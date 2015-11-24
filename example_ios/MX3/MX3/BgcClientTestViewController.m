//
//  BgcClientTestViewController.m
//  MX3
//
//  Created by Michael Kugler on 24/11/15.
//
//

#import "BgcClientTestViewController.h"
#import "gen/MX3BgcClient.h"
#import "MXAppDelegate.h"

@implementation BgcClientTestViewController {
    
    __weak IBOutlet UITextView *_receivedWebSocketMessagesTextView;
    __weak IBOutlet UITextView *_logTextView;
    __weak IBOutlet UIButton *_sendHelloButton;
}

-(MX3BgcClient *)bgcClient {
    return [((MXAppDelegate*) [[UIApplication sharedApplication] delegate]) bgcClient];
}

-(void)viewDidLoad {
    [super viewDidLoad];
    
    // register for notifications
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onWebSocketMessage:) name:@"MX3ObjcEventListener-OnWebSocketMessage" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onLog:) name:@"MX3ObjcLogger-log" object:nil];
    
    // initialize bgcClient:
    
    [self.bgcClient setServerUri:@"ws://192.168.1.1:4080"];
    [self.bgcClient connect];

    
}

-(void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}


- (IBAction)sendHelloButtonTouchUpInside:(UIButton *)sender {
    static NSString* TEST_HELLO_MESSAGE = @"{\"CMD\":0,\"ARG\":[{\"userid\":\"testuser-a40uvsv0SG\",\"useragent\":\"testuser\"}]}";
    
    [self.bgcClient send:TEST_HELLO_MESSAGE];
}


#pragma mark NSNotificationCenter

- (void)onWebSocketMessage:(NSNotification*)notification {
    _receivedWebSocketMessagesTextView.text = [NSString stringWithFormat:@"%@\n%@", notification.userInfo[@"message"], _receivedWebSocketMessagesTextView.text];
}

- (void)onLog:(NSNotification*)notification {
    _logTextView.text = [NSString stringWithFormat:@"%@\n%@", notification.userInfo[@"logString"], _logTextView.text];

}

@end
