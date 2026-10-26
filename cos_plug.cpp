#include "PlugInter.h"
#include <cmath>


double my_cos(double x) {
    return std::cos(x * 3.14159265 / 180.0);
}

extern "C" PLUGIN_EXPORT PluginInfo GetPluginInfo() {
    return { "cos", my_cos };
}