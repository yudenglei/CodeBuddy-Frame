#pragma once
#include <string>
#include <vector>

/// @brief 数据库引擎抽象接口
/// 插件通过此接口访问PCB设计数据库，与底层实现解耦
class IDBEngine {
public:
    virtual ~IDBEngine() = default;

    /// @brief 打开设计文件/数据库
    virtual bool open(const std::string& path) = 0;

    /// @brief 关闭当前数据库
    virtual void close() = 0;

    /// @brief 检查是否已打开
    virtual bool isOpen() const = 0;

    /// @brief 执行查询，返回结果集（JSON格式字符串）
    virtual std::string query(const std::string& sql) = 0;

    /// @brief 开始事务
    virtual bool beginTransaction() = 0;

    /// @brief 提交事务
    virtual bool commitTransaction() = 0;

    /// @brief 回滚事务
    virtual bool rollbackTransaction() = 0;

    /// @brief 获取数据库引擎名称（如 "PCBSQLite"）
    virtual std::string getEngineName() const = 0;
};
