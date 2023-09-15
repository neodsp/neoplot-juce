#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <BinaryData.h>

namespace neo::plot
{
struct PlotStyle
{
    // PLOT
    juce::Colour background = juce::Colours::black;
    juce::Colour grid = juce::Colours::white.withBrightness(0.2f);
    juce::Colour zeroLines = juce::Colours::white.withBrightness(0.6f);
    // LEGEND
    juce::Colour legendBackground = juce::Colours::black.withAlpha(0.4f);
    juce::Colour legendOutline = juce::Colours::white;
    juce::Colour legendText = juce::Colours::white.withBrightness(0.6f);
    juce::Colour legendTextHovered = juce::Colours::white;
    float legendFontSize = 20.f;
    // MOUSE LABEL
    juce::Colour mouseLabelText = juce::Colours::white;
    float mouseLabelTextSize = 15.f;
    // AXIS LABEL
    juce::Colour axisLabelText = juce::Colours::white;
    float axisLabelFontSize = 15.f;
};
} // namespace neo::plot
