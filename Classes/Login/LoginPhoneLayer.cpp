#include "LoginPhoneLayer.h"

USING_NS_CC;

bool LoginPhoneLayer::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    DrawNode* t_drawNode = DrawNode::create();
    this->addChild( t_drawNode );
    
//    BlendFunc bf = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
//    t_drawNode->setBlendFunc(bf);
//
//    glEnable(GL_LINE_SMOOTH);


    const int t_segments = 50;
    const float t_radius = 3.0f;

    Vec2 t_origin = Vec2( 100, 100 );
    Vec2 t_destination = Vec2( 200, 150 );
    
    const float t_coef    = 0.5f * (float)M_PI / t_segments;
    Point * t_vertices    = new Point[t_segments + 1];
    Point * t_thisVertices = t_vertices;
    for(unsigned int i = 0; i <= t_segments; ++i, ++t_thisVertices)
    {
        float t_rads        = (t_segments - i) * t_coef;
        t_thisVertices->x    = (int)(t_radius * sinf(t_rads));
        t_thisVertices->y    = (int)(t_radius * cosf(t_rads));
    }
    
    float t_minX    = MIN(t_origin.x, t_destination.x);
    float t_maxX    = MAX(t_origin.x, t_destination.x);
    float t_minY    = MIN(t_origin.y, t_destination.y);
    float t_maxY    = MAX(t_origin.y, t_destination.y);

//    Vec2 t_points[4 * t_segments] = { Vec2( 0.0f, 0.0f) };
//
    Point t_tagCenter;
    
    unsigned int t_dwPolygonPtMax = (t_segments + 1) * 4;
    Point * t_pPolygonPtArr = new Point[t_dwPolygonPtMax];
    Point * t_thisPolygonPt = t_pPolygonPtArr;
    
    //左上角
    t_tagCenter.x = t_minX + t_radius;
    t_tagCenter.y = t_maxY - t_radius;
    t_thisVertices    = t_vertices;
    
    for(unsigned int i = 0; i <= t_segments; ++i, ++t_thisPolygonPt, ++t_thisVertices)
    {
        t_thisPolygonPt->x    = t_tagCenter.x - t_thisVertices->x;
        t_thisPolygonPt->y    = t_tagCenter.y + t_thisVertices->y;
    }
    
    //右上角
    t_tagCenter.x        = t_maxX - t_radius;
    t_tagCenter.y        = t_maxY - t_radius;
    t_thisVertices    = t_vertices + t_segments;
    for(unsigned int i = 0; i <= t_segments; ++i, ++t_thisPolygonPt, --t_thisVertices)
    {
        t_thisPolygonPt->x    = t_tagCenter.x + t_thisVertices->x;
        t_thisPolygonPt->y    = t_tagCenter.y + t_thisVertices->y;
    }
    //右下角
    t_tagCenter.x        = t_maxX - t_radius;
    t_tagCenter.y        = t_minY + t_radius;
    t_thisVertices    = t_vertices;
    for(unsigned int i = 0; i <= t_segments; ++i, ++t_thisPolygonPt, ++t_thisVertices)
    {
        t_thisPolygonPt->x    = t_tagCenter.x + t_thisVertices->x;
        t_thisPolygonPt->y    = t_tagCenter.y - t_thisVertices->y;
    }
    //左下角
    t_tagCenter.x        = t_minX + t_radius;
    t_tagCenter.y        = t_minY + t_radius;
    t_thisVertices    = t_vertices + t_segments;
    for(unsigned int i = 0; i <= t_segments; ++i, ++t_thisPolygonPt, --t_thisVertices)
    {
        t_thisPolygonPt->x    = t_tagCenter.x - t_thisVertices->x;
        t_thisPolygonPt->y    = t_tagCenter.y - t_thisVertices->y;
    }
    
    t_drawNode->drawPolygon( t_pPolygonPtArr, t_dwPolygonPtMax, Color4F(1, 0, 0, 1), 1, Color4F(0, 1, 0, 1));
    
    CC_SAFE_DELETE_ARRAY( t_vertices );
    CC_SAFE_DELETE_ARRAY( t_pPolygonPtArr );
    
//    thisVertices    = vertices;
    
//
//    t_points[0] = t_origin;
//    t_points[1] = t_origin + Vec2( t_destination.x - t_origin.x, 0.0f );
//    t_points[2] = t_destination;
//    t_points[3] = t_origin + Vec2( 0, t_destination.y - t_origin.y );

//    drawNode->drawPolygon(t_points, 4, Color4F(1, 0, 0, 1), 1, Color4F(0, 1, 0, 1));


    //圆点
    // drawNode->drawDot(Vec2(50, 50), 10, Color4F::RED);
    
    
    //线段
    // drawNode->drawSegment(Vec2(20, 100), Vec2(100, 100), 5, Color4F(0, 1, 0, 1));
    // drawNode->drawSegment(Vec2(20, 150), Vec2(100, 150), 10, Color4F(0, 0, 1, 1));
    
    
    //三角形
    // drawNode->drawTriangle(Vec2(20, 250), Vec2(100, 300), Vec2(50, 200), Color4F(1, 1, 0, 1));
    
    
//    //实心多边形
//     Vec2 point1[4];
//     point1[0] = Vec2(150, 50);
//     point1[1] = Vec2(150, 150);
//     point1[2] = Vec2(250, 150);
//     point1[3] = Vec2(250, 50);
//     drawNode->drawPolygon(point1, 4, Color4F(1, 0, 0, 1), 1, Color4F(0, 1, 0, 1));
//
//    //空心多边形
//     Vec2 point2[4];
//     point2[0] = Vec2(150, 200);
//     point2[1] = Vec2(150, 300);
//     point2[2] = Vec2(250, 300);
//     point2[3] = Vec2(250, 200);
//     drawNode->drawPolygon(point2, 4, Color4F(1, 0, 0, 0), 1, Color4F(0, 1, 0, 1));
    
    
    // //二次贝塞尔
    // Vec2 from1 = Vec2(300, 20);
    // Vec2 to1 = Vec2(450, 20);
    // Vec2 control = Vec2(360, 100);
    // drawNode->drawQuadraticBezier(from1, control, to1, 100, Color4F::ORANGE);
    
    // //三次贝塞尔
    // Vec2 from2 = Vec2(300, 100);
    // Vec2 to2 = Vec2(450, 100);
    // Vec2 control1 = Vec2(350, 0);
    // Vec2 control2 = Vec2(400, 200);
    // drawNode->drawCubicBezier(from2, control1, control2, to2, 100, Color4F::YELLOW);
    
    
    //颜色混合测试
    // BlendFunc bl = { GL_ONE, GL_ONE };
    // drawNode->setBlendFunc(bl);
    
    // drawNode->drawSegment(Vec2(300, 250), Vec2(450, 250), 10, Color4F::GREEN);
    // drawNode->drawTriangle(Vec2(300, 200), Vec2(400, 300), Vec2(450, 150), Color4F::RED);
    //

    return true;
}

