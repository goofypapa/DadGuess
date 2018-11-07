//js获取url参数
function GetQueryString(name){
    var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if(r!=null)return  unescape(r[2]); return null;
}
//定义变量接收url参数
var myresourceId=GetQueryString("resourceId");

if(myresourceId!=null&&myresourceId.toString().length>0){
    console.log(myresourceId);
}
$.ajax({
    type:"post",
    url:"http://www.dadpat.com/api/res/get.do",
    dataType:"jsonp",
    data:{"resId":myresourceId}, //以键/值对的形式
    // data:{"resId":'8e4011e7b93b3f959a25b9e19140706f'}, //以键/值对的形式
    async:true,
    success:function(data){
        var datas=data.data;
        console.log(datas);
        var windowRealWidth = document.documentElement.clientWidth;
        for(var i=0;i<datas.image.default.length;i++){
           $(".content").append('<div class="item"><a href="javascript:;"><img src="http://www.dadpat.com/resource/thumbnail/'+datas.image.default[i].attId+'.file"/" /></a></div>')
           $('#wrapper').append('<div class="swiper-slide" style="width: '+windowRealWidth+'"><img src="http://www.dadpat.com/resource/thumbnail/'+datas.image.default[i].attId+'.file"/></div>');
        }

        var num = datas.image.default.length;
        $(".item img").load(function() {
            num--;
            if (num > 0) {
                return;
            }
            console.log('load compeleted');
            $(".container").rowGrid({itemSelector: ".item", minMargin: 5, maxMargin: 10, firstItemClass: "first-item"});
        });

        var aBox = document.querySelectorAll('.container img');
        var swConAll = document.querySelector('.swiper-container');
        var swCon = document.querySelectorAll('.swiper-slide img');
        for(var j=0;j<aBox.length;j++){
            aBox[j].index = j;
            aBox[j].onclick=function(e){
                e.stopPropagation();
                var indexThis = this.index;
                $(swConAll).css('display','block');
                mySwiper.slideTo(indexThis+1, 1000, false);
                //设置：图片容器的高度跟图片的高度是一致的
                $('.swiper-slide').css('height',$(swCon[indexThis+1])[0].naturalHeight)
            };
            document.onclick=function(){
                $(swConAll).css('display','none');
            }
        }
        var mySwiper= new Swiper('.swiper-container',{
            direction: 'horizontal', //水平方向滑动
            loop: true,  //让Swiper看起来是循环的
            observer:true,//修改swiper自己或子元素时，自动初始化swiper
            observeParents:true,//修改swiper的父元素时，自动初始化swiper
            watchSlidesVisibility: true,
            on:{
                slideChangeTransitionStart: function () {
                    var swCon = document.querySelectorAll('.swiper-slide img');
                    var inde = this.activeIndex;
                    console.log(inde);
                    //设置：图片容器的高度跟图片的高度是一致的
                    $('.swiper-slide').css('height',$(swCon[inde])[0].naturalHeight)
                }
            }
        });

    }
});

/*检测屏幕横屏、竖屏*/
window.addEventListener("orientationchange", function() {
    if(window.orientation=="0"){
        $('.swiper-wrapper').css('marginTop','9.8rem');
    }else if(window.orientation=="90"){
        $('.swiper-wrapper').css('marginTop','3rem');
    }
}, false);
// 加载之后去除图片遮罩
window.onload=function () {
    $("#mask").fadeOut();
}
