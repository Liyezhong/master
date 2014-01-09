/****************************************************************************/
/*! \file UserPrivilegeWidget.h
 *
 *  \brief UserPrivilegeWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28, 2012-10-30
 *   $Author:  $ M.Scherer, N.Kamath, Raju
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef USERS_USERPRIVILEGEWIDGET_H
#define USERS_USERPRIVILEGEWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "MainMenu/Include/MainWindow.h"
#include <QRegExpValidator>

class QTimer;

namespace Users {

namespace Ui {
    class CUserPrivilegeWidget;
}

/****************************************************************************/
/**
 * \brief Main widget for user configuration and switching
 */
/****************************************************************************/
class CUserPrivilegeWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    friend class CTestUsers;

private:
    Ui::CUserPrivilegeWidget *mp_Ui;                //!< User interface
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;         //!< Reference to Keyboard widget
    MainMenu::CMainWindow *mp_MainWindow;           //!< pointer to main window
    bool m_StdUserBtnClicked;                       //!< True if Std user btn was clicked
    bool m_ServiceUserBtnClicked;                   //!< True if Service user btn was Clicked
    bool m_SupervisorBtnClicked;                    //!< True if Supervisor user btn was clicked
    bool m_ChangePasswdBtnClicked;                  //!< True if Change password btn was clicked
    QString m_PwdType;                              //!< Type of the password is stored i.e. Old, New and Confirm
    MainMenu::CMainWindow::UserRole_t m_UserLevel;  //!< Current user Level
    void RetranslateUI();
	QTimer* m_Timer;
    void ConnectKeyBoardSignalSlots();
    void DisconnectKeyBoardSignalSlots();
    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CUserPrivilegeWidget)

public:
    explicit CUserPrivilegeWidget(QWidget *p_Parent = 0,
                                  KeyBoard::CKeyBoard *p_KeyBoardWidget = NULL);
    ~CUserPrivilegeWidget();

protected:
    void changeEvent(QEvent *p_Event);

private slots:
    void OnBtnServiceClicked();
    void OnBtnAdministratorClicked();
    void OnBtnUserClicked();
    void OnBtnChangePasswordClicked();
    void OnOkClicked(QString EnteredString);
    void OnESCClicked();
	void AppIdleForLongTime();
public slots:
    void UserAuthenticated(const qint32 &AuthenticatedLevel);
    void ChangeInAdminPassword(const QString &PasswordType);
    void OnInteractStart();
signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when the langauge is changed.
     *
     */
    /****************************************************************************/
    void UserLevelClicked(QDataStream &);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when Change Password button is clicked.
     *
     */
    /****************************************************************************/
    void ChangePasswordClicked(QDataStream &);

};

} // end namespace Users

#endif // USERPRIVILEGEWIDGET_H
