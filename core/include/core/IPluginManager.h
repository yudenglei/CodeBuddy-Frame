#pragma once
#include <string>
#include <vector>
#include "IPlugin.h"
#include "RunMode.h"
#include "PluginGlobal.h"

/// @brief 鎻掍欢绠＄悊鍣ㄦ帴鍙?/// 
/// 璐熻矗鎻掍欢鐨勫彂鐜般€佸姞杞姐€佸垵濮嬪寲鍜屽嵏杞?class CAE_PLUGIN_EXPORT IPluginManager {
public:
    virtual ~IPluginManager() = default;

    /// @brief 鎵弿鐩綍锛屽彂鐜板苟鍔犺浇鎵€鏈夋彃浠讹紙鍚緷璧栨帓搴忥級
    /// @param pluginDir 鎻掍欢鐩綍璺緞锛屽缓璁娇鐢ㄧ粷瀵硅矾寰勬垨鐩稿浜庡彲鎵ц鏂囦欢鐨勮矾寰?    /// @return 鏄惁鍙戠幇鑷冲皯涓€涓彃浠?    virtual bool discover(const std::string& pluginDir) = 0;

    /// @brief 鎸夎繍琛屾ā寮忓垵濮嬪寲鎵€鏈夊凡鍙戠幇鐨勫吋瀹规彃浠?    /// @param mode 杩愯妯″紡锛圙UI/HEADLESS/PYTHON/GRPC锛?    /// @return 鏄惁鎵€鏈夋彃浠跺垵濮嬪寲鎴愬姛
    virtual bool initializeAll(RunMode mode) = 0;

    /// @brief 涓篣I鎻掍欢璁剧疆涓荤獥鍙ｏ紙蹇呴』鍦╥nitializeAll涔嬪悗璋冪敤锛?    /// @param mainWindow 涓荤獥鍙ｆ寚閽堬紙浠呭湪GUI妯″紡涓嬮渶瑕佽皟鐢級
    /// 姝ゆ柟娉曚細閬嶅巻鎵€鏈夊凡鍔犺浇鐨勬彃浠讹紝璋冪敤IUIPlugin::setupUI()
    virtual void setMainWindow(void* mainWindow) = 0;

    /// @brief 鍏抽棴鎵€鏈夋彃浠讹紙閫嗗簭锛?    virtual void shutdownAll() = 0;

    /// @brief 鎸夊悕绉拌幏鍙栨彃浠跺疄渚?    virtual IPlugin* getPlugin(const std::string& name) const = 0;

    /// @brief 鑾峰彇鎵€鏈夊凡鍔犺浇鎻掍欢
    virtual std::vector<IPlugin*> getAllPlugins() const = 0;

    /// @brief 鑾峰彇鎻掍欢鍔犺浇鐩綍
    virtual std::string getPluginDir() const = 0;
};
