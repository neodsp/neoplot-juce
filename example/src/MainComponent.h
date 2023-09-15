#pragma once
#include "CommonHeader.h"
#include <neoplot/NeoPlot.h>

namespace GuiApp
{
class MainComponent : public Component
{
public:
    MainComponent();

    void paint(Graphics&) override;
    void resized() override;

private:
    neo::plot::NeoPlot<double> m_plot;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace GuiApp
