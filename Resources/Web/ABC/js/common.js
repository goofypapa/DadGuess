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


