#include <array>
#include <vector>

#include "chopper.h"
#include "file.h"
#include "gui.h"
#include "instrument.h"
#include "python_interface.h"

template <class Wobbler,
          class Chopper,
          class Housekeeping,
          class Frontend,
          class Backends>
class WASPAM {
  static constexpr unsigned BackendN = Backends::N;
  static_assert(Housekeeping::is_housekeeper);
  static_assert(Chopper::is_chopping);
  static_assert(Wobbler::is_wobbling);
  static_assert(Frontend::is_frontend);
  static_assert(Backends::is_backend);
  
  Housekeeping hk;
  Chopper chop;
  Wobbler wob;
  Frontend fr;
  Backends ba;

public:
  WASPAM (Housekeeping&& h, Chopper&& c, Wobbler&& w, Frontend&& f, Backends&& b) noexcept :
  hk(std::move(h)), chop(std::move(c)), wob(std::move(w)), fr(std::move(f)), ba(std::move(ba)) {}
};


int main () {
  auto py = Python::createPython();  // Must be declared even if unused

  InitializeGUI("WASPAM");
  
  // Our states
  GUI::Config config;
  
  // Our style
  GUI::LayoutAndStyleSettings();
  
  // Main loop
  BeginWhileLoopGUI;
  
  // Main menu bar
  GUI::MainMenu::fullscreen(config, window);
  GUI::MainMenu::quitscreen(config, window);
  
  std::vector<double> y {1,2,3,4,5,6,7,6,5,4,3,2,1};
  auto startpos = ImGui::GetCursorPos();
  if (GUI::Windows::sub<2, 7, 0, 0, 1, 3>(window, startpos, "Plot tool 1")) {
    if (ImPlot::BeginPlot("Test", "X", "Y", {-1, -1})) {
      ImPlot::PlotLine("Plot", y.data(), y.size());
      ImPlot::EndPlot();
    }
  } GUI::Windows::end();
  if (GUI::Windows::sub<2, 7, 1, 0, 1, 3>(window, startpos, "Plot tool 2")) {
    if (ImPlot::BeginPlot("Test", "X", "Y", {-1, -1})) {
      ImPlot::PlotLine("Plot", y.data(), y.size());
      ImPlot::EndPlot();
    }
  } GUI::Windows::end();
  if (GUI::Windows::sub<2, 7, 0, 3, 1, 3>(window, startpos, "Plot tool 3")) {
    if (ImPlot::BeginPlot("Test", "X", "Y", {-1, -1})) {
      ImPlot::PlotLine("Plot", y.data(), y.size());
      ImPlot::EndPlot();
    }
  } GUI::Windows::end();
  if (GUI::Windows::sub<2, 7, 1, 3, 1, 3>(window, startpos, "Plot tool 4")) {
    if (ImPlot::BeginPlot("Test", "X", "Y", {-1, -1})) {
      ImPlot::PlotLine("Plot", y.data(), y.size());
      ImPlot::EndPlot();
    }
  } GUI::Windows::end();
  if (GUI::Windows::sub<3, 7, 0, 6, 1, 1>(window, startpos, "Plot tool 5")) {
    ImGui::Button("HIIIII");
  } GUI::Windows::end();
  if (GUI::Windows::sub<3, 7, 1, 6, 2, 1>(window, startpos, "Plot tool 6")) {
    ImGui::Button("HOWDY");
  } GUI::Windows::end();
  
  // End of main loop
  EndWhileLoopGUI;
  CleanupGUI;
}