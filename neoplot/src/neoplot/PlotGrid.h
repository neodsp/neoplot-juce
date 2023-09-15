#pragma once
#include "PlotSettings.h"
#include "PlotTools.h"
#include "PlotType.h"

namespace neo::plot
{
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
class PlotGrid : public juce::Component
{
public:
    explicit PlotGrid(const PlotSettings<T>& settings)
        : m_settings(settings)
    {
    }

    void paint(juce::Graphics& g) override
    {
        createGrid();

        g.fillAll(m_settings.style.background);

        g.setColour(m_settings.style.grid);

        for (const auto& value: m_xGridPositions)
        {
            const auto x = getXPosition(value, m_settings);
            g.drawVerticalLine(
                x, 0.f, static_cast<float>(m_settings.plotBounds.getHeight()));
        }

        for (const auto& value: m_yGridPositions)
        {
            g.drawHorizontalLine(getYPosition(value, m_settings),
                                 0.f,
                                 static_cast<float>(m_settings.plotBounds.getWidth()));
        }

        if (m_settings.drawZeroLines)
        {
            g.setColour(m_settings.style.zeroLines);
            drawZeroLines(g);
        }
    }

    void resized() override {}

    auto getGridValuesX() -> const std::vector<T>* { return &m_xGridPositionsToLabel; }

    auto getGridValuesY() -> const std::vector<T>* { return &m_yGridPositionsToLabel; }

    auto getSettings() -> const PlotSettings<T>* { return &m_settings; }

private:
    void createGrid()
    {
        m_xGridPositions.clear();
        m_yGridPositions.clear();
        m_xGridPositionsToLabel.clear();
        m_yGridPositionsToLabel.clear();

        if (m_settings.type == PlotType::linear)
        {
            const auto stepX = calculateGridSpaceX(m_settings.plotBounds.getWidth(),
                                                   PIXELS_BETWEEN_GRIDS_DENSE);
            const auto startX = std::floor(m_settings.xMin / stepX);
            const auto endX = std::ceil(m_settings.xMax / stepX);
            for (int i = startX; i < endX; ++i)
            {
                m_xGridPositions.push_back(i * stepX);
            }
            m_xGridPositionsToLabel = m_xGridPositions;
        }
        else
        {
            m_xGridPositions =
                std::vector<T> {10,  20,  30,  40,  50,  60,  70,  80,   90,   100,
                                200, 300, 400, 500, 600, 700, 800, 900,  1e3,  2e3,
                                3e3, 4e3, 5e3, 6e3, 7e3, 8e3, 9e3, 10e3, 20e3, 30e3};
            m_xGridPositionsToLabel = std::vector<T> {
                10, 20, 40, 60, 100, 200, 400, 600, 1e3, 2e3, 4e3, 6e3, 10e3, 20e3, 30e3};
        }

        const auto stepY = calculateGridSpaceY(m_settings.plotBounds.getHeight(),
                                               PIXELS_BETWEEN_GRIDS_Y);
        const auto startY = std::floor(m_settings.yMin / stepY);
        const auto endY = std::ceil(m_settings.yMax / stepY);
        for (int i = startY; i < endY; ++i)
        {
            m_yGridPositions.push_back(i * stepY);
        }

        m_yGridPositionsToLabel = m_yGridPositions;
    }

    auto drawZeroLines(juce::Graphics& g)
    {
        if (m_settings.type == PlotType::linear)
        {
            const auto zeroX = getXPosition(static_cast<T>(0.), m_settings);
            g.drawVerticalLine(
                zeroX, 0.f, static_cast<float>(m_settings.plotBounds.getHeight()));
        }
        const auto zeroY = getYPosition(static_cast<T>(0.), m_settings);
        g.drawHorizontalLine(
            zeroY, 0.f, static_cast<float>(m_settings.plotBounds.getWidth()));
    }

    auto calculateGridSpaceX(const int numPlotPixels,
                             const std::size_t pixelsBetweenGridLines) -> T
    {
        const auto numGridLines =
            numPlotPixels / static_cast<int>(pixelsBetweenGridLines); // 300 / 20 = 15
        const auto valueRange = m_settings.xMax - m_settings.xMin; // 30 - 20 = 10
        const auto gridSpace =
            static_cast<double>(valueRange) / static_cast<double>(numGridLines);

        const int decZeros = -numDecimalZeros(gridSpace);

        const auto factor = std::pow(10., static_cast<double>(decZeros));
        const auto roundedValue = static_cast<T>(std::round(gridSpace * factor));

        const auto valueToUse = findClosestElementSorted(ALLOWED_VALUES, roundedValue);

        return valueToUse / factor;
    }

    auto calculateGridSpaceY(const int numPlotPixels,
                             const std::size_t pixelsBetweenGridLines) -> T
    {
        const auto numGridLines =
            numPlotPixels / static_cast<int>(pixelsBetweenGridLines); // 300 / 20 = 15
        const auto valueRange = m_settings.yMax - m_settings.yMin; // 30 - 20 = 10
        const auto gridSpace =
            static_cast<double>(valueRange) / static_cast<double>(numGridLines);

        const int decZeros = -numDecimalZeros(gridSpace);

        const auto factor = std::pow(10., static_cast<double>(decZeros));
        const auto roundedValue = static_cast<T>(std::round(gridSpace * factor));

        const auto valueToUse = findClosestElementSorted(ALLOWED_VALUES, roundedValue);

        return valueToUse / factor;
    }

    std::vector<T> ALLOWED_VALUES {1, 2, 5, 10};
    static constexpr int PIXELS_BETWEEN_GRIDS_DENSE = 60;
    static constexpr int PIXELS_BETWEEN_GRIDS_Y = 50;
    static constexpr int PIXELS_BETWEEN_GRIDS_SPARSE = 100;

    const PlotSettings<T>& m_settings;
    std::vector<T> m_xGridPositions, m_yGridPositions;
    std::vector<T> m_xGridPositionsToLabel, m_yGridPositionsToLabel;
};
} // namespace neo::plot
