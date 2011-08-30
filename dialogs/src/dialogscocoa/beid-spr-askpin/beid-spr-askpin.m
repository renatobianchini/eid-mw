//
//  beid-spr-askpin.m
//  beid-spr-askpin
//
//  Created by Frank Mariën on 08/08/11.
//  Copyright 2011 FedICT. All rights reserved.
//

#import "beid-spr-askpin.h"


@implementation beid_spr_askpin


// after the GUI is constructed, initialize some working objects
////////////////////////////////////////////////////////////////////////////////////////
- (void)awakeFromNib
{
	NSRunningApplication* me=[NSRunningApplication currentApplication];
	NSArray* peers=[NSRunningApplication runningApplicationsWithBundleIdentifier:@"be.fedict.eid.cryptomodules.pkcs#11.beid-spr-askpin"];
	for(id peer in peers)
	{
		if(![peer isEqual:me])
			[peer terminate:nil];
		
	}
	
	NSArray* args=[[NSProcessInfo processInfo] arguments]; 
	if([args count]==2)
	{
		NSString* basicLabel=[textField stringValue];
		NSString* sprName=[args objectAtIndex:1];
		[textField setObjectValue:[basicLabel stringByAppendingFormat:@" \"%@\"",sprName]];
		[progress startAnimation:self];
		[window makeKeyAndOrderFront:nil];
		[me activateWithOptions:NSApplicationActivateIgnoringOtherApps];
	}
	else
	{
		[NSApp terminate:nil];
	}
}

@end
