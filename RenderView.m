#import <AppKit/AppKit.h>

#import "RenderView.h"

@implementation RenderView

- (void)drawRect:(NSRect)dirtyRect {
	[super drawRect:dirtyRect];

	[[NSColor greenColor] setFill];

	NSRectFill(self.frame);
}

@end
