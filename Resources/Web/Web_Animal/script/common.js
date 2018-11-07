function openZoobk(str){
    api.openWin({
        name: 'detailsBG',
        url: './html/bg.html?mySrc='+str+'',
        rect: {
            x: 0,
            y: 0,
            w: api.frameWidth,
            h: 'auto'
        },
        pageParam: {
            name: 'test'
        },
        bounces: false,
        bgColor: '#272e38',
        vScrollBarEnabled: false,
        hScrollBarEnabled: false
    });
}
function openMyWin(str){
    api.openWin({
        name: 'newsTest2',
        url: './html/'+str+'.html',
        rect: {
            x: 0,
            y: 0,
            w:api.frameWidth,
            h: 'auto'
        },
        pageParam: {
            name: 'test'
        },
        bounces: false,
        bgColor: '#272e38',
        vScrollBarEnabled: false,
        hScrollBarEnabled: false
    });
}
function opendetails(id){
    api.openWin({
        name: 'detailsBG',
        url:'./html/details.html?resourceId='+id+'',
        rect: {
            x: 0,
            y: 0,
            w: api.frameWidth,
            h: 'auto'
        },
        pageParam: {
            name: 'test'
        },
        bounces: false,
        bgColor: '#272e38',
        vScrollBarEnabled: false,
        hScrollBarEnabled: false
    });
}
function detailsPage(myresourceId){
    window.location.href="details.html?resourceId="+myresourceId;
}
function openDP(resourceId){
    api.openWin({
        name: 'detailsBG',
        url: './bg.html?mySrc=pics&resourceId='+resourceId+'',
        rect: {
            x: 0,
            y: 0,
            w: api.frameWidth,
            h: 'auto'
        },
        pageParam: {
            name: 'test'
        },
        bounces: false,
        bgColor: '#272e38',
        vScrollBarEnabled: false,
        hScrollBarEnabled: false
    });
}
function openimg(number){
    window.location.href="img.html?resourceId="+myresourceId+"&myNumber="+number;
}
function goback111(src) {
   // alert();
     window.location.href=src;
}

function back() {
    //ios ws
    if(typeof(goofyPapa_back) === "function"){
        goofyPapa_back();
        return;
    }
    //android ws
    if( typeof(goofyPapa) !== "undefined" ){
        goofyPapa.back();
        return;
    }
    if(history.length) {
        history.go(-1);
        return;
    }
}

/*px转rem*/
/*!function(e, t) {
    function n() {
        var n = l.getBoundingClientRect().width;
        t = t || 540,
        n > t && (n = t);
        var i = 100 * n / e;
        r.innerHTML = "html{font-size:" + i + "px;}"
    }
    var i, d = document,
        o = window,
        l = d.documentElement,
        r = document.createElement("style");
    if (l.firstElementChild) l.firstElementChild.appendChild(r);
    else {
        var a = d.createElement("div");
        a.appendChild(r),
            d.write(a.innerHTML),
            a = null
    }
    n(),
        o.addEventListener("resize",
            function() {
                clearTimeout(i),
                    i = setTimeout(n, 300)
            },
            !1),
        o.addEventListener("pageshow",
            function(e) {
                e.persisted && (clearTimeout(i), i = setTimeout(n, 300))
            },
            !1),
        "complete" === d.readyState ? d.body.style.fontSize = "16px": d.addEventListener("DOMContentLoaded",
            function(e) {
                d.body.style.fontSize = "16px"
            },
            !1)
} (750, 750);*/
