#include "PlugInter.h"
#include <cmath>

double my_sin(double x) { return std::sin(x * 3.14159265 / 180.0); }

extern "C" PLUGIN_EXPORT PluginInfo GetPluginInfo() {
    return { "sin", my_sin };
}
// Ready for review
