//调用ajax代码
//js获取url参数
function GetQueryString(name){
    var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if(r!=null)return  unescape(r[2]); return null;
}
//定义变量接收url参数
var myresourceId=GetQueryString("resourceId");

if(myresourceId!=null&&myresourceId.toString().length>0)
{
    console.log(myresourceId);
}

$.ajax({
    type:"post",
    url:"http://www.dadpat.com/resource/get.do",
    dataType:"jsonp",
    data:{"resourceId":myresourceId}, //以键/值对的形式
    async:true,
    success:function(data)
    {
        var datas=data.data;
        console.log(datas)
        //设置页面头部动物名称
       $("header p").html(datas.resourceTitle);
        //设置百科图标后动物名称和拼音
        $(".tabs>p>b").html(datas.resourceTitle);
        //设置动物百科内容
        $("article h2").html(datas.resourceWiki);

    },
    error:function(XMLResponse)
    {
        alert(XMLResponse.responseText);
    }
});
//横竖屏转换变字体大小

function orient() {
    if (window.orientation == 90 || window.orientation == -90) {
//ipad、iphone竖屏；Andriod横屏
        $("article h2").css("font-size","0.5rem");
        orientation = 'landscape';
        return false;

//$(mySwiper.wrapper[0]).addClass('my-class');
    }
    else if (window.orientation == 0 || window.orientation == 180) {
//ipad、iphone横屏；Andriod竖屏
        orientation = 'portrait';
        $("article h2").css("font-size","0.6rem");
        return false;

    }
}
//页面加载时调用
$(function(){
    orient();
});
//用户变化屏幕方向时调用
$(window).bind( 'orientationchange', function(e){
    orient();
});
//获取scrollTop值
var a2 = localStorage.name;
window.onload=function(){
    $(window).scrollTop(a2);
};

window.onload=function(){
    $(".playEnglish").click(function(){
        $(this).next("audio")[0].play();
    });
};


