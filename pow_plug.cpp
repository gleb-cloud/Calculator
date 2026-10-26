#include "PlugInter.h"
#include <cmath>


double my_pow(double base, double exp) {
    return std::pow(base, exp);
}

extern "C" PLUGIN_EXPORT PluginInfo2 GetPluginInfo2() {
    return { "pow", my_pow };
}
// Ready for review
