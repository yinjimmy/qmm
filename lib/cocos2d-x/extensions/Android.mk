LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := cocos_extension_static

LOCAL_MODULE_FILENAME := libextension

LOCAL_SRC_FILES := \
    GUI/CCControlExtension/CCControl.cpp \
    GUI/CCControlExtension/CCControlButton.cpp \
    GUI/CCControlExtension/CCInvocation.cpp \
    GUI/CCControlExtension/CCScale9Sprite.cpp \
    GUI/CCControlExtension/CCControlUtils.cpp \
    GUI/CCEditBox/CCEditBox.cpp \
    GUI/CCEditBox/CCEditBoxImplAndroid.cpp \
    ProjectConfig/SimulatorConfig.cpp \
    GUI/CCControlExtension/CCControlColourPicker.cpp \
    GUI/CCControlExtension/CCControlHuePicker.cpp \
    GUI/CCControlExtension/CCControlPotentiometer.cpp \
    GUI/CCControlExtension/CCControlSaturationBrightnessPicker.cpp \
    GUI/CCControlExtension/CCControlSlider.cpp \
    GUI/CCControlExtension/CCControlStepper.cpp \
    GUI/CCControlExtension/CCControlSwitch.cpp \
    GUI/CCScrollView/CCScrollView.cpp \
    GUI/CCScrollView/CCSorting.cpp \
    GUI/CCScrollView/CCTableView.cpp \
    GUI/CCScrollView/CCTableViewCell.cpp \
    network/WebSocket.cpp

ifeq ($(CC_DRAGONBONES_ENABLED),1)
LOCAL_SRC_FILES += \
    DragonBones/animation/Animation.cpp \
    DragonBones/animation/AnimationState.cpp \
    DragonBones/animation/TimelineState.cpp \
    DragonBones/Armature.cpp \
    DragonBones/Bone.cpp \
    DragonBones/core/DBObject.cpp \
    DragonBones/core/DragonBones.cpp \
    DragonBones/events/AnimationEvent.cpp \
    DragonBones/events/ArmatureEvent.cpp \
    DragonBones/events/Event.cpp \
    DragonBones/events/FrameEvent.cpp \
    DragonBones/events/SoundEvent.cpp \
    DragonBones/events/SoundEventManager.cpp \
    DragonBones/factories/BaseFactory.cpp \
    DragonBones/objects/dbtinyxml2.cpp \
    DragonBones/objects/DisplayData.cpp \
    DragonBones/objects/TransformTimeline.cpp \
    DragonBones/objects/XMLDataParser.cpp \
    DragonBones/renderer/CCDBFactory.cpp \
    DragonBones/renderer/CCDragonBones.cpp \
    DragonBones/renderer/CCDBAtlasNode.cpp \
    DragonBones/renderer/CCDBDisplayBridge.cpp \
    DragonBones/renderer/CCDBManager.cpp \
    DragonBones/renderer/CCDBTextureAtlas.cpp \
    DragonBones/Slot.cpp \
    DragonBones/utils/BytesType.cpp \
    DragonBones/utils/ConstValues.cpp \
    DragonBones/utils/DBDataUtil.cpp \
    DragonBones/utils/DBGeometry.cpp \
    DragonBones/utils/TransformUtil.cpp
endif

ifeq ($(CC_FILTERS_ENABLED),1)
LOCAL_SRC_FILES += \
    filters/filters/CCFilter.cpp \
    filters/filters/CCBlurFilter.cpp \
    filters/filters/CCBrightnessFilter.cpp \
    filters/filters/CCContrastFilter.cpp \
    filters/filters/CCDropShadowFilter.cpp \
    filters/filters/CCExposureFilter.cpp \
    filters/filters/CCGammaFilter.cpp \
    filters/filters/CCGrayFilter.cpp \
    filters/filters/CCHazeFilter.cpp \
    filters/filters/CCHueFilter.cpp \
    filters/filters/CCMaskFilter.cpp \
    filters/filters/CCRGBFilter.cpp \
    filters/filters/CCSaturationFilter.cpp \
    filters/filters/CCSepiaFilter.cpp \
    filters/filters/CCSharpenFilter.cpp \
    filters/filters/CCTestFilter.cpp \
    filters/nodes/CCFilteredSprite.cpp \
    filters/shaders/ccFilterShaders.cpp
endif

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

ifeq ($(CC_PHYSICS_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_chipmunk_static
endif

ifeq ($(CC_CURL_ENABLED),1)
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
endif

LOCAL_WHOLE_STATIC_LIBRARIES += libwebsockets_static


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/GUI/CCControlExtension \
                           $(LOCAL_PATH)/GUI/CCEditBox \
                           $(LOCAL_PATH)/ProjectConfig \
                           $(LOCAL_PATH)/network \
                           $(LOCAL_PATH)/Components \
                           $(LOCAL_PATH)/GUI/CCScrollView \
                           $(LOCAL_PATH)/CCArmature \
                           $(LOCAL_PATH)/CCArmature/animation \
                           $(LOCAL_PATH)/CCArmature/datas \
                           $(LOCAL_PATH)/CCArmature/display \
                           $(LOCAL_PATH)/CCArmature/external_tool \
                           $(LOCAL_PATH)/CCArmature/external_tool/Json \
                           $(LOCAL_PATH)/CCArmature/external_tool/Json/lib_json \
                           $(LOCAL_PATH)/CCArmature/utils \
                           $(LOCAL_PATH)/filters \
                           $(LOCAL_PATH)/DragonBones


LOCAL_C_INCLUDES = $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting/lua/proj.android)
$(call import-module,external/libwebsockets/android)
