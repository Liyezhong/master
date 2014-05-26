/****************************************************************************/
/*! \file ManufacturingDiagnosticsHandler.cpp
 *
 *  \brief ManufacturingDiagnosticsHandler class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include <QApplication>
#include "Global/Include/UITranslator.h"
#include "Core/Include/ManufacturingDiagnosticsHandler.h"
#include "Core/Include/CMessageString.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include "Main/Include/HimalayaServiceEventCodes.h"

namespace Core {

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_ServiceGUIConnector = GUI connector object
 *  \iparam p_MainWindow = Mainwindow object
 */
/****************************************************************************/
CManufacturingDiagnosticsHandler::CManufacturingDiagnosticsHandler(CServiceGUIConnector *p_ServiceGUIConnector,
                                                                   MainMenu::CMainWindow *p_MainWindow) :
    mp_ServiceConnector(p_ServiceGUIConnector),
    mp_MainWindow(p_MainWindow)
{

    //Diagnostics Manufacturing
    mp_DiagnosticsManufGroup = new MainMenu::CMenuGroup;
    mp_OvenManuf = new DiagnosticsManufacturing::COven(mp_ServiceConnector, mp_MainWindow);

    CONNECTSIGNALSLOTGUI(mp_OvenManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));


    /* Manufacturing SW Reset status */
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_OvenManuf, ResetTestStatus());

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CManufacturingDiagnosticsHandler::~CManufacturingDiagnosticsHandler()
{
    try {
        delete mp_OvenManuf;
        delete mp_DiagnosticsManufGroup;
    }
    catch (...) {

    }
}

/****************************************************************************/
/*!
 *  \brief Adds GUI Diagnostics Manufactruing SW Components
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::LoadManufDiagnosticsComponents()
{
//    mp_DiagnosticsManufGroup->Clear();
    //Diagnostics
    mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Oven", 0, QApplication::UnicodeUTF8), mp_OvenManuf);
    mp_MainWindow->AddMenuGroup(mp_DiagnosticsManufGroup, QApplication::translate
                   ("Core::CManufacturingDiagnosticsHandler", "Diagnostics", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Slot called for Module tests for manufacturing SW
 *  \iparam ModuleName = Name of the module
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::BeginManufacturingSWTests(Service::ModuleNames_t ModuleName, const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    qDebug()<<"CManufacturingDiagnosticsHandler::BeginManufacturingSWTests : ModuleName="<<ModuleName;
    switch(ModuleName) {
    case Service::OVEN:
        PerformManufOvenTests(TestCaseList);
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Heated Cuvettes Test response
 *  \return true or false
 */
/****************************************************************************/
bool CManufacturingDiagnosticsHandler::GetTestResponse()
{
//    mp_ServiceConnector->ShowBusyDialog();
    QTimer timer;
    qint32 ret;
    quint32 interval = 1000 * 3 * 60 * 60; // 3 hours.
    timer.setSingleShot(true);
    timer.setInterval(interval);
    timer.start();
    CONNECTSIGNALSLOT(&timer, timeout(), &m_LoopManufacturingTest, quit());
    ret = m_LoopManufacturingTest.exec();
//    mp_ServiceConnector->HideBusyDialog();
    if (ret != 1)
    {
        return false;
    }
    return true;
}

bool CManufacturingDiagnosticsHandler::ShowGuide(Service::ModuleTestCaseID Id, int Index)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QStringList Steps = DataManager::CTestCaseGuide::Instance().GetGuideSteps(TestCaseName, Index);
    QString GuideText;

    for(int i=0; i<Steps.size(); i++) {
        GuideText.append(Steps.at(i));
        if (i <= Steps.size()-1 ) {
            GuideText.append("\n");
        }
    }

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(GuideText);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, tr("Next"));
    dlg->SetButtonText(1, tr("Cancel"));
    if (Index == 1) {
        dlg->EnableButton(1, false);
    }

//    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    int ret = dlg->exec();

    qDebug()<<"return code = "<<ret;

    delete dlg;

    if ( ret == 0 )
        return true;

    return false;
}

void CManufacturingDiagnosticsHandler::ShowHeatingFailedResult(Service::ModuleTestCaseID Id)
{
    DiagnosticsManufacturing::CHeatingTestDialog *dlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
    QString Text = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    Service::ModuleTestStatus Status = p_TestCase->GetResult();
    dlg->UpdateLabel(Status);

    Text.append(" ");
    Text.append("Failed !");
    dlg->SetDialogTitle("Error");
    dlg->SetText(Text);
    dlg->HideAbort(false);
    dlg->show();
}

/****************************************************************************/
/*!
 *  \brief Function called for Module tests for manufacturing SW
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::PerformManufOvenTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufOvenTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID Id = TestCaseList.at(i);

        bool NextFlag = ShowGuide(Id, 0);
        if (NextFlag == false) {
            break;
        }

        switch( Id ) {
        case Service::OVEN_COVER_SENSOR:
            EventId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_SUCCESS;
            break;
        case Service::OVEN_HEATING_EMPTY:
            EventId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_SUCCESS;
            break;
        case Service::OVEN_HEATING_WITH_WATER:
            EventId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_SUCCESS;
            break;
        }

        Global::EventObject::Instance().RaiseEvent(EventId);
        emit PerformManufacturingTest(Id);

        bool Result = GetTestResponse();

        if (Id == Service::OVEN_COVER_SENSOR && Result == true) {
            NextFlag = ShowGuide(Id, 1);
            if (NextFlag == false)
                break;
            emit PerformManufacturingTest(Service::OVEN_COVER_SENSOR);
            Result = GetTestResponse();
        }

        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        p_TestCase->SetStatus(Result);

        if (Result == false) {
            Global::EventObject::Instance().RaiseEvent(FailureId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);

            QString Text = QString("%1 %2\n%3").arg(TestCaseDescription, "- Fail", p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            if (Id != Service::OVEN_COVER_SENSOR) {
                ShowHeatingFailedResult(Id);
            }

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
            QString Text = QString("%1 %2").arg(TestCaseDescription, "- Success");
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
        }
        mp_OvenManuf->SetTestResult(Id, Result);
    }
    mp_OvenManuf->EnableButton(true);
}


/****************************************************************************/
/*!
 *  \brief Returns message for manufacturing tests
 *  \iparam Result = true or false
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::OnReturnManufacturingMsg(bool Result)
{
    qDebug()<<"CManufacturingDiagnosticsHandler::OnReturnManufacturingMsg Result="<<Result;
    qint32 ret(-1);
    if(Result) {
        ret = 1;
    }
    if (m_LoopManufacturingTest.isRunning()) {
        m_LoopManufacturingTest.exit(ret);
    } else {
        qDebug()<<"NOTICE: Unexpected action acknowledgement";
    }
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *  \iparam Message = Dialog title
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::ShowMessage(const QString &Message)
{
    mp_ServiceConnector->HideBusyDialog();
    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Message);
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *  \iparam Message = Dialog title
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::ShowErrorMessage(const QString &Message)
{
    mp_ServiceConnector->HideBusyDialog();
    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Message);
}

} // end of namespace Core
