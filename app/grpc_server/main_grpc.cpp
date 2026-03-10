/// @file main_grpc.cpp
/// @brief gRPC 鏈嶅姟鍣ㄦā寮忓叆鍙?
///
/// 鐢ㄦ硶锛?
///   cae_grpc_server --port=50051 --plugins=./plugins

#ifdef CAE_ENABLE_GRPC

#include "adapters/grpc/GrpcAdapter.h"
#include "core/ActionManager.h"
#include "core/RunMode.h"
#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <map>

class IPluginManager;
std::unique_ptr<IPluginManager> createPluginManager();

static GrpcAdapter* gGrpcAdapter = nullptr;

void signalHandler(int sig) {
    std::cout << "\n[gRPC Server] Received signal " << sig << ", shutting down...\n";
    if (gGrpcAdapter) gGrpcAdapter->shutdown();
}

std::map<std::string, std::string> parseArgs(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto eq = arg.find('=');
        if (arg.substr(0, 2) == "--" && eq != std::string::npos) {
            args[arg.substr(2, eq - 2)] = arg.substr(eq + 1);
        }
    }
    return args;
}

int main(int argc, char* argv[]) {
    auto args = parseArgs(argc, argv);

    std::string pluginsDir = args.count("plugins") ? args["plugins"] : "./plugins";
    std::string port       = args.count("port")    ? args["port"]    : "50051";
    std::string address    = "0.0.0.0:" + port;

    std::cout << "[gRPC Server] Starting CAE gRPC server\n";

    // 鍔犺浇鎻掍欢锛坓RPC妯″紡锛?
    auto pluginMgr = createPluginManager();
    pluginMgr->discover(pluginsDir);
    pluginMgr->initializeAll(RunMode::GRPC);

    std::cout << "[gRPC Server] Plugins loaded\n";
    std::cout << "[gRPC Server] Listening on " << address << "\n";
    std::cout << "[gRPC Server] Press Ctrl+C to stop\n";

    // 娉ㄥ唽淇″彿澶勭悊
    GrpcAdapter adapter;
    gGrpcAdapter = &adapter;
    std::signal(SIGINT,  signalHandler);
    std::signal(SIGTERM, signalHandler);

    adapter.start(address); // 闃诲鐩村埌 shutdown()

    pluginMgr->shutdownAll();
    std::cout << "[gRPC Server] Stopped\n";
    return 0;
}

#else
int main() {
    std::cerr << "gRPC server requires ENABLE_GRPC=ON at build time\n";
    return 1;
}
#endif
