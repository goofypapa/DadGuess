# 笨爸爸APP-web API

1. 判断app打开
```
    window.goofypapaGame //bool
```

2. 获取token
```
    window.goofypapaToken() //func
```
3. 页面初始化完成（替代window.onload）
```
    window.goofypapaInit // window.goofypapaInit = function()...
```
4. 返回App
```
    window.location.href='goofypapa://back';
```
5. 加载音频文件
```
    window.location.href='goofypapa://loadAudio,[audio url]';
```
6.  删除音频文件
```
    window.location.href='goofypapa://deleteAudio,[audio url]';
```
7. 播放音频
```
    window.location.href='goofypapa://playAudio,[audio url]';
```
8. 停止播放音频
```
    window.location.href='goofypapa://stopAudio,[audio url]';
```
9. 停止播放所有音频
```
    window.location.href='goofypapa://stopAllAudio';
```

10. POST请求
```
    window.location.href='goofypapa://Post,[successCallback],[failCallabck],[url],[patameter],...';
```
11. GET请求
```
    window.location.href='goofypapa://Post,[successCallback],[failCallabck],[url],[patameter],...';
```
