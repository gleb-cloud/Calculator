#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <windows.h>
#include "PlugInter.h"

std::map<std::string, MathFunc> plugins;
std::vector<HMODULE> dlls;

void loadPlugins() {
    if (!std::filesystem::exists("./plugins")) return;
    for (const auto& entry : std::filesystem::directory_iterator("./plugins")) {
        if (entry.path().extension() == ".dll") {
            HMODULE h = LoadLibraryA(entry.path().string().c_str());
            if (!h) continue;
            auto getInfo = (PluginInfo(*)())GetProcAddress(h, "GetPluginInfo");
            if (getInfo) {
                try {
                    PluginInfo info = getInfo();
                    plugins[info.name] = info.func;
                    dlls.push_back(h);
                }
                catch (...) { FreeLibrary(h); }
            }
            else { FreeLibrary(h); }
        }
    }
}

void skipSpaces(const std::string& s, size_t& i) {
    while (i < s.size() && isspace(s[i])) i++;
}

double parseExpression(const std::string& s, size_t& i);

double parseFactor(const std::string& s, size_t& i) {
    skipSpaces(s, i);
    if (i >= s.size()) throw std::runtime_error("Unexpected end");

    if (s[i] == '(') {
        i++;
        double val = parseExpression(s, i);
        skipSpaces(s, i);
        if (i >= s.size() || s[i] != ')') throw std::runtime_error("Nado ')'");
        i++;
        return val;
    }
    if (s[i] == '-') { i++; return -parseFactor(s, i); }

    if (isalpha(s[i])) {
        std::string name;
        while (i < s.size() && isalpha(s[i])) name += s[i++];
        skipSpaces(s, i);
        if (i >= s.size() || s[i] != '(') throw std::runtime_error("Nado '('");
        i++;
        double arg = parseExpression(s, i);
        skipSpaces(s, i);
        if (i >= s.size() || s[i] != ')') throw std::runtime_error("Nado ')'");
        i++;
        if (plugins.find(name) == plugins.end()) throw std::runtime_error("Hz function: " + name);
        return plugins[name](arg);
    }

    size_t start = i;
    while (i < s.size() && (isdigit(s[i]) || s[i] == '.')) i++;
    if (start == i) throw std::runtime_error("Nado number");
    return std::stod(s.substr(start, i - start));
}

double parseTerm(const std::string& s, size_t& i) {
    double left = parseFactor(s, i);
    while (true) {
        skipSpaces(s, i);
        if (i < s.size() && (s[i] == '*' || s[i] == '/')) {
            char op = s[i++];
            double right = parseFactor(s, i);
            if (op == '*') left *= right;
            else {
                if (right == 0) throw std::runtime_error("Div by zero");
                left /= right;
            }
        }
        else break;
    }
    return left;
}

double parseExpression(const std::string& s, size_t& i) {
    double left = parseTerm(s, i);
    while (true) {
        skipSpaces(s, i);
        if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
            char op = s[i++];
            double right = parseTerm(s, i);
            if (op == '+') left += right;
            else left -= right;
        }
        else break;
    }
    return left;
}

int main() {
    loadPlugins();
    std::string input;
    std::cout << "Enter expression: ";
    std::getline(std::cin, input);

    try {
        size_t i = 0;
        double result = parseExpression(input, i);
        skipSpaces(input, i);
        if (i < input.size()) throw std::runtime_error("Invalid");
        std::cout << "Result: " << result << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    for (HMODULE h : dlls) FreeLibrary(h);
    return 0;
}
// Ready for review
