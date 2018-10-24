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

//js获取url参数
function GetQueryString(name){
    var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if(r!=null)return  unescape(r[2]); return null;
}
var dynastyId = GetQueryString("dynastyId");

var dynastyList;  //朝代列表
var mySwiper;  //左右滑动
var swiperP; //P标签滑动
var isplay = false;
var shouSwiperIndex = 0;  //朝代的索引
var SwiperList = {};  //将所有的朝代按朝代ID值存储到一个对象中
var sonIndex = 0;  //左右滑动时准确的拿到所滑到的朝代当先显示的内容的索引
var playAudio = null;  //正在播放的音频
var playEtext = null;  //正在播放的音频所对应的内容
var swiperAllIn = []; //所有的朝代内容
var swiperAll = [];  //所有的朝代内容分别对应的歌词
var samllIndex = 0;  //上下滑动时朝代中内容的索引
var _processK; //调用卡拉ok效果
// window.android.setScreenOrientation(1)
// window.android.pauseMusic()
var that = this

function showAudio(index) {
    console.log(shouSwiperIndex,sonIndex)
    sonIndex = index;
    var _audio = $("#swiper-container .swiperIndex:eq(" + shouSwiperIndex +  ") .swiperWrapperIndex>div:eq(" + sonIndex +  ") .audio");

    var text = _audio.text();
    console.log(text)
    playAudio = text;
}

$(function(){
    $.ajax({
        type:"post",
        url:"http://www.dadpat.com/dynasty/getAllList.do",
        dataType:"jsonp", //以键/值对的形式
        async:true,
        success:function(data){
            console.log(data.data);
            dynastyList = data.data;
            var chaodaiList = {};

            for( var i = 0; i < dynastyList.length; ++i ){
                var swiper1ID = "swiper1" + dynastyList[i]["dynastyId"];
                var swiper2ID = "swiper2" + dynastyList[i]["dynastyId"];
                var chaoDaiName = '';
                /*朝代名字*/
                if(dynastyList[i]["dynastyName"].length>1){
                    chaoDaiName = dynastyList[i]["dynastyName"];
                }else{
                    chaoDaiName=dynastyList[i]["dynastyName"]+"朝"
                }
                if( dynastyId == dynastyList[i]["dynastyId"] ){
                    shouSwiperIndex = i;
                }
                /*添加朝代名字、分别保存图片和内容的swiper的ID*/
                chaodaiList[dynastyList[i]["dynastyId"]] = $($("#template").html().replace("$chaodai$", chaoDaiName).replace("$id1$", swiper1ID ).replace("$id2$", swiper2ID));
                $(".swiper1").append( chaodaiList[dynastyList[i]["dynastyId"]] );

                //获取session中的值
                var initIndex = '';
                if(dynastyList[shouSwiperIndex]["dynastyId"] == dynastyId){
                    initIndex = localStorage.getItem(dynastyId)
                }else{
                    initIndex = localStorage.getItem(dynastyList[i]["dynastyId"] );
                }

                
                $.ajax({
                    type: "post",
                    url: "http://www.dadpat.com/dynastyInfo/getAllList.do",
                    dataType: "jsonp", //以键/值对的形式
                    async: true,
                    data:{"dynastyId": dynastyList[i]["dynastyId"] },
                    success: function (data) {        
                        var chaodaiData = data.data;  //拿到各个朝代的data
                       
                        var dynastyId = chaodaiData[0]["dynastyId"]; //拿到各个朝代的ID
                        var chaoDaiIndex = -1;
                        for( var n = 0; n < dynastyList.length; ++n ){
                            // console.log('各个朝代的ID:'+dynastyId+',当前朝代的ID'+dynastyList[n]["dynastyId"])
                            if( dynastyId == dynastyList[n]["dynastyId"] ){
                                chaoDaiIndex = n;
                                break;
                            }
                        }
                        if( chaoDaiIndex < 0 ){
                            return;
                        }
                        
                        for( var j = 0; j < chaodaiData.length; ++j ){
                            if(chaodaiData[j]["image"][0]["imageUrl"] == 'undefined'){
                                console.log(112)
                            }else{
                                chaodaiList[dynastyList[chaoDaiIndex]["dynastyId"]].find("#swiper1" + dynastyList[chaoDaiIndex]["dynastyId"] + ">.swiper-wrapper").append($("#template1").html().replace("$img$","http://www.dadpat.com/"+chaodaiData[j]["image"][0]["imageUrl"]).replace("$content$", chaodaiData[j]["dynastyDescp"].split(";")[0]).replace("$audio$","http://www.dadpat.com/"+chaodaiData[j]['audio'][0]['attUrl']));
                            }
                        }
                        //点击朝代进入时，点击音频按钮，播放当前显示的语音
                        if( shouSwiperIndex == chaoDaiIndex ) {        
                            showAudio(initIndex != null && shouSwiperIndex == chaoDaiIndex ? initIndex : 0);
                                             
                        }

                        //p标签的滑动
                        swiperP = new Swiper('#swiper1'+ dynastyId +' .swiper-container', {
                            direction: 'vertical',
                            slidesPerView: 'auto',
                            touchMoveStopPropagation : false,
                            freeMode: true,
                            mousewheel: true,
                        });
                    }
                })  ;
                //上下滑动
                SwiperList[dynastyList[i]["dynastyId"]] = new Swiper('#' + swiper1ID,{
                    direction : 'vertical',
                    observer: true,//修改swiper自己或子元素时，自动初始化swiper
                    // observeParents:true,
                    initialSlide: initIndex != null ? initIndex : 0,
                    // lazy: {
                    //     loadPrevNext: true,
                    //     loadPrevNextAmount: 4
                    // },
                    on:{
                        slideChangeTransitionStart: function () {
                            //每个朝代都会存储


                            if(dynastyList[shouSwiperIndex]["dynastyId"]){
                                localStorage.setItem( dynastyList[shouSwiperIndex]["dynastyId"], this.activeIndex );
                            }



                            window.location.href='goofypapa://stopAllAudio';
                            // if(isplay){
                            //     window.location.href='goofypapa://stopAllAudio';
                            //     // playAudio[0].pause();
                            //     console.log(playAudio, '上下')
                            //     // playAudio[0].load();
                            //
                            //     // window.location.href = "goofypapa://loadAudio," + playAudio[0].src;
                            //     isplay = false
                            // }


                            samllIndex = this.activeIndex;
                            showAudio(samllIndex);


                            // $(".swiper-slide").find(".audio")[samllIndex].pause()
                            // console.log( $(".swiperWrapperIndex"))

                        },
                        slideChangeTransitionEnd: function () {

                        }
                    }
                });
              console.log(  (SwiperList[dynastyList[i]["dynastyId"]]).realIndex)
            }
            /*左右滑动*/
            mySwiper = new Swiper('#swiper-container', {
                direction : 'horizontal',
                observer: true,
                navigation: {
                    nextEl: '.swiper-button-next',
                    prevEl: '.swiper-button-prev'
                },
                
                on:{
                   
                    slideChangeTransitionStart:function(){

                        window.location.href='goofypapa://stopAllAudio';
                        // if(isplay){
                        //     // playAudio[0].pause();
                        //     // playAudio[0].load();
                        //     // playAudio[0].src
                        //     window.location.href='goofypapa://stopAllAudio';
                        //     // window.location.href = "goofypapa://loadAudio," + playAudio[0].src;
                        //     isplay = false
                        // }


                        shouSwiperIndex = this.activeIndex;
                        sonIndex = SwiperList[dynastyList[shouSwiperIndex]["dynastyId"]].activeIndex;   //上下滑动的索引

                        showAudio(sonIndex);
                        // $(".swiper-container").find(".swiper-slide")[this.activeIndex].find("audio").pause()

                    },
                    slideChangeTransitionEnd: function () {

                    }
                }
            });


            mySwiper.slideTo( shouSwiperIndex, 200, false );


        }
    });
    
});





/*点击按钮播放音频*/
$('.audio').click(function(){
    // window.android.log('音频')
    console.log(playAudio,typeof playAudio)
    window.location.href='goofypapa://stopAllAudio;playAudio,'+playAudio;

    // if(!isplay){
    //     // playAudio[0].pause();
    //     // playAudio[0].play();
    //     window.location.href='goofypapa://stopAllAudio;playAudio,'+playAudio;
    //     // window.location.href = "goofypapa://playAudio," +playAudio;
    //     isplay = true
    // }else{
    //     window.location.href='goofypapa://stopAllAudio;playAudio,'+playAudio;
    //     // window.location.href = "goofypapa://loadAudio," + playAudio;
    //     console.log(playAudio)
    //     // playAudio[0].pause();
    //     // playAudio[0].load();
    //     isplay = false
    // }

});







