#pragma once

#define PLUGIN_EXPORT __declspec(dllexport)


typedef double (*MathFunc)(double);


typedef double (*MathFunc2)(double, double);

struct PluginInfo {
    const char* name;
    MathFunc func;     
};


struct PluginInfo2 {
    const char* name;
    MathFunc2 func;
};