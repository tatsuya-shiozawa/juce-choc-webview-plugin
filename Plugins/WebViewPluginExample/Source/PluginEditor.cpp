#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WebViewBundleData.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    chocWebView = std::make_unique<choc::ui::WebView>();
    
    juceHwndView = std::make_unique<juce::HWNDComponent>();
    juceHwndView->setHWND(chocWebView->getViewHandle());
    addAndMakeVisible(juceHwndView.get());

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    setResizable(true, true);

    //chocWebView->navigate("http://www.google.com");

    auto web_view_function_hello_cpp =
      [safe_this = juce::Component::SafePointer(this)](const choc::value::ValueView& args) -> choc::value::Value {
      if (safe_this.getComponent() == nullptr)
      {
        return choc::value::Value(-1);
      }

      juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::WarningIcon,
        "Hello from WebView!!!", "This dialog triggered by web view!!!");

      return choc::value::Value(0);
    };

    chocWebView->bind("hello_cpp", web_view_function_hello_cpp);

    const auto html = juce::String::createStringFromData(WebView::view_html, WebView::view_htmlSize);
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
}
