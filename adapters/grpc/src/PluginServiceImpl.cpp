/// @file PluginServiceImpl.cpp
/// @brief gRPC PluginService 鏈嶅姟绔疄鐜?///
/// 鎵€鏈?RPC 鏂规硶鏈€缁堝鎵樼粰 ActionManager::invoke()锛?/// 纭繚 gRPC 璋冪敤涓?GUI 鐐瑰嚮銆丳ython invoke() 璧板畬鍏ㄧ浉鍚岀殑鎵ц璺緞銆?
#ifdef CAE_ENABLE_GRPC

// 鍖呭惈 protobuf 鐢熸垚鐨勫ご鏂囦欢锛堢敱 cmake/FindgRPC.cmake 鐢熸垚鍒?build/generated/锛?#include "plugin_service.grpc.pb.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include "core/RunMode.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using cae::PluginService;
using cae::ExecuteActionRequest;
using cae::ExecuteActionResponse;
using cae::ListActionsRequest;
using cae::ListActionsResponse;
using cae::LoadPluginRequest;
using cae::LoadPluginResponse;
using cae::ListPluginsRequest;
using cae::ListPluginsResponse;
using cae::StreamActionLogRequest;
using cae::ActionLogEntry;

class PluginServiceImpl final : public PluginService::Service {
public:
    // ----------------------------------------------------------
    // ExecuteAction锛氭牳蹇?RPC锛屽鎵樼粰 ActionManager::invoke
    // ----------------------------------------------------------
    Status ExecuteAction(ServerContext* context,
                         const ExecuteActionRequest* request,
                         ExecuteActionResponse* response) override {
        ActionContext ctx;
        ctx.sourceMode = RunMode::GRPC;
        for (const auto& [k, v] : request->params()) {
            ctx.params[k] = v;
        }


        try {
            ActionManager::instance().invoke(request->action_id(), ctx);
            response->set_success(true);
            response->set_message("Action executed successfully");
        } catch (const std::exception& e) {
            response->set_success(false);
            response->set_message(e.what());
            return Status(grpc::StatusCode::NOT_FOUND, e.what());
        }
        return Status::OK;
    }


    // ----------------------------------------------------------
    // ListActions锛氳繑鍥炴墍鏈夊凡娉ㄥ唽 Action 淇℃伅
    // ----------------------------------------------------------
    Status ListActions(ServerContext* context,
                       const ListActionsRequest* request,
                       ListActionsResponse* response) override {
        const std::string& prefix = request->filter_prefix();
        for (const auto& desc : ActionManager::instance().listActions()) {
            if (!prefix.empty() && desc.id.substr(0, prefix.size()) != prefix) continue;
            auto* info = response->add_actions();
            info->set_id(desc.id);
            info->set_label(desc.label);
            info->set_tooltip(desc.tooltip);
            info->set_shortcut(desc.shortcut);
            info->set_menu_path(desc.menuPath);
        }
        return Status::OK;
    }


    // ----------------------------------------------------------
    // LoadPlugin锛氬姩鎬佸姞杞芥彃浠讹紙濮旀墭缁?PluginManager锛?    // ----------------------------------------------------------
    Status LoadPlugin(ServerContext* context,
                      const LoadPluginRequest* request,
                      LoadPluginResponse* response) override {
        // 瀹為檯瀹炵幇闇€鎸佹湁 PluginManager 寮曠敤
        // 姝ゅ涓洪鏋跺疄鐜?        response->set_success(false);
        response->set_message("Dynamic load not yet implemented in this skeleton");
        return Status::OK;
    }


    // ----------------------------------------------------------
    // ListPlugins锛氬垪鍑哄凡鍔犺浇鎻掍欢
    // ----------------------------------------------------------
    Status ListPlugins(ServerContext* context,
                       const ListPluginsRequest* request,
                       ListPluginsResponse* response) override {
        // 楠ㄦ灦瀹炵幇锛氬疄闄呴渶鎸佹湁 PluginManager 寮曠敤
        return Status::OK;
    }


    // ----------------------------------------------------------
    // StreamActionLog锛氭祦寮忔帹閫佹墽琛屾棩蹇?    // ----------------------------------------------------------
    Status StreamActionLog(ServerContext* context,
                           const StreamActionLogRequest* request,
                           ServerWriter<ActionLogEntry>* writer) override {
        // 娉ㄥ唽 ActionManager 鐨?Observer 灏嗘棩蹇楁帹閫佺粰瀹㈡埛绔?        // 姝ゅ涓洪鏋跺疄鐜?        ActionLogEntry entry;
        auto now = std::chrono::system_clock::now();
        auto t   = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::gmtime(&t), "%Y-%m-%dT%H:%M:%SZ");
        entry.set_timestamp(oss.str());
        entry.set_level("INFO");
        entry.set_message("Streaming started for action: " + request->action_id());
        writer->Write(entry);
        return Status::OK;
    }
};

#endif // CAE_ENABLE_GRPC
