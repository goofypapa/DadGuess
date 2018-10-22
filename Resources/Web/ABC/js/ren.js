//点击返回首页
$('.back').click(function(){
    window.location.href = 'index.html';
})
var arr = []; //存放的文件夹名(a,b,c,...)
var num = 0; //arr的索引
for(var itemRen in fileListRen){
    arr.push(itemRen);
}
var listRen = fileListRen[arr[num]];  //每个文件夹中图片的数据
var audioRen = audioList[arr[num]];  //每个文件夹中音频的数据
var arrRandom = []; //存放随机数的数组
//给当前页面赋值
for(var i=0;i<listRen.length;i++){
    $('.imgList').append($($('#template1').html().replace('$urlRen$','http://www.dadpat.com/app/ABC/ren/'+arr[num]+'/'+listRen[i]).replace('$classRen1$','dv'+listRen.length)));
    $('.audioAll').append($($('#template2').html().replace('$audio$','http://www.dadpat.com/app/ABC/audio/'+audioRen[i])));
    arrRandom[i]=i+1;
}
// 使用sort将原数组的顺序打乱，让有序变成无序
arrRandom.sort(function(){
  return Math.random() - 0.5;
});
var randVal = parseInt('');  //数组中索引为0的值
var deletOne = '';  //截取的数组中的第一个 
$('.laba').click(function(){
    randVal = arrRandom[0]-1;
    if($('audio')[randVal].paused){
        $('audio')[randVal].play(); 
    }else{
        $('audio')[randVal].pause();
    }
})
function chongFu(){
    $('.imgList>.dv'+listRen.length).click(function(){
        var thisIndex = $(this).index();
        if(thisIndex==randVal){
            $($(this)[0].children).addClass('animated pulse');
            var self = this;
            setTimeout(function(){
                $($(self)[0].children).removeClass("animated pulse")
            },1)
            if($(this)[0].children.length<2){
                $(this).append('<img src="image/3dui.png" class="'+'imgD'+listRen.length+'">');
                deletOne = arrRandom.shift(); 
                randVal = arrRandom[0]-1;
                if(arrRandom.length>0){
                    $('audio')[deletOne-1].pause();
                    setTimeout(function(){
                        $('audio')[randVal].play();
                    },1000)           
                }
                var duiLength = $('.imgD'+listRen.length).length; //对勾的长度
                if(duiLength == listRen.length){
                    $('audio')[deletOne-1].pause();
                    setTimeout(function(){
                        //移除所有动态加载的图片
                        $('.dv'+listRen.length).remove();
                        $('.audioAll audio').remove();
                        if(num == arr.length-1){
                            num = 0;
                        }else{
                            num++;
                        }
                        // 重新赋值
                        listRen = fileListRen[arr[num]];
                        audioRen = audioList[arr[num]];
                        for(var i=0;i<listRen.length;i++){
                            $('.imgList').append($($('#template1').html().replace('$urlRen$','http://www.dadpat.com/app/ABC/ren/'+arr[num]+'/'+listRen[i]).replace('$classRen1$','dv'+listRen.length)));
                            $('.audioAll').append($($('#template2').html().replace('$audio$','http://www.dadpat.com/app/ABC/audio/'+audioRen[i])))
                            arrRandom[i]=i+1;
                        }
                        arrRandom.sort(function(){
                          return Math.random() - 0.5;
                        });
                        chongFu();
                    },1500)
                    
                }
            }else{
                return false;
            }

        }else{
            console.log('不匹配')
            $($(this)[0].children).css('animation','mymove 0.5s linear');
            var self = this;
            setTimeout(function(){
                $($(self)[0].children).css("animation",'')
            },500)
            
        } 
    })
}
chongFu();