#pragma once
#include <Eigen/Eigen>
#include <juce_gui_basics/juce_gui_basics.h>
#include <cassert>
#include "PlotSettings.h"
#include "PlotData.h"
#include "PlotType.h"
#include "PlotSettings.h"
#include "../libInterpolate/Interpolate.hpp"

namespace neo::plot
{
constexpr double MINUS_INF = -100.;

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
auto findClosestElementIndexSorted(const std::vector<T>& data, const T element)
    -> std::size_t
{
    auto it = std::min_element(data.begin(),
                               data.end(),
                               [element](T a, T b)
                               { return std::abs(element - a) < std::abs(element - b); });

    auto index = std::distance(data.begin(), it);

    return index;
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
auto findClosestElementSorted(const std::vector<T>& data, const T element) -> T
{
    return *std::min_element(data.begin(),
                             data.end(),
                             [element](T a, T b)
                             { return std::abs(element - a) < std::abs(element - b); });
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline auto roundToNumDecimalPlaces(const T number, const std::size_t numDecimalPlaces)
    -> T
{
    const auto factor = std::pow(10., static_cast<double>(numDecimalPlaces));
    return std::round(number * factor) / factor;
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline auto numDecimalZeros(const T number) -> int
{
    return static_cast<int>(std::floor(std::log10(static_cast<double>(number))));
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline auto getXPosition(const T value, const PlotSettings<T>& settings) -> T
{
    if (settings.type == PlotType::linear)
    {
        return juce::jmap(value,
                          settings.xMin,
                          settings.xMax,
                          static_cast<T>(0),
                          static_cast<T>(settings.plotBounds.getWidth()));
    }
    else
    {
        return juce::jmap(juce::mapFromLog10(value + static_cast<T>(10e-40), settings.xMin, settings.xMax),
                          static_cast<T>(0),
                          static_cast<T>(settings.plotBounds.getWidth()));
    }
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline auto getYPosition(const T value, const PlotSettings<T>& settings) -> T
{
    return juce::jmap(value,
                      settings.yMin,
                      settings.yMax,
                      static_cast<T>(settings.plotBounds.getHeight()),
                      static_cast<T>(0));
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline auto getXValue(const T position, const PlotSettings<T>& settings) -> T
{
    if (settings.type == PlotType::linear)
    {
        return juce::jmap(position,
                          static_cast<T>(0),
                          static_cast<T>(settings.plotBounds.getWidth()),
                          settings.xMin,
                          settings.xMax);
    }
    else
    {
        return juce::mapToLog10(juce::jmap(position,
                                           static_cast<T>(0),
                                           static_cast<T>(settings.plotBounds.getWidth()),
                                           static_cast<T>(0),
                                           static_cast<T>(1)),
                                settings.xMin,
                                settings.xMax);
    }
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline auto getYValue(const T position, const PlotSettings<T>& settings) -> T
{
    return juce::jmap(position,
                      static_cast<T>(settings.plotBounds.getHeight()),
                      static_cast<T>(0),
                      settings.yMin,
                      settings.yMax);
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline void addToPath(juce::Path& path,
                      const PlotSettings<T>& settings,
                      const std::vector<T>& xData,
                      const std::vector<T>& yData,
                      const std::size_t position)
{
    const auto x = getXPosition(xData[position], settings);
    const auto y = getYPosition(yData[position], settings);
    path.lineTo(x, y);
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline void startSubPath(juce::Path& path,
                         const PlotSettings<T>& settings,
                         const std::vector<T>& xData,
                         const std::vector<T>& yData,
                         const std::size_t position)
{
    const auto x = getXPosition(xData[position], settings);
    const auto y = getYPosition(yData[position], settings);
    path.startNewSubPath(x, y);
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
inline auto calculateNumDataPoints(const PlotSettings<T>& settings,
                                   const PlotData<T>& inData) -> std::size_t
{
    const auto start = findClosestElementIndexSorted(inData.xData, settings.xMin);
    const auto end = findClosestElementIndexSorted(inData.xData, settings.xMax);
    return end - start + 1;
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
auto getStringForValue(const T value,
                       const juce::String& unit = "",
                       const int numDecimalPlaces = 0) -> juce::String
{
    if (value >= static_cast<T>(1000.) || value <= static_cast<T>(-1000.))
    {
        if (unit.isEmpty())
        {
            return juce::String(value / 1000., numDecimalPlaces) + "k";
        }
        else
        {
            return juce::String(value / 1000., numDecimalPlaces) + " k" + unit;
        }
    }
    else
    {
        return juce::String(value, numDecimalPlaces) + " " + unit;
    }
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
void transformData(const PlotSettings<T>& settings, PlotData<T>& data)
{
    assert(data.xDataReduced.size() >= settings.plotBounds.getWidth());
    assert(data.yDataReduced.size() >= settings.plotBounds.getWidth());

    const auto start = findClosestElementIndexSorted(data.xData, settings.xMin);
    const auto end = findClosestElementIndexSorted(data.xData, settings.xMax);
    const auto numDataPoints = end - start + 1;

    if (numDataPoints > settings.plotBounds.getWidth())
    {
        // reducer
        const T window = static_cast<T>(numDataPoints)
                         / static_cast<T>(settings.plotBounds.getWidth());
        const T remainder = window - static_cast<int>(window);
        T running_remainder = remainder;
        std::size_t windowToUse;
        std::size_t moving_start = start;
        for (size_t i = 0; i < settings.plotBounds.getWidth(); i++)
        {
            if (static_cast<float>(running_remainder) < 1.f)
            {
                windowToUse = static_cast<int>(window);
            }
            else
            {
                windowToUse = static_cast<int>(window) + 1;
                running_remainder -= 1.f;
            }

            data.xDataReduced[i] =
                Eigen::Map<Eigen::ArrayX<T>>(data.xData.data(),
                                             static_cast<long>(data.xData.size()))
                    .segment(moving_start, windowToUse)
                    .mean();

            if (data.isWaveform)
            {
                data.yDataReduced[i] =
                    Eigen::Map<Eigen::ArrayX<T>>(data.yData.data(),
                                                 static_cast<long>(data.yData.size()))
                        .segment(moving_start, windowToUse)
                        .maxCoeff();

                data.yDataReducedWaveformMin[i] =
                    Eigen::Map<Eigen::ArrayX<T>>(data.yData.data(),
                                                 static_cast<long>(data.yData.size()))
                        .segment(moving_start, windowToUse)
                        .minCoeff();
            }
            else
            {
                data.yDataReduced[i] =
                    Eigen::Map<Eigen::ArrayX<T>>(data.yData.data(),
                                                 static_cast<long>(data.yData.size()))
                        .segment(moving_start, windowToUse)
                        .mean();
            }

            moving_start += windowToUse;
            running_remainder += remainder;
        }
    }
    else
    {
        assert(false);
    }
}

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
auto automaticPlotBounds(PlotSettings<T>& settings, const std::vector<PlotData<T>>& data)
{
    T xMin = std::numeric_limits<T>::max(), xMax = std::numeric_limits<T>::min(),
      yMin = std::numeric_limits<T>::max(), yMax = std::numeric_limits<T>::min();
    for (const auto& d: data)
    {
        if (settings.type == PlotType::linear)
        {
            const auto xMinCandidate = d.xData.front();
            xMin = xMinCandidate < xMin ? xMinCandidate : xMin;

            const auto xMaxCandidate = d.xData.back();
            xMax = xMaxCandidate > xMax ? xMaxCandidate : xMax;
        }
        else
        {
            const auto xMinCandidate = d.xData.front();
            xMin = std::clamp(xMinCandidate < xMin ? xMinCandidate : xMin,
                              static_cast<T>(10.),
                              static_cast<T>(30e10));

            const auto xMaxCandidate = d.xData.back();
            xMax = std::clamp(xMaxCandidate > xMax ? xMaxCandidate : xMax,
                              static_cast<T>(10.),
                              static_cast<T>(30e10));
        }

        const auto yMinCandidate = *std::min_element(d.yData.begin(), d.yData.end());
        yMin = yMinCandidate < yMin ? yMinCandidate : yMin;

        const auto yMaxCandidate = *std::max_element(d.yData.begin(), d.yData.end());
        yMax = yMaxCandidate > yMax ? yMaxCandidate : yMax;
    }

    settings.xMin = settings.xMinDefault.value_or(xMin);
    settings.xMax = settings.xMaxDefault.value_or(xMax);
    settings.yMin = settings.yMinDefault.value_or(yMin);
    settings.yMax = settings.yMaxDefault.value_or(yMax);
}

template <typename T>
static inline auto lin_to_db(T lin) -> T
{
    if (lin <= static_cast<T>(0.))
        return static_cast<T>(MINUS_INF);
    return static_cast<T>(20. * std::log10(static_cast<double>(lin)));
}

template <typename T>
static inline void lin_to_db(std::vector<T>& lin)
{
    std::transform(
        lin.begin(), lin.end(), lin.begin(), [](auto& item) { return lin_to_db(item); });
}

template <typename T>
static auto warp(std::vector<T>& magnitude) -> std::vector<T>
{
    // linearly and logarithmically spaced frequency bins
    auto N = magnitude.size();
    std::vector<T> n_lin(N), n_log(N);
    std::iota(n_lin.begin(), n_lin.end(), 1.);
    std::iota(n_log.begin(), n_log.end(), 0.);
    std::for_each(n_log.begin(),
                  n_log.end(),
                  [&](auto& item)
                  { item = std::pow(N, item / static_cast<T>((N - 1))); });

    _1D::CubicSplineInterpolator<double> interp;
    interp.setData(N, n_lin.data(), magnitude.data());

    std::for_each(n_log.begin(), n_log.end(), [&](auto& item) { item = interp(item); });

    return n_log;
}

template <typename T>
static auto smooth_warped(const std::vector<T>& warped, int num_fractions, bool hanning = true)
    -> std::vector<T>
{
    auto N = warped.size();
    std::vector<T> n_lin(N), n_log(N);
    std::iota(n_lin.begin(), n_lin.end(), 1.);
    std::iota(n_log.begin(), n_log.end(), 0.);
    std::for_each(n_log.begin(),
                  n_log.end(),
                  [&](auto& item)
                  { item = std::pow(N, item / static_cast<T>((N - 1))); });

    // frequency bin spacing in octaves: log2(n_log[n]/n_log[n-1])
    // Note: n_log[0] = 1
    auto delta_n_old = std::log2(n_log[1]);
    auto delta_n = std::log2(std::pow(N, 1. / static_cast<T>((N - 1))));
    assert(delta_n_old == delta_n);

    // width of the window in logarithmically spaced samples
    // Note: Forcing the window to have an odd length increases the deviation
    //       from the exact width, but makes sure that the delay introduced in
    //       the convolution is integer and can be easily compensated
    auto n_window = static_cast<int>(
        2 * std::floor(1. / static_cast<double>(num_fractions * delta_n * 2)) + 1);

    // generate window
    std::vector<T> window(n_window, 1);
    if (hanning)
    {
        hanning_window(window);
    }
    auto windowSum = std::reduce(window.begin(), window.end());

    std::vector<T> tmpVec;
    std::copy(warped.cbegin(), warped.cend(), std::back_inserter(tmpVec));

    tmpVec.resize(warped.size() + n_window);

    // convolve window
    auto temp = convolve(window, tmpVec);
    auto index = size_t(n_window / 2);
    auto smoothed = std::vector<T>(warped.size());
    for (size_t i = 0; i < warped.size(); ++i)
    {
        smoothed[i] = temp[index + i] / static_cast<T>(windowSum);
    }

    return smoothed;
}
} // namespace neo::plot
