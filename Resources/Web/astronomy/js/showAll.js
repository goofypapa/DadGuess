//参数及构成
var showAll = {
    //水星
    mercury:[
        "xq_title_pic_sx.png",
        "xq_pic_sx.png",
        "天文符号：",
        "xq_pic_sym_sx.png",
        "八大行星之一，第一轨道行星",
        "运行周期，T公=88天   T自=59天",
        "半径：R=2439km",
        "水资源：无;磁场：有",
        "mercury.png",
    ],
    //金星
    venus:[
        "xq_title_pic_jx.png",
        "xq_pic_jx.png",
        "天文符号：",
        "xq_pic_sym_jx.png",
        "八大行星之一，第二轨道行星",
        "运行周期，T公=224.71天   T自=243天",
        "半径：R=6051km",
        "水资源：无;磁场：无",
        "venus.png",
        
    ],
    //地球
    earth:[
        "xq_title_pic_earth.png",
        "xq_pic_earth.png",
        "天文符号：",
        "xq_pic_sym_earth.png",
        "八大行星之一，第三轨道行星",
        "运行周期，T公=1年   T自=1天",
        "半径：R=6378km",
        "水资源：有;磁场：有",
        "earth.png",
        
    ],
    //火星
    mars:[
        "xq_title_pic_hx.png",
        "xq_pic_hx.png",
        "天文符号：",
        "xq_pic_sym_hx.png",
        "八大行星之一，第四轨道行星",
        "运行周期，T公=1.88年   T自=1.03天",
        "半径：R=3395km",
        "水资源：无;磁场：有",
        "mars.png",
        
    ],
    //木星
    jupiter:[
        "xq_title_pic_mx.png",
        "xq_pic_mx.png",
        "天文符号：",
        "xq_pic_sym_mx.png",
        "八大行星之一，第五轨道行星",
        "运行周期，T公=11.86年   T自=398.88天",
        "半径：R=69911km",
        "水资源：有;磁场：有",
        "jupiter.png",
        
    ],
    //土星
    saturn:[
        "xq_title_pic_tx.png",
        "xq_pic_tx.png",
        "天文符号：",
        "xq_pic_sym_tx.png",
        "八大行星之一，第六轨道行星",
        "运行周期,T公=29.65年 T自=10小时32分",
        "半径：R=57316km",
        "水资源：有;磁场：有",
        "saturn.png",
        
    ],
    //天王星
    uranus:[
        "xq_title_pic_twx.png",
        "xq_pic_twx.png",
        "天文符号：",
        "xq_pic_sym_twx.png",
        "八大行星之一，第七轨道行星",
        "运行周期，T公=84年   T自=约16h",
        "半径：R=25,559km",
        "水资源：有;磁场：有",
        "uranus.png",
        
    ],
    //海王星
    neptune:[
        "xq_title_pic_hwx.png",
        "xq_pic_hwx.png",
        "天文符号：",
        "xq_pic_sym_hwx.png",
        "八大行星之一，第八轨道行星",
        "运行周期，T公=164.8年   T自=约18h",
        "半径：R=24,788 km",
        "水资源：有;磁场：有",
        "neptune.png",
        
    ],
    //太阳
    sun:[
        "xq_title_pic_sun.png",
        "xq_pic_sun.png",
        "天文符号：",
        "xq_pic_sym_sun.png",
        "太阳系的中心恒星",
        "运行周期，T公=约2.5×10⁸年   T自=25.05天",
        "半径：R=69.6万km",
        "水资源：无;磁场：无",
        "",
        
    ],
    //冥王星
    pluto:[
        "xq_title_pic_mwx.png",
        "xq_pic_mwx.png",
        "天文符号：",
        "xq_pic_sym_mwx.png",
        "曾经是太阳系中离太阳最远的行星",
        "运行周期，T公=248年   T自=6.39天",
        "半径：R=2372 km",
        "水资源：有;磁场：无",
        "",        
    ],
    //月亮
    moon:[
        "xq_title_pic_moon.png",
        "xq_pic_moon.png",
        "天文符号：",
        "xq_pic_sym_moon.png",
        "地球的天然卫星，太阳系中第五大卫星",
        "运行周期，T公=27.32天   T自=27.32天",
        "半径：R=1738 km",
        "水资源：无;磁场：无",
        "moon.png",
    ],
    //小行星
    asteroid:[
        "xq_title_pic_xxx.png"
    ]
};

// 缩略图
var imgAll = {
    //水星
    mercury:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_sx.png",
        "small_xq_sx.png"
    ],
    //金星
    venus:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_jx.png",
        "small_xq_jx.png"      
    ],
    //地球
    earth:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_earth.png",
        "small_xq_earth.png"  
    ],
    //火星
    mars:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_hx.png",
        "small_xq_hx.png"
    ],
    //木星
    jupiter:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_mx.png",
        "small_xq_mx.png"
    ],
    //土星
    saturn:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_tx.png",
        "small_xq_tx.png"      
    ],
    //天王星
    uranus:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_twx.png",
        "small_xq_twx.png" 
    ],
    //海王星
    neptune:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_hwx.png",
        "small_xq_hwx.png"  
    ],
    //太阳
    sun:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_sun.png",
        "small_xq_sun.png" 
    ],
    //冥王星
    pluto:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_mwx.png",
        "small_xq_mwx.png"      
    ],
    //月亮
    moon:[
        "xq_dz.png",
        "small_xq_dz.png",
        "xq_moon.png",
        "small_xq_moon.png"
    ],
    //小行星
    asteroid:[]
};

// 百分比
var percent  = {
    //水星
    mercury:[
        ['He', 42],
        ['Na', 42],
        ['O₂', 15]
    ],
    //金星
    venus:[
        ['CO₂', 96],
        ['N₂', 3]   
    ],
    //地球
    earth:[
        ['H₂O',1],
        ['N₂', 78.08],
        ['CO₂', 0.0402],
        ['O₂', 20.95],
        ['Ar',0.93]
    ],
    //火星
    mars:[
        ['Ar', 1.93],
        ['CO₂', 95.97],
        ['N₂',2.08]
    ],
    //木星
    jupiter:[
        ['Ar', 10],
        ['H₂', 89],
        ['其它', 1]
    ],
    //土星
    saturn:[
        ['H₂', 96],
        ['He', 3]   
    ],
    //天王星
    uranus:[
        ['无具体数据',100],
    ],
    //海王星
    neptune:[
        ['He', 22.2],
        ['H₂', 76.8],
        ['CH4', 1]
    ],
    //冥王星
    pluto:[
        ['N₂', 98],
        ['其它', 2] 
    ],
    //太阳
    sun:[
        ['H₂', 74.9],
        ['He', 23.8]
    ],
    //月亮
    moon:[
        ['无具体数据',100],
    ],
    //小行星
    asteroid:[
        ['无具体数据',100],
    ]
};
