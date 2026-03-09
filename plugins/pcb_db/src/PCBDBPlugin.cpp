#include "core/IPlugin.h"
#include "core/IDBEngine.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>
#include <string>
#include <memory>

class PCBDBEngine : public IDBEngine {
public:
    bool open(const std::string& path) override { dbPath_ = path; isOpen_ = true; std::cout << "[PCBDBEngine] Opened: " << path << "\n"; return true; }
    void close() override { isOpen_ = false; }
    bool isOpen() const override { return isOpen_; }
    std::string query(const std::string& sql) override { return "{\"result\":\"mock\",\"query\":\"" + sql + "\"}"; }
    bool beginTransaction()    override { return true; }
    bool commitTransaction()   override { return true; }
    bool rollbackTransaction() override { return true; }
    std::string getEngineName() const override { return "PCBSQLite"; }
private:
    std::string dbPath_;
    bool isOpen_{false};
};

class PCBDBPlugin : public IPlugin {
public:
    PluginMeta getMeta() const override {
        return {"pcb_db", "1.0.0", "PCB Database Plugin", PluginType::DB_ONLY, {}};
    }
    bool isCompatible(RunMode) const override { return true; }
    bool initialize(RunMode) override {
        dbEngine_ = std::make_unique<PCBDBEngine>();
        registerLayoutActions();
        std::cout << "[PCBDBPlugin] Initialized\n";
        return true;
    }
    void shutdown() override {
        if (dbEngine_ && dbEngine_->isOpen()) dbEngine_->close();
        std::cout << "[PCBDBPlugin] Shutdown\n";
    }
private:
    void registerLayoutActions() {
        auto& am = ActionManager::instance();
        am.registerAction({"Layout.LayerManager",  "Layer Manager...",  "Open layer management",  "", "L",  "layout_menu",
            [this](const ActionContext&){ std::cout << "[Action] Layout.LayerManager\n"; if(dbEngine_) dbEngine_->query("SELECT layers"); }});
        am.registerAction({"Layout.CellHierarchy", "Cell Hierarchy...", "Show cell hierarchy",    "", "",   "layout_menu",
            [](const ActionContext&){ std::cout << "[Action] Layout.CellHierarchy\n"; }});
        am.registerAction({"Layout.Sep1", "", "", "", "", "layout_menu/---", nullptr});
        am.registerAction({"Layout.RunDRC", "Design Rule Check", "Run DRC", "", "F5", "layout_menu",
            [this](const ActionContext&){
                std::cout << "[Action] Layout.RunDRC\n";
                if (dbEngine_ && dbEngine_->isOpen()) std::cout << "  " << dbEngine_->query("DRC CHECK ALL") << "\n";
                else std::cout << "  No design open\n";
            }});
        am.registerAction({"Layout.NetInspector", "Net Inspector...", "Open net inspector", "", "", "layout_menu",
            [](const ActionContext&){ std::cout << "[Action] Layout.NetInspector\n"; }});
    }
    std::unique_ptr<PCBDBEngine> dbEngine_;
};

extern "C" {
    IPlugin* createPlugin()        { return new PCBDBPlugin(); }
    void destroyPlugin(IPlugin* p) { delete p; }
}
