#pragma once
#include "PlotTools.h"

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class PlotMouseInteraction : public juce::Component
{
public:
    explicit PlotMouseInteraction(PlotSettings<T>& settings,
                                  const std::vector<PlotData<T>>& data)
        : m_settings(settings)
        , m_data(data)
    {
    }

    void mouseMove(const juce::MouseEvent& event) override
    {
        m_mousePosition.x = event.getPosition().x;
        m_mousePosition.y = event.getPosition().y;

        if (m_settings.mouseLabel)
        {
            getParentComponent()->repaint();
        }
    }

    void mouseEnter(const juce::MouseEvent&) override
    {
        if (m_settings.mouseLabel)
        {
            m_drawMouseLabel = true;
            getParentComponent()->repaint();
        }
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        if (m_settings.mouseLabel)
        {
            m_drawMouseLabel = false;
            getParentComponent()->repaint();
        }
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        if (m_settings.mouseInteraction)
        {
            if (event.mods.isShiftDown())
            {
                moveOnXAxis(m_mousePosition.x - event.getPosition().x);
            }
            else
            {
                moveOnXAxis(m_mousePosition.x - event.getPosition().x);
                moveOnYAxis(event.getPosition().y - m_mousePosition.y);
            }

            m_mousePosition = event.getPosition();
            getParentComponent()->repaint();
        }
    }

    void mouseMagnify(const juce::MouseEvent& event, float scaleFactor) override
    {
        if (m_settings.mouseInteraction)
        {
            zoomXAxis(1 - scaleFactor, event.position.getX());
            zoomYAxis(1 - scaleFactor, event.position.getY());

            getParentComponent()->repaint();
        }
    }

    void mouseWheelMove(const juce::MouseEvent& event,
                        const juce::MouseWheelDetails& wheel) override
    {
        if (m_settings.mouseInteraction)
        {
            if (event.mods.isCommandDown())
            {
                zoomXAxis(wheel.deltaY, event.position.getX());
            }
            else if (event.mods.isAltDown())
            {
                zoomYAxis(wheel.deltaY, event.position.getY());
            }
            else
            {
                moveOnXAxis(wheel.deltaX * -300.f);
                moveOnYAxis(wheel.deltaY * 300.f);
            }

            getParentComponent()->repaint();
        }
    }

    void mouseDoubleClick(const juce::MouseEvent&) override
    {
        if (m_settings.mouseInteraction)
        {
            automaticPlotBounds(m_settings, m_data);
            getParentComponent()->repaint();
        }
    }

    [[nodiscard]] auto getMousePosition() const -> juce::Point<int>
    {
        return m_mousePosition;
    }

    [[nodiscard]] auto getDrawMouseLabel() const -> bool { return m_drawMouseLabel; }

private:
    void moveOnXAxis(const int pixels)
    {
        const auto xMin = getXValue(static_cast<T>(pixels), m_settings);
        const auto xMax = getXValue(
            static_cast<T>(m_settings.plotBounds.getWidth() + pixels), m_settings);
        m_settings.xMin = xMin;
        m_settings.xMax = xMax;
    }

    void moveOnYAxis(const int pixels)
    {
        const auto yMin = getYValue(
            static_cast<T>(m_settings.plotBounds.getHeight() - pixels), m_settings);
        const auto yMax = getYValue(static_cast<T>(-pixels), m_settings);
        m_settings.yMin = yMin;
        m_settings.yMax = yMax;
    }

    void zoomXAxis(const float percent, const float xPos)
    {
        const auto pixels = percent * m_settings.plotBounds.getWidth();

        const auto normPos = xPos / static_cast<float>(m_settings.plotBounds.getWidth());
        const auto factorXMin = normPos;
        const auto factorXMax = 1 - normPos;

        const auto xMinNew = static_cast<T>(-pixels * factorXMin);
        const auto xMaxNew =
            static_cast<T>(m_settings.plotBounds.getWidth() + (pixels * factorXMax));

        const auto xMin = getXValue(xMinNew, m_settings);
        const auto xMax = getXValue(xMaxNew, m_settings);

        m_settings.xMin = xMin;
        m_settings.xMax = xMax;
    }

    void zoomYAxis(const float percent, const float yPos)
    {
        const auto pixels = percent * m_settings.plotBounds.getHeight();

        const auto normPos = yPos / static_cast<float>(m_settings.plotBounds.getHeight());
        const auto factorYMin = 1 - normPos;
        const auto factorYMax = normPos;

        const auto yMinNew =
            static_cast<T>(m_settings.plotBounds.getHeight() + (pixels * factorYMin));
        const auto yMaxNew = static_cast<T>(-pixels * factorYMax);

        const auto yMin = getYValue(yMinNew, m_settings);
        const auto yMax = getYValue(yMaxNew, m_settings);

        m_settings.yMin = yMin;
        m_settings.yMax = yMax;
    }

    PlotSettings<T>& m_settings;
    const std::vector<PlotData<T>>& m_data;
    juce::Point<int> m_mousePosition;
    bool m_drawMouseLabel = false;
};
} // namespace neo::plot
