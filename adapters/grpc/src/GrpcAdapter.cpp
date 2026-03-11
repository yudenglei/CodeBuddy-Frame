#ifdef CAE_ENABLE_GRPC

#include "adapters/grpc/GrpcAdapter.h"
#include "plugin_service.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <thread>
#include <iostream>
#include <memory>

// 引入服务实现（同编译单元）
// PluginServiceImpl 在 PluginServiceImpl.cpp 中定义
class PluginServiceImpl;

struct GrpcAdapter::Impl {
    std::unique_ptr<grpc::Server> server;
    std::unique_ptr<PluginServiceImpl> service;
    std::thread serverThread;
    int port{0};
};

GrpcAdapter::~GrpcAdapter() {
    shutdown();
}

bool GrpcAdapter::start(const std::string& address) {
    impl_ = std::make_unique<Impl>();
    impl_->service = std::make_unique<PluginServiceImpl>();

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials(), &impl_->port);
    builder.RegisterService(impl_->service.get());

    impl_->server = builder.BuildAndStart();
    if (!impl_->server) {
        std::cerr << "[GrpcAdapter] Failed to start server on " << address << "\n";
        return false;
    }
    std::cout << "[GrpcAdapter] Server listening on " << address
              << " (port=" << impl_->port << ")\n";
    impl_->server->Wait(); // 阻塞
    return true;
}

bool GrpcAdapter::startAsync(const std::string& address) {
    impl_ = std::make_unique<Impl>();
    impl_->service = std::make_unique<PluginServiceImpl>();

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials(), &impl_->port);
    builder.RegisterService(impl_->service.get());

    impl_->server = builder.BuildAndStart();
    if (!impl_->server) {
        std::cerr << "[GrpcAdapter] Failed to start server on " << address << "\n";
        return false;
    }

    impl_->serverThread = std::thread([this]() {
        impl_->server->Wait();
    });

    std::cout << "[GrpcAdapter] Server started async on port=" << impl_->port << "\n";
    return true;
}

void GrpcAdapter::shutdown() {
    if (impl_ && impl_->server) {
        impl_->server->Shutdown();
        if (impl_->serverThread.joinable()) {
            impl_->serverThread.join();
        }
        std::cout << "[GrpcAdapter] Server shut down\n";
    }
}

int GrpcAdapter::getListeningPort() const {
    return impl_ ? impl_->port : 0;
}

#endif // CAE_ENABLE_GRPC
