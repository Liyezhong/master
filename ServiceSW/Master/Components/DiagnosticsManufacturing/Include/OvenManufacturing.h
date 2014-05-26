/****************************************************************************/
/*! \file OvenManufacturing.cpp
 *
 *  \brief COven class definition for Manufacturing SW
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

#ifndef OVEN_DIAGNOSTICSMANUFACTURING_H
#define OVEN_DIAGNOSTICSMANUFACTURING_H

#include "DiagnosticsManufacturing/Include/TestReportGeneration.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
#include "Core/Include/ServiceGUIConnector.h"
#include "Core/Include/ServiceDefines.h"
#include <MainMenu/Include/BaseTable.h>
#include <KeyBoard/Include/KeyBoard.h>
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/MainWindow.h"
#include <QStandardItemModel>
#include <QWidget>
#include <QPixmap>
#include <QEvent>
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include "Core/Include/ServiceDefines.h"

namespace DiagnosticsManufacturing {

namespace Ui {
    class COvenManufacturing;
}

/****************************************************************************/
/**
 * \brief This widget lists all the tests to be performed for X-Arm
 */
/****************************************************************************/
class COven : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_DataConnector = Service GUI connector object
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit COven(Core::CServiceGUIConnector *p_DataConnector = NULL, MainMenu::CMainWindow *p_Parent = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~COven();

    /****************************************************************************/
    /*!
     *  \brief  Set result to test case
     *  \iparam Id = Test Case Id
     *  \iparam Result = Test result
     */
    /****************************************************************************/
    void SetTestResult(Service::ModuleTestCaseID Id, bool Result);

    /****************************************************************************/
    /*!
     *  \brief  Enable 'begin test' button
     *  \iparam EnableFlag = Enable or Disable
     */
    /****************************************************************************/
    void EnableButton(bool EnableFlag);

private:
    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam Index = Test Case No.
     *  \iparam Id = Test Case Id
     */
    /****************************************************************************/
    void AddItem(quint8 Index, Service::ModuleTestCaseID_t Id);

    /****************************************************************************/
    /*!
     *  \brief  Sets the Line edit string text
     *  \iparam Value = Serial number of the module
     */
    /****************************************************************************/
    void SetLineEditText(QString Value) {
        m_LineEditString = Value;
    }

protected:
    bool eventFilter(QObject *p_Object, QEvent *p_Event);
    void changeEvent(QEvent *p_Event);

private:
    Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    MainMenu::CMainWindow *mp_MainWindow;       //!< Main window object
    Ui::COvenManufacturing *mp_Ui;         //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;       //!< Base Table widget
    QStandardItemModel m_Model;                 //!< Model for the table
    QString m_LineEditString;                   //!< Stores serial number string
    QString m_OvenSNString;                     //!< Stores serial number of Oven module
    QString m_HeaterSNString;                   //!< Stores serial number of Heater sub-module
    QString m_CoverSensorSNString;              //!< Stores serial number of Cover sensor sub-module
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;     //!< Keyboard widget
    QHash<QString, QString> m_TestReport;       //!< Test report
    QList<QString> m_TestNames;                 //!< List of test names   
    DiagnosticsManufacturing::CTestReportGeneration *mp_TestReport;     //!< Test report generation object
    QStringList m_TestResult;                                           //!< StringList stores test result
    MainMenu::CMessageDlg *mp_MessageDlg;                               //!< Information dialog
    QString m_FinalTestResult;                                          //!< Stores Final test result

    void ConnectKeyBoardSignalSlots();
    void DisconnectKeyBoardSignalSlots();

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emitted for module test
     */
    /****************************************************************************/
    void BeginModuleTest(Service::ModuleNames_t, const QList<Service::ModuleTestCaseID> &TestCaseList);

private slots:
    /****************************************************************************/
    /*!
     *  \brief This function is called when OK is clicked
     *  \iparam EnteredString = Stores line edit string
     */
    /****************************************************************************/
    void OnOkClicked(QString EnteredString);

    /****************************************************************************/
    /*!
     *  \brief This function hides the keyboard when Esc is clicked
     */
    /****************************************************************************/
    void OnESCClicked();

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void RetranslateUI();
public slots:
    void BeginTest();
    void SendTestReport();
    void ResetTestStatus();

};

} // end namespace DiagnosticsManufacturing


#endif // DIAGNOSTICSMANUFACTURING_OVEN_H
