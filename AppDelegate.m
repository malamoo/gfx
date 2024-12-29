#import "AppDelegate.h"
#include <Foundation/Foundation.h>

@interface AppDelegate ()

@property(strong) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	NSLog(@"Finished launching");
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
	return YES;
}

@end
