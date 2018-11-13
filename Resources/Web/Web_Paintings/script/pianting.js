

var myarray=['http://www.dadpat.com/app/paintingImg/pianting1.png','http://www.dadpat.com/app/paintingImg/pianting2.png','http://www.dadpat.com/app/paintingImg/pianting3.png','http://www.dadpat.com/app/paintingImg/pianting4.png','http://www.dadpat.com/app/paintingImg/pianting5.png','http://www.dadpat.com/app/paintingImg/pianting6.png','http://www.dadpat.com/app/paintingImg/pianting7.png','http://www.dadpat.com/app/paintingImg/pianting8.png','http://www.dadpat.com/app/paintingImg/pianting9.png','http://www.dadpat.com/app/paintingImg/pianting10.png','http://www.dadpat.com/app/paintingImg/pianting11.png','http://www.dadpat.com/app/paintingImg/pianting12.png']
var sum=3;
var MoveSpeed=1;//每次改变的大小为N个像素
var imgLeft=window.screen.availWidth/3*2-50;
var imgTop=window.screen.availHeight/3+50;
var div=document.getElementById("one");
var div1=document.getElementById("two");
var div2=document.getElementById("three");
console.log(div);
window.onload=function SetDiv()
{
    div.src=myarray[0];
    div1.src=myarray[1];
    div2.src=myarray[2];
    ChangeWH=setInterval("SetWH()",30);

}
function SetWH()
{
    var oldwidth=div.offsetWidth;
    oldwidth=oldwidth+MoveSpeed;
    var oldleft=div.offsetLeft;
    oldleft=oldleft-MoveSpeed;
    var oldtop=div.offsetTop;
    oldtop=oldtop-MoveSpeed;
    console.log(oldleft, oldtop);

    var oldwidth1=div1.offsetWidth;
    oldwidth1=oldwidth1+MoveSpeed;
    var oldleft1=div1.offsetLeft;
    oldleft1=oldleft1+MoveSpeed;
    var oldtop1=div1.offsetTop;
    oldtop1=oldtop1-MoveSpeed;
    console.log(oldleft1, oldtop1)

    var oldwidth2=div2.offsetWidth;
    oldwidth2=oldwidth2+MoveSpeed;
    var oldleft2=div2.offsetLeft;
    oldleft2=oldleft2+MoveSpeed;
    var oldtop2=div2.offsetTop;
    oldtop2=oldtop2-MoveSpeed;
    console.log(oldleft2, oldtop2)

    // console.log(imgLeft,imgTop);
    // console.log(oldwidth);
    // console.log(oldleft);

    if(oldwidth>=150)
    {

        if(sum>=myarray.length-1)
        {
            sum=0;
        }
        else
        {
            sum++;
        }
        div.src=myarray[sum];
        div.style.width=0+"px";
        div.style.left=200+"px";
        div.style.top=160+"px";
        // console.log("aaa"+myarray[sum]+"aaa"+sum);

    }
    else
    {
        div.style.width=oldwidth+"px";
        div.style.zIndex=oldwidth;
        div.style.left=oldleft+"px";
        div.style.top=oldtop+"px";
        // console.log("aaa"+myarray[sum]+"bbb"+sum);

    }

    if(oldwidth1>=150)
    {


        if(sum>=myarray.length-1)
        {
            sum=0;
        }
        else
        {
            sum++;
        }
        div1.src=myarray[sum];
        div1.style.width=0+"px";
        div1.style.left=imgLeft+"px";
        div1.style.top=imgTop+"px";


        //console.log("bbb"+myarray[sum]+"bbb"+sum);

    }
    else
    {
        div1.style.width=oldwidth1+"px";
        div1.style.left=oldleft1+"px";
        div1.style.top=oldtop1+"px";
        div1.style.zIndex=oldwidth1;

    }
    if(oldwidth2>=150)
    {

        if(sum>=myarray.length-1)
        {
            sum=0;
        }
        else
        {
            sum++;
        }
        div2.src=myarray[sum];
        div2.style.width=0+"px";
        div2.style.left=imgLeft+"px";
        div2.style.top=imgTop+"px";
        // console.log("ccc"+myarray[sum]+"ccc"+sum);

        //zclearInterval(ChangeWH);//停止计时器

    }
    else
    {
        div2.style.width=oldwidth2+"px";
        // div2.style.height=oldHeight2+"px";
        div2.style.zIndex=oldwidth2;
        div2.style.left=oldleft2+"px";
        div2.style.top=oldtop2+"px";

    }


}
function getNum(text){
    var imgSrc=$(text)[0].src;
    var index = imgSrc .lastIndexOf("\/");
    imgSrc  = imgSrc .substring(index + 1, imgSrc .length);
    var value = imgSrc.replace(/[^0-9]/ig,"");
    console.log(value);
    value=value-1;
    window.location.href="piantingLunboNew.html?index="+value;
}
$("#one").click(function(){
    getNum("#one");
})
$("#two").click(function(){
    getNum("#two");
})
$("#three").click(function(){
    getNum("#three");
})