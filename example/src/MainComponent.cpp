#include "MainComponent.h"

namespace GuiApp
{
MainComponent::MainComponent()
{
    m_plot.settings = neo::plot::PlotSettings<double>::getTimePreset();

    neo::plot::PlotData<double> data;
    data.xData = std::vector<double> {0, 1, 2, 3, 4, 5, 6};
    data.yData = std::vector<double> {0, 1, 0, 1, 0, 1, 0};
    data.clr = juce::Colours::blue;
    data.name = "Data";

    m_plot.addData(data);

    addAndMakeVisible(m_plot);
    setSize(400, 400);
}

void MainComponent::paint(Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setFont(50.f);
    g.setColour(juce::Colours::white);
    g.drawText(
        "NeoPlot", getLocalBounds().removeFromTop(50), juce::Justification::centred);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(50);
    m_plot.setBounds(bounds.reduced(10));
}
} // namespace GuiApp
