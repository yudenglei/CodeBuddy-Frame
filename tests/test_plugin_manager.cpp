/// @file test_plugin_manager.cpp
/// @brief PluginManager 鍗曞厓娴嬭瘯楠ㄦ灦锛坢ock鎻掍欢锛岄獙璇佸姞杞介『搴忓拰渚濊禆鎺掑簭锛?

#include <gtest/gtest.h>
#include "core/IPlugin.h"
#include "core/ActionManager.h"
#include "core/RunMode.h"
#include <vector>
#include <string>

// ---- Mock 鎻掍欢瀹炵幇 ----
class MockPlugin : public IPlugin {
public:
    MockPlugin(const std::string& name,
               PluginType type,
               const std::vector<std::string>& deps)
        : meta_{name, "1.0.0", "Mock", type, deps} {}

    PluginMeta getMeta() const override { return meta_; }
    bool initialize(RunMode) override { initialized_ = true; return true; }
    void shutdown() override { initialized_ = false; }
    bool isCompatible(RunMode) const override { return true; }

    bool isInitialized() const { return initialized_; }

private:
    PluginMeta meta_;
    bool initialized_{false};
};

// ---- 娴嬭瘯锛歁ock鎻掍欢鍙甯稿垵濮嬪寲 ----
TEST(PluginManagerTest, MockPluginInitialize) {
    MockPlugin plugin("test_plugin", PluginType::DB_ONLY, {});
    EXPECT_FALSE(plugin.isInitialized());
    EXPECT_TRUE(plugin.initialize(RunMode::HEADLESS));
    EXPECT_TRUE(plugin.isInitialized());
    plugin.shutdown();
    EXPECT_FALSE(plugin.isInitialized());
}

// ---- 娴嬭瘯锛歎I_ONLY 鎻掍欢鍦?GUI 妯″紡涓嬪彲鐢?----
TEST(PluginManagerTest, UIOnlyCompatibility) {
    MockPlugin uiPlugin("ui_only", PluginType::UI_ONLY, {});
    EXPECT_TRUE(uiPlugin.isCompatible(RunMode::GUI));
    EXPECT_TRUE(uiPlugin.isCompatible(RunMode::HEADLESS)); // Mock榛樿鍏ㄥ吋瀹?
}

// ---- 娴嬭瘯锛歡etMeta 杩斿洖姝ｇ‘淇℃伅 ----
TEST(PluginManagerTest, MetaInfoCorrect) {
    MockPlugin plugin("my_plugin", PluginType::HYBRID, {"dep_a", "dep_b"});
    auto meta = plugin.getMeta();
    EXPECT_EQ(meta.name, "my_plugin");
    EXPECT_EQ(meta.type, PluginType::HYBRID);
    ASSERT_EQ(meta.dependencies.size(), 2u);
    EXPECT_EQ(meta.dependencies[0], "dep_a");
    EXPECT_EQ(meta.dependencies[1], "dep_b");
}

// ---- 娴嬭瘯锛氭彃浠跺垵濮嬪寲鍚?ActionManager 鏈夊搴?Action ----
TEST(PluginManagerTest, PluginRegistersActions) {
    ActionManager::instance().clear();

    // 妯℃嫙 PCBDBPlugin 琛屼负锛氬垵濮嬪寲鏃舵敞鍐?Layout.RunDRC
    ActionDescriptor desc;
    desc.id    = "Layout.RunDRC";
    desc.label = "Design Rule Check";
    desc.callback = [](const ActionContext&) {};
    ActionManager::instance().registerAction(desc);

    const auto* found = ActionManager::instance().findAction("Layout.RunDRC");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->label, "Design Rule Check");

    ActionManager::instance().clear();
}
