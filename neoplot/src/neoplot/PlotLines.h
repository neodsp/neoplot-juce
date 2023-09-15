#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PlotSettings.h"
#include "PlotData.h"
#include "PlotTools.h"
#include "PlotMouseInteraction.h"

namespace neo::plot
{
// for same fontsize Y Label should have 2 * width of X Label height
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class PlotLines : public juce::Component
{
public:
    explicit PlotLines(const PlotSettings<T>& settings, std::vector<PlotData<T>>& data)
        : m_settings(settings)
        , m_data(data)
    {
    }

    void paint(juce::Graphics& g) override
    {
        for (auto& data: m_data)
        {
            if (data.visible)
            {
                if (data.hovered)
                {
                    g.setColour(data.clr);
                }
                else
                {
                    g.setColour(data.clr.withAlpha(0.8f));
                }
                juce::Path dataPath;
                const auto numDataPoints = calculateNumDataPoints(m_settings, data);
                if (numDataPoints > m_settings.plotBounds.getWidth())
                {
                    transformData(m_settings, data);

                    if (data.isWaveform)
                    {
                        startSubPath(dataPath,
                                     m_settings,
                                     data.xDataReduced,
                                     data.yDataReducedWaveformMin,
                                     0);
                        for (std::size_t i = 0; i < data.xDataReduced.size(); ++i)
                        {
                            // const auto x = getXPosition(data.xDataReduced[i], m_settings);
                            // const auto yMax =
                            //     getYPosition(data.yDataReduced[i], m_settings);
                            // const auto yMin =
                            //     getYPosition(data.yDataReducedWaveformMin[i], m_settings);
                            // g.drawVerticalLine(x, yMax, yMin);

                            addToPath(dataPath,
                                      m_settings,
                                      data.xDataReduced,
                                      data.yDataReducedWaveformMin,
                                      i);

                            addToPath(dataPath,
                                      m_settings,
                                      data.xDataReduced,
                                      data.yDataReduced,
                                      i);
                        }
                        g.strokePath(dataPath, juce::PathStrokeType(data.lineThickness));
                    }
                    else
                    {
                        startSubPath(dataPath,
                                     m_settings,
                                     data.xDataReduced,
                                     data.yDataReduced,
                                     0);
                        for (std::size_t i = 0; i < data.xDataReduced.size(); ++i)
                        {
                            addToPath(dataPath,
                                      m_settings,
                                      data.xDataReduced,
                                      data.yDataReduced,
                                      i);
                        }

                        g.strokePath(dataPath, juce::PathStrokeType(data.lineThickness));
                    }
                }
                else
                {
                    int start =
                        findClosestElementIndexSorted(data.xData, m_settings.xMin);
                    int end = findClosestElementIndexSorted(data.xData, m_settings.xMax);

                    start = std::clamp(start - 1, 0, int(data.xData.size()));
                    end = std::clamp(end + 2, 0, int(data.xData.size()));

                    startSubPath(dataPath, m_settings, data.xData, data.yData, start);
                    for (auto i = static_cast<size_t>(start);
                         i < static_cast<size_t>(end);
                         ++i)
                    {
                        addToPath(dataPath, m_settings, data.xData, data.yData, i);
                    }

                    g.strokePath(dataPath, juce::PathStrokeType(data.lineThickness));
                }
            }
        }
    }

    void resized() override
    {
        for (auto& data: m_data)
        {
            data.prepare(m_settings.plotBounds.getWidth());
        }
    }

private:
    const PlotSettings<T>& m_settings;
    std::vector<PlotData<T>>& m_data;
};
} // namespace neo::plot
