#include "core/MenuDescriptor.h"
#include <sstream>
#include <algorithm>
#include <cctype>

MenuInsertInfo MenuDescriptor::parse(const std::string& menuPath) {
    MenuInsertInfo info;
    if (menuPath == "---") { info.isSeparator = true; return info; }

    std::vector<std::string> segments;
    std::stringstream ss(menuPath);
    std::string segment;
    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) segments.push_back(segment);
    }
    if (segments.empty()) return info;

    const std::string& last = segments.back();
    if (last.substr(0, 7) == "before:" || last.substr(0, 6) == "after:") {
        info.insertPosition = last;
        segments.pop_back();
    }
    for (const auto& seg : segments) {
        info.menuHierarchy.push_back(toDisplayName(seg));
    }
    return info;
}

std::string MenuDescriptor::toDisplayName(const std::string& pathSegment) {
    std::string name = pathSegment;
    for (const char* suffix : {"_menu", "_group", "_bar"}) {
        std::string sfx(suffix);
        if (name.size() > sfx.size() && name.substr(name.size() - sfx.size()) == sfx) {
            name = name.substr(0, name.size() - sfx.size());
            break;
        }
    }
    if (!name.empty()) name[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(name[0])));
    std::replace(name.begin(), name.end(), '_', ' ');
    return name;
}
