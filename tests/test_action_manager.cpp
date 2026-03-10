/// @file test_action_manager.cpp
/// @brief ActionManager 鍗曞厓娴嬭瘯
/// 瑕嗙洊锛歳egister/invoke/閲嶅娉ㄥ唽瑕嗙洊/invoke涓嶅瓨鍦↖D/Observer閫氱煡椤哄簭

#include <gtest/gtest.h>
#include "core/ActionManager.h"
#include "core/ActionContext.h"

class ActionManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        ActionManager::instance().clear();
    }
};

// ---- 鍩烘湰娉ㄥ唽鍜?invoke ----
TEST_F(ActionManagerTest, RegisterAndInvoke) {
    bool executed = false;
    ActionDescriptor desc;
    desc.id       = "Test.Hello";
    desc.label    = "Hello";
    desc.callback = [&executed](const ActionContext&) { executed = true; };

    ActionManager::instance().registerAction(desc);
    ActionManager::instance().invoke("Test.Hello");

    EXPECT_TRUE(executed);
}

// ---- 閲嶅娉ㄥ唽搴旇鐩栧師鏈?callback ----
TEST_F(ActionManagerTest, DuplicateRegisterOverwrites) {
    int counter = 0;

    ActionDescriptor d1;
    d1.id = "Test.Counter";
    d1.callback = [&counter](const ActionContext&) { counter += 1; };
    ActionManager::instance().registerAction(d1);

    ActionDescriptor d2;
    d2.id = "Test.Counter";
    d2.callback = [&counter](const ActionContext&) { counter += 10; };
    ActionManager::instance().registerAction(d2);

    ActionManager::instance().invoke("Test.Counter");
    EXPECT_EQ(counter, 10); // 搴旀墽琛岀浜屼釜callback
}

// ---- invoke 涓嶅瓨鍦ㄧ殑 ID 搴旀姏鍑哄紓甯?----
TEST_F(ActionManagerTest, InvokeUnknownThrows) {
    EXPECT_THROW(
        ActionManager::instance().invoke("NonExistent.Action"),
        std::runtime_error
    );
}

// ---- Observer 閫氱煡椤哄簭 ----
TEST_F(ActionManagerTest, ObserverNotificationOrder) {
    std::vector<std::string> callOrder;

    ActionManager::instance().onActionRegistered(
        [&callOrder](const ActionDescriptor& d) { callOrder.push_back("obs1:" + d.id); }
    );
    ActionManager::instance().onActionRegistered(
        [&callOrder](const ActionDescriptor& d) { callOrder.push_back("obs2:" + d.id); }
    );

    ActionDescriptor desc;
    desc.id = "Test.ObsTest";
    ActionManager::instance().registerAction(desc);

    ASSERT_EQ(callOrder.size(), 2u);
    EXPECT_EQ(callOrder[0], "obs1:Test.ObsTest");
    EXPECT_EQ(callOrder[1], "obs2:Test.ObsTest");
}

// ---- listActions 淇濇寔娉ㄥ唽椤哄簭 ----
TEST_F(ActionManagerTest, ListActionsPreservesOrder) {
    for (const char* id : {"Test.C", "Test.A", "Test.B"}) {
        ActionDescriptor d;
        d.id = id;
        ActionManager::instance().registerAction(d);
    }

    auto actions = ActionManager::instance().listActions();
    ASSERT_EQ(actions.size(), 3u);
    EXPECT_EQ(actions[0].id, "Test.C");
    EXPECT_EQ(actions[1].id, "Test.A");
    EXPECT_EQ(actions[2].id, "Test.B");
}

// ---- findAction ----
TEST_F(ActionManagerTest, FindAction) {
    ActionDescriptor desc;
    desc.id    = "Test.Find";
    desc.label = "Find Me";
    ActionManager::instance().registerAction(desc);

    const auto* found = ActionManager::instance().findAction("Test.Find");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->label, "Find Me");

    const auto* notFound = ActionManager::instance().findAction("Test.NotExist");
    EXPECT_EQ(notFound, nullptr);
}

// ---- ActionContext 鍙傛暟浼犻€?----
TEST_F(ActionManagerTest, ActionContextParamsPassed) {
    std::string receivedPath;

    ActionDescriptor desc;
    desc.id = "Test.WithParams";
    desc.callback = [&receivedPath](const ActionContext& ctx) {
        receivedPath = ctx.get<std::string>("path", "");
    };
    ActionManager::instance().registerAction(desc);

    ActionContext ctx;
    ctx.params["path"] = std::string("/tmp/test.pcb");
    ActionManager::instance().invoke("Test.WithParams", ctx);

    EXPECT_EQ(receivedPath, "/tmp/test.pcb");
}
