#pragma once
#include <string>
#include "ActionContext.h"

/// @brief 缁熶竴Action鎺ュ彛锛圕ommand Pattern锛?
/// GUI鑿滃崟銆丳ython invoke()銆乬RPC ExecuteAction 鍧囪皟鐢ㄦ鎺ュ彛
class IAction {
public:
    virtual ~IAction() = default;

    /// @brief 鑾峰彇Action鍞竴ID锛堝 "Desktop.Save"锛?
    virtual std::string getId() const = 0;

    /// @brief 鑾峰彇鏄剧ず鏍囩
    virtual std::string getLabel() const = 0;

    /// @brief 鑾峰彇宸ュ叿鎻愮ず
    virtual std::string getTooltip() const { return ""; }

    /// @brief 鎵цAction锛堜笁鏉¤矾寰勭粺涓€鍏ュ彛锛?
    virtual void execute(const ActionContext& ctx) = 0;

    /// @brief 褰撳墠鏄惁鍙敤锛堢敤浜庤彍鍗曠伆鍖栵級
    virtual bool isEnabled() const { return true; }

    /// @brief 褰撳墠鏄惁澶勪簬閫変腑鐘舵€侊紙鐢ㄤ簬Toggle Action锛?
    virtual bool isChecked() const { return false; }
};
