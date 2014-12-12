/****************************************************************************/
/*! \file TestCaseReporter.h
 *
 *  \brief Definition file for class CTestCaseReporter.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-5-21
 *   $Author:  $ Dixiong.li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef TESTCASEREPORTER_H
#define TESTCASEREPORTER_H

#include <QString>
#include <QTextStream>
#include <QFile>
#include <QSet>
#include "Core/Include/ServiceDefines.h"
#include "Core/Include/ServiceGUIConnector.h"
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/MainWindow.h"
#include "IENetworkClient/Include/IENetworkClient.h"

namespace DiagnosticsManufacturing {

/****************************************************************************/
/**
 * \brief This class to report test report
 */
/****************************************************************************/
class CTestCaseReporter: public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /**
     * \brief Default constructor.
     */
    /****************************************************************************/
    CTestCaseReporter(const QString ModuleName, Core::CServiceGUIConnector *p_DataConnector = NULL,
                      MainMenu::CMainWindow *p_Parent = NULL);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CTestCaseReporter();

    /****************************************************************************/
    /**
     * \brief Generate report file.
     * \return false if generate file error.
     */
    /****************************************************************************/
    bool GenReportFile();

    /****************************************************************************/
    /**
     * \brief send report file to server.
     * \return false if send file error.
     */
    /****************************************************************************/
    bool SendReportFile();

    /****************************************************************************/
    /**
     * \brief Set the Module serial number.
     * \param SerialNumber = the serial number.
     */
    /****************************************************************************/
    void SetSerialNumber(const QString SerialNumber) {
        m_SerialNumber = SerialNumber;
    }

    /****************************************************************************/
    /**
     * \brief Add test case Id number.
     * \param Id = test calse Id.
     */
    /****************************************************************************/
    void AddTestCaseId(Service::ModuleTestCaseID Id) {
        m_TestCaseList.insert(Id);
    }

    /****************************************************************************/
    /**
     * \brief To check system serial number.
     * \return false if serial number is empty
     */
    /****************************************************************************/
    bool CheckSystemSN();

private:
    /****************************************************************************/
    /**
     * \brief Init IE network client.
     */
    /****************************************************************************/
    void InitIEClient();

    /****************************************************************************/
    /**
     * \brief write report file.
     */
    /****************************************************************************/
    void WriteReportFile(QTextStream& TextStream);

    QString m_ModuleName;                            //!< Stores module name
    Core::CServiceGUIConnector *mp_DataConnector;    //!< Data Connector object
    NetworkClient::IENetworkClient *mp_IEClient;     //!< IE client for send test report
    QSet<Service::ModuleTestCaseID> m_TestCaseList;  //!< Stores test case list;
    QString m_TestReportFile;                        //!< Stores Test report file name.
    QString m_ReportDir;                             //!< The server report folder
    QString m_SerialNumber;                          //!< Stores serial number
    MainMenu::CMessageDlg *mp_MessageDlg;            //!< Information dialog
    MainMenu::CWaitDialog *mp_WaitDlg;               //!< Waiting dialog
    MainMenu::CMainWindow *mp_Parent;                //!< Parent widget
};

} // end namespace DiagnosticsManufacturing

#endif // TESTCASEREPORTER_H
