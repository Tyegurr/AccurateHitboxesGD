#pragma once
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/cocos/draw_nodes/CCDrawNode.h>

using namespace cocos2d;

bool isPointInsideRect(cocos2d::CCPoint pt, cocos2d::CCRect rect);
float deg2rad(float degrees);

float signPt(cocos2d::CCPoint p1, cocos2d::CCPoint p2, cocos2d::CCPoint p3);
bool pointIsInTriangle(cocos2d::CCPoint pt, cocos2d::CCPoint v1, cocos2d::CCPoint v2, cocos2d::CCPoint v3);
bool isGameObjectASpike(GameObject* gObj);
bool isGameObjectASaw(GameObject* gObj);

bool isPlayerInsideBasicSpikeShape(PlayerObject* player, GameObject* gObj);
bool isPlayerInsideSawShape(PlayerObject* player, GameObject* gObj);


// debug draw functions
void drawSpikeHitboxFor(GameObject* gObj, cocos2d::CCDrawNode* drawNode);
void drawSawHitboxFor(GameObject* gObj, CCDrawNode* drawNode);