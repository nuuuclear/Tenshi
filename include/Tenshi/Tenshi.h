#pragma once

#ifdef _WIN32
# define DLL_EXPORT_TYP __declspec(dllexport)
#else
# define DLL_EXPORT_TYP
#endif

#include "../src/Engine.h"
#include "../src/Log.h"
#include "../src/Input.h"
#include "../src/AssetManager.h"
#include "../src/Camera.h"
#include "../src/Collider.h"
#include "../src/Tilemap.h"
#include "../src/Font.h"
#include "../src/Text.h"
#include "../src/Background.h"
#include "../src/ResourceLoading.h"
#include "../src/Audio.h"
#include "../src/Scene.h"
#include "../src/Game.h"
#include "../src/Player.h"
#include "../src/Subroutine.h"