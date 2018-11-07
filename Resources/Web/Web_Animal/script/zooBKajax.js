 /*图片随机加载*/
 // for(var i=1;i<6;i++){
 //     $('.swiper-wrapper').append('<div class="swiper-slide"><img style="width: 100%;height: 100%" src="local/'+i+'.png" alt=""/></div>')
 // }

 var mySwiper = new Swiper('#header', {
     loop: true,  //让Swiper看起来是循环的
     autoplay: {
         delay:3000
     },//可选选项，自动滑动
     pagination: {
         el: '.swiper-pagination',
         type: 'fraction',
     },
     observer:true,//修改swiper自己或子元素时，自动初始化swiper
     observeParents:true,//修改swiper的父元素时，自动初始化swiper
 });

//最新上传接口
$.ajax({
    type:"get",
    url:"http://www.dadpat.com/api/res/batch/list/top.do",
    dataType:"jsonp",
    async:true,
    success:function(data){
        var datas=data.data;
        console.log(datas)
        for(var i=0;i<datas.length;i++){
            $(".pics").append('<li style="margin: 0 0.3rem 0 0.3rem"><a><img src="http://www.dadpat.com/'+datas[i].coverImage+'" alt=""/></a><span><p>'+datas[i].batchSource+'</p></span></li>')
        }
        $(".pics li").click(function(){
            var index = $(this).index();
             window.location.href="list.html?ownerId="+datas[index].batchId+"&source="+datas[index].batchSource;
             console.log(datas[index].batchSource)
        })
    }
});

//最受欢迎接口
$.ajax({
    type:"get",
    url:"http://www.dadpat.com/api/res/list/mostPopular.do",
    dataType:"jsonp",
    data:{"top":9},
    async:true,
    success:function(data){
        var datas=data.data;
        console.log(datas);
        /*简单粗暴的做法*/
        /*for(var i=0;i<datas.length;i++){
            $("#pics").append('<li style="margin: 0 0.25rem 1rem 0.2rem;"><a><img src="http://www.dadpat.com/'+datas[i]["image-2"]+'" alt=""/><p>'+datas[0].total+'次播放</p></a></li>')
        }*/

        //原先的写法
        $("#pics").append('<li><a><img src="http://www.dadpat.com/'+datas[0]["image-1"]+
            '" alt=""/><p>'+datas[0].total+'次播放</p></a></li><li style="margin: 0 0.68rem 0 0.68rem"><a><img src="http://www.dadpat.com/'+
            datas[1]["image-2"]+'" alt=""/><p>'+datas[1].total+'次播放</p></a></li><li><a><img src="http://www.dadpat.com/'+
            datas[2]["image-2"]+'" alt=""/><p>'+datas[2].total+'次播放</p></a></li><li><a><img src="http://www.dadpat.com/'+
            datas[3]["image-2"]+'" alt=""/><p>'+datas[3].total+'次播放</p></a></li><li style="margin: 0 0.68rem 0 0.68rem"><a><img src="http://www.dadpat.com/'+
            datas[4]["image-2"]+'" alt=""/><p>'+datas[4].total+'次播放</p></a></li><li><a><img src="http://www.dadpat.com/'+
            datas[5]["image-2"]+'" alt=""/><p>'+datas[5].total+'次播放</p></a></li><li><a><img src="http://www.dadpat.com/'+
            datas[6]["image-2"]+'" alt=""/><p>'+datas[6].total+'次播放</p></a></li><li style="margin: 0 0.68rem 0 0.68rem"><a><img src="http://www.dadpat.com/'+
            datas[7]["image-2"]+'" alt=""/><p>'+datas[7].total+'次播放</p></a></li><li><a><img src="http://www.dadpat.com/'+
            datas[8]["image-2"]+'" alt=""/><p>'+datas[8].total+'次播放</p></a></li>');

        $("#pics li").click(function(){
            var index = $(this).index();
            detailsPage(datas[index].resourceId);
        })
    }
});
