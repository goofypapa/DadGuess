
//调用ajax代码
//js获取url参数
function GetQueryString(name) {
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if (r != null) return unescape(r[2]); return null;
}
//定义变量接收url参数
var srcI = GetQueryString("name");
console.log(srcI)

$('.back').click(function () {
    window.location.href = 'list.html';
});
var showPlanetName = showAll[srcI];
var imgName = imgAll[srcI]
//console.log(imgName)
// console.log(showPlanetName)
// console.log(showPlanetName[8].length)
$('.top').append('<img src="http://www.dadpat.com/app/astronomy/sign/' + showPlanetName[0] + '" class="topName">');
var addParameter = $($('#template1').html().replace('$picImg$', 'http://www.dadpat.com/app/astronomy/sign/' + showPlanetName[1]).replace('$showImg$', 'http://www.dadpat.com/app/astronomy/sign/' + showPlanetName[3]).replace('$p1$', showPlanetName[4]).replace('$p2$', showPlanetName[5]).replace('$p3$', showPlanetName[6]).replace('$p4$', showPlanetName[7]));
var interiorImg = $($('#template2').html().replace('$interiorImg$', 'http://www.dadpat.com/app/astronomy/sign/' + showPlanetName[8]));
var imgSuo = $($('#template5').html().replace('$imgShow1$', 'http://www.dadpat.com/app/astronomy/sign/' + imgName[3]).replace('$imgShow2$', 'http://www.dadpat.com/app/astronomy/sign/' + imgName[1]))
$('#introduce').append(addParameter);
$('#structure').append(interiorImg);
$('#swiper-container2 .swiper-wrapper').append(imgSuo);
if (showPlanetName.length < 2) {
    $('#introduce').remove();;
    $('#structure .structureLeft').remove();;
    $('#structure .structureRight').css('left', '5rem');
} else if (showPlanetName[8].length < 1) {
    $('#structure .structureLeft').remove();;
    $('#structure .structureRight').css('left', '5rem');
}
if (imgName.length < 4) {
    $('#swiper-container2 .swiper-slide:eq(0)').remove();
}
if (imgName.length == 0) {
    $('#swiper-container2 .swiper-slide:eq(0)').remove();
}

/*画饼图*/
var chart = Highcharts.chart('container', {
    chart: {
        height:'165px' ,
        backgroundColor: 'rgba(12,165,253,0.36)',
        plotBackgroundColor: null,  //:绘图区背景色
        plotBorderWidth: null,  //绘图区边框宽度
        plotShadow: false  //绘图区阴影
    },
    tooltip: {
        enabled: false //不启用提示框
    },
    exporting: {
        enabled: false  //去掉右上角的打印及导出按钮
    },
    credits: {
        enabled: false //不显示LOGO
    },
    title: {
        text: ' '
    },
    colors: ['#01517e', '#026096', '#0168a3', '#0089d7'],
    //数据列配置
    plotOptions: {
        //饼图
        pie: {
            //数据标签
            dataLabels: {
                enabled: true,//是否直接呈现数据 也就是外围显示数据与否
                distance: 0,//通过设置这个属性，将每个小饼图的显示名称和每个饼图重叠
                formatter: function (index) {
                    return '<b>' + this.point.name + '</b>:<span>' + this.point.y + '%' + '</span>';
                },
                style: {
                    color: '#ffffff',
                    fontWeight: 'lighter'
                },
            },
            borderColor: '#0ca3fb',
            states: {
                hover: {
                    enabled: false
                }
            },
        }
    },
    series: [{
        type: 'pie',
        size:'90%',
        innerSize: '50%',
        data: []
    }]
});
/*动态添加饼图数据*/
var pieresult = percent[srcI];
if (pieresult.length > 1) {
    var chart = $('#container').highcharts();
    chart.series[0].setData(pieresult);
} else {
    $('#structure .structureRight').remove();;
    $('#structure .structureLeft').css('left', '5rem');
    if ($('#structure')[0].children.length < 1) {
        $('#structure').remove();
    }
}



$.ajax({
    type: "post",
    url: "http://www.dadpat.com/file/json.do",
    dataType: "jsonp", //以键/值对的形式
    async: true,
    data: { path: 'app/astronomy', name: 'data' },
    success: function (data) {
        console.log(data.data[srcI])
        var datas = data.data[srcI];
        for (var i = 0; i < datas.length; i++) {
            var textAll = $($('#template3').html().replace('$contentImg$', 'http://www.dadpat.com/app/astronomy/' + srcI + '/big/' + datas[i].imgB).replace('$content$', datas[i].text).replace('$audio$', 'http://www.dadpat.com/app/astronomy/audio/' + datas[i].audioUrl));
            var textAll2 = $($('#template4').html().replace('$imgCont$', 'http://www.dadpat.com/app/astronomy/' + srcI + '/small/' + datas[i].imgS));
            $('#swiper-container1 .swiper-wrapper').append(textAll);
            $('#swiper-container2 .swiper-wrapper').append(textAll2);
        }
        var index = ""*1
        var that = this
        var mySwiper1 = new Swiper('#swiper-container1', {
            // loop: true,
            //前进后退按钮
            navigation: {
                nextEl: '.swiper-button-next',
                prevEl: '.swiper-button-prev',
            },
            centeredSlides: true,  //设定为true时，活动块会居中，而不是默认状态下的居左。
            on:{
                slideChangeTransitionStart:function(){
                    if(audio.length == 1){
                        audio[0].pause();
                        audio[0].load();
                    }
                },


                slideChange:function(){

                   console.log(that.index)

                    // if($('.swiper-slide-active audio').length==1){
                    //     $('.audioPlay')[0].src =  'img/audioPaused.png'
                    // }else{
                    //     $('.audioPlay')[0].src =''
                    // }
                },
                slideChangeTransitionEnd:function(){
                    // 改变音频按钮
                    if($('.swiper-slide-active audio').length==1){
                        console.log('------------------------------------')

                        $('.audioPlay')[0].style.display = 'block'
                        $('.audioPlay1')[0].style.display = 'none'
                    }else{
                        $('.audioPlay')[0].style.display = 'none'
                        $('.audioPlay1')[0].style.display = 'none'
                    }
                }
            }
        });

        that.index  = mySwiper1.realIndex

        if($('#swiper-container1 .swiper-slide-active audio').length == 1){
            $('.audioPlay')[0].style.display = 'block'

        }else{
            $('.audioPlay')[0].style.display = 'none'
        }
        

        var mySwiper2 = new Swiper('#swiper-container2', {
            centeredSlides: true,  //设定为true时，活动块会居中，而不是默认状态下的居左。
            slidesPerView: 'auto',
            initialSlide: 0,
            on: {
                slideChangeTransitionStart: function () {
                    $('#swiper-container2 .addImg').remove();
                },
                slideChangeTransitionEnd: function () {
                    var imgurl = $('#swiper-container2 .swiper-slide-active img').attr("src");
                    var index = imgurl.lastIndexOf("\/");
                    imgurl = imgurl.substring(index + 7, imgurl.length);
                    $('#swiper-container2 .swiper-slide-active').append('<img src="http://www.dadpat.com/app/astronomy/' + srcI + '/small/' + imgurl + '" class="addImg">');
                },
            },
        })
        mySwiper1.controller.control = mySwiper2;
        mySwiper2.controller.control = mySwiper1;
    }
});

window.onload = function () {
    if($('#swiper-container1 .swiper-slide-active audio').length == 1){
        $('.audioPlay')[0].style.display = 'block'

    }else{
        $('.audioPlay')[0].style.display = 'none'
    }
}

var audio = '';

$('.audioPlay').click(function () {
    var that = this
    this.style.display = 'none'
    $('.audioPlay1').css({
        display:'block'
    })


    // $('.audioPlay').css({
    //
    // })
    audio = $('.swiper-slide-active audio');
    if(audio[0].paused){
        audio[0].play();
    }else{
        audio[0].pause();
        $('.audioPlay1').css({
            display:'block'
        })
    }
    audio[0].loop = false;
    audio[0].addEventListener('ended', function () {
        $('.audioPlay1').css({
            display:'block'
        })
    }, false)
})

