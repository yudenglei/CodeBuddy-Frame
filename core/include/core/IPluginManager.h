#pragma once
#include <string>
#include <vector>
#include "IPlugin.h"
#include "RunMode.h"

class IPluginManager {
public:
    virtual ~IPluginManager() = default;
    virtual bool discover(const std::string& pluginDir) = 0;
    virtual bool initializeAll(RunMode mode) = 0;
    virtual void shutdownAll() = 0;
    virtual IPlugin* getPlugin(const std::string& name) const = 0;
    virtual std::vector<IPlugin*> getAllPlugins() const = 0;
};
