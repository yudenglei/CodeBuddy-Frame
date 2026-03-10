#pragma once
#include <string>
#include <vector>

/// @brief 鏁版嵁搴撳紩鎿庢娊璞℃帴鍙?
/// 鎻掍欢閫氳繃姝ゆ帴鍙ｈ闂甈CB璁捐鏁版嵁搴擄紝涓庡簳灞傚疄鐜拌В鑰?
class IDBEngine {
public:
    virtual ~IDBEngine() = default;

    /// @brief 鎵撳紑璁捐鏂囦欢/鏁版嵁搴?
    virtual bool open(const std::string& path) = 0;

    /// @brief 鍏抽棴褰撳墠鏁版嵁搴?
    virtual void close() = 0;

    /// @brief 妫€鏌ユ槸鍚﹀凡鎵撳紑
    virtual bool isOpen() const = 0;

    /// @brief 鎵ц鏌ヨ锛岃繑鍥炵粨鏋滈泦锛圝SON鏍煎紡瀛楃涓诧級
    virtual std::string query(const std::string& sql) = 0;

    /// @brief 寮€濮嬩簨鍔?
    virtual bool beginTransaction() = 0;

    /// @brief 鎻愪氦浜嬪姟
    virtual bool commitTransaction() = 0;

    /// @brief 鍥炴粴浜嬪姟
    virtual bool rollbackTransaction() = 0;

    /// @brief 鑾峰彇鏁版嵁搴撳紩鎿庡悕绉帮紙濡?"PCBSQLite"锛?
    virtual std::string getEngineName() const = 0;
};
