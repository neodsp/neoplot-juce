#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PlotSettings.h"
#include "PlotData.h"

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class LegendButton : public juce::Component
{
public:
    explicit LegendButton(const PlotSettings<T>& settings, PlotData<T>& data)
        : m_settings(settings)
        , m_data(data)
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(m_data.clr);
        if (m_data.visible)
        {
            g.fillEllipse(m_circleBounds.reduced(2));
        }
        else
        {
            g.drawEllipse(m_circleBounds.reduced(3), 2.f);
        }

        g.setColour(m_settings.style.legendText);
        if (m_data.hovered)
        {
            g.setColour(m_settings.style.legendTextHovered);
            g.drawEllipse(m_circleBounds.reduced(1), 1.f);
        }
        g.setFont(m_settings.style.legendFontSize);
        g.drawFittedText(m_data.name, m_textBounds, juce::Justification::centredRight, 1);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();

        const auto circleSize = static_cast<float>(getHeight());

        m_textBounds = bounds.removeFromLeft(getWidth() - circleSize - 5);
        bounds.removeFromLeft(5);
        m_circleBounds = bounds.withHeight(circleSize).withWidth(circleSize).toFloat();
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        m_data.visible = !m_data.visible;
        getParentComponent()->getParentComponent()->repaint();
    }

    void mouseEnter(const juce::MouseEvent&) override
    {
        m_hovered = true;
        m_data.hovered = true;
        getParentComponent()->getParentComponent()->repaint();
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        m_hovered = false;
        m_data.hovered = false;
        getParentComponent()->getParentComponent()->repaint();
    }

private:
    const PlotSettings<T>& m_settings;
    PlotData<T>& m_data;
    juce::Rectangle<float> m_circleBounds;
    juce::Rectangle<int> m_textBounds;
    bool m_hovered = false;
};

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class PlotLegend : public juce::Component
{
public:
    PlotLegend(const PlotSettings<T>& settings, std::vector<PlotData<T>>& data)
        : m_settings(settings)
        , m_data(data)
    {
    }

    void paint(juce::Graphics& g) override
    {
        if (m_settings.legend && !m_data.empty())
        {
            auto legendBounds = getLocalBounds();
            g.setColour(m_settings.style.legendBackground);
            const auto rounded = 0.f;
            g.fillRoundedRectangle(legendBounds.toFloat(), rounded);
            g.setColour(m_settings.style.legendOutline);
            g.drawRoundedRectangle(legendBounds.toFloat(), rounded, 1.f);
        }
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        int y = 5;
        for (auto& button: m_buttons)
        {
            button->setBounds(5, y, getWidth() - 10, 20);
            y += 25;
        }
    }

    void dataAdded()
    {
        m_buttons.clear();
        for (auto& data: m_data)
        {
            m_buttons.push_back(std::make_unique<LegendButton<T>>(m_settings, data));
        }

        for (const auto& button: m_buttons)
        {
            addAndMakeVisible(button.get());
        }
    }

    [[nodiscard]] auto getNecessaryHeight() const -> int
    {
        return 25 * m_buttons.size() + 5;
    }

    [[nodiscard]] auto getNecessaryWidth() const -> int
    {
        auto maxWidth = 0;
        for (const auto& data: m_data)
        {
            const auto f = juce::Font(20.f);
            const auto width = f.getStringWidth(data.name);
            if (width > maxWidth)
            {
                maxWidth = width;
            }
        }

        return maxWidth + 30;
    }

private:
    const PlotSettings<T>& m_settings;
    std::vector<PlotData<T>>& m_data;
    std::vector<std::unique_ptr<LegendButton<T>>> m_buttons;
};
} // namespace neo::plot
