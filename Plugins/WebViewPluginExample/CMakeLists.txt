project(WebViewPluginExample VERSION 0.0.1)

message(${choc_SOURCE_DIR})
message(${fmt_SOURCE_DIR})
message(${clap-juce-extensions_SOURCE_DIR})

set (BaseTargetName WebViewPluginExample)

juce_add_plugin("${BaseTargetName}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "MyCompany"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE Juce
        PLUGIN_CODE NPtm
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "WebViewPluginExample")

clap_juce_extensions_plugin(TARGET "${BaseTargetName}"
        CLAP_ID "com.my-cool-plugs.my-target"
        CLAP_FEATURES audio-effect filter mono stereo)

juce_add_binary_data(WebViewBundleData
        HEADER_NAME "WebViewBundleData.h"
        NAMESPACE "WebView"
        SOURCES
                WebView/view_hello.html
                WebView/view_webgl.html
                WebView/view_threejs.html
                WebView/view_vrm.html
                WebView/view_keyboard_threejs.html
                WebView/view_babylonjs.html
        )

target_sources(${BaseTargetName} PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        )

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

# Use PUBLIC to effect linked target like VST3, Standalone, CLAP, ...
target_compile_features(${BaseTargetName}
    PUBLIC
        cxx_std_17)

target_include_directories(${BaseTargetName} PUBLIC
        ${choc_SOURCE_DIR}
)

target_link_libraries(${BaseTargetName}
    PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_audio_devices
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        fmt::fmt
        WebViewBundleData
)
