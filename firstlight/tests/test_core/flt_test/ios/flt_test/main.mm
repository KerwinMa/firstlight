//
//  main.m
//  flt_test
//
//  Created by wu hao on 10-9-6.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "testApp.h"
#import "firstlight.h"

//FLT
FLT_IMPLEMENT_ENGINE

int main(int argc, char *argv[]) {
    
	FLT_IMPLEMENT_APP(TestApp)	
	
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"fltAppDelegate");
    [pool release];
    return retVal;
}
