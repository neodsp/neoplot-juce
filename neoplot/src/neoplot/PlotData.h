#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
struct PlotData
{
    std::vector<T> xData;
    std::vector<T> yData;
    std::vector<T> xDataReduced;
    std::vector<T> yDataReduced;
    std::vector<T> yDataReducedWaveformMin;

    juce::Colour clr;
    float lineThickness = 2.f;

    juce::String name;

    bool visible = true;
    bool isWaveform = false;
    bool hovered = false;

    explicit PlotData(std::size_t initialSize = 0,
                      juce::Colour clr_ = juce::Colours::transparentWhite)
    {
        prepare(initialSize);
        clr = clr_;
    }

    PlotData(std::vector<T>& xData_,
             std::vector<T>& yData_,
             juce::Colour clr_ = juce::Colours::transparentWhite)
    {
        xData = xData_;
        yData = yData_;
        clr = clr_;
    }

    explicit PlotData(const std::size_t numPoints) { prepare(numPoints); }

    void prepare(const std::size_t numPoints)
    {
        xDataReduced.resize(numPoints, 0.);
        yDataReduced.resize(numPoints, 0.);
        yDataReducedWaveformMin.resize(numPoints, 0.);
    }
};
} // namespace neo::plot
