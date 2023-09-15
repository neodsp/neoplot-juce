cmake_minimum_required(VERSION 3.15)
project(neoplot)

add_library(${PROJECT_NAME} STATIC)

target_compile_definitions(neoplot
        PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0
        )

target_sources(${PROJECT_NAME} PUBLIC
        src/neoplot/AxisLabel.h
        src/neoplot/NeoPlot.h
        src/neoplot/PlotData.h
        src/neoplot/PlotGrid.h
        src/neoplot/PlotLegend.h
        src/neoplot/PlotLines.h
        src/neoplot/PlotMouseInteraction.h
        src/neoplot/PlotMouseLabel.h
        src/neoplot/PlotOverlay.h
        src/neoplot/PlotSettings.h
        src/neoplot/PlotStyle.h
        src/neoplot/PlotTools.h
        src/neoplot/PlotType.h
        )

juce_add_binary_data(NeoPlotData SOURCES
        assets/JetBrainsMono.ttf)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        NeoPlotData
        Eigen3::Eigen
        juce::juce_gui_extra
        Boost::range
        Boost::geometry
    PRIVATE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_include_directories(${PROJECT_NAME} PUBLIC src)
