/// @file PythonModule.cpp
/// @brief pybind11 妯″潡鍏ュ彛锛氬鍑?Application / ActionManager / ActionDescriptor
///
/// Python 浣跨敤绀轰緥锛?
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
// Python-friendly ActionManager 鍖呰绫?
// ============================================================
class PyActionManager {
public:
    static PyActionManager& instance() {
        static PyActionManager inst;
        return inst;
    }

    /// 瑙﹀彂 Action锛堢瓑浠蜂簬 GUI 鑿滃崟鐐瑰嚮锛?
    void invoke(const std::string& id,
                const std::unordered_map<std::string, std::string>& params = {}) {
        py::gil_scoped_release release; // 閲婃斁 GIL锛岄伩鍏嶅洖璋冧腑鐨勬閿?
        PythonAdapter::instance().invoke(id, params);
    }

    /// 娉ㄥ唽 Python Action锛坈allback 涓?Python callable锛?
    void registerAction(const std::string& id,
                        const std::string& label,
                        py::object callback,
                        const std::string& menuPath = "") {
        // 鎸佹湁 Python callable 鐨勭敓鍛藉懆鏈?
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

    /// 鑾峰彇鎵€鏈夊凡娉ㄥ唽 Action
    std::vector<ActionDescriptor> listActions() const {
        return PythonAdapter::instance().listActions();
    }

    void setShortcut(const std::string& id, const std::string& shortcut) {
        PythonAdapter::instance().setShortcut(id, shortcut);
    }
};

// ============================================================
// Application 鍗曚緥鍖呰锛圞Layout椋庢牸锛歛pp = cae_plugin.Application.instance()锛?
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
// PYBIND11_MODULE 瀹氫箟
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

    // ---- ActionDescriptor 鍙瑙嗗浘 ----
    py::class_<ActionDescriptor>(m, "ActionDescriptor")
        .def_readonly("id",        &ActionDescriptor::id)
        .def_readonly("label",     &ActionDescriptor::label)
        .def_readonly("tooltip",   &ActionDescriptor::tooltip)
        .def_readonly("shortcut",  &ActionDescriptor::shortcut)
        .def_readonly("menu_path", &ActionDescriptor::menuPath)
        .def("__repr__", [](const ActionDescriptor& d) {
            return "<ActionDescriptor id='" + d.id + "' label='" + d.label + "'>";
        });

    // ---- ActionManager Python鍖呰 ----
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

    // ---- Application 鍗曚緥 ----
    py::class_<PyApplication>(m, "Application")
        .def_static("instance",    &PyApplication::instance,
                    py::return_value_policy::reference)
        .def("action_manager",     &PyApplication::actionManager,
                    py::return_value_policy::reference,
                    "Get the global ActionManager instance")
        .def("version",            &PyApplication::version);
}
