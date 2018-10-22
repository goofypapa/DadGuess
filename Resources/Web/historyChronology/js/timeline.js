/*朝代的霓虹灯效果*/
// window.android.setScreenOrientation(1)
// window.android.pauseMusic()
var me = 0;
var imgObj = document.querySelectorAll('.lamp img');
var timer = setInterval(function(){
    imgObj[me].classList.add('anima');
    me=me+1;
    if(me == imgObj.length){
        clearInterval(timer)
    }
},3000);
var tim = setTimeout(function () {
    var timers = setInterval(function () {
        if(imgObj[me-2].classList.contains('anima')){
            imgObj[me-2].classList.remove('anima')
        }
        if(imgObj[imgObj.length-1].classList.contains('anima')){
            imgObj[imgObj.length-1].classList.remove('anima')
        }
        if(me == imgObj.length){
            clearInterval(timers);
            clearTimeout(tim)
        }
    },3000);
},3000);


function GetQueryString(name){
    var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if(r!=null)return  unescape(r[2]); return null;
}
$.ajax({
    type:"post",
    url:"http://www.dadpat.com/dynasty/getAllList.do",
    dataType:"jsonp", //以键/值对的形式
    async:true,
    success:function(data){
        console.log(data.data);
        var datas =data.data;
        for(let i=0;i<datas.length;i++){
            var pObj = document.querySelectorAll('.track p');
            let datasA = datas[i];
            $('.track p').click(function(){
                console.log(datasA);
                if(datasA.dynastyName == this.innerText){
                    window.location.href="details.html?dynastyId="+datasA.dynastyId;
                }else{
                    return false;
                }
            })
        }
    }
});
