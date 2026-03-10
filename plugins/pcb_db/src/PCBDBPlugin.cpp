#include "core/IPlugin.h"
#include "core/IDBEngine.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>
#include <string>

// ============================================================
// PCB鏁版嵁搴撳紩鎿庡疄鐜帮紙绠€鍖栫ず渚嬶級
// ============================================================
class PCBDBEngine : public IDBEngine {
public:
    bool open(const std::string& path) override {
        dbPath_ = path;
        isOpen_ = true;
        std::cout << "[PCBDBEngine] Opened: " << path << "\n";
        return true;
    }
    void close() override {
        isOpen_ = false;
        std::cout << "[PCBDBEngine] Closed\n";
    }
    bool isOpen() const override { return isOpen_; }
    std::string query(const std::string& sql) override {
        return "{\"result\": \"mock\", \"query\": \"" + sql + "\"}";
    }
    bool beginTransaction()    override { return true; }
    bool commitTransaction()   override { return true; }
    bool rollbackTransaction() override { return true; }
    std::string getEngineName() const override { return "PCBSQLite"; }

private:
    std::string dbPath_;
    bool isOpen_{false};
};

// ============================================================
// PCBDBPlugin锛氱函DB鎻掍欢锛圖B_ONLY锛夛紝娉ㄥ唽 Layout 鑿滃崟 Actions
// ============================================================
class PCBDBPlugin : public IPlugin {
public:
    PluginMeta getMeta() const override {
        return PluginMeta{
            "pcb_db",
            "1.0.0",
            "PCB Database Plugin: Layer/Cell/DRC/Net management",
            PluginType::DB_ONLY,
            {}  // 鏃犱緷璧?
        };
    }

    bool isCompatible(RunMode /*mode*/) const override {
        return true; // DB_ONLY 鍦ㄦ墍鏈夋ā寮忎笅鍧囧彲鐢?
    }

    bool initialize(RunMode mode) override {
        dbEngine_ = std::make_unique<PCBDBEngine>();
        registerLayoutActions();
        std::cout << "[PCBDBPlugin] Initialized\n";
        return true;
    }

    void shutdown() override {
        if (dbEngine_ && dbEngine_->isOpen()) {
            dbEngine_->close();
        }
        std::cout << "[PCBDBPlugin] Shutdown\n";
    }

    IDBEngine* getDBEngine() { return dbEngine_.get(); }

private:
    void registerLayoutActions() {
        auto& am = ActionManager::instance();

        am.registerAction({
            "Layout.LayerManager", "Layer Manager...",
            "Open layer management panel", "",
            "L", "layout_menu",
            [this](const ActionContext& ctx) {
                std::cout << "[Action] Layout.LayerManager - layers in DB: "
                          << (dbEngine_ ? dbEngine_->query("SELECT layers") : "N/A") << "\n";
            }
        });

        am.registerAction({
            "Layout.CellHierarchy", "Cell Hierarchy...",
            "Show cell hierarchy browser", "",
            "", "layout_menu",
            [this](const ActionContext& ctx) {
                std::cout << "[Action] Layout.CellHierarchy executed\n";
            }
        });

        am.registerAction({
            "Layout.Sep1", "",
            "", "", "", "layout_menu/---",
            nullptr
        });

        am.registerAction({
            "Layout.RunDRC", "Design Rule Check",
            "Run design rule check on current layout", "",
            "F5", "layout_menu",
            [this](const ActionContext& ctx) {
                std::cout << "[Action] Layout.RunDRC - running DRC...\n";
                if (dbEngine_ && dbEngine_->isOpen()) {
                    auto result = dbEngine_->query("DRC CHECK ALL");
                    std::cout << "  DRC result: " << result << "\n";
                } else {
                    std::cout << "  No design open\n";
                }
            }
        });

        am.registerAction({
            "Layout.NetInspector", "Net Inspector...",
            "Open net inspection tool", "",
            "", "layout_menu",
            [this](const ActionContext& ctx) {
                std::cout << "[Action] Layout.NetInspector executed\n";
            }
        });
    }

    std::unique_ptr<PCBDBEngine> dbEngine_;
};

// ============================================================
// 瀵煎嚭鍑芥暟
// ============================================================
extern "C" {
    IPlugin* createPlugin()         { return new PCBDBPlugin(); }
    void destroyPlugin(IPlugin* p)  { delete p; }
}
