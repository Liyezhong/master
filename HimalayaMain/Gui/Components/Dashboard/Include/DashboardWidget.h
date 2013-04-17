/****************************************************************************/
/*! \file DashboardWidget.h
 *
 *  \brief Dashboard Widget Class Definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-03
 *   $Author:  $ Swati Tiwari
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Dashboard/Include/DashboardScene.h"
#include "Dashboard/Include/DashboardComboBox.h"
#include "Dashboard/Include/DashboardDataTypes.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include <QFrame>
#include <QGridLayout>

namespace MsgClasses
{
    class CmdProgramStartReady;
}

namespace Dashboard {

namespace Ui {
    class CDashboardWidget;
}

/****************************************************************************/
/**
 * \brief This is the main widget for Dash Board.
 */
/****************************************************************************/
class CDashboardWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

private:
    Ui::CDashboardWidget *mp_Ui;                                //!< User interface
    MainMenu::CMainWindow *mp_MainWindow;                           //!< Reference to MainWindow.
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    CDashboardScene *mp_DashboardScene;                         //!< Displays the station items
    QFrame *mp_Separator;                                       //!< Separator Line between View & Operation
    DataManager::ProgramActionType_t m_ProgramCurrentAction;    //!< Program Current Action
    DataManager::ProgramActionType_t m_ProgramNextAction;       //!< Program Next Action
    DataManager::CDataProgramList *mp_ProgramList;
    QStringList m_FavProgramIDs;

    Dashboard::CDashboardComboBoxModel *mp_ComboBoxModel;
    QString m_SelectedProgramId;
    QButtonGroup m_btnGroup;                                    //!< Button Group
    Global::RMSOptions_t m_RMSState;                            //!< Current RMS State
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;        //!< Current user role
    bool m_UserRoleChanged;                                     //!< Flag to Verify the Change in User Role
    MainMenu::CMessageDlg   *mp_MessageDlg;                      //!< Message Dialogue

    void PlayProgram();
    void PauseProgram();
    void AbortProgram();
    void EnablePlayButton(bool bSetEnable);

public:
    explicit CDashboardWidget(Core::CDataConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent = NULL);
    ~CDashboardWidget();

    bool CheckPreConditionsToRunProgram();
    bool CheckPreConditionsToPauseProgram();
    bool CheckPreConditionsToAbortProgram();

protected:
    void DrawSeparatorLine();


signals:
    void ProgramAction(const QString& ProgramID, DataManager::ProgramActionType_t ActionType);
    void UpdateProgramName(QString SelectedProgramName);
    void UpdateDashboardSceneReagentStations(QString& ProgramID);
    void ProgramSelected(QString & ProgramId);

public slots:
    void RetortSliderPositionChanged(MainMenu::CSliderControl::Position_t Position);
    void OnUserRoleChanged();
    void OnButtonClicked(int whichBtn);
    void OnRMSValueChanged(Global::RMSOptions_t);
    void AddItemsToComboBox();
    void OnActivated(int);
    void OnProgramActionStarted(DataManager::ProgramActionType_t ActionType);
    void OnProgramStartReadyUpdated(const MsgClasses::CmdProgramStartReady& cmd);
    void OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus& cmd);

    // Warning Message Slots
    void OnProgramStartConfirmation();
};

}
#endif // DASHBOARDWIDGET_H
