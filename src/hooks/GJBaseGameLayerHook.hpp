#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "../ids.h"
#include "../utils.h"
#include "../GlobalSettings.h"
using namespace geode::prelude;

// small hitbox offset to account for laggy-ness ish
const float hitboxOffset = 1.f;

class $modify(GJBaseGameLayerHook, GJBaseGameLayer)
{
    void updateDebugDraw() {
        GJBaseGameLayer::updateDebugDraw();

        if (GlobalSettings::isModEnabled == false) return;

        for (int i = 0; i < m_activeObjectsCount; i++)
        {
            GameObject* gObj = m_activeObjects.at(i);
            if (basicSpikeIDs.contains(gObj->m_objectID))
            {
                drawSpikeHitboxFor(gObj, m_debugDrawNode);
            } else if (allSawIDs.contains(gObj->m_objectID))
            {
                drawSawHitboxFor(gObj, m_debugDrawNode);
            }
        }
    }
    void collisionCheckObjects(PlayerObject* object, gd::vector<GameObject*>* objects, int objectCount, float dt) {
        GJBaseGameLayer::collisionCheckObjects(object, objects, objectCount, dt);
        
        if (GlobalSettings::isModEnabled == false) return;

        for (int i = 0; i < objectCount; i++)
        {
            GameObject* gObj = objects->at(i);

            if (gObj->m_isActivated == false) continue;

            CCPoint playerPos = object->getPosition();
            CCPoint objectPos = gObj->getPosition();
            CCSize objectContentSize = gObj->getContentSize();

            // i have found working with solids/boxes to be difficult so we will not include them right now.
            switch (gObj->m_objectType)
            {
            case GameObjectType::Hazard:
                if (isGameObjectASpike(gObj))
                {
                    if (basicSpikeIDs.contains(gObj->m_objectID))
                    {
                        if (isPlayerInsideBasicSpikeShape(object, gObj))
                            destroyPlayer(object, gObj);
                    }
                } else if (isGameObjectASaw(gObj))
                {
                    if (isPlayerInsideSawShape(object, gObj))
                        destroyPlayer(object, gObj);
                }
                break;
            default:
                break;
            }
        }
    }
};