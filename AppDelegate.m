#import <Cocoa/Cocoa.h>

#import "AppDelegate.h"
#import "RenderView.h"

@interface AppDelegate ()
@property(weak) IBOutlet NSWindow *window;
@property(weak) NSTimer *timer;
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
}

@end
