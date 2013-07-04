require "helper"

require "ActionsTest.ActionsTest"
require "TransitionsTest.TransitionsTest"
require "ActionsProgressTest.ActionsProgressTest"
require "EffectsTest.EffectsTest"
require "ClickAndMoveTest.ClickAndMoveTest"
require "RotateWorldTest.RotateWorldTest"
require "ParticleTest.ParticleTest"
require "ActionsEaseTest.ActionsEaseTest"
require "MotionStreakTest.MotionStreakTest"
--require "DrawPrimitivesTest.DrawPrimitivesTest"
require "NodeTest.NodeTest"

require "PerformanceTest.PerformanceTest"

----------------------------------------------------

-- tests scene
Test_Table =
{
	"TEST_ACTIONS",
    "TEST_TRANSITIONS",
    "TEST_PROGRESS_ACTIONS",
    "TEST_EFFECTS",
    "TEST_CLICK_AND_MOVE",
    "TEST_ROTATE_WORLD",
    "TEST_PARTICLE",
    "TEST_EASE_ACTIONS",
    "TEST_MOTION_STREAK",
    "TEST_DRAW_PRIMITIVES",
    "TEST_COCOSNODE",
    "TEST_TOUCHES",
    "TEST_MENU",
    "TEST_ACTION_MANAGER",
    "TEST_LAYER",
    "TEST_SCENE",
    "TEST_PARALLAX",
    "TEST_TILE_MAP",
    "TEST_INTERVAL",
    "TEST_CHIPMUNKACCELTOUCH",
    "TEST_LABEL",
    "TEST_TEXT_INPUT",
    "TEST_SPRITE",
    "TEST_SCHEDULER",
    "TEST_RENDERTEXTURE",
    "TEST_TEXTURE2D",
    "TEST_BOX2D",
    "TEST_BOX2DBED",
    "TEST_EFFECT_ADVANCE",
    "TEST_ACCELEROMRTER",
    "TEST_KEYPAD",
    "TEST_COCOSDENSHION",
    "TEST_PERFORMANCE",
    "TEST_ZWOPTEX",
    "TEST_CURL",
    "TEST_USERDEFAULT",
    "TEST_BUGS",
    "TEST_FONTS",
    "TEST_CURRENT_LANGUAGE",
    "TEST_TEXTURECACHE",
    "TEST_EXTENSIONS",
    "TEST_SHADER",
    "TEST_MUTITOUCH",
    "TESTS_COUNT"
}

Test_Table = CreateEnumTable(Test_Table)

Test_Name =
{
    [0] = "ActionsTest",
    "TransitionsTest",
    "ActionsProgressTest",
    "EffectsTest",
    "ClickAndMoveTest",
    "RotateWorldTest",
    "ParticleTest",
    "ActionsEaseTest",
    "MotionStreakTest",
    "DrawPrimitivesTest",
    "NodeTest",
    "TouchesTest",
    "MenuTest",
    "ActionManagerTest",
    "LayerTest",
    "SceneTest",
    "ParallaxTest",
    "TileMapTest",
    "IntervalTest",
    "ChipmunkAccelTouchTest",
    "LabelTest",
    "TextInputTest",
    "SpriteTest",
    "SchdulerTest",
    "RenderTextureTest",
    "Texture2DTest",
    "Box2dTest",
    "Box2dTestBed",
    "EffectAdvancedTest",
    "Accelerometer",
    "KeypadTest",
    "CocosDenshionTest",
    "PerformanceTest",
    "ZwoptexTest",
    "CurlTest",
    "UserDefaultTest",
    "BugsTest",
    "FontTest",
    "CurrentLanguageTest",
    "TextureCacheTest",
    "ExtensionsTest",
    "ShaderTest",
    "MutiTouchTest"
}
