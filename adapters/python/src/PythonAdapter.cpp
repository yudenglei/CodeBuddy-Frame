#include "adapters/python/PythonAdapter.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include "core/RunMode.h"
#include <stdexcept>
#include <unordered_map>

PythonAdapter& PythonAdapter::instance() {
    static PythonAdapter inst;
    return inst;
}

void PythonAdapter::invoke(const std::string& actionId,
                           const std::unordered_map<std::string, std::string>& params) {
    ActionContext ctx;
    ctx.sourceMode = RunMode::PYTHON;
    for (const auto& [k, v] : params) {
        ctx.params[k] = v; // string 类型参数
    }
    ActionManager::instance().invoke(actionId, ctx);
}

void PythonAdapter::registerAction(const std::string& id,
                                   const std::string& label,
                                   std::function<void()> callback,
                                   const std::string& menuPath) {
    ActionDescriptor desc;
    desc.id       = id;
    desc.label    = label;
    desc.menuPath = menuPath;
    desc.callback = [cb = std::move(callback)](const ActionContext&) {
        if (cb) cb();
    };
    ActionManager::instance().registerAction(desc);
}

std::vector<ActionDescriptor> PythonAdapter::listActions() const {
    return ActionManager::instance().listActions();
}

void PythonAdapter::setShortcut(const std::string& id, const std::string& shortcut) {
    ActionManager::instance().setShortcut(id, shortcut);
}
