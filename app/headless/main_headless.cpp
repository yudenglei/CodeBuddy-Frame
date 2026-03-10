/// @file main_headless.cpp
/// @brief 鏃犵晫闈紙Headless锛夋ā寮忓叆鍙?///
/// 鐢ㄦ硶绀轰緥锛?///   cae_headless --plugins=./plugins --action=Layout.RunDRC
///   cae_headless --plugins=./plugins --action=Desktop.Save --param=path=/tmp/out.pcb

#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include "core/RunMode.h"
#include "core/IPluginManager.h"
#include <iostream>
#include <string>
#include <memory>
#include <map>

// PluginManager 宸ュ巶鍑芥暟锛堝湪 PluginManager.cpp 涓畾涔夛級
std::unique_ptr<IPluginManager> createPluginManager();

/// @brief 绠€鍗曞懡浠よ鍙傛暟瑙ｆ瀽
std::map<std::string, std::string> parseArgs(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto eq = arg.find('=');
        if (arg.substr(0, 2) == "--" && eq != std::string::npos) {
            args[arg.substr(2, eq - 2)] = arg.substr(eq + 1);
        } else if (arg.substr(0, 2) == "--") {
            args[arg.substr(2)] = "true";
        }
    }
    return args;
}

int main(int argc, char* argv[]) {
    auto args = parseArgs(argc, argv);

    std::string pluginsDir = args.count("plugins") ? args["plugins"] : "./plugins";
    std::string actionId   = args.count("action")  ? args["action"]  : "";

    std::cout << "[Headless] Starting CAE in headless mode\n";
    std::cout << "[Headless] Plugins dir: " << pluginsDir << "\n";

    // 鍔犺浇鎻掍欢
    auto pluginMgr = createPluginManager();
    pluginMgr->discover(pluginsDir);
    pluginMgr->initializeAll(RunMode::HEADLESS);

    std::cout << "[Headless] Plugins loaded successfully\n";

    if (!actionId.empty()) {
        std::cout << "[Headless] Executing action: " << actionId << "\n";

        ActionContext ctx;
        ctx.sourceMode = RunMode::HEADLESS;
        // 灏嗛澶?--param=key:value 鍙傛暟鍔犲叆涓婁笅鏂?        for (const auto& [k, v] : args) {
            if (k != "plugins" && k != "action") {
                ctx.params[k] = v;
            }
        }

        try {
            ActionManager::instance().invoke(actionId, ctx);
            std::cout << "[Headless] Action completed\n";
        } catch (const std::exception& e) {
            std::cerr << "[Headless] Action failed: " << e.what() << "\n";
            return 1;
        }
    } else {
        // 鏃?--action 鏃跺垪鍑烘墍鏈夊彲鐢?Actions
        std::cout << "[Headless] Available actions:\n";
        for (const auto& desc : ActionManager::instance().listActions()) {
            std::cout << "  " << desc.id;
            if (!desc.shortcut.empty()) std::cout << " [" << desc.shortcut << "]";
            std::cout << "\n";
        }
    }

    pluginMgr->shutdownAll();
    return 0;
}
