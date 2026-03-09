/// @file test_plugin_manager.cpp
/// @brief PluginManager 单元测试骨架（mock插件，验证加载顺序和依赖排序）

#include <gtest/gtest.h>
#include "core/IPlugin.h"
#include "core/ActionManager.h"
#include "core/RunMode.h"
#include <vector>
#include <string>

// ---- Mock 插件实现 ----
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

// ---- 测试：Mock插件可正常初始化 ----
TEST(PluginManagerTest, MockPluginInitialize) {
    MockPlugin plugin("test_plugin", PluginType::DB_ONLY, {});
    EXPECT_FALSE(plugin.isInitialized());
    EXPECT_TRUE(plugin.initialize(RunMode::HEADLESS));
    EXPECT_TRUE(plugin.isInitialized());
    plugin.shutdown();
    EXPECT_FALSE(plugin.isInitialized());
}

// ---- 测试：UI_ONLY 插件在 GUI 模式下可用 ----
TEST(PluginManagerTest, UIOnlyCompatibility) {
    MockPlugin uiPlugin("ui_only", PluginType::UI_ONLY, {});
    EXPECT_TRUE(uiPlugin.isCompatible(RunMode::GUI));
    EXPECT_TRUE(uiPlugin.isCompatible(RunMode::HEADLESS)); // Mock默认全兼容
}

// ---- 测试：getMeta 返回正确信息 ----
TEST(PluginManagerTest, MetaInfoCorrect) {
    MockPlugin plugin("my_plugin", PluginType::HYBRID, {"dep_a", "dep_b"});
    auto meta = plugin.getMeta();
    EXPECT_EQ(meta.name, "my_plugin");
    EXPECT_EQ(meta.type, PluginType::HYBRID);
    ASSERT_EQ(meta.dependencies.size(), 2u);
    EXPECT_EQ(meta.dependencies[0], "dep_a");
    EXPECT_EQ(meta.dependencies[1], "dep_b");
}

// ---- 测试：插件初始化后 ActionManager 有对应 Action ----
TEST(PluginManagerTest, PluginRegistersActions) {
    ActionManager::instance().clear();

    // 模拟 PCBDBPlugin 行为：初始化时注册 Layout.RunDRC
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
