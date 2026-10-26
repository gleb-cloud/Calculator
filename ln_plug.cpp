#include "PlugInter.h"
#include <cmath>
#include <stdexcept>

double my_ln(double x) {
    if (x <= 0) { throw std::runtime_error("No no no, mr.Fish (ln(-100))"); }
    return std::log(x);
}

extern "C" PLUGIN_EXPORT PluginInfo GetPluginInfo() {
    return { "ln", my_ln };
}
// Ready for review
