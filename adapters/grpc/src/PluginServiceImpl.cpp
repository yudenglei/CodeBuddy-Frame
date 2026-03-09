/// @file PluginServiceImpl.cpp
/// @brief gRPC PluginService 服务端实现
///
/// 所有 RPC 方法最终委托给 ActionManager::invoke()，
/// 确保 gRPC 调用与 GUI 点击、Python invoke() 走完全相同的执行路径。

#ifdef CAE_ENABLE_GRPC

// 包含 protobuf 生成的头文件（由 cmake/FindgRPC.cmake 生成到 build/generated/）
#include "plugin_service.grpc.pb.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
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
    // ExecuteAction：核心 RPC，委托给 ActionManager::invoke
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
    // ListActions：返回所有已注册 Action 信息
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
    // LoadPlugin：动态加载插件（委托给 PluginManager）
    // ----------------------------------------------------------
    Status LoadPlugin(ServerContext* context,
                      const LoadPluginRequest* request,
                      LoadPluginResponse* response) override {
        // 实际实现需持有 PluginManager 引用
        // 此处为骨架实现
        response->set_success(false);
        response->set_message("Dynamic load not yet implemented in this skeleton");
        return Status::OK;
    }

    // ----------------------------------------------------------
    // ListPlugins：列出已加载插件
    // ----------------------------------------------------------
    Status ListPlugins(ServerContext* context,
                       const ListPluginsRequest* request,
                       ListPluginsResponse* response) override {
        // 骨架实现：实际需持有 PluginManager 引用
        return Status::OK;
    }

    // ----------------------------------------------------------
    // StreamActionLog：流式推送执行日志
    // ----------------------------------------------------------
    Status StreamActionLog(ServerContext* context,
                           const StreamActionLogRequest* request,
                           ServerWriter<ActionLogEntry>* writer) override {
        // 注册 ActionManager 的 Observer 将日志推送给客户端
        // 此处为骨架实现
        ActionLogEntry entry;
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
