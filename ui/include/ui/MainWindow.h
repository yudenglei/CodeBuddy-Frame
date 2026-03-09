#pragma once
#ifdef CAE_ENABLE_GUI

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QLabel>
#include <memory>

class MenuBuilder;

/// @brief CAE主窗口（参考HFSS 3D Layout界面风格）
///
/// 菜单结构：
///   Desktop | View | Layout | Tools
///
/// 菜单内容由插件通过 ActionManager::registerAction() 动态注入，
/// MenuBuilder 监听 onActionRegistered 事件后自动创建 QAction 并插入菜单。
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    /// @brief 获取顶层菜单（供插件或测试使用）
    QMenu* getMenu(const std::string& menuName) const;

    /// @brief 获取MenuBuilder实例（供UIActionBridge使用）
    MenuBuilder* getMenuBuilder() const;

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();

    std::unique_ptr<MenuBuilder> menuBuilder_;
    QLabel* statusLabel_{nullptr};
};

#endif // CAE_ENABLE_GUI
