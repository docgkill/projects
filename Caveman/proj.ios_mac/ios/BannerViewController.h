//
//  BannerViewController.h
//  Caveman
//
//  Created by Kim Ji Hoon on 2014. 7. 5..
//
//

#import <UIKit/UIKit.h>
#import <iAd/iAd.h>

extern NSString * const BannerViewActionWillBegin;
extern NSString * const BannerViewActionDidFinish;

@interface BannerViewController : UIViewController

- (instancetype)initWithContentViewController:(UIViewController *)contentController;

@end