#pragma once
#ifdef CAE_ENABLE_GUI

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QLabel>
#include <QToolBar>
#include <memory>

class MenuBuilder;
class RibbonBar;

/// @brief CAE主窗口（参考ANSYS Electronics Desktop界面风格）
///
/// 界面布局：
///   菜单栏 | 快捷工具栏
///   ─────────────────────────
///   Ribbon工具栏（Tab+Group）
///   ─────────────────────────
///   中心工作区
///   ─────────────────────────
///   状态栏
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    /// @brief 获取顶层菜单（供插件或测试使用）
    QMenu* getMenu(const std::string& menuName) const;

    /// @brief 获取MenuBuilder实例（供UIActionBridge使用）
    MenuBuilder* getMenuBuilder() const;

    /// @brief 获取RibbonBar实例
    RibbonBar* getRibbonBar() const;

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void setupMenuBar();
    void setupQuickToolBar();
    void setupRibbonBar();
    void setupStatusBar();
    void setupCentralWidget();

    std::unique_ptr<MenuBuilder> menuBuilder_;
    std::unique_ptr<RibbonBar> ribbonBar_;
    QToolBar* quickToolBar_;
    QLabel* statusLabel_;
};

#endif // CAE_ENABLE_GUI
