/// @file PythonModule.cpp
/// @brief pybind11 模块入口：导出 Application / ActionManager / ActionDescriptor
///
/// Python 使用示例：
///   import cae_plugin
///   app = cae_plugin.Application.instance()
///   am  = app.action_manager()
///
///   am.invoke("Desktop.NewProject")
///   am.invoke("Layout.RunDRC")
///
///   am.register_action("Tools.MyScript", "Run My Script",
///                       lambda: print("hello"), "tools_menu")
///
///   for a in am.list_actions():
///       print(a.id, a.label, a.shortcut)

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "adapters/python/PythonAdapter.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include "core/RunMode.h"

namespace py = pybind11;

// ============================================================
// Python-friendly ActionManager 包装类
// ============================================================
class PyActionManager {
public:
    static PyActionManager& instance() {
        static PyActionManager inst;
        return inst;
    }

    /// 触发 Action（等价于 GUI 菜单点击）
    void invoke(const std::string& id,
                const std::unordered_map<std::string, std::string>& params = {}) {
        py::gil_scoped_release release; // 释放 GIL，避免回调中的死锁
        PythonAdapter::instance().invoke(id, params);
    }

    /// 注册 Python Action（callback 为 Python callable）
    void registerAction(const std::string& id,
                        const std::string& label,
                        py::object callback,
                        const std::string& menuPath = "") {
        // 持有 Python callable 的生命周期
        auto cb = [callback]() {
            py::gil_scoped_acquire acquire;
            try {
                callback();
            } catch (const py::error_already_set& e) {
                std::cerr << "[PyActionManager] Python callback error: " << e.what() << "\n";
            }
        };
        PythonAdapter::instance().registerAction(id, label, std::move(cb), menuPath);
    }

    /// 获取所有已注册 Action
    std::vector<ActionDescriptor> listActions() const {
        return PythonAdapter::instance().listActions();
    }

    void setShortcut(const std::string& id, const std::string& shortcut) {
        PythonAdapter::instance().setShortcut(id, shortcut);
    }
};

// ============================================================
// Application 单例包装（KLayout风格：app = cae_plugin.Application.instance()）
// ============================================================
class PyApplication {
public:
    static PyApplication& instance() {
        static PyApplication inst;
        return inst;
    }

    PyActionManager& actionManager() {
        return PyActionManager::instance();
    }

    std::string version() const { return "1.0.0"; }
};

// ============================================================
// PYBIND11_MODULE 定义
// ============================================================
PYBIND11_MODULE(cae_plugin, m) {
    m.doc() = R"doc(
        CAE PCB Plugin System - Python Interface
        
        Usage:
            import cae_plugin
            app = cae_plugin.Application.instance()
            am  = app.action_manager()
            
            # Invoke existing action (equivalent to GUI menu click)
            am.invoke("Desktop.NewProject")
            am.invoke("Layout.RunDRC", {"net": "GND"})
            
            # Register new action
            am.register_action("Tools.MyDRC", "My DRC", lambda: print("done"), "tools_menu")
            
            # List all actions
            for a in am.list_actions():
                print(a.id, a.label, a.shortcut)
    )doc";

    // ---- ActionDescriptor 只读视图 ----
    py::class_<ActionDescriptor>(m, "ActionDescriptor")
        .def_readonly("id",        &ActionDescriptor::id)
        .def_readonly("label",     &ActionDescriptor::label)
        .def_readonly("tooltip",   &ActionDescriptor::tooltip)
        .def_readonly("shortcut",  &ActionDescriptor::shortcut)
        .def_readonly("menu_path", &ActionDescriptor::menuPath)
        .def("__repr__", [](const ActionDescriptor& d) {
            return "<ActionDescriptor id='" + d.id + "' label='" + d.label + "'>";
        });

    // ---- ActionManager Python包装 ----
    py::class_<PyActionManager>(m, "ActionManager")
        .def_static("instance",    &PyActionManager::instance,
                    py::return_value_policy::reference)
        .def("invoke",             &PyActionManager::invoke,
                    py::arg("id"), py::arg("params") = std::unordered_map<std::string,std::string>{},
                    "Invoke an action by ID (equivalent to GUI menu click)")
        .def("register_action",    &PyActionManager::registerAction,
                    py::arg("id"), py::arg("label"),
                    py::arg("callback"), py::arg("menu_path") = "",
                    "Register a new action with Python callback")
        .def("list_actions",       &PyActionManager::listActions,
                    "Return list of all registered ActionDescriptors")
        .def("set_shortcut",       &PyActionManager::setShortcut,
                    py::arg("id"), py::arg("shortcut"),
                    "Update keyboard shortcut for an action");

    // ---- Application 单例 ----
    py::class_<PyApplication>(m, "Application")
        .def_static("instance",    &PyApplication::instance,
                    py::return_value_policy::reference)
        .def("action_manager",     &PyApplication::actionManager,
                    py::return_value_policy::reference,
                    "Get the global ActionManager instance")
        .def("version",            &PyApplication::version);
}
