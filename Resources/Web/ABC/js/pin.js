//点击返回首页
$('.back').click(function () {
    window.location.href = 'index.html';
})

var template_box = "<div class=\"swiper-slide\"></div>";
var template_content = "<img src=\"$urlPin$\">\
<input type=\"hidden\" value=\"$audio$\" />";

// 音频文件
var audioAll = [];
console.log( fileListPin.big.length);
for(var index in audioList){
    audioAll = audioAll.concat(audioList[index])
}
var dataList = [];
var html = "";
var smallImgHtml = "";
for (var item in fileListPin) {
    var pinList = fileListPin[item];
    for (var i = 0; i < pinList.length; i++) {
        html += template_box;
        var imgUrl = 'http://www.dadpat.com/app/ABC/pin/' + item + '/' + pinList[i];
        var audioUrl = 'http://www.dadpat.com/app/ABC/audio/'+audioAll[i];
        smallImgHtml += "<div style=\"display:none;\"></div>";
        dataList.push( [imgUrl, audioUrl] );
    }
}
$('.swiper-container .swiper-wrapper').html(html);
$("#smallImg").html( smallImgHtml );

function arrayRemove(array, dx) {
    if (isNaN(dx) || dx > array.length) { return false; }
    for (var i = 0, n = 0; i < array.length; i++) {
        if (array[i] != array[dx]) {
            array[n++] = array[i]
        }
    }
    array.length = array.length - 1;
}

var imgDongTai = '';
var initIndex="";

if(localStorage.getItem("check")==""){
    initIndex=0;
}else{
    initIndex=localStorage.getItem("check");
}
console.log(initIndex);
//拼图
var puzzleImg = '';
var mySwiper = new Swiper('.swiper-container', {
    noSwiping: true,
    noSwipingSelector: 'div',
    navigation: {
        nextEl: '.swiper-button-next',
        prevEl: '.swiper-button-prev',
    },
    // lazy: true,
    on: {
        slideChangeTransitionStart: function () {
            
        },
        slideChangeTransitionEnd: function () {
            localStorage.setItem("check",mySwiper.activeIndex);
            lazyLoad();
        }
    }
})

var currGameIndex = -1;
function lazyLoad(){

    if( currGameIndex === mySwiper.activeIndex )
    {
        return;
    }

    var list = [];
    if( mySwiper.activeIndex > 0 )
    {
        list.push( mySwiper.activeIndex - 1 );
    }

    list.push( mySwiper.activeIndex );

    if( mySwiper.activeIndex < dataList.length )
    {
        list.push( mySwiper.activeIndex + 1 );
    }

    for( var i = 0; i < list.length; ++i )
    {
        var t_index = list[i];
        var t_slide = $(".swiper-slide:eq(" + t_index +  ")");

        if( t_slide.html().length === 0 )
        {
            t_slide.html( template_content.replace('$urlPin$', dataList[ t_index ][0]).replace('$audio$',dataList[ t_index ][1]) );
        }

        var smallImgDiv = $("#smallImg div:eq(" + t_index +  ")");
        if( smallImgDiv.html().length === 0 )
        {
            smallImgDiv.html( "<img src=\"" + dataList[ t_index ][0] +  "\" />" )
        }

        console.log( t_slide.html() );
    }

    $("#smallImg div").hide();
    $("#smallImg div:eq(" + mySwiper.activeIndex +  ")").show();
    
    var t_img = $('.swiper-slide-active img')[0];
    if( t_img.complete )
    {
        pinTuGame( mySwiper.activeIndex );
    }else{
        t_img.onload = function(){
            pinTuGame( mySwiper.activeIndex );
        }
    }
}


mySwiper.slideTo(initIndex);
lazyLoad();


//=============================================================================================================================

// 拼图游戏
var imgSibling = '';
function pinTuGame( p_index ) {

    $('.pinTu div').remove();

    $('.smallImg img').attr('src', dataList[ p_index ][0]);

    var puzzleImg = $('.swiper-slide-active img');
    imgSibling = puzzleImg[0].nextElementSibling;
    
    var imgSrc = puzzleImg[0].src;
    console.log(puzzleImg[0]);
    console.log(puzzleImg[0].height);
    //图片整体的宽高
    var imgWidth = parseInt(puzzleImg[0].height);
    var imgHeight = parseInt(puzzleImg[0].height);
    //3*3排列
    var num = 3;
    //每一块碎片的宽高
    var cellWidth = parseInt(imgWidth / num);
    var cellHeight = parseInt(imgHeight / num);
    var correctArr = [];  //正确的数组
    var errorArr = [];  //打乱的数组
    var windowSize = { width: $(window).width(), height: $(window).height() };  //屏幕的宽高
    var offset = { x: windowSize.width * 0.292, y: windowSize.height * 0.13 };  //拼图的位置
    // $('#imgArea').css( { "top": offset.y + "px", "left": offset.x + "px" } );
    for (var i = 0; i < num; i++) {
        for (var j = 0; j < num; j++) {
            //将碎片所属div的下标存入数组，用于最终校验是否排序完成
            correctArr.push(i * num + j);
            cell = document.createElement("div");
            cell.className = "imgCell";
            $(cell).css({
                'width': (cellWidth - 2) + 'px',
                'height': (cellHeight - 2) + 'px',
                'left': offset.x + j * cellWidth + 'px',
                'top': offset.y + i * cellHeight + 'px',
                "background": "url('" + imgSrc + "')",
                'backgroundPosition': (-j) * cellWidth + 'px ' + (-i) * cellHeight + 'px',
                "backgroundSize": imgWidth + "px " + imgHeight + "px",
            });
            // $(cell)[0].setAttribute('data-index', i * num + j)
            $('.pinTu').append(cell);
        }
    }

    var randomPool = [];
    function randomArea() {
        errorArr = [];
        randomPool = [];
        for (var item in correctArr) {
            randomPool.push(correctArr[item]);
        }

        for (var key in correctArr) {
            var ranIndex, ranNumber;

            do {
                ranIndex = Math.floor(Math.random(new Date().getTime()) * randomPool.length);
                ranIndex = ranIndex === randomPool.length ? ranIndex - 1 : ranIndex;

                ranNumber = randomPool[ranIndex];

            } while (ranNumber === correctArr[key] && randomPool.length > 1)

            arrayRemove(randomPool, ranIndex);

            errorArr[key] = ranNumber;
        }
    }


    function init() {
        // 初始化内容 
        touch();
    }
    //开关
    var self = 0;
    $('.imgCell').click(function () {
        // console.log(self)
        if (self == 0) {
            touch();
            replace();
            // init();
            self = 1;
        }
    })
    var imgCell = document.querySelectorAll('.imgCell');
    //打乱图片
    function replace() {
        for (var key in errorArr) {
            $(imgCell[errorArr[key]]).animate({
                left: offset.x + key % 3 * cellWidth + 'px',
                top: offset.y + Math.floor(key / 3) * cellHeight + 'px',
            }, 500, "", function () {

            });
            // imgCell
        }
    }
    function touch() {
        randomArea();
        var img = null;
        $('.imgCell').on('touchstart', function (e) {
            //缓存起点位置

            if ( img ) {
                return;
            }
            
            img = this;
        })
        $('.imgCell').on('touchmove', function (e) {

            if (img !== this ) return;
            this.style["z-index"] = 1000;
            var sizeHalf = { width: $(this).width() * 0.5, height: $(this).height() * 0.5 };
            var position = { x: e.targetTouches[0].pageX - sizeHalf.width, y: e.targetTouches[0].pageY - sizeHalf.height };
            if (position.x > windowSize.width - sizeHalf.width) {
                position.x = windowSize.width - sizeHalf.width;
            }
            if (position.x < 0) {
                position.x = 0;
            }
            if (position.y > windowSize.height - sizeHalf.height) {
                position.y = windowSize.height - sizeHalf.height;
            }
            if (position.y < 0) {
                position.y = 0;
            }

            this.style.left = position.x + "px";
            this.style.top = position.y + "px";
        })
        $('.imgCell').on('touchend', function (e) {

            if  (img !== this ) return;

            var x = e.changedTouches[0].pageX - offset.x;
            var y = e.changedTouches[0].pageY - offset.y;

            var from = -1;
            var to = -1;

            for (var i = 0; i < 9; ++i) {
                var beginX = i % 3 * cellWidth;
                var beginY = Math.floor(i / 3) * cellHeight;

                if (imgCell[errorArr[i]] === this) {
                    from = i;
                }

                if (x > beginX && x < beginX + cellWidth && y > beginY && y < beginY + cellHeight) {
                    to = i;
                }
            }

            if (to < 0) {
                to = from;
            }

            imgCell[errorArr[to]].style["z-index"] = 999;

            $(imgCell[errorArr[from]]).animate({
                left: offset.x + to % 3 * cellWidth + 'px',
                top: offset.y + Math.floor(to / 3) * cellHeight + 'px',
            }, 500, "", function () {
                this.style["z-index"] = 10;
            });

            if (to !== from) {
                $(imgCell[errorArr[to]]).animate({
                    left: offset.x + from % 3 * cellWidth + 'px',
                    top: offset.y + Math.floor(from / 3) * cellHeight + 'px',
                }, 500, "", function () {
                    this.style["z-index"] = 10;
                });
            }

            if (from !== to)
            {
                swtichIndex(from, to);
            }  
            img = null;
        })
    }
    // //交换两个下标
    function swtichIndex(oriIndex, targetIndex) {
        var tmp = errorArr[oriIndex];
        errorArr[oriIndex] = errorArr[targetIndex];
        errorArr[targetIndex] = tmp;

        var isOk = true;
        for (var i = 0; i < errorArr.length - 1; ++i) {
            if (errorArr[i] > errorArr[i + 1]) {
                isOk = false;
                break;
            }
        }
        if (isOk) {
            self = 0;
            $("#shade").css("display","block");

            playAudio( imgSibling.value );

            var nextTime=Math.floor(imgSibling.duration* 1000)/1000;
            nextTime=nextTime*1000;
            setTimeout(function(){
                mySwiper.slideNext();
                $("#shade").css("display","none");
            },nextTime);
        }
    }
}
//=============================================================================================================================
