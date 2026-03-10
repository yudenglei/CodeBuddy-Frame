#pragma once
#include "PluginGlobal.h"
#include "PluginMeta.h"
#include "RunMode.h"

// ============================================================================
// IPlugin.h - 鎻掍欢鍩烘帴鍙?// ============================================================================
// 姣忎釜鎻掍欢鍔ㄦ€佸簱蹇呴』瀹炵幇姝ゆ帴鍙ｅ苟瀵煎嚭宸ュ巶鍑芥暟
// 
// 瀵煎嚭鍑芥暟浣跨敤 CAE_PLUGIN_EXPORT 瀹忥紝纭繚璺ㄥ钩鍙扮紪璇戞纭?// 鍦ㄦ彃浠剁殑 CMakeLists.txt 涓繀椤诲畾涔夋彃浠朵笓灞炲畯锛屽锛?DCAE_PLUGIN_BASE_UI
// ============================================================================

/// @brief 鎻掍欢鍩烘帴鍙?class CAE_PLUGIN_EXPORT IPlugin {
public:
    virtual ~IPlugin() = default;

    /// @brief 鑾峰彇鎻掍欢鍏冧俊鎭紙鍚嶇О/鐗堟湰/绫诲瀷/渚濊禆锛?    virtual PluginMeta getMeta() const = 0;

    /// @brief 鍒濆鍖栨彃浠讹紙娉ㄥ唽Actions锛屽缓绔婦B杩炴帴绛夛級
    /// @param mode 褰撳墠杩愯妯″紡锛屾彃浠舵嵁姝ゅ喅瀹氬垵濮嬪寲鍐呭
    /// @return true=鎴愬姛锛宖alse=鍒濆鍖栧け璐ワ紙鎻掍欢绠＄悊鍣ㄥ皢璺宠繃姝ゆ彃浠讹級
    virtual bool initialize(RunMode mode) = 0;

    /// @brief 鍏抽棴鎻掍欢锛堥噴鏀捐祫婧愶級
    virtual void shutdown() = 0;

    /// @brief 妫€鏌ユ彃浠舵槸鍚﹀吋瀹瑰綋鍓嶈繍琛屾ā寮?    virtual bool isCompatible(RunMode mode) const = 0;
};


// ============================================================================
// 鎻掍欢宸ュ巶鍑芥暟绫诲瀷瀹氫箟
// ============================================================================

/// @brief 鍒涘缓鎻掍欢瀹炰緥锛堝伐鍘傚嚱鏁帮級
using CreatePluginFunc = IPlugin* (*)();
/// @brief 閿€姣佹彃浠跺疄渚?using DestroyPluginFunc = void (*)(IPlugin*);


// ============================================================================
// 瀵煎嚭鍑芥暟锛圕鎺ュ彛锛屼娇鐢?extern "C" 纭繚绗﹀彿涓€鑷达級
// ============================================================================

extern "C" {
    /// @brief 鍒涘缓鎻掍欢瀹炰緥锛堝伐鍘傚嚱鏁帮級
    /// @return 鎻掍欢瀹炰緥鎸囬拡
    CAE_PLUGIN_EXPORT IPlugin* createPlugin();
    
    /// @brief 閿€姣佹彃浠跺疄渚?    /// @param plugin 瑕侀攢姣佺殑鎻掍欢瀹炰緥鎸囬拡
    CAE_PLUGIN_EXPORT void destroyPlugin(IPlugin* plugin);
}
