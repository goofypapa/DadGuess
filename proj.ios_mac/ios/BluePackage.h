//
//  BluePackage.h
//  dadpat
//
//  Created by 吴思 on 2018/11/26.
//

#import <UIKit/UIKit.h>
#import <CoreBluetooth/CoreBluetooth.h>

@interface BluePackage : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate> {
    
    
}
@property (nonatomic, strong) CBCentralManager *cMgr;
@property (nonatomic, strong) CBPeripheral *peripheral;

@end
