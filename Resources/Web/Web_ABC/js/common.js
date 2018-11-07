/*控制页面的高度*/
document.documentElement.style.fontSize = innerWidth/16+'px';
onresize = function(){
    document.documentElement.style.fontSize = innerWidth/16+'px';
};

console.log(innerWidth)

/*每个页面最大的div的宽高*/
var docuWidth = document.documentElement.clientWidth;  //可见区域宽度
var docuHeight = document.documentElement.clientHeight; //可见区域高度
document.getElementById('dv').style.height = docuHeight +'px';
document.getElementById('dv').style.width = docuWidth +'px';


function playAudio( p_url )
{
    if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
        // window.location.href = "goofypapa://stopAllAudio;playAudio," + p_url;
        goofypapaStopAllAndPlayAudio( p_url, function(){
            mySwiper.slideNext();
            $("#shade").css("display","none");
        } );
    }else if( typeof( window.android ) != "undefined" ) {
        window.android.initMusic(p_url);
        window.android.startMusic();
    }else{
        // imgSibling.play();
        console.log( p_url );
    }
}


