/// @file test_dependency_sort.cpp
/// @brief Kahn鎷撴墤鎺掑簭楠岃瘉锛氭甯镐緷璧栭摼銆佸惊鐜緷璧栧紓甯?

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

// ---- 澶嶅埗 PluginManager 涓殑 Kahn 鎺掑簭閫昏緫鐢ㄤ簬鐙珛娴嬭瘯 ----
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

// ---- 娴嬭瘯锛氭棤渚濊禆锛岄『搴忎换鎰?----
TEST(DependencySortTest, NoDependencies) {
    std::vector<MockPlugin> plugins = {{"A", {}}, {"B", {}}, {"C", {}}};
    auto sorted = kahnSort(plugins);
    EXPECT_EQ(sorted.size(), 3u);
    // 鎵€鏈夎妭鐐归兘搴斿嚭鐜?
    EXPECT_NE(std::find(sorted.begin(), sorted.end(), "A"), sorted.end());
    EXPECT_NE(std::find(sorted.begin(), sorted.end(), "B"), sorted.end());
    EXPECT_NE(std::find(sorted.begin(), sorted.end(), "C"), sorted.end());
}

// ---- 娴嬭瘯锛欱渚濊禆A锛孉蹇呴』鍦˙鍓?----
TEST(DependencySortTest, SimpleDependency) {
    std::vector<MockPlugin> plugins = {{"B", {"A"}}, {"A", {}}};
    auto sorted = kahnSort(plugins);
    EXPECT_EQ(sorted.size(), 2u);
    auto posA = std::find(sorted.begin(), sorted.end(), "A");
    auto posB = std::find(sorted.begin(), sorted.end(), "B");
    EXPECT_LT(posA, posB); // A 鍦?B 鍓嶉潰
}

// ---- 娴嬭瘯锛氶摼寮忎緷璧?C鈫払鈫扐锛岄『搴忓繀椤?A B C ----
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

// ---- 娴嬭瘯锛歱cb_analysis 渚濊禆 pcb_db锛堝疄闄呭満鏅級----
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
    EXPECT_LT(posDB, posAN); // pcb_db 蹇呴』鍦?pcb_analysis 涔嬪墠
}

// ---- 娴嬭瘯锛氬惊鐜緷璧栧簲鎶涘嚭寮傚父 ----
TEST(DependencySortTest, CircularDependencyThrows) {
    std::vector<MockPlugin> plugins = {
        {"A", {"B"}},
        {"B", {"A"}}
    };
    EXPECT_THROW(kahnSort(plugins), std::runtime_error);
}

// ---- 娴嬭瘯锛氳嚜渚濊禆搴旀姏鍑哄紓甯?----
TEST(DependencySortTest, SelfDependencyThrows) {
    std::vector<MockPlugin> plugins = {{"A", {"A"}}};
    EXPECT_THROW(kahnSort(plugins), std::runtime_error);
}
