#import <Cocoa/Cocoa.h>

enum {
  NCOM = 3,                         // components per pixel
  COMBITS = 8,                      // bits per component
  PIXBITS = NCOM * COMBITS,         // bits per pixel
  PIXSIZE = NCOM * sizeof(uint8_t), // bytes per pixel
};

@interface View : NSView
@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property(strong) NSWindow *window;
@property(strong) View *view;
@property BOOL shouldTerminate;
@end

@implementation AppDelegate

- (void)run {
  @autoreleasepool {
    NSEvent *event;
    while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES])) {
      [NSApp sendEvent:event];
      [NSApp updateWindows];
    }
  }
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
  NSRect frame = [[self.window contentView] frame];
  self.view = [[View alloc] initWithFrame:frame];
  [self.window setContentView:self.view];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
  self.shouldTerminate = YES;
  return NO;
}

@end

@implementation View {
  double lastTime;
  double fps;
  CGColorSpaceRef colorSpace;
  CGDataProviderRef provider;
  CGImageRef image;
}

- (instancetype)initWithFrame:(NSRect)frame {
  self = [super initWithFrame:frame];
  if (self) {
    lastTime = CACurrentMediaTime();
    fps = 0.0;
  }
  return self;
}

- (void)setImage:(void *)data width:(int)width height:(int)height {
  if (image) {
    CGImageRelease(image);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    image = nil;
    provider = nil;
    colorSpace = nil;
  }

  colorSpace = CGColorSpaceCreateDeviceRGB();
  provider = CGDataProviderCreateWithData(NULL, data, width * height * PIXSIZE, NULL);
  image = CGImageCreate(width, height, COMBITS, PIXBITS, width * PIXSIZE, colorSpace, kCGBitmapByteOrderDefault | kCGImageAlphaNone, provider, NULL, true, kCGRenderingIntentDefault);
}

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];

  if (image) {
    CGContextRef ctx = [[NSGraphicsContext currentContext] CGContext];
    CGContextSaveGState(ctx);
    CGContextTranslateCTM(ctx, 0, self.bounds.size.height);
    CGContextScaleCTM(ctx, 1.0, -1.0);
    CGContextDrawImage(ctx, self.bounds, image);
    CGContextRestoreGState(ctx);

    double now = CACurrentMediaTime();
    double delta = now - lastTime;
    if (delta > 0)
      fps = 1.0 / delta;
    lastTime = now;

    NSString *text = [NSString stringWithFormat:@"%.1f FPS", fps];
    NSDictionary *attrs = @{
      NSFontAttributeName : [NSFont systemFontOfSize:24],
      NSForegroundColorAttributeName : [NSColor orangeColor]
    };
    NSSize textSize = [text sizeWithAttributes:attrs];
    NSRect bounds = self.bounds;
    NSPoint point = NSMakePoint(10, NSMaxY(bounds) - textSize.height - 10);
    [text drawAtPoint:point withAttributes:attrs];
  }
}

- (void)dealloc {
  if (image) {
    CGImageRelease(image);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
  }
}

@end

void processinput(void) {
  if (!NSApp) {
    [NSApplication sharedApplication];
    [[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:NSApp topLevelObjects:nil];
    [NSApp finishLaunching];
  }
  AppDelegate *delegate = [NSApp delegate];
  [delegate run];
}

void displayimage(void *img, int width, int height) {
  AppDelegate *delegate = [NSApp delegate];
  View *view = [delegate view];
  [view setImage:img width:width height:height];
  [view display];
}

int running(void) {
  AppDelegate *delegate = [NSApp delegate];
  return ![delegate shouldTerminate];
}
