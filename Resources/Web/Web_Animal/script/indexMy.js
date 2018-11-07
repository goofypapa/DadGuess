var docuWidth = document.documentElement.clientWidth;
var docuHeight = document.documentElement.clientHeight;
//js获取url参数
function GetQueryString(name) {
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if (r != null) return unescape(r[2]); return null;
}
var isAudoPlay = false;
window.onload = function()
{

    $('body').css({
        'height': docuHeight,
        'width': docuWidth,
        'background-color': '#272E38',
        'overflow': 'hidden'
    });

    var allOwnerId=["b50f11e6bddbe784523d7571e05bd41a","c7eb11e6bf1701d2cf56e9cd8d9bd25a","e1f811e68d9bbf6a4aa799bdde2479b0","fa4211e6ab8ea59afa157f6173462122","c99811e7b302bd97e4475a5f9f7f95bc","c99811e7b302bd97e4475a5fa49a0add"];
    var random = Math.floor(Math.random() * allOwnerId.length);
    random = random >= allOwnerId.length ? allOwnerId.length - 1 : random;
    var myownerId = allOwnerId[random];

    var t_getAnimalCount = 0;

    $.ajax({
        type: "post",
        url: "http://www.dadpat.com/api/res/list/summary.do",
        dataType: "jsonp",
        data: { "ownerId": myownerId }, //以键/值对的形式
        async: true,
        success: function (data) {
            var dataAnimal=data.data;
            var animalId=[];
            for(var i=0;i<dataAnimal.length;i++){
                animalId.push(dataAnimal[i].resourceId);
            }
            Array.prototype.shuffle = function() {
                var input = this;
                for (var i = input.length-1; i >=0; i--) {
                    var randomIndex = Math.floor(Math.random()*(i+1));
                    var itemAtIndex = input[randomIndex];
                    input[randomIndex] = input[i];
                    input[i] = itemAtIndex;
                }
                console.log(input) ;
            };
            animalId.shuffle();
            // var allId=[];
            t_getAnimalCount = animalId.length;
            for(var i=0;i<animalId.length;i++){
                $.ajax({
                    type: "post",
                    url: "http://www.dadpat.com/api/res/get.do",
                    dataType: "jsonp",
                    data: { "resId": animalId[i] }, //以键/值对的形式
                    async: true,
                    success: function (data) {
                        var indexAnimal = data.data;
                        var sourceWidth = 1080;
                        var sourceHeight = 1920;
                        // 随机一个声音

                        var audioRandom=Math.floor(Math.random()*(indexAnimal.audio.length));

                        var t_item = $('<div class="swiper-slide"><div class="animal" style="background: -webkit-linear-gradient(left top,' +
                        indexAnimal.colorBegin + ',' + indexAnimal.colorEnd + ')"id="a1"><a href="details.html?resourceId=' + indexAnimal.resourceId +
                        '"><img data-src="http://www.dadpat.com/' + indexAnimal.image.cover.attUrl +
                        '" class="swiper-lazy" alt=""/><div class="swiper-lazy-preloader"></div></a><div class="voice"><span id="'+indexAnimal.audio.length+'" style="display:none;" name="'+indexAnimal.resourceId+'">http://www.dadpat.com/'+indexAnimal.audio[audioRandom].attUrl +'</span><img  src="image/gif.gif" class="swiperes" alt=""/><a onclick="opendetails(' +
                        indexAnimal.resourceId + ')">' + indexAnimal.resourceTitle + '</a></div><p class="animalInfo">'+indexAnimal.simpleDesc+'</p></div></div>');
                    

                        console.log( $(t_item).find('.animal a img') );

                        $(t_item).find('.animal').css({
                            'width': Math.floor(0.73 * docuWidth),
                            'height': Math.floor(0.74 * docuHeight),
                            'margin': '0 auto',
                            'border-radius': '0.3rem',
                            'position': 'relative',
                            'margin-top': '3.5rem'
                        });
                        
                        $(t_item).find('.animal a img').css({
                            'position': 'absolute',
                            'max-width': Math.floor(874 / sourceWidth * docuWidth),
                            'max-height': Math.floor(760 / sourceHeight * docuHeight),
                            'display': 'block',
                            'left': Math.floor(-0.1 * 1* docuWidth),
                            'top': Math.floor(0.08 * 0.5 * docuHeight),
                            'cursor': 'pointer',
                            'padding-bottom': '8rem'
                        });
                        $(t_item).find('.animal .voice').css({
                            'position': 'absolute',
                            'right': Math.floor(0.04 * 0.73 * docuWidth),
                            // 'top': Math.floor(0.61 * 0.74 * docuHeight)
                            'bottom':'4.6rem'
                        });

                        $('#wrapper').append(t_item);
                        if( --t_getAnimalCount <= 0)
                        {
                            loadSuccess(indexAnimal);
                        }
                    },
                    error:function(){
                        if( --t_getAnimalCount <= 0)
                        {
                            loadSuccess(indexAnimal);
                        }
                    }
                });
            }
        }
    });

}

function myPlayAudio( p_url ) {
    if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
        if( isAudoPlay )
        {
            goofypapaStopAllAudio();
            $(".swiper-slide-active .voice>img").attr('src', 'image/voiced.png');
            isAudoPlay = false;
        }else{
            isAudoPlay = true;
            goofypapaStopAllAndPlayAudio( p_url, function(){
                $(".swiper-slide-active .voice>img").attr('src', 'image/voiced.png');
                isAudoPlay = false;
            } );
            $(".swiper-slide-active .voice>img").attr('src', 'image/gif.gif');
        }
    }else if( typeof( window.android ) != "undefined" ) {
            if(window.android.isMusicPlaying()){
            alert("111");
            window.android.pauseMusic();
            // $(this).attr('src', 'image/voiced.png');
        }else{
            isAudoPlay = true;
            // window.android.initMusic( p_url ,function playCompleteCallBack(){
            //     $(".voice>img").attr('src', 'image/voiced.png');
            //     isAudoPlay = false;
                // 播放完成得回调
            // } );
            window.android.startMusic();
            $(this).attr('src', 'image/gif.gif');
        }

        // window.android.initMusic( p_url );
        // window.android.startMusic();
    }else{
        console.log( "play audio: ", p_url );
    }
}

function goofypapaInit() {
    isAudoPlay = true;
    goofypapaStopAllAndPlayAudio( $(".swiper-slide-active .voice span")[0].innerHTML, function(){
        $(".swiper-slide-active .voice>img").attr('src', 'image/voiced.png');
        isAudoPlay = false;
    } );
}

function loadSuccess( indexAnimal ){
    var allAudio=$(".voice span");
    if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
        window.location.href = "goofypapa://playAudio," + allAudio[0].innerHTML;
    }else if( typeof( window.android ) != "undefined" ) {
        window.android.initMusic(allAudio[0].innerHTML,function playCompleteCallBack(){
            //播放完成得回调
        } );
        // window.android.initMusic(allAudio[0].innerHTML);
        window.android.startMusic();
    }else{
        console.log(allAudio[0].innerHTML);
    }
    var swiper = new Swiper('.swiper-container', {
        loop:true,
        lazy: {
            loadPrevNext: true  //设置为true允许将延迟加载应用到最接近的slide的图片（前一个和后一个slide）
        },
        on:{
            slideChangeTransitionEnd:function(){
                var randomA=Math.floor(Math.random()*(allAudio[this.realIndex].id));
                console.log(allAudio[this.realIndex].id);
                console.log(randomA);
                console.log(allAudio[this.realIndex]);
                var randomAnimalId=allAudio[this.realIndex].getAttribute("name") ;
                console.log(randomAnimalId);
                $.ajax({
                    type: "post",
                    url: "http://www.dadpat.com/api/res/get.do",
                    dataType: "jsonp",
                    data: { "resId": randomAnimalId }, //以键/值对的形式
                    async: true,
                    success: function (data) {
                        console.log(data.data);
                        console.log(randomA);
                        isAudoPlay = false;
                        myPlayAudio( allAudio[swiper.realIndex].innerHTML );
                    }
                });
            }
        }
    });


    $(".voice>img").click(function(){
        // $(this).attr('src', 'image/gif.gif');
        if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
            if( isAudoPlay )
            {
                goofypapaStopAllAudio();
                $(this).attr('src', 'image/voiced.png');
            }else{
                isAudoPlay = true;
                goofypapaStopAllAndPlayAudio( $(this).prev()[0].innerHTML, function(){
                    // t_playState2.innerHTML = "false";

                    $(".voice>img").attr('src', 'image/voiced.png');
                    isAudoPlay = false;
                } );
                // t_playState2.innerHTML = "true";
                $(this).attr('src', 'image/gif.gif');
            }
            // window.location.href = "goofypapa://stopAllAudio;playAudio," + $(this).prev()[0].innerHTML;
        }else if( typeof( window.android ) != "undefined" ) {
            // if(!window.android.isMusicPlaying()){
            //     window.android.initMusic($(this).prev()[0].innerHTML);
            //     window.android.startMusic();
            //     $(this).attr('src', 'image/gif.gif');
            //     // window.android.initMusic($(this).prev()[0].innerHTML,function playCompleteCallBack(){
            //     //     $(this).attr('src', 'image/voiced.png');
            //     // });
            // }else{
            //     window.android.pauseMusic();
            //     $(this).attr('src', 'image/voiced.png');
            // }
            // window.android.initMusic($(this).prev()[0].innerHTML);
            // window.android.startMusic();
        }else{
            console.log($(this).prev()[0].innerHTML);
        }
    });

}

//调用Android退出app
$('.back').click(function () {
    if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
        window.location.href='goofypapa://back';
    }else{
        window.android.exitApp()
    }
});


