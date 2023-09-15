#pragma once
#include <type_traits>
#include "PlotType.h"
#include "PlotStyle.h"

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
struct PlotSettings
{
    T xMin, xMax, yMin, yMax;
    PlotType type = PlotType::linear;
    bool yAxisInDb = false;
    bool mouseInteraction = true;
    bool mouseLabel = true;
    bool legend = true;
    bool drawZeroLines = true;
    juce::Rectangle<int> plotBounds;
    juce::String xUnit, yUnit;

    PlotStyle style;

    std::optional<T> xMinDefault, xMaxDefault, yMinDefault, yMaxDefault;

    PlotSettings() = default;

    PlotSettings(T xMin_,
                 T xMax_,
                 T yMin_,
                 T yMax_,
                 juce::Rectangle<int> plotBounds_,
                 PlotType type_)
    {
        xMin = xMin_;
        xMax = xMax_;
        yMin = yMin_;
        yMax = yMax_;
        plotBounds = plotBounds_;
        type = type_;
    }

    static auto getFreqPreset() -> PlotSettings<T>
    {
        PlotSettings<T> settings;
        settings.type = PlotType::logarithmic;
        settings.yAxisInDb = true;
        settings.mouseInteraction = true;
        settings.mouseLabel = true;
        settings.legend = true;
        settings.xUnit = "Hz";
        settings.yUnit = "dB";
        settings.xMin = 20;
        settings.xMax = 20e3;
        return settings;
    }

    static auto getTimePreset() -> PlotSettings<T>
    {
        PlotSettings<T> settings;
        settings.type = PlotType::linear;
        settings.yAxisInDb = false;
        settings.mouseInteraction = true;
        settings.mouseLabel = true;
        settings.legend = true;
        settings.xUnit = "s";
        settings.yUnit = "";
        return settings;
    }

    static auto getTimeDbPreset() -> PlotSettings<T>
    {
        PlotSettings<T> settings;
        settings.type = PlotType::linear;
        settings.yAxisInDb = true;
        settings.mouseInteraction = true;
        settings.mouseLabel = true;
        settings.legend = true;
        settings.xUnit = "s";
        settings.yUnit = "dB";
        return settings;
    }
};
} // namespace neo::plot
