var __playCompleteCallBack = null;
//初始化播放资源
function initMusic(url,playCompleteCallBack){
     if(window.android != null && typeof( window.android ) != "undefined"){
         window.android.initMusic(url);
         __playCompleteCallBack =  playCompleteCallBack
     }
}
//暂停播放
function pauseMusic(){
 if(window.android != null && typeof( window.android ) != "undefined"){
         window.android.pauseMusic();
     }
}
//开始播放
function startMusic(){
 if(window.android != null && typeof( window.android ) != "undefined"){
         window.android.startMusic();
 }
}
//释放资源
function releasePlayer(){
   if(window.android != null && typeof( window.android ) != "undefined"){
         window.android.releasePlayer();
     }
}
//播放完成的回调
function playCompleteCallBack(){
//todo 处理播放完成的事件的
if(__playCompleteCallBack != null){
   __playCompleteCallBack();
}
}