#import <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>
#import <stdlib.h>
#import <stdint.h>

#import "RayView.h"
#import "ray.h"

@implementation RayView

- (void)drawRect:(NSRect)dirtyRect {
	[super drawRect:dirtyRect];

	int width = self.bounds.size.width;
	int height = self.bounds.size.height;
	uint8_t *bitmap = malloc(width * height * 3);
	if (bitmap == NULL) {
		NSLog(@"Failed to allocate memory for image");
		exit(EXIT_FAILURE);
	}

	if (renderimage(bitmap, width, height) != 0) {
		NSLog(@"Failed to render image");
		exit(EXIT_FAILURE);
	}

	NSBitmapImageRep *imageRep = [[NSBitmapImageRep alloc]
	    initWithBitmapDataPlanes:&bitmap
	                  pixelsWide:width
	                  pixelsHigh:height
	               bitsPerSample:8
	             samplesPerPixel:3
	                    hasAlpha:NO
	                    isPlanar:NO
	              colorSpaceName:NSCalibratedRGBColorSpace
	                 bytesPerRow:width * 3
	                bitsPerPixel:24];
	NSImage *image = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
	[image addRepresentation:imageRep];

	[image drawInRect:self.bounds
	         fromRect:NSZeroRect
	        operation:NSCompositingOperationSourceOver
	         fraction:1.0];

	free(bitmap);
}

@end
