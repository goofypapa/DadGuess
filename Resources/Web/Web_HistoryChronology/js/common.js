<!--强制横屏-->
window.onresize = function force(){
       var docuWidth = document.documentElement.clientWidth;
       var docuHeight = document.documentElement.clientHeight;
    var dvObj = document.getElementById('dv');
    if(docuWidth>docuHeight){
        dvObj.style.width = docuWidth + 'px';
        dvObj.style.height = docuHeight + 'px';
        dvObj.style.transform = 'none';
        dvObj.style.marginTop =  '0px';
        dvObj.style.marginLeft = '0px';
        var width1 = '';
        var height1 = '';
        function random1() {
            width1 = Math.floor(Math.random() * docuWidth +1);
            height1 = Math.floor(Math.random() * docuHeight +1);
        }
        setInterval(random1, 1000);
    }else{
        dvObj.style.width = docuHeight + 'px';
        dvObj.style.height = docuWidth + 'px';
        dvObj.style.transform = 'rotate(90deg)';
        dvObj.style.marginTop = (docuHeight-docuWidth)/2 + 'px';
        dvObj.style.marginLeft = -(docuHeight-docuWidth)/2 + 'px';
    }
};


/*控制页面的高度*/
document.documentElement.style.fontSize = innerWidth/18+'px';
onresize = function(){
    document.documentElement.style.fontSize = innerWidth/18+'px';
};

/*每个页面最大的div的宽高*/
var docuWidth = document.documentElement.clientWidth;  //可见区域宽度
var docuHeight = document.documentElement.clientHeight; //可见区域高度
document.getElementById('dv').style.height = docuHeight +'px';
document.getElementById('dv').style.width = docuWidth +'px';

function audioPause() {
    document.querySelectorAll('audio').pause()
    window.android.log('执行了')
}
// window.android.setScreenOrientation(1)