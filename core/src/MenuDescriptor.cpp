#include "core/MenuDescriptor.h"
#include <sstream>
#include <algorithm>
#include <cctype>

MenuInsertInfo MenuDescriptor::parse(const std::string& menuPath) {
    MenuInsertInfo info;

    if (menuPath == "---") {
        info.isSeparator = true;
        return info;
    }

    // 按 "/" 分割路径
    std::vector<std::string> segments;
    std::stringstream ss(menuPath);
    std::string segment;
    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }

    if (segments.empty()) return info;

    // 检查最后一个段是否包含 before: 或 after: 前缀
    const std::string& last = segments.back();
    if (last.substr(0, 7) == "before:") {
        info.insertPosition = last;
        segments.pop_back();
    } else if (last.substr(0, 6) == "after:") {
        info.insertPosition = last;
        segments.pop_back();
    }

    // 转换为显示名称层级
    for (const auto& seg : segments) {
        info.menuHierarchy.push_back(toDisplayName(seg));
    }

    return info;
}

std::string MenuDescriptor::toDisplayName(const std::string& pathSegment) {
    // 移除 _menu / _group 后缀，首字母大写
    std::string name = pathSegment;

    // 移除已知后缀
    for (const char* suffix : {"_menu", "_group", "_bar"}) {
        std::string sfx(suffix);
        if (name.size() > sfx.size() &&
            name.substr(name.size() - sfx.size()) == sfx) {
            name = name.substr(0, name.size() - sfx.size());
            break;
        }
    }

    // 将下划线替换为空格并首字母大写
    if (!name.empty()) {
        name[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(name[0])));
    }
    std::replace(name.begin(), name.end(), '_', ' ');

    return name;
}
