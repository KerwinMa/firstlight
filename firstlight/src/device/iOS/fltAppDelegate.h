//
//  fltAppDelegate.h
//  firstlight
//
//  Created by wu hao on 11-6-26.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import <UIKit/UIKit.h>

@class fltGLView;

@interface fltAppDelegate : NSObject <UIApplicationDelegate> {
@private
    UIWindow *m_window;
    fltGLView *m_glView;
}


@end

