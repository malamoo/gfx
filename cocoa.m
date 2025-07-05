#import <Cocoa/Cocoa.h>

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
  self.window.contentView = self.view;

  [NSApp stop:nil];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
  self.shouldTerminate = YES;
  return NO;
}

@end

static const NSUInteger SamplesPerPixel = 3;
static const NSUInteger BitsPerSample = 8;
static const NSUInteger BitsPerPixel = SamplesPerPixel * BitsPerSample;
static const NSUInteger BytesPerPixel = SamplesPerPixel * sizeof(uint8_t);

@implementation View {
  double lastTime;
  double fps;
  NSBitmapImageRep *bitmapRep;
}

- (instancetype)initWithFrame:(NSRect)frame {
  self = [super initWithFrame:frame];
  if (self) {
    lastTime = CACurrentMediaTime();
    fps = 0.0;
    bitmapRep = nil;
  }
  return self;
}

- (BOOL)isFlipped {
  return YES;
}

- (void)updateImage:(unsigned char *)data width:(int)width height:(int)height {
  bitmapRep = nil;
  bitmapRep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&data
                                                      pixelsWide:width
                                                      pixelsHigh:height
                                                   bitsPerSample:BitsPerSample
                                                 samplesPerPixel:SamplesPerPixel
                                                        hasAlpha:NO
                                                        isPlanar:NO
                                                  colorSpaceName:NSCalibratedRGBColorSpace
                                                     bytesPerRow:width * BytesPerPixel
                                                    bitsPerPixel:BitsPerPixel];
  }

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];

  if (!bitmapRep)
    return;

  [bitmapRep drawInRect:self.bounds];

  double now = CACurrentMediaTime();
  double delta = now - lastTime;
  if (delta > 0.0)
    fps = 1.0 / delta;
  lastTime = now;
  NSString *text = [NSString stringWithFormat:@"%.1f FPS", fps];
  NSDictionary *attrs = @{
    NSFontAttributeName : [NSFont systemFontOfSize:24.0],
    NSForegroundColorAttributeName : [NSColor orangeColor]
  };
  [text drawAtPoint:NSMakePoint(10, 10) withAttributes:attrs];
}

@end

void processinput(void) {
  if (!NSApp) {
    [NSApplication sharedApplication];
    [[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:NSApp topLevelObjects:nil];
    [NSApp run];
  }
  AppDelegate *delegate = [NSApp delegate];
  [delegate run];
}

void displayimage(void *img, int width, int height) {
  AppDelegate *delegate = [NSApp delegate];
  View *view = [delegate view];
  [view updateImage:img width:width height:height];
  [view display];
}

int running(void) {
  AppDelegate *delegate = [NSApp delegate];
  return ![delegate shouldTerminate];
}
