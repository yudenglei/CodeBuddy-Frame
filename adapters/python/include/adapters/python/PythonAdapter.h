#pragma once
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include "core/RunMode.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

/// @brief Python閫傞厤鍣ㄦ帴鍙ｏ紙pybind11灏佽鍓嶇殑C++鍖呰灞傦級
///
/// 鎻愪緵 Python 鍙嬪ソ鐨勬帴鍙ｏ紝鏈€缁堝鎵樼粰 ActionManager 鍗曚緥
class PythonAdapter {
public:
    static PythonAdapter& instance();

    /// @brief 瑙﹀彂Action锛堜笌GUI鐐瑰嚮瀹屽叏绛変环锛?    void invoke(const std::string& actionId,
                const std::unordered_map<std::string, std::string>& params = {});

    /// @brief 娉ㄥ唽鑷畾涔堿ction锛圥ython lambda鍥炶皟锛?    void registerAction(const std::string& id,
                        const std::string& label,
                        std::function<void()> callback,
                        const std::string& menuPath = "");

    /// @brief 鑾峰彇鎵€鏈夊凡娉ㄥ唽Action淇℃伅
    std::vector<ActionDescriptor> listActions() const;

    /// @brief 鏇存柊蹇嵎閿?    void setShortcut(const std::string& id, const std::string& shortcut);

private:
    PythonAdapter() = default;
};
