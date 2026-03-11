#pragma once
#include <string>
#include <memory>

/// @brief gRPC服务器适配器
///
/// 封装 grpc::Server 的生命周期：构建 → 启动 → 优雅关闭
class GrpcAdapter {
public:
    GrpcAdapter() = default;
    ~GrpcAdapter();

    /// @brief 启动 gRPC 服务器（阻塞）
    /// @param address 监听地址，如 "0.0.0.0:50051"
    /// @return 是否成功启动
    bool start(const std::string& address);

    /// @brief 异步启动（在后台线程运行）
    bool startAsync(const std::string& address);

    /// @brief 优雅关闭服务器
    void shutdown();

    /// @brief 获取实际监听端口
    int getListeningPort() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
