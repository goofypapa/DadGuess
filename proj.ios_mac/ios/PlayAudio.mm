//
//  CPPBridging.m
//  DadGuess-mobile
//
//  Created by 吴思 on 2019/4/2.
//
#import "cocos2d.h"
#import "Common.h"
#import "PlayAudio.h"
#import <AVFoundation/AVFoundation.h>
#import <mutex>
#import <stack>


std::map< int, std::string > s_playAudioPlayPathMap;
int PlayAudio::sm_playId = 0;
std::list<int> s_playAudioPlayIdList;
std::map< int, PlayAudio::FINISH_CALLBACK_FUNC > s_playAudioPlayerFinishCallbackMap;
std::stack< AVAudioPlayer * > s_playAudioPlayerStack;

std::mutex s_playAudioMutex;

@interface PlayerDelegate : NSObject<AVAudioPlayerDelegate>
{
    
}

@property (nonatomic) int playId;
@property (nonatomic,retain)AVAudioPlayer *audioPlayer;

@end

std::map< int, PlayerDelegate * > s_playerMap;

@implementation PlayerDelegate

// 音频播放完成时

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{
    
    // 音频播放完成时，调用该方法。
    
    // 参数flag：如果音频播放无法解码时，该参数为NO。
    
    //当音频被终端时，该方法不被调用。而会调用audioPlayerBeginInterruption方法
    
    // 和audioPlayerEndInterruption方法
    
    s_playAudioMutex.lock();
    {
        
        auto t_finishIt = s_playAudioPlayerFinishCallbackMap.find( _playId );
        if( t_finishIt != s_playAudioPlayerFinishCallbackMap.end() )
        {
            s_playAudioPlayerFinishCallbackMap.erase(t_finishIt);
        }
        
        auto t_it = s_playerMap.find( _playId );
        if( t_it == s_playerMap.end() )
        {
            s_playAudioMutex.unlock();
            return;
        }
        
        s_playAudioPlayPathMap.erase( _playId );
        s_playAudioPlayIdList.remove( _playId );
        s_playerMap.erase( _playId );

        [_audioPlayer.url release];
        [_audioPlayer.data release];
        _audioPlayer.delegate = nil;
        s_playAudioPlayerStack.push( _audioPlayer );
        [self dealloc];
    }
    s_playAudioMutex.unlock();
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError*)error{
    
    printf("------------->> 解码错误！\n");
}

// 当音频播放过程中被中断时

- (void)audioPlayerBeginInterruption:(AVAudioPlayer *)player{
    
    // 当音频播放过程中被中断时，执行该方法。比如：播放音频时，电话来了！
    
    // 这时候，音频播放将会被暂停。
    
    printf( "-------------------> delegate pause \n" );
    
}

// 当中断结束时

- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player withOptions:(NSUInteger)flags{
    
    // AVAudioSessionInterruptionFlags_ShouldResume 表示被中断的音频可以恢复播放了。
    
    // 该标识在iOS 6.0 被废除。需要用flags参数，来表示视频的状态。

    NSLog(@"中断结束，恢复播放");
    
    if (flags == AVAudioSessionInterruptionOptionShouldResume && player != nil){
        
        [player play];
    }
}

@end


int PlayAudio::play( const std::string & p_audioPath )
{
    int t_playId = -1;
    NSString * t_realAudioPath = ( p_audioPath.find( "audio" ) == 0 ? [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:p_audioPath.c_str()] ofType:@""] : [NSString stringWithUTF8String:cocos2d::FileUtils::getInstance()->fullPathForFilename( p_audioPath ).c_str()] );
    
    
    AVAudioPlayer * t_player = nullptr;
    
    NSURL *url= [NSURL fileURLWithPath:t_realAudioPath];
    
    if( s_playAudioPlayerStack.size() )
    {
        s_playAudioMutex.lock();
        {
            t_player = s_playAudioPlayerStack.top();
            s_playAudioPlayerStack.pop();
        }
        s_playAudioMutex.unlock();
    }else{
        t_player = [AVAudioPlayer alloc];
    }
    [t_player initWithContentsOfURL:url error:nil];
    
    if( t_player == nil )
    {
        printf( "player is nil \n" );
        return -1;
    }
    
    PlayerDelegate * p_delegate = [PlayerDelegate alloc];
    t_player.delegate = p_delegate;
    [t_player prepareToPlay];

    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setCategory:AVAudioSessionCategoryPlayback error:nil];

    [session setActive:YES error:nil];
    
    s_playAudioMutex.lock();
    {
        t_playId = sm_playId++;
        
        p_delegate.playId = t_playId;
        p_delegate.audioPlayer = t_player;
        
        s_playAudioPlayIdList.push_back( t_playId );
        s_playAudioPlayPathMap[t_playId] = p_audioPath;

        s_playerMap[t_playId] = p_delegate;
        
        s_playAudioMutex.unlock();
    }
    [t_player play];
    
    return t_playId;
}

void PlayAudio::pause( const int p_playId )
{
    auto t_it = s_playerMap.find( p_playId );
    if( t_it == s_playerMap.end() )
    {
        return;
    }
    
    [t_it->second.audioPlayer pause];
}

void PlayAudio::pauseAll()
{
    for( auto item : s_playerMap )
    {
        [item.second.audioPlayer pause];
    }
}

void PlayAudio::resume( const int p_playId )
{
    auto t_it = s_playerMap.find( p_playId );
    if( t_it == s_playerMap.end() )
    {
        return;
    }
    
    [t_it->second.audioPlayer play];
}

void PlayAudio::resumeAll()
{
    
}

void PlayAudio::stop( const int p_playId )
{
    s_playAudioMutex.lock();
    {
        
        auto t_finishIt = s_playAudioPlayerFinishCallbackMap.find( p_playId );
        if( t_finishIt != s_playAudioPlayerFinishCallbackMap.end() )
        {
            s_playAudioPlayerFinishCallbackMap.erase(t_finishIt);
        }
        
        auto t_it = s_playerMap.find( p_playId );
        if( t_it == s_playerMap.end() )
        {
            s_playAudioMutex.unlock();
            return;
        }
        
        [t_it->second.audioPlayer stop];
        
        auto t_delegate = s_playerMap[p_playId];
        
        s_playAudioPlayPathMap.erase( p_playId );
        s_playAudioPlayIdList.remove( p_playId );
        s_playerMap.erase( p_playId );
        
        [t_delegate.audioPlayer.url release];
        [t_delegate.audioPlayer.data release];
        t_delegate.audioPlayer.delegate = nil;
        s_playAudioPlayerStack.push( t_delegate.audioPlayer );
        [t_delegate dealloc];
        
    }
    s_playAudioMutex.unlock();
}

void PlayAudio::stopAll( const int p_playId )
{
    for( auto item : s_playerMap )
    {
        stop( item.second.playId );
    }
}

PlayAudio::AudioState PlayAudio::getState( const int p_playId )
{
    auto t_it = s_playAudioPlayPathMap.find( p_playId );
    
    if( t_it == s_playAudioPlayPathMap.end() )
    {
        return ERROR;
    }
    
    return PLAYING;
}

void PlayAudio::setFinishCallback( int p_playId, const PlayAudio::FINISH_CALLBACK_FUNC & p_finishCallbackFunc )
{
    s_playAudioPlayerFinishCallbackMap[p_playId] = p_finishCallbackFunc;
}
