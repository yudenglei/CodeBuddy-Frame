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

    // 鎸?"/" 鍒嗗壊璺緞
    std::vector<std::string> segments;
    std::stringstream ss(menuPath);
    std::string segment;
    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }

    if (segments.empty()) return info;

    // 妫€鏌ユ渶鍚庝竴涓鏄惁鍖呭惈 before: 鎴?after: 鍓嶇紑
    const std::string& last = segments.back();
    if (last.substr(0, 7) == "before:") {
        info.insertPosition = last;
        segments.pop_back();
    } else if (last.substr(0, 6) == "after:") {
        info.insertPosition = last;
        segments.pop_back();
    }

    // 杞崲涓烘樉绀哄悕绉板眰绾?
    for (const auto& seg : segments) {
        info.menuHierarchy.push_back(toDisplayName(seg));
    }

    return info;
}

std::string MenuDescriptor::toDisplayName(const std::string& pathSegment) {
    // 绉婚櫎 _menu / _group 鍚庣紑锛岄瀛楁瘝澶у啓
    std::string name = pathSegment;

    // 绉婚櫎宸茬煡鍚庣紑
    for (const char* suffix : {"_menu", "_group", "_bar"}) {
        std::string sfx(suffix);
        if (name.size() > sfx.size() &&
            name.substr(name.size() - sfx.size()) == sfx) {
            name = name.substr(0, name.size() - sfx.size());
            break;
        }
    }

    // 灏嗕笅鍒掔嚎鏇挎崲涓虹┖鏍煎苟棣栧瓧姣嶅ぇ鍐?
    if (!name.empty()) {
        name[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(name[0])));
    }
    std::replace(name.begin(), name.end(), '_', ' ');

    return name;
}
