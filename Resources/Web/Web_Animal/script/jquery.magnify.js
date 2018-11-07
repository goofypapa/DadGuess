/*!
 * jQuery Magnify Plugin v2.3.0 by T. H. Doan (https://thdoan.github.io/magnify/)
 * Based on http://thecodeplayer.com/walkthrough/magnifying-glass-for-images-using-jquery-and-css3
 *
 * jQuery Magnify by T. H. Doan is licensed under the MIT License.
 * Read a copy of the license in the LICENSE file or at https://choosealicense.com/licenses/mit/
 */

  $.fn.magnify = function(oOptions) {
    // Default options  默认选项
    oOptions = $.extend({
      'src': '',
      'speed': 100,
      'timeout': -1,
      'touchBottomOffset': 0,
      'finalWidth': null,
      'finalHeight': null,
      'magnifiedWidth': null,
      'magnifiedHeight': null,
      'limitBounds': false,
      'mobileCloseEvent': 'touchstart',
      'afterLoad': function(){}
    }, oOptions);

    var $that = this, // Preserve scope 保护范围
      $html = $('html'),

      // Initiate 启动
      init = function(el) {
        var $image = $(el),
          $anchor = $image.closest('a'),
          oDataAttr = {};
        // Get data attributes 获取数据的属性
        for (var i in oOptions) {
          oDataAttr[i] = $image.attr('data-magnify-' + i.toLowerCase());
        }

        // Disable zooming if no valid large image source  如果没有有效的大型图像源，禁用缩放。
        var sZoomSrc = oDataAttr['src'] || oOptions['src'] || $anchor.attr('href') || '';
        if (!sZoomSrc) return;

        var $container,
          $lens,
          nImageWidth,
          nImageHeight,
          nMagnifiedWidth,
          nMagnifiedHeight,
          nLensWidth,
          nLensHeight,
          nBoundX = 0,
          nBoundY = 0,
          oContainerOffset, // Relative to document  相对于文档
          oImageOffset,     // Relative to container  相对于容器
          // Get true offsets  得到真正的补偿
          getOffset = function() {
            var o = $container.offset();
            // Store offsets from container border to image inside  存储偏移量从容器边界到图像内部。
            // NOTE: .offset() does NOT take into consideration image border and padding.  注意:.offset()不考虑图像边界和填充。
            oImageOffset = {
              'top': ($image.offset().top-o.top) + parseInt($image.css('border-top-width')) + parseInt($image.css('padding-top')),
              'left': ($image.offset().left-o.left) + parseInt($image.css('border-left-width')) + parseInt($image.css('padding-left'))
            };
            o.top += oImageOffset['top'];
            o.left += oImageOffset['left'];
            return o;
          },
          // Hide the lens  隐藏的镜头
          hideLens = function() {
            if ($lens.is(':visible')) $lens.fadeOut(oOptions['speed'], function() {
              $html.removeClass('magnifying').trigger('magnifyend'); // Reset overflow-x 重置overflow-x
              var fjdBox=document.querySelector('.fjdBox');
              fjdBox.style.display="none";
            });
          };
        // Data attributes have precedence over options object  数据属性优先于选项对象。
        if (!isNaN(+oDataAttr['speed'])) oOptions['speed'] = +oDataAttr['speed'];
        if (!isNaN(+oDataAttr['timeout'])) oOptions['timeout'] = +oDataAttr['timeout'];
        if (!isNaN(+oDataAttr['finalWidth'])) oOptions['finalWidth'] = +oDataAttr['finalWidth'];
        if (!isNaN(+oDataAttr['finalHeight'])) oOptions['finalHeight'] = +oDataAttr['finalHeight'];
        if (!isNaN(+oDataAttr['magnifiedWidth'])) oOptions['magnifiedWidth'] = +oDataAttr['magnifiedWidth'];
        if (!isNaN(+oDataAttr['magnifiedHeight'])) oOptions['magnifiedHeight'] = +oDataAttr['magnifiedHeight'];
        if (oDataAttr['limitBounds']==='true') oOptions['limitBounds'] = true;
        if (typeof window[oDataAttr['afterLoad']]==='function') oOptions.afterLoad = window[oDataAttr['afterLoad']];

        // Implement touch point bottom offset only on mobile devices  只在移动设备上实现触点底部偏移。
        if (/\b(Android|BlackBerry|IEMobile|iPad|iPhone|Mobile|Opera Mini)\b/.test(navigator.userAgent)) {
          if (!isNaN(+oDataAttr['touchBottomOffset'])) oOptions['touchBottomOffset'] = +oDataAttr['touchBottomOffset'];
        } else {
          oOptions['touchBottomOffset'] = 0;
        }

        // Save any inline styles for resetting  保存任何内联样式的重置。
        $image.data('originalStyle', $image.attr('style'));

        // Activate magnification: 激活放大:
        // 1. Try to get large image dimensions   1.尝试获得较大的图像尺寸。
        // 2. Proceed only if able to get large image dimensions OK   2.只有在能够获得较大的图像尺寸时才进行。

        // [1] Calculate the native (magnified) image dimensions. The zoomed version is only shown  [1]计算本机(放大)的图像尺寸。只显示放大版。
        // after the native dimensions are available. To get the actual dimensions we have to create   在本机维度可用之后。为了得到实际的维度，我们必须创建。
        // this image object.  这张图片对象。
        var elZoomImage = new Image();  //大图
        $(elZoomImage).on({
          'load': function() {
            // [2] Got image dimensions OK.  [2]得到了图像尺寸。
            var nX, nY;
            // Fix overlap bug at the edges during magnification  修正放大时的边缘重叠错误。
            $image.css('display', 'block');
            // Create container div if necessary  必要时创建容器div。
            if (!$image.parent('.magnify').length) {
              $image.wrap('<div class="magnify"></div>');
            }
            $container = $image.parent('.magnify');
            // Create the magnifying lens div if necessary  必要时创建放大镜分区。
            if ($image.prev('.magnify-lens').length) {
              $container.children('.magnify-lens').css('background-image', 'url(\'' + sZoomSrc + '\')');
            } else {
              $image.before('<div class="magnify-lens loading" style="background:url(\'' + sZoomSrc + '\') 0 0 no-repeat"></div>');
            }
            $lens = $container.children('.magnify-lens');  //放大镜
            // Remove the "Loading..." text  删除“加载…”文本
            $lens.removeClass('loading');
            // Cache dimensions and offsets for improved performance  缓存大小和偏移量以提高性能。
            // NOTE: This code is inside the load() function, which is important. The width and  注意:此代码位于load()函数中，这很重要。
            // height of the object would return 0 if accessed before the image is fully loaded.  如果在加载图像之前访问该对象的宽度和高度，则返回0。
            /*nImageWidth = oOptions['finalWidth'] || $image.width();    //div(chainBox)的宽度
            nImageHeight = oOptions['finalHeight'] || $image.height(); */   //div(chainBox)的高度
            nImageWidth = oOptions['finalWidth'] || document.documentElement.clientWidth;    //div(chainBox)的宽度
            nImageHeight = oOptions['finalHeight'] || document.documentElement.clientHeight;    //div(chainBox)的高度
            nMagnifiedWidth = oOptions['magnifiedWidth'] || elZoomImage.width;   //大图的宽
            nMagnifiedHeight = oOptions['magnifiedHeight'] || elZoomImage.height;   //大图的高
            nLensWidth = $lens.width();  //放大镜的宽
            nLensHeight = $lens.height(); //放大镜的高
            oContainerOffset = getOffset(); // Required by refresh()  需要刷新()
            // Set zoom boundaries  设置放大边界
            if (oOptions['limitBounds']) {
              nBoundX = (nLensWidth/2) / (nMagnifiedWidth/nImageWidth);
              nBoundY = (nLensHeight/2) / (nMagnifiedHeight/nImageHeight);
            }
            // Enforce non-native large image size?  强制执行非本地大图像大小?
            if (nMagnifiedWidth!==elZoomImage.width || nMagnifiedHeight!==elZoomImage.height) {
              $lens.css('background-size', nMagnifiedWidth + 'px ' + nMagnifiedHeight + 'px');
            }
            // Store zoom dimensions for mobile plugin  存储缩放尺寸的移动插件。
            $image.data('zoomSize', {
              'width': nMagnifiedWidth,
              'height': nMagnifiedHeight
            });
            // Store mobile close event for mobile plugin  为移动插件存储移动关闭事件。
            $container.data('mobileCloseEvent', oDataAttr['mobileCloseEvent'] || oOptions['mobileCloseEvent']);
            // Clean up  清理
            elZoomImage = null;
            // Execute callback  执行回调
            oOptions.afterLoad();
            // Handle mouse movements  处理鼠标移动
            $container.off().on({
              'touchmove': function(e) {
                e.preventDefault();
                // Reinitialize if image initially hidden  重新初始化如果图像最初隐藏。
                if (!nImageHeight) {
                  refresh();
                  return;
                }
                // x/y coordinates of the mouse pointer or touch point. This is the position of  鼠标指针或触点的x/y坐标。这是位置。
                // .magnify relative to the document.  .放大相对于文件。
                //
                // We deduct the positions of .magnify from the mouse or touch positions relative to  我们从鼠标或触摸位置中扣除。放大的位置。
                // the document to get the mouse or touch positions relative to the container.  获取鼠标或触摸相对于容器的位置的文档。
                nX = (e.pageX || e.originalEvent.touches[0].pageX) - oContainerOffset['left']-25;
                nY = ((e.pageY || e.originalEvent.touches[0].pageY) - oContainerOffset['top']) - oOptions['touchBottomOffset']-30;

                // Toggle magnifying lens  切换放大透镜
                if (!$lens.is(':animated')) {
                  if (nX>nBoundX && nX<nImageWidth-nBoundX && nY>nBoundY && nY<nImageHeight-nBoundY) {
                    if ($lens.is(':hidden')) {
                      $html.addClass('magnifying').trigger('magnifystart'); // Hide overflow-x while zooming  隐藏overflow-x而缩放
                      $lens.fadeIn(oOptions['speed']);
                    }
                  } else {
                    hideLens();
                  }
                }
                if ($lens.is(':visible')) {
                  // Move the magnifying lens with the mouse  Move the magnifying lens with the mouse
                  var sBgPos = '';
                  if (nMagnifiedWidth && nMagnifiedHeight) {
                    // Change the background position of .magnify-lens according to the position of  根据这个位置改变。放大镜的背景位置。
                    // the mouse over the .magnify-image image. This allows us to get the ratio of  鼠标经过。放大-图像图像。这就得到了比值。
                    // the pixel under the mouse pointer with respect to the image and use that to  鼠标指针下的像素对图像的使用和使用。
                    // position the large image inside the magnifying lens.  把大的图像放在放大镜里。
                    // var nRatioX = -Math.round((nX/nImageWidth*nMagnifiedWidth-nLensWidth*2)-306),
                    //   nRatioY = -Math.round((nY/nImageHeight*nMagnifiedHeight-nLensHeight*2)-357);
                    /*安卓与ios的判断*/
                      var u = navigator.userAgent;
                      var isAndroid = u.indexOf('Android') > -1 || u.indexOf('Adr') > -1; //android终端
                      var isiOS = !!u.match(/\(i[^;]+;( U;)? CPU.+Mac OS X/); //ios终端
                      /*alert('是否是Android：'+isAndroid);
                      alert('是否是iOS：'+isiOS);*/
                      if(isAndroid){
                          var nRatioX = -Math.round(nX/nImageWidth*nMagnifiedWidth-nLensWidth*7),
                              nRatioY = -Math.round(nY/nImageHeight*nMagnifiedHeight-nLensHeight*9);
                      }else if(isiOS){
                          var nRatioX = -Math.round(nX/nImageWidth*nMagnifiedWidth-nLensWidth*5),
                              nRatioY = -Math.round(nY/nImageHeight*nMagnifiedHeight-nLensHeight*5);
                      }

                    if (oOptions['limitBounds']) {
                      // Enforce bounds to ensure only image is visible in lens  执行边界，以确保只有图像在镜头中可见。
                      var nBoundRight = -Math.round((nImageWidth-nBoundX)/nImageWidth*nMagnifiedWidth-nLensWidth/2),
                        nBoundBottom = -Math.round((nImageHeight-nBoundY)/nImageHeight*nMagnifiedHeight-nLensHeight/2);
                      // Left and right edges  左和右边缘
                      if (nRatioX>0) nRatioX = 0;
                      else if (nRatioX<nBoundRight) nRatioX = nBoundRight;
                      // Top and bottom edges  顶部和底部的边缘
                      if (nRatioY>0) nRatioY = 0;
                      else if (nRatioY<nBoundBottom) nRatioY = nBoundBottom;
                    }
                    sBgPos = nRatioX + 'px ' + nRatioY + 'px';
                  }
                  // Now the lens moves with the mouse. The logic is to deduct half of the lens's  现在镜头随着鼠标移动。逻辑是要减去镜头的一半。
                  // width and height from the mouse coordinates to place it with its center at the  从鼠标坐标的宽度和高度将它与它的中心放在一起。
                  // mouse coordinates. If you hover on the image now, you should see the magnifying  鼠标的坐标。如果你现在在图像上停留，你应该看到放大。
                  // lens in action.  镜头在行动。
                  $lens.css({
                    'top': Math.round(nY-nLensHeight/1) + oImageOffset['top'] + 'px',
                    'left': Math.round(nX-nLensWidth/1) + oImageOffset['left'] + 'px',
                    'background-position': sBgPos
                  });
                    $('.fjdBox').show();
                    $('.fjdBox').css({
                        'top': Math.round(nY-nLensHeight/1) + oImageOffset['top'] -9 + 'px',
                        'left': Math.round(nX-nLensWidth/1) + oImageOffset['left'] -4 + 'px',
                    });
                }
              },
              'mouseenter': function() {
                // Need to update offsets here to support accordions  需要更新偏移量来支持手风琴。
                oContainerOffset = getOffset();
              },
              'mouseleave': hideLens
            });

            // Prevent magnifying lens from getting "stuck"  防止放大镜被“卡住”
            if (oOptions['timeout']>=0) {
                console.log($container)
              $container.on('touchend', function() {
                setTimeout(hideLens, oOptions['timeout']);
              });
            }
              $container.on('touchend', function() {
                  var magnifyLens=document.querySelector('.magnify-lens');
                  var fjdBox=document.querySelector('.fjdBox');
                  magnifyLens.style.display="none";
                  fjdBox.style.display="none";
              });
            // Ensure lens is closed when tapping outside of it  当在外面敲打时，确保镜头关闭。
            $('body').not($container).on('touchstart', function (){
                hideLens
            });

            // Support image map click-throughs while zooming   支持图像地图点击，缩放。
            var sUsemap = $image.attr('usemap');
            if (sUsemap) {
              var $map = $('map[name=' + sUsemap.slice(1) + ']');
              // Image map needs to be on the same DOM level as image source  图像映射需要与图像源相同的DOM级别。
              $image.after($map);
              $container.click(function(e) {
                // Trigger click on image below lens at current cursor position  在当前光标位置，触发按下图像。
                if (e.clientX || e.clientY) {
                  $lens.hide();
                  var elPoint = document.elementFromPoint(
                      e.clientX || e.originalEvent.touches[0].clientX,
                      e.clientY || e.originalEvent.touches[0].clientY
                    );

                  if (elPoint.nodeName==='AREA') {
                    elPoint.click();
                  } else {
                    // Workaround for buggy implementation of elementFromPoint()  用于elementFromPoint()的bug实现的解决方案
                    // See https://bugzilla.mozilla.org/show_bug.cgi?id=1227469  参见https://bugzilla.mozilla.org/show_bug.cgi?id=1227469
                    $('area', $map).each(function() {
                      var a = $(this).attr('coords').split(',');
                      if (nX>=a[0] && nX<=a[2] && nY>=a[1] && nY<=a[3]) {
                        this.click();
                        return false;
                      }
                    });
                  }
                }
              });
            }

            if ($anchor.length) {
              // Make parent anchor inline-block to have correct dimensions  使父锚定线块具有正确的尺寸。
              $anchor.css('display', 'inline-block');
              // Disable parent anchor if it's sourcing the large image  禁用父锚，如果它正在寻找大的图像。
              if ($anchor.attr('href') && !(oDataAttr['src'] || oOptions['src'])) {
                $anchor.click(function(e) {
                  e.preventDefault();
                });
              }
            }

          },
          'error': function() {
            // Clean up  清理
            elZoomImage = null;
          }
        });

        elZoomImage.src = sZoomSrc;
      }, // END init()  结束init()

      // Simple debounce  简单的防反跳
      nTimer = 0,
      refresh = function() {
        clearTimeout(nTimer);
        nTimer = setTimeout(function() {
          $that.destroy();
          $that.magnify(oOptions);
        }, 100);
      };

    /**
     * Public Methods  公共方法
     */

    // Turn off zoom and reset to original state  关闭缩放和复位到原始状态。
    this.destroy = function() {
      this.each(function() {
        var $this = $(this),
          $lens = $this.prev('div.magnify-lens'),
          sStyle = $this.data('originalStyle');
        if ($this.parent('div.magnify').length && $lens.length) {
          if (sStyle) $this.attr('style', sStyle);
          else $this.removeAttr('style');
          $this.unwrap();
          $lens.remove();
        }
      });
      // Unregister event handler  注销事件处理程序
      $(window).off('resize', refresh);
      return $that;
    }

    // Handle window resizing  处理窗口大小调整
    $(window).resize(refresh);
    return this.each(function() {
      // Initiate magnification powers  启动放大权力
      var chainBox = document.querySelector('.chainBox');
      init(chainBox);
    });
  };
