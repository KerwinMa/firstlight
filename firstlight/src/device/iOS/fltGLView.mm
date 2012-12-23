//
//  fltGLView.m
//  firstlight
//
//  Created by wu hao on 11-6-26.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import "fltGLView.h"
#import "app/App.h"
#import "device/iOS/iOSDevice.h"
#import "renderer/OpenGL/OpenGLES1Renderer.h"
#import "event/EventManager.h"
#import "event/EvtTouch.h"

//declared in "firstlight.h"
namespace flt 
{
	extern flt::device::IDevicePtr createDevice(const flt::device::RenderDeviceAttribute& renderDeviceAttr);
}


@implementation fltGLView

@synthesize animating;
@dynamic animationFrameInterval;



// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame]) 
	{
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
        
		eaglLayer.opaque = YES;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
				
		
		flt::device::RenderDeviceAttribute rda;
		rda.width = CGRectGetWidth(frame);
		rda.height = CGRectGetHeight(frame);
		rda.colorBits = 32;
		rda.rendererType = flt::renderer::RENDERER_OPENGLES1;
		
		flt::device::IDevicePtr device = flt::createDevice(rda);

		backingWidth = rda.width;
		backingHeight = rda.height;
		
		if(flt::App::TheApp==0 || !flt::App::TheApp->init(device, flt::k_max_fps_default, -1))
		{
			[self release];
			return nil;	
		}
		
		self.multipleTouchEnabled = FALSE;//TRUE;
		
        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;
		
        // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
        // class is used as fallback when it isn't available.
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
		
		
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        
        [[NSNotificationCenter defaultCenter]
            addObserver:self
            selector:@selector(didRotate:)
            name:UIDeviceOrientationDidChangeNotification
            object:nil];
    }
    return self;
}

- (void)drawView:(id)sender
{	
	flt::App::TheApp->stepFrame();
}

- (void)layoutSubviews
{
    //[renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
	flt::device::IDevicePtr device = flt::App::TheApp->getDevice();
	flt::device::iOSDevice* iOSDevice = static_cast<flt::device::iOSDevice*>(device.get());
	flt::renderer::OpenGLES1Renderer* renderer = static_cast<flt::renderer::OpenGLES1Renderer*>(iOSDevice->getRenderer().get());
	renderer->resizeFromLayer((CAEAGLLayer*)self.layer);
    [self drawView:nil];
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;

        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        if (displayLinkSupported)
        {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.

            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
        }

        animating = FALSE;
    }
}

- (void)dealloc
{
	flt::App::TheApp->close();

    [super dealloc];
}

- (CGPoint)convertCoordinate:(CGPoint)p
{
	CGPoint result = p;
	//UIInterfaceOrientation ui_orientation = [[UIApplication sharedApplication] statusBarOrientation];
	//UIDeviceOrientation ui_orientation = [[UIDevice currentDevice] orientation];
	flt::device::IDevicePtr device = flt::App::TheApp->getDevice();
	flt::EDeviceOrientation orien = device->getOrientation();
	
	switch(orien)
	{
		case flt::EDO_LandscapeLeft:
		{
			result.x = p.y;
			result.y = backingWidth-p.x;
		}
			break;
		case flt::EDO_LandscapeRight:
		{
			result.x = backingHeight-p.y;
			result.y = p.x;
		}
			break;
	}
	return result;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		pt = [self convertCoordinate:pt];
		const flt::EvtTouch evt((long)touch, flt::Touch_Began, pt.x, pt.y);
		flt::EventManager::getInstance().raiseEvent(evt);
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		pt = [self convertCoordinate:pt];
		const flt::EvtTouch evt((long)touch, flt::Touch_Moved, pt.x, pt.y);
		flt::EventManager::getInstance().raiseEvent(evt);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		pt = [self convertCoordinate:pt];
		const flt::EvtTouch evt((long)touch, flt::Touch_Ended, pt.x, pt.y);
		flt::EventManager::getInstance().raiseEvent(evt);
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		pt = [self convertCoordinate:pt];
		const flt::EvtTouch evt((long)touch, flt::Touch_Ended, pt.x, pt.y);
		flt::EventManager::getInstance().raiseEvent(evt);
	}
}

- (void)didRotate:(NSNotification *)notification 
{
	UIDeviceOrientation device_orientation = [[UIDevice currentDevice] orientation];
//		UIDeviceOrientationUnknown,
//		UIDeviceOrientationPortrait,            // Device oriented vertically, home button on the bottom
//		UIDeviceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
//		UIDeviceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
//		UIDeviceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
//		UIDeviceOrientationFaceUp,              // Device oriented flat, face up
//		UIDeviceOrientationFaceDown             // Device oriented flat, face down

	if(flt::App::TheApp)
	{
		flt::App::TheApp->onRotate(flt::EDeviceOrientation(device_orientation));
	}

}


@end
