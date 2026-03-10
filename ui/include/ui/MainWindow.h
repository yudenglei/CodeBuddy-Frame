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

/// @brief CAE涓荤獥鍙ｏ紙鍙傝€傾NSYS Electronics Desktop鐣岄潰椋庢牸锛?///
/// 鐣岄潰甯冨眬锛?///   鑿滃崟鏍?| 蹇嵎宸ュ叿鏍?///   鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€
///   Ribbon宸ュ叿鏍忥紙Tab+Group锛?///   鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€
///   涓績宸ヤ綔鍖?///   鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€
///   鐘舵€佹爮
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    /// @brief 鑾峰彇椤跺眰鑿滃崟锛堜緵鎻掍欢鎴栨祴璇曚娇鐢級
    QMenu* getMenu(const std::string& menuName) const;

    /// @brief 鑾峰彇MenuBuilder瀹炰緥锛堜緵UIActionBridge浣跨敤锛?    MenuBuilder* getMenuBuilder() const;

    /// @brief 鑾峰彇RibbonBar瀹炰緥
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
