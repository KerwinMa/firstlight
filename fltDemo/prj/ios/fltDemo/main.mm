//
//  main.m
//  fltDemo
//
//  Created by wu hao on 11-6-26.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GameApp.h"
#import "firstlight.h"

FLT_IMPLEMENT_ENGINE

int main(int argc, char *argv[]) {
	
	FLT_IMPLEMENT_APP(GameApp)	
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"fltAppDelegate");
    [pool release];
    return retVal;
}
