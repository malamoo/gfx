#import <AppKit/AppKit.h>
#include <CoreGraphics/CoreGraphics.h>

#import "RenderView.h"

@implementation RenderView

- (void)drawRect:(NSRect)dirtyRect {
	[super drawRect:dirtyRect];

	NSGraphicsContext *context = [NSGraphicsContext currentContext];
	CGContextRef cgContext = [context CGContext];

	CGContextSetRGBFillColor(cgContext, 0.25, 0.5, 1, 1);
	CGContextFillRect(cgContext, self.bounds);

	static float angle = 0;
	float radius = 50;
	float centerX = NSMidX(self.bounds);
	float centerY = NSMidY(self.bounds);

	float circleX = centerX + cos(angle) * 100;
	float circleY = centerY + sin(angle) * 100;

	CGContextSetRGBFillColor(cgContext, 0.5, 0.7, 0.6, 1);
	CGContextFillEllipseInRect(cgContext, CGRectMake(circleX - radius, circleY - radius, 2 * radius, 2 * radius));

	angle += 0.05;
	if (angle > 2 * M_PI)
		angle -= 2 * M_PI;
}

@end
