/// @file test_dependency_sort.cpp
/// @brief Kahn拓扑排序验证：正常依赖链、循环依赖异常

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

// ---- 复制 PluginManager 中的 Kahn 排序逻辑用于独立测试 ----
struct MockPlugin {
    std::string name;
    std::vector<std::string> deps;
};

std::vector<std::string> kahnSort(const std::vector<MockPlugin>& plugins) {
    std::unordered_map<std::string, int> inDegree;
    std::unordered_map<std::string, std::vector<std::string>> dependents;

    for (const auto& p : plugins) {
        if (inDegree.find(p.name) == inDegree.end()) inDegree[p.name] = 0;
        for (const auto& dep : p.deps) {
            inDegree[p.name]++;
            dependents[dep].push_back(p.name);
        }
    }

    std::vector<std::string> queue;
    for (const auto& [name, deg] : inDegree) {
        if (deg == 0) queue.push_back(name);
    }

    std::vector<std::string> sorted;
    size_t head = 0;
    while (head < queue.size()) {
        std::string cur = queue[head++];
        sorted.push_back(cur);
        for (const auto& dep : dependents[cur]) {
            if (--inDegree[dep] == 0) {
                queue.push_back(dep);
            }
        }
    }

    if (sorted.size() != plugins.size()) {
        throw std::runtime_error("Circular dependency detected!");
    }
    return sorted;
}

// ---- 测试：无依赖，顺序任意 ----
TEST(DependencySortTest, NoDependencies) {
    std::vector<MockPlugin> plugins = {{"A", {}}, {"B", {}}, {"C", {}}};
    auto sorted = kahnSort(plugins);
    EXPECT_EQ(sorted.size(), 3u);
    // 所有节点都应出现
    EXPECT_NE(std::find(sorted.begin(), sorted.end(), "A"), sorted.end());
    EXPECT_NE(std::find(sorted.begin(), sorted.end(), "B"), sorted.end());
    EXPECT_NE(std::find(sorted.begin(), sorted.end(), "C"), sorted.end());
}

// ---- 测试：B依赖A，A必须在B前 ----
TEST(DependencySortTest, SimpleDependency) {
    std::vector<MockPlugin> plugins = {{"B", {"A"}}, {"A", {}}};
    auto sorted = kahnSort(plugins);
    EXPECT_EQ(sorted.size(), 2u);
    auto posA = std::find(sorted.begin(), sorted.end(), "A");
    auto posB = std::find(sorted.begin(), sorted.end(), "B");
    EXPECT_LT(posA, posB); // A 在 B 前面
}

// ---- 测试：链式依赖 C→B→A，顺序必须 A B C ----
TEST(DependencySortTest, ChainDependency) {
    std::vector<MockPlugin> plugins = {
        {"C", {"B"}},
        {"B", {"A"}},
        {"A", {}}
    };
    auto sorted = kahnSort(plugins);
    ASSERT_EQ(sorted.size(), 3u);
    EXPECT_EQ(sorted[0], "A");
    EXPECT_EQ(sorted[1], "B");
    EXPECT_EQ(sorted[2], "C");
}

// ---- 测试：pcb_analysis 依赖 pcb_db（实际场景）----
TEST(DependencySortTest, PCBAnalysisDependsOnPCBDB) {
    std::vector<MockPlugin> plugins = {
        {"pcb_analysis", {"pcb_db"}},
        {"base_ui",       {}},
        {"pcb_db",        {}}
    };
    auto sorted = kahnSort(plugins);
    ASSERT_EQ(sorted.size(), 3u);

    auto posDB  = std::find(sorted.begin(), sorted.end(), "pcb_db");
    auto posAN  = std::find(sorted.begin(), sorted.end(), "pcb_analysis");
    EXPECT_LT(posDB, posAN); // pcb_db 必须在 pcb_analysis 之前
}

// ---- 测试：循环依赖应抛出异常 ----
TEST(DependencySortTest, CircularDependencyThrows) {
    std::vector<MockPlugin> plugins = {
        {"A", {"B"}},
        {"B", {"A"}}
    };
    EXPECT_THROW(kahnSort(plugins), std::runtime_error);
}

// ---- 测试：自依赖应抛出异常 ----
TEST(DependencySortTest, SelfDependencyThrows) {
    std::vector<MockPlugin> plugins = {{"A", {"A"}}};
    EXPECT_THROW(kahnSort(plugins), std::runtime_error);
}
