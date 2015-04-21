/****************************************************************************/
/*! \file SystemSetupSettingsWidget.cpp
 *
 *  \brief AgitationSettingsWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer
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

#include <QMap>

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Settings/Include/SystemSetupSettingsWidget.h"
#include "ui_SystemSetupSettingsWidget.h"
#include "Core/Include/GlobalHelper.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Dashboard/Include/CommonString.h"
#include "Core/Include/DataConnector.h"

namespace Settings {


//! Minimal scroll wheel temperature in degree Celsius
#define MIN_CENTIGRADE_TEMP   50
//! Maximal scroll wheel temperature in degree Celsius
#define MAX_CENTIGRADE_TEMP   70
//! Minimal scroll wheel temperature in degree Fahrenheit
#define MIN_FARENHEIT_TEMP    122
//! Maximal scroll wheel temperature in degree Fahrenheit
#define MAX_FARENHEIT_TEMP    158

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function GetTemperatureMap
 *
 *  \return from GetTemperatureMap
 */
/****************************************************************************/
QMap<int, int> GetTemperatureMap(int MinC, int MaxC, int StepC)
{
    QMap<int, int> MapTemp;
    for (int Cel = MinC; Cel <= MaxC; Cel += StepC)
    {
        int Faren =
                qRound(((static_cast<double>(Cel)  - 50.0) / 5.0) * 9.0) + 122;

        (void)MapTemp.insert(Faren, Cel);
    }

    return MapTemp;
}

static QMap<int, int> s_MapTemperature =       ///<  Definition/Declaration of variable s_MapTemperature
        GetTemperatureMap(MIN_CENTIGRADE_TEMP, MAX_CENTIGRADE_TEMP, 1);


/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CSystemSetupSettingsWidget::CSystemSetupSettingsWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CSystemSetupSettingsWidget), mp_UserSettings(NULL), mp_MainWindow(NULL),m_ProcessRunning(false),
    m_CurrentUserRole(MainMenu::CMainWindow::Operator),
    mp_TableWidget(NULL),
    mp_DataConnector(NULL)
{
    mp_Ui->setupUi(GetContentFrame());

    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->scrollPanelWidget->Init(1);
    mp_Ui->scrollPanelWidget->AddScrollWheel(mp_ScrollWheel, 0);

    CONNECTSIGNALSLOT(mp_Ui->btnSave, clicked(), this, OnApply());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSystemSetupSettingsWidget::~CSystemSetupSettingsWidget()
{
    try {
        delete mp_ScrollWheel;
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CSystemSetupSettingsWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            RetranslateUI();
            break;
        default:
            break;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets pointer to DataConnector object
 *
 *  \iparam p_DataConnector = the data connector
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::SetDataConnector(Core::CDataConnector *p_DataConnector)
{
    if (p_DataConnector)
    {
        mp_DataConnector = p_DataConnector;
        m_PrecheckProgramModel.SetProgramList(p_DataConnector->ProgramList);

        mp_TableWidget = new MainMenu::CBaseTable;
        mp_TableWidget->setModel(&m_PrecheckProgramModel);
        mp_Ui->programTable->SetContent(mp_TableWidget);
        m_PrecheckProgramModel.SetVisibleRowCount(6);
        mp_TableWidget->horizontalHeader()->show();
        mp_TableWidget->SetVisibleRows(6);
        mp_TableWidget->horizontalHeader()->resizeSection(0, 90);
        mp_TableWidget->horizontalHeader()->resizeSection(1, 120);
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::SetUserSettings(DataManager::CHimalayaUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}

/****************************************************************************/
/*!
 *  \brief Initializes the temperature scroll wheel
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::InitTemperatureWidget()
{
    if (mp_UserSettings == NULL) {
        return;
    }

    mp_ScrollWheel->ClearItems();

    // Temperature Control
    if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
        for (int i = MIN_CENTIGRADE_TEMP; i <= MAX_CENTIGRADE_TEMP; i += 1) {
            mp_ScrollWheel->AddItem(QString::number(i).rightJustified(2, '0'), i);
        }
        mp_Ui->scrollPanelWidget->SetSubtitle(QApplication::translate("CSystemSetupSettingsWidget", "\302\260C", 0, QApplication::UnicodeUTF8), 0);
    }
    else {
        for (QMap<int, int>::iterator itr = s_MapTemperature.begin();
             s_MapTemperature.end() != itr;
             ++itr)
        {
            mp_ScrollWheel->AddItem(
                        QString::number(itr.key()).rightJustified(2, '0'),
                        itr.value());
        }

        mp_Ui->scrollPanelWidget->SetSubtitle(QApplication::translate("CSystemSetupSettingsWidget", "\302\260F", 0, QApplication::UnicodeUTF8), 0);
    }
    mp_ScrollWheel->SetNonContinuous();
}


/****************************************************************************/
/*!
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::showEvent(QShowEvent *p_Event)
{
    if ((mp_UserSettings != NULL) && (p_Event != NULL) && !p_Event->spontaneous()) {
        InitTemperatureWidget();

        if (mp_UserSettings->GetTemperatureFormat() == Global::TEMP_FORMAT_CELSIUS) {
            mp_ScrollWheel->SetThreeDigitMode(false);
            mp_Ui->scrollPanelWidget->SetThreeDigitMode(false);
        }
        else {
            mp_ScrollWheel->SetThreeDigitMode(true);
            mp_Ui->scrollPanelWidget->SetThreeDigitMode(true);
        }

        int Temp = mp_UserSettings->GetTemperatureParaffinBath();
        mp_ScrollWheel->SetCurrentData(Temp);

        m_PrecheckProgramModel.SetProgramList(mp_DataConnector->ProgramList);
        ResetButtons();
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::OnUserRoleChanged()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_PrecheckProgramModel.SetUserRole(m_CurrentUserRole);
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::OnProcessStateChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the apply button based on the user role/process
 *         running status
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::ResetButtons()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();

    if (Core::CGlobalHelper::CheckIfCanEdit() == false) {
        if (m_CurrentUserRole == MainMenu::CMainWindow::Service) {
            mp_Ui->btnSave->setEnabled(true);
        }
        else {
            mp_Ui->btnSave->setEnabled(false);
        }
        return ;
    }

    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
         (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnSave->setEnabled(true);
    }
    else {
        mp_Ui->btnSave->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CSystemSetupSettingsWidget", "System Setup", 0, QApplication::UnicodeUTF8));
   m_strConfirmChangeMeltingPoint =  QApplication::translate("Settings::CSystemSetupSettingsWidget",
                                                             "Do you really want to change the paraffin bath temperature?", 0, QApplication::UnicodeUTF8);
   m_strChangeMeltPointConfirm12Hrs = QApplication::translate("Settings::CSystemSetupSettingsWidget",
    "With Paraffin bath temperture changed, if you put the solid paraffin in, please select 'Yes' to wait %1 hours for completely melting the solid paraffin; if you put the melted liquid paraffin in, please select 'No' and no need to wait any time.",
                                                              0, QApplication::UnicodeUTF8);
   (void) m_PrecheckProgramModel.setHeaderData(0, Qt::Horizontal,QApplication::translate("Settings::CPrecheckProgramModel",
                                                                                "Apply", 0, QApplication::UnicodeUTF8),0);

   (void) m_PrecheckProgramModel.setHeaderData(1, Qt::Horizontal,QApplication::translate("Settings::CPrecheckProgramModel",
                                                                                "Program", 0, QApplication::UnicodeUTF8),0);

}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 *  \param p_MainWindow = the application's main window
 */
/****************************************************************************/
void CSystemSetupSettingsWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
}

/****************************************************************************/
/*!
 *  \brief Called when the Save button is clicked
 */
/****************************************************************************/

void CSystemSetupSettingsWidget::OnApply()
{
    QStringList checkflags = m_PrecheckProgramModel.GetBottleCheckFlags();
    for (int i = 0; i < checkflags.count(); i++)
    {
        DataManager::CProgram* pProgram = mp_DataConnector->ProgramList->GetProgram(i);
        QString strTemp = checkflags[i];
        if ((strTemp == "1") && (pProgram->GetBottleCheck()))
            continue;
        if ((strTemp == "0") && !(pProgram->GetBottleCheck()))
            continue;

        bool bRevertSelectedProgram = false;
        if (!Core::CGlobalHelper::CheckSelectedProgram(bRevertSelectedProgram, pProgram->GetID()))
            return;//cancel

        if (bRevertSelectedProgram)
        {
            emit UnselectProgram();
        }

        if (strTemp == "1")
            pProgram->SetBottleCheck(true);
        else
            pProgram->SetBottleCheck(false);

        emit UpdateProgram(*pProgram);
    }

    int temp = mp_ScrollWheel->GetCurrentData().toInt();
    int lastMeltPoint = mp_UserSettings->GetTemperatureParaffinBath();
    if (temp == lastMeltPoint)
        return;

    MainMenu::CMessageDlg ConfirmationMessageDlg(this);
    ConfirmationMessageDlg.SetTitle(CommonString::strWarning);
    ConfirmationMessageDlg.SetIcon(QMessageBox::Warning);
    ConfirmationMessageDlg.SetText(m_strConfirmChangeMeltingPoint);
    ConfirmationMessageDlg.SetButtonText(1, CommonString::strYes);//right
    ConfirmationMessageDlg.SetButtonText(3, CommonString::strCancel);//left
    ConfirmationMessageDlg.HideCenterButton();
    if (!ConfirmationMessageDlg.exec())
    {
        return;
    }

    m_UserSettingsTemp = *mp_UserSettings;
    m_UserSettingsTemp.SetTemperatureParaffinBath(temp);
    emit TemperatureChanged(m_UserSettingsTemp);

    if (temp <= 64)
    {
        ConfirmationMessageDlg.SetText(m_strChangeMeltPointConfirm12Hrs.arg("12"));
    }
    else
        ConfirmationMessageDlg.SetText(m_strChangeMeltPointConfirm12Hrs.arg("15"));

    ConfirmationMessageDlg.SetButtonText(1, CommonString::strYes);//right
    ConfirmationMessageDlg.SetButtonText(3, CommonString::strNo);//left
    ConfirmationMessageDlg.HideCenterButton();
    if (ConfirmationMessageDlg.exec())
    {
        emit ParaffinMeltPointchanged(lastMeltPoint, temp);
    }
}

} // end namespace Settings
