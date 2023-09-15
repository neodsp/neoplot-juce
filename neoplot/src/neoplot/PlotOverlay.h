#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PlotSettings.h"
#include "PlotData.h"

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class PlotOverlay : public juce::Component
{
public:
    PlotOverlay(PlotSettings<T>& settings, std::vector<PlotData<T>>& data)
        : m_settings(settings)
        , m_data(data)
    {
    }

private:
    PlotSettings<T>& m_settings;
    std::vector<PlotData<T>>& m_data;
};
} // namespace neo::plot
