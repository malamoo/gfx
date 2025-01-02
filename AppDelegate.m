#import <Foundation/Foundation.h>

#import "AppDelegate.h"
#import "RenderView.h"

@interface AppDelegate ()
@property(strong) IBOutlet NSWindow *window;
@property(strong) NSTimer *timer;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	self.timer = [NSTimer scheduledTimerWithTimeInterval:1.0 / 60.0
	                                              target:self
	                                            selector:@selector(updateGraphics)
	                                            userInfo:nil
	                                             repeats:YES];
}

- (void)updateGraphics {
	[self.window.contentView setNeedsDisplay:YES];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	[self.timer invalidate];
	self.timer = nil;
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
	return YES;
}

@end
