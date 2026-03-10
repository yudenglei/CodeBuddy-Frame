#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
#include "core/MenuDescriptor.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <unordered_map>
#include <string>
#include <memory>

/// @brief 鑿滃崟鑷姩鏋勫缓鍣紙鏀寔澶氱骇瀛愯彍鍗曪級
///
/// 鐩戝惉 ActionManager::onActionRegistered 浜嬩欢锛?/// 鏍规嵁 ActionDescriptor::menuPath 鑷姩鍦?QMenuBar 涓垱寤?瀹氫綅 QMenu 骞舵彃鍏?QAction銆?///
/// menuPath 鏍煎紡绾﹀畾锛圓NSYS椋庢牸锛夛細
///   "File"                          鈫?File鑿滃崟鏈熬
///   "File/Save"                     鈫?File鑿滃崟Save瀛愰」
///   "File/New"                      鈫?File鑿滃崟New瀛愰」
///   "File/before:Save"              鈫?File鑿滃崟Save涔嬪墠
///   "File/after:New"                鈫?File鑿滃崟New涔嬪悗
///   "File/Save---"                  鈫?Save涔嬪悗鍔犲垎闅旂
///   "---"                           鈫?鐙珛鍒嗛殧绗?class MenuBuilder : public QObject {
    Q_OBJECT
public:
    explicit MenuBuilder(QMenuBar* menuBar, QObject* parent = nullptr);
    ~MenuBuilder() override = default;

    /// @brief 棰勫缓椤跺眰鑿滃崟锛圓NSYS椋庢牸锛欶ile/Edit/View/Project/Draw/Modeler/HFSS/Tools/Window/Help锛?    void buildTopLevelMenus();

    /// @brief 灏嗕竴涓狝ctionDescriptor鎻掑叆鍒板搴旇彍鍗曚綅缃紙渚汷bserver鍥炶皟锛?    void insertAction(const ActionDescriptor& desc);

    /// @brief 鑾峰彇椤跺眰鑿滃崟锛堜緵澶栭儴鏌ヨ锛?    QMenu* getTopMenu(const std::string& displayName) const;

private:
    /// @brief 瑙ｆ瀽menuPath锛屾壘鍒版垨鍒涘缓瀵瑰簲鐨凲Menu锛堟敮鎸佸绾у瓙鑿滃崟锛?    QMenu* resolveMenu(const std::string& menuPath);

    /// @brief 瑙ｆ瀽menuPath骞惰繑鍥炶矾寰勪笂鐨勬墍鏈夎彍鍗?    QList<QMenu*> resolveMenuPath(const std::string& menuPath);

    /// @brief 鍦≦Menu涓寜insertPosition瀹氫綅鎻掑叆QAction
    void insertIntoMenu(QMenu* menu, QAction* action, const std::string& insertPos);

    QMenuBar* menuBar_{nullptr};
    
    /// 椤跺眰鑿滃崟琛細key(濡?File") 鈫?QMenu*
    std::unordered_map<std::string, QMenu*> topMenus_;
    
    /// 鎵€鏈夊凡鍒涘缓鐨凲Action琛細actionId 鈫?QAction*
    std::unordered_map<std::string, QAction*> actionMap_;
    
    /// 鑿滃崟璺緞缂撳瓨锛歮enuPath 鈫?QMenu*锛堢敤浜庡揩閫熸煡鎵撅級
    std::unordered_map<std::string, QMenu*> menuPathCache_;
};

#endif // CAE_ENABLE_GUI
