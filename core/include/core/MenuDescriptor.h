#pragma once
#include <string>
#include <vector>

struct MenuInsertInfo {
    std::vector<std::string> menuHierarchy;
    std::string insertPosition;
    bool isSeparator{false};
};

class MenuDescriptor {
public:
    static MenuInsertInfo parse(const std::string& menuPath);
    static std::string toDisplayName(const std::string& pathSegment);
};
