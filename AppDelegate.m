#import <Cocoa/Cocoa.h>

#import "AppDelegate.h"
#import "RayView.h"

@interface AppDelegate ()
@property(strong) IBOutlet NSWindow *window;
@property(strong) NSTimer *timer;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	RayView *rayView = [[RayView alloc] initWithFrame:self.window.contentView.frame];
	[self.window setContentView:rayView];

	self.timer = [NSTimer scheduledTimerWithTimeInterval:1.0 / 60.0
	                                              target:self
	                                            selector:@selector(updateGraphics)
	                                            userInfo:nil
	                                             repeats:YES];
}

- (void)updateGraphics {
	RayView *rayView = (RayView *)self.window.contentView;
	[rayView setNeedsDisplay:YES];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
	[self.timer invalidate];
	self.timer = nil;
}

@end
