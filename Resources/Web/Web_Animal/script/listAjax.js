//js获取url参数
function GetQueryString(name){
    var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if(r!=null)return  unescape(r[2]); return null;
}
//定义变量接收url参数，此处的变量名不能与url参数名相同,否则会报错不知道为啥
var myOwnerId=GetQueryString("ownerId");
var mySource=GetQueryString("source");
var myRecordTime=GetQueryString("recordTime");

$.ajax({
    type: "post",
    url: "http://www.dadpat.com/api/res/list.do",
    dataType: "jsonp",
    data:{"ownerId":myOwnerId},
    async: true,
    success: function (data) {
        //设置动物资源总数
        $("header p").html(mySource);
        console.log(mySource)
        //设置资料录制时间
        $("header p b").html(myRecordTime);
        //遍历返回的数组
        var datas=data.data;
        console.log(datas);
        for(var i=0;i<datas.length;i++){
            //把录制时间、地点去掉了
            $("#list").append('<li style="width:100%;height:6rem;display:block;background-image: url(http://www.dadpat.com/'+datas[i].headImage+');background-size:100% 6rem;"><div class="info" style="margin-top: 1.2rem"><div><p>'+datas[i].resourceTitle+'</p></div><div style="display:none">'+datas[i].resourceDesc+'</div></div></li>');
        }
        $("#list li").click(function(){
            var index = $(this).index();
            detailsPage(datas[index].resourceId);
        });
        //之前的写法
        /*$("#list div.info>h4").each(function(){
            //字符个数
            var maxwidth=27;
            if($(this).text().length>maxwidth){
                $(this).text($(this).text().substring(0,maxwidth));
                $(this).html($(this).html()+"......");
            }
        });*/
        var arr = [];
        $("#list div.info").each(function(){
        	// console.log($(this)[0].children[1].innerText)
        	var textAll  = $(this)[0].children[1].innerText;
            //字符个数
			arr = textAll.split(";")
			// console.log(arr)
			$(this).append('<div class="dvPin"><p>'+arr[0]+'</p><br><p>'+arr[1]+'</p></div>')
        });
        //设置列表页头像地址
        data.image;
    }
});