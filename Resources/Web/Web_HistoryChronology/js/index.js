/*最大的div的宽高*/
var docuWidth = document.documentElement.clientWidth;  //可见区域宽度
var docuHeight = document.documentElement.clientHeight; //可见区域高度
$('.showMove').css({'height':docuHeight,'width':docuWidth});
// window.android.setScreenOrientation(1)
// window.android.pauseMusic()

$('.progress .img3').on('touchstart',function () {
    $('.showMove img').remove();
});

var prvedate = 0.0;
$('.progress .img3').on('touchmove',function (e) {
    $(this).addClass("heartAnimation");
    var oEvent=e||event;
    var x = oEvent.touches[0].pageX;
    var y = oEvent.touches[0].pageY;
    var img2Left = $('.progress .img2')[0].offsetLeft;
    var img2Width = $('.progress .img2')[0].offsetWidth;
    var moveImg1 = x - img2Left + 10;
    var img1Width = img2Width - moveImg1;
    if(x < img2Left/1.3){
        x = img2Left/1.3
    }
    if(x > img2Width + img2Left/1.2){
        x = img2Width + img2Left/1.2
    }
    if(img1Width > img2Width){
        img1Width = img2Width
    }
    $('.progress .img3').css('left',x + 'px');
    $('.progress .img1').css('width',img1Width + 'px');

    
    //触摸有小星星跟随移动特效
    //判断触摸时间，毫秒
    var newdate = new Date();
    if( newdate - prvedate < 20 ){
        return;
    }
    prvedate = newdate;
    var randX = parseInt(Math.random()*40+1);
    var randY = parseInt(Math.random()*40+1);
    var img1X = $('.progress .img3')[0].x - randX;
    var img1Y = $('.progress .img3')[0].y + randY;
    var img1 = $('<img src="./image/subball1.png">');
    $('.showMove').append(img1);
    $(img1).css({
        'top':img1Y-10,
        'left':img1X,
        'z-index':100
    });
    $(img1).animate({
        opacity:'0',
        left:img1X,
    },500, function(){
        $(this).remove();
    });
});

$('.progress .img3').on('touchend',function () {
    $('.showMove img').css({
        'z-index':0
    });
});

//调用Android的退出app
$('.back').click(function(){
    if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
        window.location.href='goofypapa://back';
    }else{
        window.android.exitApp();
    }

})

