var ajaxBox=function () {
    function GetQueryString(name){
        var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
        var r = window.location.search.substr(1).match(reg);
        if(r!=null)return  unescape(r[2]); return null;
    }
//定义变量接收url参数
    var myresId=GetQueryString("resourceId");
    $.ajax({
        type:"post",
        url:"http://www.dadpat.com/foodChain/get.do",
        dataType:"jsonp",
        data:{"resId":myresId}, //以键/值对的形式
        async:false,
        success:function (data) {
            var datas=data.data;
            // console.log(datas)
            var docuWidth = document.documentElement.clientWidth;
            var docuHeight = document.documentElement.clientHeight;
            var sourceWidth = 1080;
            var sourceHeight = 1920;
            //强制竖屏
            var dvObj = document.getElementsByClassName('magnify');
            console.log(dvObj)
            if(docuWidth<docuHeight){
                dvObj[0].style.width = docuWidth + 'px';
                dvObj[0].style.height = docuHeight + 'px';
                dvObj[0].style.transform = 'none';
                dvObj[0].style.marginTop =  '0px';
                dvObj[0].style.marginLeft = '0px';
            }else{
                dvObj[0].style.width = docuHeight + 'px';
                dvObj[0].style.height = docuWidth + 'px';
                dvObj[0].style.transform = 'rotate(270deg)';
                dvObj[0].style.marginTop = (docuHeight-docuWidth)/2 + 'px';
                dvObj[0].style.marginLeft = -(docuHeight-docuWidth)/2 + 'px';
            }
            var windowRealHeight = $('.magnify')[0].offsetHeight;
            var windowRealWidth = $('.magnify')[0].offsetWidth;
            document.documentElement.style.fontSize = windowRealWidth/16+'px';
            //背景图
            $('body').css({
                'height': windowRealHeight,
                'width': windowRealWidth,
                'overflow': 'hidden'
            });
            $('.magnify').append('<img src=http://www.dadpat.com/'+datas.bgImg+' class="backgroundImg">');
            $('.backgroundImg').css({
                'height': windowRealHeight,
                'width': windowRealWidth,
                'position': 'absolute',
                'top': 0,
                'z-index': -1
            });
            $('.chainBox').css({
                'height': windowRealHeight,
                'width': windowRealWidth,
                'position': 'absolute',
                'top': 0,
                'overflow':'hidden'
            });
            $('.chainUlBox').css({
                'height': windowRealHeight,
                'width': windowRealWidth,
            });
            $('.chainUlBox .chainLi1 img').css({
                "width": Math.floor(53 / sourceWidth * windowRealWidth),
                "height": Math.floor(118 / sourceHeight * windowRealHeight),
                "margin-top": Math.floor(30 / sourceHeight * windowRealHeight)
            });
            $('.chainUlBox .chainLi3 img').css({
                "width": Math.floor(53 / sourceWidth * windowRealWidth),
                "height": Math.floor(118 / sourceHeight * windowRealHeight),
                "margin-top": Math.floor(30 / sourceHeight * windowRealHeight)
            });
            $('#mirror').css({
                'height':windowRealHeight/6,
                'width':windowRealHeight/6,
            });
            $("#mirror").append('<img src=http://www.dadpat.com/'+datas.fullImg+' id="fullImg">');
            $('#fullImg').css({
                'position':'absolute'
            });
            // alert(datas.fullImg)
            $('.fjdBox').css({
                'height':windowRealHeight/4,
                'width':windowRealHeight/4.3,
            });

            /*===============================放大镜调用---开始========================================================================================*/
            var mirror = document.getElementById("mirror");
            var pic = document.getElementById("pic");
            var fdj = document.getElementById("fdj");
            var fullImg = document.getElementById("fullImg");
            function moving(e){
                var x = e.touches[0].clientX -70;  //触摸的位置
                var y = e.touches[0].clientY -70;
                var mirrorHeight = mirror.offsetHeight;  //放大镜的宽高
                var mirrorWidth = mirror.offsetWidth;
                var picHeight = pic.offsetHeight;  //小图的宽高
                var picWidth = pic.offsetWidth;
                if(x <= picWidth && y <= picHeight && x >= -picWidth && y>= -picHeight){
                    mirror.style.left = x - mirrorWidth / 2 + "px";
                    mirror.style.top = y - mirrorHeight / 2 + "px";
                    if(x - mirrorWidth / 1.8 < windowRealWidth - fdj.offsetWidth){
                        fdj.style.left = x - mirrorWidth / 1.8 + "px";
                    }else{
                        fdj.style.display = 'none';
                        mirror.style.display = 'none';
                    }
                    if(y - mirrorHeight / 1.8 < windowRealHeight - fdj.offsetHeight){
                        fdj.style.top = y - mirrorHeight / 1.8 + "px";
                    }else{
                        fdj.style.display = 'none';
                        mirror.style.display = 'none';
                    }
                    var bl = x * sourceWidth / windowRealWidth * 3 - mirrorWidth / 2;
                    var bt = y * sourceHeight / windowRealHeight * 3 - mirrorHeight / 2;
                    fullImg.style.left =- bl + "px";
                    fullImg.style.top = - bt + "px";
                    // console.log("x: ",windowRealWidth - fdj.offsetWidth, "y: ",windowRealHeight - fdj.offsetHeight);
                }
            }
            window.ontouchstart = function(){
                $('#mirror').css('display','none');
                $('#fdj').css('display','none')
            };
            window.ontouchmove = function(e){
                $('#mirror').css('display','block');
                $('#fdj').css('display','block');
                var e = e?e:window.event;
                moving(e);
            };
            window.ontouchend = function(){
                $('#mirror').css('display','none');
                $('#fdj').css('display','none')
            };
            /*===================================放大镜调用---结束=====================================================================================*/

            var itemLent = datas.items;
            console.log(itemLent);
            var arr = [];
            for(var i=0,item;i<itemLent.length;i++){
                /*获取上、中、下游的动、植物*/
                if(itemLent[i].itemLocation=='up'){
                    //获取上游动物
                    $(".chainDiv0").append('<div><span class="animalAll"><img src=http://www.dadpat.com/'+
                        itemLent[i].itemImg+' alt="" class="img01" style="height:'+ Math.floor(itemLent[i].itemHeight / sourceHeight * windowRealHeight ) +'px; width:'
                        + Math.floor(itemLent[i].itemWidth / sourceWidth * windowRealWidth ) +'px;margin-left: '+ Math.floor(itemLent[i].itemLeft / sourceWidth * windowRealWidth ) +'px;margin-top:'+
                        Math.floor(itemLent[i].itemTop / sourceHeight * windowRealHeight )+'px ;"/><p style="margin-left: '+ Math.floor(itemLent[i].itemLeft / sourceWidth * windowRealWidth ) +'px">'+itemLent[i].itemName+'</p></span></div>')
                }else if(itemLent[i].itemLocation=='self'){
                    $(".chainDiv2").append('<div class="arrSelfTwo"><span class="animalAll"><img src=http://www.dadpat.com/'+
                        itemLent[i].itemImg+' alt="" class="img01" style="height:'+ Math.floor(itemLent[i].itemHeight / sourceHeight * windowRealHeight ) +'px; width:'
                        + Math.floor(itemLent[i].itemWidth / sourceWidth * windowRealWidth ) +'px;margin-left: '+ Math.floor(itemLent[i].itemLeft / sourceWidth * windowRealWidth ) +'px;margin-top:'+
                        Math.floor(itemLent[i].itemTop / sourceHeight * windowRealHeight )+'px ;"/><p style="margin-left: '+ Math.floor(itemLent[i].itemLeft / sourceWidth * windowRealWidth ) +'px">'+itemLent[i].itemName+'</p></span></div>')
                }else if(itemLent[i].itemLocation=='down'){
                    //获取下游动物
                    $(".chainDiv4").append('<div><span class="animalAll"><img src=http://www.dadpat.com/'+
                        itemLent[i].itemImg+' alt="" class="img01" style="height:'+ Math.floor(itemLent[i].itemHeight / sourceHeight * windowRealHeight ) +'px; width:'
                        + Math.floor(itemLent[i].itemWidth / sourceWidth * windowRealWidth ) +'px;margin-left: '+ Math.floor(itemLent[i].itemLeft / sourceWidth * windowRealWidth ) +'px;margin-top:'+
                        Math.floor(itemLent[i].itemTop / sourceHeight * windowRealHeight )+'px ;"/><p style="margin-left: '+ Math.floor(itemLent[i].itemLeft / sourceWidth * windowRealWidth ) +'px">'+itemLent[i].itemName+'</p></span></div>')
                }
                /*获取动、植物的描述*/
                var pDesc = document.querySelectorAll('.chainUlBox p');
                var imgDesc=document.querySelectorAll('.animalAll img');
                for (var n=0;n<pDesc.length;n++){
                	if(pDesc[n].innerText == '鸟类'){
                		pDesc[n].style.marginLeft = '3rem';
                	}
                	if(pDesc[n].innerText == '鹰鹃'){
                		pDesc[n].style.marginLeft = '5.5rem';
                	}
                	if(pDesc[n].innerText == '麋鹿'){
                		pDesc[n].style.marginLeft = '5.2rem';
                	}
                	if(pDesc[n].innerText == '白腹锦鸡'){
                		pDesc[n].style.marginLeft = '6.5rem';
                	}
                	if(pDesc[n].innerText == '驼鹿'){
                		pDesc[n].style.marginLeft = '5.8rem';
                    }
                    if(pDesc[n].innerText == '环颈雉'){
                		pDesc[n].style.marginLeft = '3.8rem';
                    }
                    if(pDesc[n].innerText == '喜马拉雅旱獭'){
                		pDesc[n].style.marginLeft = '6.2rem';
                    }
                    if(pDesc[n].innerText == '小熊猫'){
                		pDesc[n].style.marginLeft = '5.5rem';
                    }
                    if(pDesc[n].innerText == '长颈鹿'){
                		pDesc[n].style.marginLeft = '7rem';
                    }
                    if(pDesc[n].innerText == '骆驼'){
                		pDesc[n].style.marginLeft = '6rem';
                    }
                    if(pDesc[n].innerText == '鬼鸮'){
                		pDesc[n].style.marginLeft = '6.5rem';
                    }
                    if(pDesc[n].innerText == '浣熊'){
                		pDesc[n].style.marginLeft = '5.5rem';
                    }
                    if(pDesc[n].innerText == '猛禽'){
                		pDesc[n].style.marginLeft = '2.5rem';
                    }
                    if(imgDesc[n].src== 'http://www.dadpat.com/upload/20180518/5a5a11e8b7008d0059dd0b57477ef8aa.png'){
                        pDesc[n].style.marginLeft = '1.2rem';
                    }
                    if(imgDesc[n].src== 'http://www.dadpat.com/upload/20180905/b0d911e88ec45f21705f1db236d91c4c.png'){
                        pDesc[n].style.marginLeft = '-1.5rem';
                    }
                    if(imgDesc[n].src== 'http://www.dadpat.com/upload/20180906/b19d11e88ec45f21705f1db260b11576.png'){
                        pDesc[n].style.marginLeft = '-1rem';
                    }
                    if(imgDesc[n].src== 'http://www.dadpat.com/upload/20180906/b19e11e88ec45f21705f1db2d8f4a1c9.png'){
                        pDesc[n].style.marginLeft = '1rem';
                    }
                    if(imgDesc[n].src== 'http://www.dadpat.com/upload/20180829/ab2c11e8945481befe7496c044f09c1a.png'){
                        pDesc[n].style.marginLeft = '6rem';
                    }
                    if(pDesc[n].innerText == '赤腹松鼠'){
                		pDesc[n].style.marginLeft = '6rem';
                	}
                    let itemIdP = itemLent[i].resId;
                	console.log(itemIdP);
                    if(itemIdP!=null){  //判断是否存在可跳转的resId
                        if(pDesc[n].innerText==itemLent[i].itemName){
                            let imDesc= pDesc[n].previousSibling;
                            imDesc.onclick=function () {
                                $.ajax({
                                    type:"post",
                                    url:"http://www.dadpat.com/resource/getResourceInfo.do",
                                    dataType:"jsonp",
                                    data:{"resourceId":itemIdP}, //以键/值对的形式
                                    async:true,
                                    success:function(){
                                        window.location.href="details.html?resourceId="+itemIdP+"&imgType=default";
                                    }
                                })
                            }
                        }
                    }else if(itemLent[i].itemDesc!=null){ //判断动物是否有简介--描述
                        let itemLentI =itemLent[i].itemDesc;
                        if(pDesc[n].innerText==itemLent[i].itemName){
                            var imDescS= pDesc[n].previousSibling;  //点击的div
                            imDescS.onclick=function () {
                                document.querySelector(".abstract").addEventListener("click", function (e) {
                                    e.stopPropagation()
                                });
                                document.querySelector(".abstract").addEventListener("touchmove", function (e) {
                                    e.stopPropagation()
                                });
                                document.querySelector('.abstract').style.display="block";
                                document.querySelector('.dang').style.display="block";
                                $('.abstract').append('<button>'+itemLentI+'</button><img src="image/closeTwo.png">');
                                $(".abstract img").click(function () {
                                    document.querySelector('.abstract').style.display="none";
                                    document.querySelector('.dang').style.display="none";
                                    document.querySelector('.abstract').innerHTML = "";
                                })
                            }
                        }
                    }else if(pDesc[n].innerText==itemLent[i].itemName&&itemLent[i].itemType=='type'){
                        var imDescT= pDesc[n].previousSibling;
                        $(pDesc[n]).append('<img src="image/downTriangle.png" style="height: 0.25rem;">');
                        arr.push(imDescT);
                        let abc = itemLent[i];
                        var body = document.querySelector('body');
                        let imDescTc = imDescT;
                        imDescT.onclick=function(even) {
                            var imDescTParent = imDescTc.parentNode;
                            even.stopPropagation();
                            var pThis = $(this)[0].nextSibling;
                            $(pThis).after('<ul class="chainUlTwo"></ul>');
                            $('.chainUlTwo').css({
                                'display': 'flex',
                                'justify-content': 'center',
                                'position': 'absolute',
                                'z-index': 100,
                                'top': (imDescTParent.offsetTop+imDescTParent.offsetHeight+8)+'px',
                                'left': 0,
                                'width': '100%',
                                'border-top': '0.1rem solid rgba(255,255,255,0.7)',
                                'border-bottom': '0.1rem solid rgba(255,255,255,0.7)',
                                'background-color': 'rgba(0,0,0,0.5)'
                            });
                            $.ajax({
                                type: "post",
                                url: "http://www.dadpat.com/foodChain/item/list.do",
                                dataType: "jsonp",
                                data: {"ownerId": abc.itemId}, //以键/值对的形式
                                async: true,
                                success: function (data) {
                                    var listDatas = data.data;
                                    document.querySelector('.dang').style.display = "block";

                                    /*当遮挡层出现时，禁用放大镜*/
                                    $('.chainUlTwo').append('<li><img src="image/zhengsanjiao.png" class="chainUlImg"></li>');
                                    $('.chainUlImg').css({
                                        'position': 'absolute',
                                        'z-index': 110,
                                        'top': '-0.55rem',
                                        'left': (imDescTc.offsetLeft+imDescTc.offsetWidth/2-18)+'px',
                                        'width':'1rem',
                                    });
                                    document.querySelector(".chainUlTwo").addEventListener("click", function (e) {
                                        e.stopPropagation()
                                    });
                                    document.querySelector(".chainUlTwo").addEventListener("touchmove", function (e) {
                                        e.stopPropagation()
                                    });
                                    for (var k = 0; k < listDatas.length; k++) {
                                        $('.chainUlTwo').append('<li><img src=http://www.dadpat.com/'+ listDatas[k].itemImg + ' style="height:'+ Math.floor(listDatas[k].itemHeight / sourceHeight * windowRealHeight ) +'px; width:'+ Math.floor(listDatas[k].itemWidth / sourceWidth * windowRealWidth ) +'px;"/><p style="margin-bottom: 0.1rem">' + listDatas[k].itemName + '</p></li>')
                                        var pDesc = document.querySelectorAll('.chainUlTwo p');
                                        for (var n=0;n<pDesc.length;n++){
                                        /*获取分类下动、植物的描述*/
                                            if(listDatas[k].itemDesc!=null){
                                                let itemLentI =listDatas[k].itemDesc;
                                                if(pDesc[n].innerText==listDatas[k].itemName){
                                                    var imDesc= pDesc[n].previousSibling;
                                                    imDesc.onclick=function () {
                                                        document.querySelector('.abstract').style.display="block";
                                                        document.querySelector('.dang').style.display="block";
                                                        $('.abstract').append('<button>'+itemLentI+'</button><img src="image/closeTwo.png">');
                                                        if($('.abstract')[0].style.display=='block'){
                                                            document.onclick=null;
                                                            $(".abstract img").click(function (e) {
                                                                e.stopPropagation();
                                                                document.querySelector('.abstract').style.display="none";
                                                                document.querySelector('.dang').style.display="block";
                                                                document.querySelector('.abstract').innerHTML = "";
                                                                if ($('.abstract')[0].style.display=='none'){
                                                                    document.onclick = function () {
                                                                        var chainUlTwo = document.querySelector('.chainUlTwo');
                                                                        if(chainUlTwo==null){
                                                                            return false
                                                                        }else{
                                                                            chainUlTwo.parentNode.removeChild(chainUlTwo);
                                                                            document.querySelector('.dang').style.display = "none";
                                                                        }
                                                                    };
                                                                }
                                                            });
                                                        }
                                                    };
                                                    document.onclick = function () {
                                                        var chainUlTwo = document.querySelector('.chainUlTwo');
                                                        if(chainUlTwo==null){
                                                            return false
                                                        }else{
                                                            chainUlTwo.parentNode.removeChild(chainUlTwo);
                                                            document.querySelector('.dang').style.display = "none";
                                                        }
                                                    };
                                                }
                                            }else if(listDatas[k].resId!=null){
                                                console.log(listDatas[k].resId);
                                                var chirldItemId=listDatas[k].resId;
                                                if(pDesc[n].innerText==listDatas[k].itemName){
                                                    console.log(pDesc[n].innerText);
                                                    console.log(listDatas[k].itemName);
                                                    let imDescF= pDesc[n].previousSibling;
                                                    console.log(pDesc[n].previousSibling);
                                                    imDescF.onclick=function () {
                                                        $.ajax({
                                                            type:"post",
                                                            url:"http://www.dadpat.com/resource/getResourceInfo.do",
                                                            dataType:"jsonp",
                                                            data:{"resourceId":chirldItemId}, //以键/值对的形式
                                                            async:true,
                                                            success:function(){
                                                                console.log(chirldItemId);
                                                                window.location.href="details.html?resourceId="+chirldItemId+"&imgType=default";
                                                            }
                                                        })
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            });
                        };
                        document.onclick = function () {
                            var chainUlTwo = document.querySelector('.chainUlTwo');
                            if(chainUlTwo==null){
                                return false
                            }else{
                                chainUlTwo.parentNode.removeChild(chainUlTwo);
                                document.querySelector('.dang').style.display = "none";
                            }
                        };
                    }
                }
            }
            //设置问号下边的文字为空字符串
            var pAll = document.querySelectorAll('p');
            for(var i=0;i<pAll.length;i++){
                if(pAll[i].innerText=='未知'){
                    pAll[i].innerText='';
                }
            }
            if($('.chainDiv2 .animalAll').length>1){
                $('.chainLi1').remove();
                $('.chainDiv2 .animalAll').before('<li class="chainLiAdd"><img src="image/jiantou.png"></li>');
                $('.arrSelfTwo').css({
                    'display':'fixed',
                    'flex-direction': 'column',
                    'float': 'left'
                });
                var chainDiv2Img = document.querySelectorAll('.chainDiv2 .animalAll img');
                var chainLiAddImg = document.querySelectorAll('.chainLiAdd img');
                for(var j=0;j<chainDiv2Img.length;j++){
                    chainLiAddImg[j].style.height = Math.floor(118 / sourceHeight * windowRealHeight)+'px';
                    chainLiAddImg[j].style.width = Math.floor(53 / sourceWidth * windowRealWidth)+'px';
                    chainLiAddImg[j].style.marginTop = Math.floor(30 / sourceHeight * windowRealHeight)+'px';
                    chainLiAddImg[j].style.marginLeft =Math.floor(chainDiv2Img[j].width/2-chainLiAddImg[j].width/2+parseInt(chainDiv2Img[j].style.marginLeft))+'px';
                }
            }
            $( document ).bind( "enhance", function(){
                $( "body" ).addClass( "enhanced" );
            });
            $( document ).trigger( "enhance" );
            setTimeout(function(){
            	document.querySelector('.button').style.display='none';
            },2000)
        }
    });
};