// console.log(audioList)
// var audioAll = [];
// for(var item in audioList){
//     audioAll = audioAll.concat(audioList[item])
// }
// console.log(audioAll)
//动态获取图片
var fileListPin = {big:["04pin_a.png", "04pin_a_alice.png", "04pin_a_apple.png", "04pin_a_juzi.png", "04pin_b.png", "04pin_b_bike.png", "04pin_b_brother.png","04pin_b_blue.png", "04pin_b_juzi.png", "04pin_c.png", "04pin_c_cake.png", "04pin_c_cici.png", "04pin_c_coffee.png", "04pin_c_juzi.png", "04pin_d.png", "04pin_d_dad.png", "04pin_d_dog.png", "04pin_d_door.png", "04pin_d_juzi.png", "04pin_e.png", "04pin_e_ear.png", "04pin_e_earings.png", "04pin_e_emily.png", "04pin_e_juzi.png", "04pin_f.png", "04pin_f_fish.png", "04pin_f_food.png", "04pin_f_frank.png", "04pin_f_juzi.png", "04pin_g.png", "04pin_g_grandma.png", "04pin_g_grandpa.png", "04pin_g_grape.png", "04pin_g_juzi.png", "04pin_h.png", "04pin_h_harry.png", "04pin_h_horse.png", "04pin_h_house.png", "04pin_h_juzi.png", "04pin_i.png", "04pin_icecream.png", "04pin_i_insect.png", "04pin_i_juzi.png", "04pin_j.png", "04pin_j_jim.png", "04pin_j_juice.png", "04pin_j_juzi.png", "04pin_k.png", "04pin_k_kangaroo.png", "04pin_k_kid.png", "04pin_k_juzi.png", "04pin_l.png", "04pin_l_lamp.png", "04pin_l_lucy.png", "04pin_l_juzi.png", "04pin_m.png", "04pin_m_mom.png", "04pin_m_music.png", "04pin_m_juzi.png", "04pin_n.png", "04pin_n_nora.png","04pin_n_number.png", "04pin_n_nine.png", "04pin_n_juzi.png", "04pin_o.png", "04pin_o_old.png", "04pin_o_on.png","04pin_o_owl.png", "04pin_o_juzi.png", "04pin_p.png", "04pin_p_parent.png", "04pin_p_park.png", "04pin_p_piano.png", "04pin_p_juzi.png", "04pin_q.png", "04pin_q_queen.png", "04pin_q_quilt.png", "04pin_q_qxu.png", "04pin_q_juzi.png", "04pin_r.png", "04pin_r_raft.png", "04pin_r_ride.png","04pin_r_rose.png", "04pin_r_juzi.png", "04pin_s.png", "04pin_s_sand.png", "04pin_s_sleep.png", "04pin_s_sunny.png", "04pin_s_juzi.png", "04pin_t.png", "04pin_t_teenis.png", "04pin_t_twins.png", "04pin_t_juzi.png", "04pin_u.png", "04pin_u_umbrella.png","04pin_u_up.png" ,"04pin_u_juzi.png", "04pin_v.png", "04pin_v_vegetable.png", "04pin_v_vicky.png", "04pin_v_juzi.png", "04pin_w.png", "04pin_w_walus.png", "04pin_w_wave.png", "04pin_w_juzi.png", "04pin_x.png", "04pin_x_max.png", "04pin_x_six.png", "04pin_x_juzi.png", "04pin_y.png", "04pin_y_young.png", "04pin_y_yourt.png", "04pin_y_juzi.png", "04pin_z.png", "04pin_z_zebra.png", "04pin_z_zoo.png", "04pin_z_juzi.png"] }
console.log(fileListPin);
for( var item in fileListTing ){  //fileList文件列表
    var imgList = fileListTing[item];  //图片列表
    // console.log(imgList)
    // console.log(audioList[item])
    var vice_audioList = audioList[item];   //音频列表
    var listP = pList[item];
    var letterList = {};
    for(var i=0;i<imgList.length;i++){
        var str = imgList[i];
        // console.log(str)
        // console.log(vice_audioList[i])
        var className = str.substring(0,str.length-4);  //截取文件名获取类名
        letterList[i] = $($('#template').html().replace('$url$','http://www.dadpat.com/app/ABC/ting/'+item+'/'+str).replace('$class$',className).replace('$audio$','http://www.dadpat.com/app/ABC/audio/'+vice_audioList[i]).replace('$content$',listP[i]));
        $('#swiper-container1 .swiper-wrapper').append(letterList[i])
    }
}
for(var item2 in fileListPin){
    var pinList = fileListPin[item2];
    for(var j=0;j<pinList.length;j++){
        $('#swiper-container2 .swiper-wrapper').append($($('#template2').html().replace('$url2$','http://www.dadpat.com/app/ABC/pin/big/'+pinList[j])))
    }
}

//点击返回首页
$('.back').click(function(){
    window.location.href = 'index.html';
})

//点击让卡片显示、隐藏
$('.xiaLa img').click(function(oEvent){
    if(document.all){
        oEvent.cancelBubble = true;
    }else{
        oEvent.stopPropagation();
    }
    this.src = 'image/2shangla.png';
    $('.kpList').css('display','block');
    //点击卡片跳转到对应的页面
    $('.kpList .swiper-slide').click(function(oEvent){
        var to = $(this).index();
        swiper1.slideTo(to);
        oEvent = oEvent || window.event;
        if(document.all){
            oEvent.cancelBubble = true;
        }else{
            oEvent.stopPropagation();
        }
    })
});
document.onclick = function(){
    $(".xiaLa img").attr('src','image/2xiala.png'); 
    //隐藏卡片列表
    $('.kpList').css('display','none');
    
};

// 卡片
var swiper2 = new Swiper('#swiper-container2', {
    spaceBetween: 30,  //在slide之间设置距离（单位px）
    slidesPerView: 'auto',  //设置slider容器能够同时显示的slides数量(carousel模式)。
    //touchRatio: 0.2,  //触摸比例。触摸距离与slide滑动距离的比率。
    observer:true,//修改swiper自己或子元素时，自动初始化swiper
    observeParents:true,//修改swiper的父元素时，自动初始化swiper
    on:{
        slideChangeTransitionStart:function(){

        },
        slideChangeTransitionEnd: function () {

        }
    },
    lazy: {
        loadPrevNext: true,
        loadPrevNextAmount: 10,  //设置在延迟加载图片时提前多少个slide。
    },
});
//黑板
var swiper1 = new Swiper('#swiper-container1', {
    navigation: {
        nextEl: '.swiper-button-next',
        prevEl: '.swiper-button-prev',
    },
    pagination: {
        el: '.swiper-pagination',
        type: 'progressbar',
    },
    scrollbar: {
        el: '.swiper-scrollbar',
        hide: true,
    },
    lazy: {
        loadPrevNext: true,
    },
    on:{
        slideChangeTransitionStart:function(){

            // 播放当前页面音频文件
            $('.swiper-slide-active audio')[0].play();
            console.log("11");
            // 获取当前图片的transform值
            var transForm = $('#swiper-container1 .swiper-scrollbar-drag')[0].style.transform;
            // 获取当前图片的transform的X值
            var moveTransFormX = parseInt(transForm.slice(12,-13));
            //动态给星星赋值
            $('.moveXX').css('left',moveTransFormX +'px')
            $('.progress p').css('left',moveTransFormX +'px')
            //带格子的进度条的宽度
            var widthAll = $('.slide')[0].width;
            $('#swiper-container1 .swiper-pagination').css('width',(widthAll - moveTransFormX) +'px')
            // 星星上的P标签
            $('.progress p')[0].innerText = $('.swiper-slide-active p')[0].innerText;

            $(".xiaLa img").attr('src','image/2xiala.png'); 
            //隐藏卡片列表
            $('.kpList').css('display','none');


            if(audio){
                audio.pause();
                audio.load();
            }

        },
        slideChangeTransitionEnd: function () {
            $('.swiper-slide-prev audio')[0].pause();
            $('.swiper-slide-prev audio')[0].currentTime=0;
            $('.swiper-slide-next audio')[0].pause();
            $('.swiper-slide-next audio')[0].currentTime=0;
        },
        click: function(){
            // console.log(swiper1.activeIndex);
        },
    }

});

var audio = '';
$('.live').click(function(){
    audio = $('.swiper-slide-active audio')[0]; 
    if(audio.paused){
        audio.play();
    }else{
        audio.pause();
        audio.load();

    }
})