#pragma once
#include <string>
#include <vector>

class IDBEngine {
public:
    virtual ~IDBEngine() = default;
    virtual bool open(const std::string& path) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual std::string query(const std::string& sql) = 0;
    virtual bool beginTransaction() = 0;
    virtual bool commitTransaction() = 0;
    virtual bool rollbackTransaction() = 0;
    virtual std::string getEngineName() const = 0;
};
