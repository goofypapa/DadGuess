//
//  BluePackage.m
//  DadGuess-mobile
//
//  Created by 吴思 on 2018/11/26.
//

#import <Foundation/Foundation.h>
#import "BluePackage.h"

#import "BlueDeviceListener.h"

#import <list>
#import <string>
#import <vector>

const char * Service_UUID = "FAA0";
const char * Characteristic_UUID = "FAA1";
const char * DeviceNamePrefix = "爸爸猜猜";

NSTimer * m_timer = nil;

@implementation BluePackage

-(CBCentralManager *)cmgr
{
    if (!_cMgr) {
        _cMgr = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    }
    return _cMgr;
}

//只要中心管理者初始化 就会触发此代理方法 判断手机蓝牙状态
- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    switch (central.state) {
        case 0:
            NSLog(@"CBCentralManagerStateUnknown");
            break;
        case 1:
            NSLog(@"CBCentralManagerStateResetting");
            break;
        case 2:
            NSLog(@"CBCentralManagerStateUnsupported");//不支持蓝牙
            break;
        case 3:
            NSLog(@"CBCentralManagerStateUnauthorized");
            break;
        case 4:
        {
            NSLog(@"CBCentralManagerStatePoweredOff");//蓝牙未开启
            BlueDeviceListener::_onConnectStateChanged( false );
        }
            break;
        case 5:
        {
            NSLog(@"CBCentralManagerStatePoweredOn");//蓝牙已开启
            
            if( m_timer != nil )
            {
                [m_timer invalidate];
                m_timer = nil;
            }
            
            // 创建定时器
            m_timer = [NSTimer timerWithTimeInterval:3 target:self selector:@selector(scan) userInfo:nil repeats:YES];
            // 将定时器添加到runloop中，否则定时器不会启动
            [[NSRunLoop mainRunLoop] addTimer:m_timer forMode:NSRunLoopCommonModes];
            
            // 搜索成功之后,会调用我们找到外设的代理方法
            // - (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI; //找到外设
        }
            break;
        default:
            break;
    }
}

// 发现外设后调用的方法
- (void)centralManager:(CBCentralManager *)central // 中心管理者
 didDiscoverPeripheral:(CBPeripheral *)peripheral // 外设
     advertisementData:(NSDictionary *)advertisementData // 外设携带的数据
                  RSSI:(NSNumber *)RSSI // 外设发出的蓝牙信号强度
{
    //NSLog(@"%s, line = %d, cetral = %@,peripheral = %@, advertisementData = %@, RSSI = %@", __FUNCTION__, __LINE__, central, peripheral, advertisementData, RSSI);
    
    /*
     peripheral = <CBPeripheral: 0x166668f0 identifier = C69010E7-EB75-E078-FFB4-421B4B951341, Name = "OBand-75", state = disconnected>, advertisementData = {
     kCBAdvDataChannel = 38;
     kCBAdvDataIsConnectable = 1;
     kCBAdvDataLocalName = OBand;
     kCBAdvDataManufacturerData = <4c69616e 0e060678 a5043853 75>;
     kCBAdvDataServiceUUIDs =     (
     FEE7
     );
     kCBAdvDataTxPowerLevel = 0;
     }, RSSI = -55
     根据打印结果,我们可以得到运动手环它的名字叫 OBand-75
     
     */
    
    // 需要对连接到的外设进行过滤
    // 1.信号强度(40以上才连接, 80以上连接)
    // 2.通过设备名(设备字符串前缀是 OBand)
    // 在此时我们的过滤规则是:有OBand前缀并且信号强度大于35
    // 通过打印,我们知道RSSI一般是带-的
    
    if( [peripheral.name length] > 0 )
    {
        static std::list< std::string > s_deviceNameList;
        std::string t_deviceName = std::string( [peripheral.name UTF8String] );
        if( std::find( s_deviceNameList.begin(), s_deviceNameList.end(), t_deviceName ) == s_deviceNameList.end() )
        {
            printf( "---------> %s \n", t_deviceName.c_str() );
            s_deviceNameList.push_back( t_deviceName );
        }
    }
    
//    printf( "---------> %s \n", [peripheral.name cStringUsingEncoding:NSUTF8StringEncoding] );
    
    if ([peripheral.name hasPrefix:[[NSString alloc] initWithCString:DeviceNamePrefix encoding:NSUTF8StringEncoding]]) {
        // 在此处对我们的 advertisementData(外设携带的广播数据) 进行一些处理
        
        // 通常通过过滤,我们会得到一些外设,然后将外设储存到我们的可变数组中,
        // 这里由于附近只有1个运动手环, 所以我们先按1个外设进行处理
        
        // 标记我们的外设,让他的生命周期 = vc
        self.peripheral = peripheral;
        // 发现完之后就是进行连接
        [self.cMgr connectPeripheral:self.peripheral options:nil];
        NSLog(@"%s, line = %d", __FUNCTION__, __LINE__);
    }
}

// 中心管理者连接外设成功
- (void)centralManager:(CBCentralManager *)central // 中心管理者
  didConnectPeripheral:(CBPeripheral *)peripheral // 外设
{
    NSLog(@"%s, line = %d, %@=连接成功", __FUNCTION__, __LINE__, peripheral.name);
    // 连接成功之后,可以进行服务和特征的发现
    
    //取消搜索
    if( m_timer != nil )
    {
        [m_timer invalidate];
        m_timer = nil;
    }
    
    [self.cMgr stopScan];
    
    //  设置外设的代理
    self.peripheral.delegate = self;
    
    // 外设发现服务,传nil代表不过滤
    // 这里会触发外设的代理方法 - (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
    [self.peripheral discoverServices:nil];
    
    BlueDeviceListener::_onConnectStateChanged( true );
    
}
// 外设连接失败
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSLog(@"%s, line = %d, %@=连接失败", __FUNCTION__, __LINE__, peripheral.name);
}

// 丢失连接
- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSLog(@"%s, line = %d, %@=断开连接", __FUNCTION__, __LINE__, peripheral.name);
    
    // 创建定时器
    m_timer = [NSTimer timerWithTimeInterval:3 target:self selector:@selector(scan) userInfo:nil repeats:YES];
    // 将定时器添加到runloop中，否则定时器不会启动
    [[NSRunLoop mainRunLoop] addTimer:m_timer forMode:NSRunLoopCommonModes];
    
    BlueDeviceListener::_onConnectStateChanged( false );
}

//返回的蓝牙服务通知通过代理实现
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error{
    for (CBService * service in peripheral.services) {
        NSLog(@"Service found with UUID :%@",service.UUID);
        if ([service.UUID isEqual:[CBUUID UUIDWithString:[[NSString alloc] initWithCString:Service_UUID encoding:NSUTF8StringEncoding]]]) {
            [peripheral discoverCharacteristics:nil forService:service];
        }
    }
}

//查找到该设备所对应的服务
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error{
    //每个peripheral都有很多服务service（这个依据蓝牙而定），每个服务都会有几个特征characteristic，区分这些就是UUID
    //这里可以利用开头说的LightBlue软件连接蓝牙看看你的蓝牙硬件有什么服务和每个服务所包含的特征，然后根据你的协议里面看看你需要用到哪个特征的哪个服务
    for (CBCharacteristic * characteristic in service.characteristics) {
        //        NSLog(@"查找到的服务（属性）%@",characteristic);
        //所对应的属性用于接收和发送数据
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[[NSString alloc] initWithCString:Characteristic_UUID encoding:NSUTF8StringEncoding]]]) {
            [peripheral setNotifyValue:YES forCharacteristic:characteristic];//监听这个服务发来的数据
            [peripheral readValueForCharacteristic:characteristic];//主动去读取这个服务发来的数据
        }
//        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"2AF1"]]) {
//            _characteristic = characteristic;
//            //*****此处已经连接好蓝牙，可以在这里给蓝牙发指令，也就是写入数据
//            //            [self sendMessageWithType:_type];//1.查询数量
//            NSLog(@"%@",value);
//            [_peripheral writeValue:value forCharacteristic:_characteristic type:CBCharacteristicWriteWithResponse];
//        }
    }
}

//接收数据的函数.处理蓝牙发过来得数据   读数据代理，这里已经收到了蓝牙发来的数据
-(void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error{
    if (error) {
        NSLog(@"Error discovering characteristics: %@", [error localizedDescription]);
        return;
    }
    if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[[NSString alloc] initWithCString:Characteristic_UUID encoding:NSUTF8StringEncoding]]]) {
//        NSLog(@"收到蓝牙发来的数据%@",characteristic.value);
//        NSString * t_str = [self convertDataToHexStr: characteristic.value];
        
        std::vector< unsigned char > t_vectorData;
        
        void * t_data = malloc( sizeof( char * ) * [characteristic.value length] );

        [characteristic.value getBytes: t_data range:( (NSRange) { 0, [characteristic.value length] })];

        for( int i = 0; i < [characteristic.value length]; ++i )
        {
            t_vectorData.push_back( *((unsigned char *)t_data + i) );
        }

        free( t_data );
        
        BlueDeviceListener::_onRecvedData( t_vectorData );
        
        //在这里解析收到的数据，一般是data类型的数据，这里要根据蓝牙厂商提供的协议进行解析并且配合LightBlue来查看数据结构，我当时收到的数据是十六进制的数据但是是data类型，所以我先讲data解析出来之后转为十进制来使用。具体方法后面我会贴出
        //还有一点收到数据后有的硬件是需要应答的，如果应答的话就是在这里再给蓝牙发一个指令（写数据）：“我收到发的东西了，你那边要做什么操作可以做了”。
    }
    
//    if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:@"FAA1"]]) {
////        _characteristic = characteristic;
//        //*****此处已经连接好蓝牙，可以在这里给蓝牙发指令，也就是写入数据
//        //            [self sendMessageWithType:_type];//1.查询数量
////        例：
//        NSMutableData *value = [NSMutableData data];
////        在这里把数据转成data存储到value里面
//        NSLog(@"%@",value);
//        [_peripheral writeValue:value forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
//    }

}


- (NSString *)convertDataToHexStr:(NSData *)data {
    if (!data || [data length] == 0) {
        return @"";
    }
    NSMutableString *string = [[NSMutableString alloc] initWithCapacity:[data length]];
    
    [data enumerateByteRangesUsingBlock:^(const void *bytes, NSRange byteRange, BOOL *stop) {
        unsigned char *dataBytes = (unsigned char*)bytes;
        for (NSInteger i = 0; i < byteRange.length; i++) {
            NSString *hexStr = [NSString stringWithFormat:@"%x", (dataBytes[i]) & 0xff];
            if ([hexStr length] == 2) {
                [string appendString:hexStr];
            } else {
                [string appendFormat:@"0%@", hexStr];
            }
        }
    }];
    
    return string;
    
}

//*****写数据代理，上面写入数据之后就会自动调用这个函数
- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error{
    NSLog(@"%@",characteristic.UUID);
    if (error) {
        NSLog(@"Error changing notification state: %@",[error localizedDescription]);
    }
    //其实这里貌似不用些什么（我是没有写只是判断了连接状态）
}


-(void)scan{
    
    [self.cMgr stopScan];
    
    // 在中心管理者成功开启后再进行一些操作
    // 搜索外设
    [self.cMgr scanForPeripheralsWithServices:nil // 通过某些服务筛选外设
                                      options:nil]; // dict,条件
}

@end

void BlueDeviceListener::listen( void )
{
    if( m_listened )
    {
        return;
    }
    
    [[BluePackage alloc]cmgr];
    m_listened = true;
}
