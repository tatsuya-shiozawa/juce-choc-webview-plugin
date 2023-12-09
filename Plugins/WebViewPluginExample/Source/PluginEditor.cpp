#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WebViewBundleData.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    choc::ui::WebView::Options options;
    options.enableDebugMode = true;
    chocWebView = std::make_unique<choc::ui::WebView>(options);
    
    juceHwndView = std::make_unique<juce::HWNDComponent>();
    juceHwndView->setHWND(chocWebView->getViewHandle());
    addAndMakeVisible(juceHwndView.get());

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    setResizable(true, true);

    //chocWebView->navigate("http://www.google.com");

    auto web_view_on_click_button =
      [safe_this = juce::Component::SafePointer(this)](const choc::value::ValueView& args) -> choc::value::Value {

      auto message = "web_view_on_click_button() called with args: " + choc::json::toString(args);
      juce::Logger::outputDebugString(choc::json::toString(args));

      auto json = juce::JSON::parse(choc::json::toString(args));
      juce::Logger::outputDebugString(juce::JSON::toString(json));

      if (safe_this.getComponent() == nullptr)
      {
        return choc::value::Value(-1);
      }

      juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::WarningIcon,
        "onClickButton is called from WebView!!!", message);

      return choc::value::Value(0);
    };

    auto web_view_on_sliider_changed =
      [safe_this = juce::Component::SafePointer(this)](const choc::value::ValueView& args) -> choc::value::Value {

      auto message = "web_view_on_sliider_changed() called with args: " + choc::json::toString(args);
      juce::Logger::outputDebugString(choc::json::toString(args));

      auto json = juce::JSON::parse(choc::json::toString(args));
      juce::Logger::outputDebugString(juce::JSON::toString(json));

      if (safe_this.getComponent() == nullptr)
      {
        return choc::value::Value(-1);
      }

      return choc::value::Value(0);
      };

    chocWebView->bind("onClickButton", web_view_on_click_button);
    chocWebView->bind("onSliderChanged", web_view_on_sliider_changed);

    const auto html = juce::String::createStringFromData(WebView::view_hello_html, WebView::view_hello_htmlSize);
    chocWebView->setHTML(html.toStdString());


}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

  juceHwndView->setBounds(getLocalBounds());

  juce::MessageManager::callAsync([safe_this = juce::Component::SafePointer(this)]() {
    safe_this->chocWebView->evaluateJavascript("onCallbackFromCpp()");
    }
  );
}