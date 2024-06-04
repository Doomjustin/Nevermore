#include "tui.h"
#include "BuildInfo.h"
#include "calc/calc.h"

#include <ftxui/dom/elements.hpp>
#include <string>

namespace sf {

using namespace ftxui;
using namespace std;

string a{"12"}, b{"90"};

auto input_a = Input(&a, "");
auto input_b = Input(&b, "");
auto component = Container::Vertical({input_a, input_b});

Component getTui() {
    return Renderer(component, [&] {
        auto sum = sf::sum(stoi(a), stoi(b));
        return vbox({
                        text("CalcConsole " + std::string{ Version }),
                        text("Built: " + std::string{ Timestamp }),
                        text("SHA: " + std::string{ CommitSHA }),
                        separator(),
                        input_a->Render(),
                        input_b->Render(),
                        separator(),
                        text("Sum: " + to_string(sum)),
                    }) |
               border;
    });
}

} // namespace sf
