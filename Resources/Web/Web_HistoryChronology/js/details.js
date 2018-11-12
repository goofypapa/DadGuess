/*点击返回时空隧道页*/
$('.back2').click(function(){
    // window.location.href="tunnel.html";
    // window.android.log('返回')
    window.history.go(-1)
});

/*3秒后将上下提示箭头隐藏*/
setTimeout(function(){
    $('.icon').css('display','none')
},3000);

var isplay = false;

//js获取url参数
function GetQueryString(name){
    var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if(r!=null)return  unescape(r[2]); return null;
}
var dynastyId = GetQueryString("dynastyId");



    // $.ajax({
    //     type: "post",
    //     url: "http://www.dadpat.com/dynasty/getAllList.do",
    //     dataType: "jsonp", //以键/值对的形式
    //     async: true,
    //     success: function (data) {
    //         // console.log(data.data);
    //         window.localStorage.setItem('mainData',JSON.stringify(data.data))
    //
    //         for( var i = 0; i < data.data.length; ++i )
    //         {
    //             var dyId1 = data.data[i].dynastyId;
    //             dataList.push([dyId1])
    //
    //             $.ajax({
    //                 type: "post",
    //                 url: "http://www.dadpat.com/dynastyInfo/getAllList.do",
    //                 dataType: "jsonp", //以键/值对的形式
    //                 async: true,
    //                 data: {"dynastyId": dyId1},
    //                 success: function (data) {
    //                     var chaodaiData = data.data;  //拿到各个朝代的data
    //
    //                     var isOver = true;
    //                     for( var i = 0; i < dataList.length; ++i )
    //                     {
    //                         if( chaodaiData[0].dynastyId === dataList[i][0] )
    //                         {
    //                             dataList[i].push(chaodaiData);
    //                         }
    //                         if( isOver && dataList[i].length === 1 )
    //                         {
    //                             isOver = false;
    //                         }
    //                     }
    //
    //                     if( isOver )
    //                     {
    //                         time += "," + (new Date().getTime());
    //                         // alert(time);
    //                         console.log( dataList );
    //
    //                         window.localStorage.setItem('dataList',JSON.stringify(dataList))
    //                     }
    //                 }
    //             });
    //         }
    //     }
    // });

    var index
    var index1

        //滑动
    var SwiperList = new Swiper("#swiper-container",{
        direction : 'horizontal',
        observer: true,
        observeParents:true,//修改swiper的父元素时，自动初始化swiper
        initialSlide: 0,
        navigation: {
            nextEl: '.swiper-button-next',
            prevEl: '.swiper-button-prev',
        },
        on:{
            slideChange: function () {
                // document.querySelectorAll('.audioplay').pause()
                // $(".audioplay").pause()
                $(".swiper-slide").find(".audioplay")[index].pause()
            },
            // slideChangeTransitionEnd:function () {
            //     $(".swiper-slide").find(".audioplay")[index1].play()
            // }

        }
    });

    $.ajax({
        type: "post",
        url: "http://www.dadpat.com/dynastyInfo/getAllList.do",
        dataType: "jsonp", //以键/值对的形式
        async: true,
        data:{"dynastyId": dynastyId },
        success: function (data) {
            var chaodaiData = data.data;  //拿到各个朝代的data

            console.log(chaodaiData, 'chaodaiData')

            for( var j = 0; j < chaodaiData.length; ++j ){
                var imgUrl = chaodaiData[j]["image"][0]["imageUrl"]
                var content = chaodaiData[j]["dynastyDescp"].split(";")[0]
                var audioUrl = chaodaiData[j]['audio'][0]['attUrl']
                var chaodaiName = chaodaiData[j]['dynastyName']
                var s = $("#template1").html().replace("$img$", "http://www.dadpat.com/" + imgUrl).replace("$chaodai$",chaodaiName).replace("$content$", content).replace("$audio$", "http://www.dadpat.com/" + audioUrl)
                $("#swiper1").append(s);
            }

            // setTimeout(function () {
            //     showAudio(dyId1)
            // })

            //p标签的滑动
            swiperP = new Swiper('#swiper1 .swiper-container', {
                direction: 'vertical',
                slidesPerView: 'auto',
                touchMoveStopPropagation : false,
                freeMode: true,
                mousewheel: true,
            });
        }
    });


/*点击按钮播放音频*/
$('.audio').click(function(){

    index = SwiperList.realIndex
    index1 = index+1

    if(!isplay){
        // playAudio[0].pause()
        // playAudio[0].play();
        // isplay = true
        $(".swiper-slide").find(".audioplay")[SwiperList.realIndex].pause()
        $(".swiper-slide").find(".audioplay")[SwiperList.realIndex].play()
        isplay = true
    }else{
        // playAudio[0].pause();
        // playAudio[0].load();
        // isplay = false
        $(".swiper-slide").find(".audioplay")[SwiperList.realIndex].pause()
        $(".swiper-slide").find(".audioplay")[SwiperList.realIndex].play()
        isplay = false
    }

});





