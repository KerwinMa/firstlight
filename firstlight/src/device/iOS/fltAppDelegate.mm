//
//  fltAppDelegate.m
//  firstlight
//
//  Created by wu hao on 11-6-26.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import "fltAppDelegate.h"
#import "fltGLView.h"



@implementation fltAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    m_window = [[UIWindow alloc] initWithFrame: screenBounds];
    m_glView = [[fltGLView alloc] initWithFrame: screenBounds];
    
    [m_window addSubview: m_glView];
    [m_window makeKeyAndVisible];
	
	[m_glView startAnimation];
    return YES;
}

- (void) dealloc
{
    [m_glView release];
    [m_window release];
    [super dealloc];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [m_glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [m_glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [m_glView stopAnimation];
}


@end
