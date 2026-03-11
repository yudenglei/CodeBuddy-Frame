/// @file GrpcClient.cpp
/// @brief gRPC 客户端示例（用于集成测试和远程调用演示）

#ifdef CAE_ENABLE_GRPC

#include "plugin_service.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

/// @brief PluginService gRPC 客户端
class PluginServiceClient {
public:
    explicit PluginServiceClient(const std::string& serverAddress)
        : stub_(cae::PluginService::NewStub(
              grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials())))
    {}

    /// @brief 触发远程 Action
    bool executeAction(const std::string& actionId,
                       const std::unordered_map<std::string, std::string>& params = {}) {
        cae::ExecuteActionRequest request;
        request.set_action_id(actionId);
        for (const auto& [k, v] : params) {
            (*request.mutable_params())[k] = v;
        }


        cae::ExecuteActionResponse response;
        grpc::ClientContext context;
        grpc::Status status = stub_->ExecuteAction(&context, request, &response);


        if (!status.ok()) {
            std::cerr << "[GrpcClient] ExecuteAction RPC failed: "
                      << status.error_message() << "\n";
            return false;
        }


        std::cout << "[GrpcClient] Action '" << actionId << "' result: "
                  << (response.success() ? "OK" : "FAIL")
                  << " - " << response.message() << "\n";
        return response.success();
    }


    /// @brief 列出所有远程 Actions
    void listActions(const std::string& prefix = "") {
        cae::ListActionsRequest request;
        request.set_filter_prefix(prefix);


        cae::ListActionsResponse response;
        grpc::ClientContext context;
        grpc::Status status = stub_->ListActions(&context, request, &response);


        if (!status.ok()) {
            std::cerr << "[GrpcClient] ListActions RPC failed\n";
            return;
        }


        std::cout << "[GrpcClient] Available actions (" << response.actions_size() << "):\n";
        for (const auto& action : response.actions()) {
            std::cout << "  " << action.id();
            if (!action.shortcut().empty()) std::cout << " [" << action.shortcut() << "]";
            std::cout << " - " << action.label() << "\n";
        }
    }


private:
    std::unique_ptr<cae::PluginService::Stub> stub_;
};

#endif // CAE_ENABLE_GRPC
