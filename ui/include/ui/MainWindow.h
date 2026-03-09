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
/// 菜单结构：Desktop | View | Layout | Tools
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    QMenu* getMenu(const std::string& menuName) const;
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
