#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PlotTools.h"

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class AxisLabel : public juce::Component
{
public:
    enum AxisLabelType
    {
        XBottom,
        XTop,
        YLeft,
        YRight
    };

    AxisLabel(PlotSettings<T>& settings,
              PlotGrid<T>& grid,
              AxisLabelType type,
              std::string_view title = "")
        : m_settings(settings)
        , m_grid(grid)
        , m_type(type)
        , m_title(title)
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(m_settings.style.axisLabelText);
        const auto fontSize = m_settings.style.axisLabelFontSize;
        g.setFont(fontSize);

        const auto height = fontSize;
        const auto width = height * 2;

        switch (m_type)
        {
            case AxisLabelType::XBottom:
            {
                auto xValues = m_grid.getGridValuesX();
                for (const auto& value: *xValues)
                {
                    auto x = getXPosition(value, *m_grid.getSettings()) - (width / 2.);
                    const auto area =
                        juce::Rectangle<int>(x, 5, width, height - DISTANCE);
                    g.drawFittedText(getStringForValue(value),
                                     area,
                                     juce::Justification::centredTop,
                                     1);
                }
                break;
            }
            case AxisLabelType::YLeft:
            {
                auto yValues = m_grid.getGridValuesY();
                for (const auto& value: *yValues)
                {
                    auto y = getYPosition(value, *m_grid.getSettings()) - (fontSize / 2.);
                    const auto area =
                        juce::Rectangle<int>(0, y, width - DISTANCE, fontSize);
                    g.drawFittedText(getStringForValue(value),
                                     area,
                                     juce::Justification::centredRight,
                                     1);
                }
                break;
            }
            default:
                break;
        }
    }

    auto getNecessaryHeight() -> float
    {
        switch (m_type)
        {
            case AxisLabelType::XBottom:
            case AxisLabelType::XTop:
                return m_settings.style.axisLabelFontSize + DISTANCE;
            case AxisLabelType::YLeft:
            case AxisLabelType::YRight:
                return m_settings.plotBounds.getHeight();
        }
    }

    auto getNecessaryWidth() -> float
    {
        switch (m_type)
        {
            case AxisLabelType::XBottom:
            case AxisLabelType::XTop:
                return m_settings.plotBounds.getWidth();
            case AxisLabelType::YLeft:
            case AxisLabelType::YRight:
                return m_settings.style.axisLabelFontSize * 2.f + DISTANCE;
        }
    }

    // void paint2(juce::Graphics& g)
    // {
    //     g.setColour(juce::Colours::white);
    //     switch (m_type)
    //     {
    //         case AxisLabelType::XBottom:
    //         {
    //             const auto height = getHeight() - DISTANCE;
    //             const auto width = height * 2;
    //             g.setFont(height + 3);
    //             auto xValues = m_grid.getGridValuesX();
    //             for (const auto& value: *xValues)
    //             {
    //                 auto x = getXPosition(value, *m_grid.getSettings()) - (width / 2.);
    //                 const auto area = juce::Rectangle<int>(x, 5, width, height);
    //                 g.drawFittedText(neo::getStringForValue(value),
    //                                  area,
    //                                  juce::Justification::centredTop,
    //                                  1);
    //             }
    //         }
    //         break;
    //         case AxisLabelType::XTop:
    //         {
    //             const auto height = getHeight() - DISTANCE;
    //             const auto width = height * 2;
    //             const auto fontSize = height;
    //             g.setFont(fontSize);
    //             g.setColour(juce::Colours::white);
    //             auto xValues = m_grid.getGridValuesX();
    //             for (const auto& value: *xValues)
    //             {
    //                 auto x = getXPosition(value, *m_grid.getSettings()) - (width / 2.);
    //                 const auto area = juce::Rectangle<int>(x, 0, width, height);
    //                 g.drawFittedText(neo::getStringForValue(value),
    //                                  area,
    //                                  juce::Justification::centredBottom,
    //                                  1);
    //             }
    //         }
    //         break;
    //         case AxisLabelType::YLeft:
    //         {
    //             const auto width = getWidth() - DISTANCE;
    //             const auto fontSize = width * 0.5f;
    //             g.setFont(fontSize);
    //             g.setColour(juce::Colours::white);
    //             auto yValues = m_grid.getGridValuesY();
    //             for (const auto& value: *yValues)
    //             {
    //                 auto y = getYPosition(value, *m_grid.getSettings()) - (fontSize / 2.);
    //                 const auto area = juce::Rectangle<int>(0, y, width, fontSize);
    //                 g.drawFittedText(neo::getStringForValue(value),
    //                                  area,
    //                                  juce::Justification::centredRight,
    //                                  1);
    //             }
    //         }
    //         break;
    //         case AxisLabelType::YRight:
    //         {
    //             const auto width = getWidth() - DISTANCE;
    //             const auto fontSize = width * 0.5f;
    //             g.setFont(fontSize);
    //             g.setColour(juce::Colours::white);
    //             auto yValues = m_grid.getGridValuesY();
    //             for (const auto& value: *yValues)
    //             {
    //                 auto y = getYPosition(value, *m_grid.getSettings()) - (fontSize / 2.);
    //                 const auto area = juce::Rectangle<int>(DISTANCE, y, width, fontSize);
    //                 g.drawFittedText(neo::getStringForValue(value),
    //                                  area,
    //                                  juce::Justification::centredLeft,
    //                                  1);
    //             }
    //         }
    //         break;
    //         default:
    //             assert(false);
    //             break;
    //     }
    // }

private:
    const PlotSettings<T>& m_settings;
    static constexpr int DISTANCE = 5;
    AxisLabelType m_type;
    PlotGrid<T>& m_grid;
    std::string m_title;
};
} // namespace neo::plot
