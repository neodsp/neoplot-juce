#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PlotMouseInteraction.h"
#include "PlotSettings.h"

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class PlotMouseLabel : public juce::Component
{
public:
    PlotMouseLabel(const PlotSettings<T>& settings,
                   const PlotMouseInteraction<T>& mouseInteraction)
        : m_settings(settings)
        , m_mouseInteraction(mouseInteraction)
    {
    }

    void paint(juce::Graphics& g) override
    {
        if (m_mouseInteraction.getDrawMouseLabel() && m_settings.mouseLabel)
        {
            auto mouseLabelBounds = getLocalBounds();
            g.setColour(m_settings.style.mouseLabelText);
            const auto fontsize = m_settings.style.mouseLabelTextSize;
            g.setFont(fontsize);
            const auto mousePos = m_mouseInteraction.getMousePosition().toFloat();
            auto xVal = getXValue(static_cast<T>(mousePos.getX()), m_settings);
            auto yVal = getYValue(static_cast<T>(mousePos.getY()), m_settings);
            g.drawFittedText("y: " + getStringForValue(yVal, m_settings.yUnit, 2),
                             mouseLabelBounds.removeFromBottom(fontsize),
                             juce::Justification::centredLeft,
                             1);
            g.drawFittedText("x: " + getStringForValue(xVal, m_settings.xUnit, 2),
                             mouseLabelBounds.removeFromBottom(fontsize),
                             juce::Justification::centredLeft,
                             1);
        }
    }

private:
    const PlotSettings<T>& m_settings;
    const PlotMouseInteraction<T>& m_mouseInteraction;
};
} // namespace neo::plot
