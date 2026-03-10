#pragma once
#include <string>
#include <memory>

/// @brief gRPC鏈嶅姟鍣ㄩ€傞厤鍣?
///
/// 灏佽 grpc::Server 鐨勭敓鍛藉懆鏈燂細鏋勫缓 鈫?鍚姩 鈫?浼橀泤鍏抽棴
class GrpcAdapter {
public:
    GrpcAdapter() = default;
    ~GrpcAdapter();

    /// @brief 鍚姩 gRPC 鏈嶅姟鍣紙闃诲锛?
    /// @param address 鐩戝惉鍦板潃锛屽 "0.0.0.0:50051"
    /// @return 鏄惁鎴愬姛鍚姩
    bool start(const std::string& address);

    /// @brief 寮傛鍚姩锛堝湪鍚庡彴绾跨▼杩愯锛?
    bool startAsync(const std::string& address);

    /// @brief 浼橀泤鍏抽棴鏈嶅姟鍣?
    void shutdown();

    /// @brief 鑾峰彇瀹為檯鐩戝惉绔彛
    int getListeningPort() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
