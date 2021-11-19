//
// Created by william on 2021/11/18.
//

#import <Cocoa/Cocoa.h>
#import <MetalKit/MTKView.h>

@interface MetalView : NSWindowController<MTKViewDelegate>
    @property (nonatomic) MTKView* metalView;
    @property (nonatomic) id<MTLDevice> mtlDevice;
@end


@implementation MetalView
- (void)windowDidLoad
{
    [super windowDidLoad];
    NSLog(@"windowDidLoad");
}

- (void)drawInMTKView:(MTKView*)view
{
    NSLog(@"drawInMTKView");
}
@end

int metalTest(int argc, const char* argv[])
{
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        MetalView* metalViewController = [[MetalView alloc] init];
        [metalViewController loadWindow];
        [metalViewController windowDidLoad];
    }
    NSLog(@"metalTest");
    return NSApplicationMain(argc, argv);
}
