#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "ActionContext.h"

// ============================================================================
// RibbonInfo - 宸ュ叿鏍忥紙Ribbon锛変俊鎭?// ============================================================================

/// @brief 宸ュ叿鏍忔寜閽ぇ灏忕被鍨?enum class ToolButtonSize {
    Large,      ///< 澶ф寜閽細鍥炬爣鍦ㄤ笂锛屾枃瀛楀湪涓嬶紙32x32鍥炬爣锛?    Small,      ///< 灏忔寜閽細鍥炬爣鍦ㄥ乏锛屾枃瀛楀湪鍙筹紙16x16鍥炬爣锛?    IconOnly    ///< 浠呭浘鏍囷細鏃犳枃瀛?};

/// @brief 宸ュ叿鏍?Ribbon淇℃伅
/// 鎻掍欢閫氳繃姝ょ粨鏋勬敞鍐屽伐鍏锋爮鎸夐挳
struct RibbonInfo {
    std::string tabId;           ///< 鎵€灞濼ab ID锛堝 "Draw", "Modeler", "HFSS"锛?    std::string tabTitle;        ///< Tab鏄剧ず鏍囬锛堝 "缁樺埗", "寤烘ā"锛?    std::string groupId;         ///< 鎵€灞濭roup ID锛堝 "Primitives", "Boolean"锛?    std::string groupTitle;      ///< Group鏄剧ず鏍囬锛堝 "鍩烘湰浣?, "甯冨皵杩愮畻"锛?    ToolButtonSize size{ToolButtonSize::Large}; ///< 鎸夐挳澶у皬
    int tabOrder{0};            ///< Tab鎺掑簭锛堣秺灏忚秺闈犲墠锛?    int groupOrder{0};          ///< Group鎺掑簭锛堣秺灏忚秺闈犲墠锛?    int itemOrder{0};           ///< 鎸夐挳鍦℅roup鍐呮帓搴?    bool isSplitButton{false};  ///< 鏄惁涓哄垎瑁傛寜閽紙涓嬫媺鑿滃崟锛?    std::vector<std::string> splitItems; ///< 鍒嗚鎸夐挳鐨勫瓙椤笽D鍒楄〃
};

/// @brief Action鎻忚堪绗︼紙鎵╁睍鐗堬級
/// 娉ㄥ唽鏃舵惡甯︽墍鏈夊厓淇℃伅锛屽寘鎷彍鍗曟彃鍏ヤ綅缃拰宸ュ叿鏍忛厤缃?struct ActionDescriptor {
    std::string id;
    std::string label;
    std::string tooltip;
    std::string iconPath;
    std::string shortcut;
    std::string menuPath;
    RibbonInfo ribbon;
    std::function<void(const ActionContext&)> callback;
    
    ActionDescriptor() = default;
};

class ActionManager {
public:
    static ActionManager& instance();
    ActionManager(const ActionManager&) = delete;
    ActionManager& operator=(const ActionManager&) = delete;

    void registerAction(const ActionDescriptor& desc);
    void invoke(const std::string& id, const ActionContext& ctx = {});
    void setShortcut(const std::string& id, const std::string& shortcut);
    const ActionDescriptor* findAction(const std::string& id) const;
    std::vector<ActionDescriptor> listActions() const;
    void onActionRegistered(std::function<void(const ActionDescriptor&)> cb);
    void clear();

private:
    ActionManager() = default;
    std::unordered_map<std::string, ActionDescriptor> actions_;
    std::vector<std::string> registrationOrder_;
    std::vector<std::function<void(const ActionDescriptor&)>> observers_;
};
