#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/PluginGlobal.h"
#include "core/ActionManager.h"

// 鍓嶅悜澹版槑閬垮厤寰幆渚濊禆
class MainWindow;

/// @brief UI鎻掍欢鎵╁睍鎺ュ彛
/// UI_ONLY鍜孒YBRID绫诲瀷鐨勬彃浠跺疄鐜版鎺ュ彛浠ヨ础鐚甎I鍏冪礌
/// 
/// 浣跨敤鏂规硶锛?/// 1. 鎻掍欢鍦?initialize() 闃舵琚瘑鍒负 IUIPlugin 绫诲瀷
/// 2. 瀹夸富绋嬪簭璋冪敤 setupUI(mainWindow) 浼犲叆涓荤獥鍙ｆ寚閽?/// 3. 鎻掍欢鍦?setupUI 涓繚瀛?mainWindow 鎸囬拡锛屽苟鍒涘缓UI鍏冪礌
/// 4. 鎻掍欢鍦?teardownUI 涓竻鐞哢I鍏冪礌
class CAE_PLUGIN_EXPORT IUIPlugin {
public:
    virtual ~IUIPlugin() = default;

    /// @brief 鎻掍欢鍚戜富绐楀彛娉ㄥ叆UI鍏冪礌锛堥潰鏉裤€佸仠闈犵獥鍙ｇ瓑锛?    /// @param mainWindow 涓荤獥鍙ｆ寚閽堬紝鎻掍欢搴斾繚瀛樻鎸囬拡鐢ㄤ簬鍒涘缓瀵硅瘽妗嗙瓑
    virtual void setupUI(MainWindow* mainWindow) = 0;

    /// @brief 鎻掍欢绉婚櫎鍏禪I鍏冪礌锛堝叧闂椂璋冪敤锛?    virtual void teardownUI() = 0;
};

#endif // CAE_ENABLE_GUI
