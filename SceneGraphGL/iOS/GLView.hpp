//
//  GLView.h
//  Hyperspace
//
//  Created by mutexre on 12/05/2017.
//
//

#import <AppKit/AppKit.h>

@protocol GLViewDelegate;

@interface GLView : NSView

@property (nonatomic) id<GLViewDelegate> delegate;
@property (nonatomic) BOOL updatesContinuously;

//- (void)makeCurrent;
//- (void)update;

- (void)drawAndPresentToScreen;
- (void)presentToScreen;

@end

@protocol GLViewDelegate <NSObject>
@end
