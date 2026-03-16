#include "hooks/GJBaseGameLayerHook.hpp"
#include "GlobalSettings.h"
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

$execute {
    GlobalSettings::isModEnabled = Mod::get()->getSettingValue<bool>("mod-enabled");

    listenForSettingChanges<bool>("mod-enabled", [](bool value) {
        GlobalSettings::isModEnabled = value;
    });
}