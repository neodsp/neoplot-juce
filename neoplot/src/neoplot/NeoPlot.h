#pragma once
#include "PlotData.h"
#include "PlotGrid.h"
#include "AxisLabel.h"
#include "PlotLines.h"
#include "PlotMouseInteraction.h"
#include "PlotLegend.h"
#include "PlotMouseLabel.h"
#include "PlotOverlay.h"
#include <BinaryData.h>

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class NeoPlot : public juce::Component
{
public:
    NeoPlot()
        : m_plotLine(settings, m_data)
        , m_grid(settings)
        , m_mouseInteraction(settings, m_data)
        , m_overlay(settings, m_data)
        , m_legend(settings, m_data)
        , m_mouseLabel(settings, m_mouseInteraction)
    {
        getLookAndFeel().setDefaultSansSerifTypeface(getFont());

        addAndMakeVisible(m_grid);
        addAndMakeVisible(m_labelLeft);
        addAndMakeVisible(m_labelBottom);
        addAndMakeVisible(m_plotLine);
        addAndMakeVisible(m_mouseLabel);
        addAndMakeVisible(m_mouseInteraction);
        if (settings.legend)
        {
            addAndMakeVisible(m_legend);
        }

        addMouseListener(this, true);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(settings.style.background);
        // g.setColour(juce::Colours::white);
        // g.drawRect(getLocalBounds());
    }

    void resized() override
    {
        auto bounds = this->getLocalBounds();
        m_labelLeft.setBounds(bounds.removeFromLeft(m_labelLeft.getNecessaryWidth()));
        m_labelBottom.setBounds(
            bounds.removeFromBottom(m_labelBottom.getNecessaryHeight()));

        settings.plotBounds = bounds;

        m_grid.setBounds(settings.plotBounds);
        m_plotLine.setBounds(settings.plotBounds);
        m_mouseInteraction.setBounds(settings.plotBounds);
        m_overlay.setBounds(settings.plotBounds);

        if (settings.legend)
        {
            const auto legendBorder = 5;
            const auto legendWidth = m_legend.getNecessaryWidth();
            m_legend.setBounds(settings.plotBounds.getRight() - legendWidth
                                   - legendBorder,
                               legendBorder,
                               legendWidth,
                               m_legend.getNecessaryHeight());
        }

        m_mouseLabel.setBounds(settings.plotBounds.getX() + 10,
                               settings.plotBounds.getBottom() - 55,
                               75,
                               50);

        resizedOverlay();
    }

    virtual void resizedOverlay() {}

    void addData(PlotData<T>& data, bool fitBounds = true)
    {
        if (settings.type == PlotType::logarithmic && !data.isAlreadyWarped)
        {
            data.xData = warp(data.xData);
            data.yData = warp(data.yData);
        }
        if (settings.yAxisInDb)
        {
            plot::lin_to_db(data.yData);
        }
        m_data.push_back(data);
        m_legend.dataAdded();
        if (fitBounds)
            automaticPlotBounds(settings, m_data);
        resized();
    }

    void setDataVisible(size_t id, bool visible)
    {
        if (id < m_data.size())
        {
            m_data[id].visible = visible;
            repaint();
        }
    }

    PlotSettings<T> settings;

    auto getFont() -> juce::Typeface::Ptr
    {
        return juce::Typeface::createSystemTypefaceFor(BinaryData::JetBrainsMono_ttf,
                                                       BinaryData::JetBrainsMono_ttfSize);
    }

    auto setDefaultBounds(std::optional<T> xMin,
                          std::optional<T> xMax,
                          std::optional<T> yMin,
                          std::optional<T> yMax)
    {
        settings.xMinDefault = xMin;
        settings.xMaxDefault = xMax;
        settings.yMinDefault = yMin;
        settings.yMaxDefault = yMax;
        automaticPlotBounds(settings, m_data);
        repaint();
    }

private:
    PlotLines<T> m_plotLine;
    PlotGrid<T> m_grid;
    AxisLabel<T> m_labelBottom {settings, m_grid, AxisLabel<T>::AxisLabelType::XBottom};
    AxisLabel<T> m_labelLeft {settings, m_grid, AxisLabel<T>::AxisLabelType::YLeft};
    PlotMouseInteraction<T> m_mouseInteraction;
    PlotLegend<T> m_legend;
    PlotMouseLabel<T> m_mouseLabel;
    PlotOverlay<T> m_overlay;
    std::vector<PlotData<T>> m_data;
};
} // namespace neo::plot
