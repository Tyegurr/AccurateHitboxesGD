#include "utils.h"
#include "ids.h"
#include <cmath>

// very lazy
#define PI 3.14159265359f

bool isPointInsideRect(cocos2d::CCPoint pt, cocos2d::CCRect rect)
{
    return (pt.x >= rect.origin.x && pt.x <= (rect.origin.x + rect.size.width)) && (pt.y >= rect.origin.y && pt.y <= (rect.origin.y + rect.size.height));
}
float deg2rad(float degrees)
{
    return degrees * (PI / 180.f);
}

// stay in school or this could be you
// thanks https://stackoverflow.com/a/2049593
float signPt(cocos2d::CCPoint p1, cocos2d::CCPoint p2, cocos2d::CCPoint p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
bool pointIsInTriangle(cocos2d::CCPoint pt, cocos2d::CCPoint v1, cocos2d::CCPoint v2, cocos2d::CCPoint v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = signPt(pt, v1, v2);
    d2 = signPt(pt, v2, v3);
    d3 = signPt(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}


bool isGameObjectASpike(GameObject* gObj)
{
    return basicSpikeIDs.contains(gObj->m_objectID);
}
bool isGameObjectASaw(GameObject* gObj)
{
    return allSawIDs.contains(gObj->m_objectID);
}

bool isPlayerInsideBasicSpikeShape(PlayerObject* player, GameObject* gObj)
{
    cocos2d::CCPoint playerPos = player->getPosition();
    cocos2d::CCPoint gObjPos = gObj->getPosition();
    cocos2d::CCSize gObjContSize = gObj->getContentSize();
    gObjContSize.width *= gObj->getScaleX();
    gObjContSize.height *= gObj->getScaleY();
    float gObjRotation = deg2rad(gObj->getRotation());
    
    float playerSize = 14;
    if (player->m_vehicleSize < 1.0f)
    {
        playerSize = 7;
    }
    cocos2d::CCRect playerRect(playerPos.x - playerSize, playerPos.y - playerSize, playerSize * 2, playerSize * 2);

    cocos2d::CCPoint spikeTopPointUnmoved(0, gObjContSize.height / 2);
    cocos2d::CCPoint spikeBottomLeftUnmoved(gObjContSize.width / -2, gObjContSize.height / -2);
    cocos2d::CCPoint spikeBottomRightUnmoved(gObjContSize.width / 2, gObjContSize.height / -2);
    cocos2d::CCPoint spikeTopPoint(
        gObjPos.x + ((spikeTopPointUnmoved.x * cosf(gObjRotation)) + (spikeTopPointUnmoved.y * sinf(gObjRotation))),
        gObjPos.y + ((-spikeTopPointUnmoved.x * sinf(gObjRotation)) + (spikeTopPointUnmoved.y * cosf(gObjRotation)))
    );
    cocos2d::CCPoint spikeBottomLeftPoint(
        gObjPos.x + ((spikeBottomLeftUnmoved.x * cosf(gObjRotation)) + (spikeBottomLeftUnmoved.y * sinf(gObjRotation))),
        gObjPos.y + ((-spikeBottomLeftUnmoved.x * sinf(gObjRotation)) + (spikeBottomLeftUnmoved.y * cosf(gObjRotation)))
    );
    cocos2d::CCPoint spikeBottomRightPoint(
        gObjPos.x + ((spikeBottomRightUnmoved.x * cosf(gObjRotation)) + (spikeBottomRightUnmoved.y * sinf(gObjRotation))),
        gObjPos.y + ((-spikeBottomRightUnmoved.x * sinf(gObjRotation)) + (spikeBottomRightUnmoved.y * cosf(gObjRotation)))
    );

    // checking point and rects
    if (isPointInsideRect(spikeTopPoint, playerRect)) return true;
    if (isPointInsideRect(spikeBottomLeftPoint, playerRect)) return true;
    if (isPointInsideRect(spikeBottomRightPoint, playerRect)) return true;

    // specific points now
    cocos2d::CCPoint playerBottomRight(playerRect.origin.x + playerRect.size.width, playerRect.origin.y + playerRect.size.height);
    cocos2d::CCPoint playerTopLeft(playerRect.origin.x, playerRect.origin.y);
    cocos2d::CCPoint playerBottomLeft(playerRect.origin.x, playerRect.origin.y + playerRect.size.height);
    cocos2d::CCPoint playerTopRight(playerRect.origin.x + playerRect.size.width, playerRect.origin.y);

    if (pointIsInTriangle(playerBottomRight, spikeTopPoint, spikeBottomLeftPoint, spikeBottomRightPoint)) return true;
    if (pointIsInTriangle(playerTopLeft, spikeTopPoint, spikeBottomLeftPoint, spikeBottomRightPoint)) return true;
    if (pointIsInTriangle(playerBottomLeft, spikeTopPoint, spikeBottomLeftPoint, spikeBottomRightPoint)) return true;
    if (pointIsInTriangle(playerTopRight, spikeTopPoint, spikeBottomLeftPoint, spikeBottomRightPoint)) return true;

    return false;
}
bool isPlayerInsideSawShape(PlayerObject* player, GameObject* gObj)
{
    cocos2d::CCPoint playerPos = player->getPosition();
    cocos2d::CCPoint gObjPos = gObj->getPosition();
    cocos2d::CCSize gObjContSize = gObj->getContentSize();

    float averageSize = (gObjContSize.width + gObjContSize.height) / 2;
    float distFromSaw = gObjPos.getDistance(playerPos);

    return distFromSaw <= (averageSize / 1.5f);
}


// debug draw functions
const float kDrawThickness = 0.5f;
void drawSpikeHitboxFor(GameObject* gObj, cocos2d::CCDrawNode* drawNode)
{
    CCPoint gObjPos = gObj->getPosition();
    CCSize gObjContSize = gObj->getContentSize();
    gObjContSize.width *= gObj->getScaleX();
    gObjContSize.height *= gObj->getScaleY();
    float gObjRotation = deg2rad(gObj->getRotation());

    cocos2d::CCPoint spikeTopPointUnmoved(0, gObjContSize.height / 2);
    cocos2d::CCPoint spikeBottomLeftUnmoved(gObjContSize.width / -2, gObjContSize.height / -2);
    cocos2d::CCPoint spikeBottomRightUnmoved(gObjContSize.width / 2, gObjContSize.height / -2);

    CCPoint verts[] = {
        CCPoint(
            gObjPos.x + ((spikeTopPointUnmoved.x * cosf(gObjRotation)) + (spikeTopPointUnmoved.y * sinf(gObjRotation))),
            gObjPos.y + ((-spikeTopPointUnmoved.x * sinf(gObjRotation)) + (spikeTopPointUnmoved.y * cosf(gObjRotation)))
        ),
        CCPoint(
            gObjPos.x + ((spikeBottomLeftUnmoved.x * cosf(gObjRotation)) + (spikeBottomLeftUnmoved.y * sinf(gObjRotation))),
            gObjPos.y + ((-spikeBottomLeftUnmoved.x * sinf(gObjRotation)) + (spikeBottomLeftUnmoved.y * cosf(gObjRotation)))
        ),
        CCPoint(
            gObjPos.x + ((spikeBottomRightUnmoved.x * cosf(gObjRotation)) + (spikeBottomRightUnmoved.y * sinf(gObjRotation))),
            gObjPos.y + ((-spikeBottomRightUnmoved.x * sinf(gObjRotation)) + (spikeBottomRightUnmoved.y * cosf(gObjRotation)))
        )
    };
    //drawNode->drawDot(gObj->getPosition(), 2.f, { 1.f, 0.f, 0.f, 1.f });
    drawNode->drawPolygon(verts, 3, { 0.f, 0.f, 0.f, 0.f }, kDrawThickness, { 1.f, 0.f, 0.f, 1.f });
}
void drawSawHitboxFor(GameObject* gObj, CCDrawNode* drawNode)
{
    cocos2d::CCPoint gObjPos = gObj->getPosition();
    cocos2d::CCSize gObjContSize = gObj->getContentSize();
    float averageSize = (gObjContSize.width + gObjContSize.height) / 2;

    drawNode->drawCircle(gObjPos, averageSize / 2, { 1.f, 0.f, 0.f, 0.2f }, kDrawThickness, { 1.f, 0.f, 0.f, 1.f }, 16);
}