//点击返回首页
$('.back').click(function () {
    window.location.href = 'index.html';
})
// 音频文件
var audioAll = [];
console.log( fileListPin.big.length);
for(var index in audioList){
    audioAll = audioAll.concat(audioList[index])
}

for (var item in fileListPin) {
    var pinList = fileListPin[item];
    for (var i = 0; i < pinList.length; i++) {
        var imgAll = $($('#template').html().replace('$urlPin$', 'http://www.dadpat.com/app/ABC/pin/' + item + '/' + pinList[i]).replace('$audio$','http://www.dadpat.com/app/ABC/audio/'+audioAll[i]))
        $('.swiper-container .swiper-wrapper').append(imgAll)
    }
}


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
var pg = '';
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
    lazy: {
        loadPrevNext: true,
        loadPrevNextAmount: 2,
        loadOnTransitionStart: true,
    },
    on: {
        slideChangeTransitionStart: function () {
            localStorage.setItem("check",mySwiper.activeIndex);
            //反转页面时存储
            console.log(localStorage.getItem("check"));
            pg.hasStart = 0;
            imgDongTai = $('.swiper-slide-active img').attr("src")  //动态获取图片的src，给缩略图赋值
            audioDongTai = imgDongTai.substring(0, imgDongTai.length - 4);  //动态获取图片的src，拿相对于的音频文件
            puzzleImg = $('.swiper-slide-active img');
            $(function () {
                $('.pinTu div').remove();

                // 音乐暂停
                if(imgSibling){
                    imgSibling.pause();
                    imgSibling.load();
                }
                pinTuGame(puzzleImg);
                
               
            });
            // 右上角缩图
            $('.smallImg img').attr('src', imgDongTai)

        },
        slideChangeTransitionEnd: function () {

        }
    }
})
mySwiper.slideTo(initIndex);

//=============================================================================================================================

// 拼图游戏
var imgSibling = '';
pinTuGame($('.swiper-slide-active img'));
function pinTuGame(puzzleImg) {
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

            // if (img) {
            //     var index = -1;

            //     for (var i = 0; i < imgCell.length; ++i) {
            //         if ( imgCell[i] === img ) {
            //             index = i;
            //             break;
            //         }
            //     }

            //     $(img).animate({
            //         left: offset.x + errorArr.indexOf(index) % 3 * cellWidth + 'px',
            //         top: offset.y + Math.floor(errorArr.indexOf(index) / 3) * cellHeight + 'px',
            //     }, 500, "", function () {

            //     });
            // }
            
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
        console.log(imgSibling.src);
        if (isOk) {
            self = 0;
            console.log("游戏结束",imgSibling.src);
            $("#shade").css("display","block");
            // imgSibling.play();
            // if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
            //     window.location.href = "goofypapa://playAudio," + imgSibling.src;
            // }else{
            //     window.android.initMusic(imgSibling.src);
            //     window.android.startMusic();
            // }
            if( typeof( goofypapaGame ) != "undefined" && goofypapaGame ){
                window.location.href = "goofypapa://playAudio," + imgSibling.src;
            }else if( typeof( window.android ) != "undefined" ) {
                window.android.initMusic(imgSibling.src);
                window.android.startMusic();
            }else{
                imgSibling.play();
            }
            var nextTime=Math.floor(imgSibling.duration* 1000)/1000;
            nextTime=nextTime*1000;
            setTimeout(function(){
                mySwiper.slideNext();
                $("#shade").css("display","none");
            },nextTime);
            // if(imgSibling.paused){
            //     imgSibling.play();
            // }else{
            //     imgSibling.pause();
            //     imgSibling.load();
            // }
        }
    }
}
//=============================================================================================================================
