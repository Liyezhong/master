/****************************************************************************/
/*! \file InitialSystemCheck.h
 *
 *  \brief Declaration of Initial system check.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-15
 *   $Author:  $ Sunny.Qu
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

#ifndef DIAGNOSTICS_INITIALSYSTEMCHECK_H
#define DIAGNOSTICS_INITIALSYSTEMCHECK_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"
#include "Core/Include/ServiceDefines.h"
#include "Core/Include/ServiceGUIConnector.h"

namespace Diagnostics {

namespace InitialSystem {

const QString MSG_TITLE("Initial System Check");  //!< The string of dialog title

/****************************************************************************/
/*!
*   \brief This class implements the functionality to all module initial check
*/
/****************************************************************************/
class CInitialSystemCheck : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_DataConnector = Service GUI connector object
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    CInitialSystemCheck(Core::CServiceGUIConnector *p_DataConnector = NULL, QWidget *parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CInitialSystemCheck(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test check start run
     */
    /****************************************************************************/
    int Run(void);

public slots:
    /****************************************************************************/
    /*!
     *  \brief Slot for retort pre heating
     */
    /****************************************************************************/
    void RetortPreHeating();

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emitted for refresh status
     *  \iparam Type = The pre test module type
     *  \Ret  = The pre test result
     */
    /****************************************************************************/
    void RefreshStatusToGUI(Service::InitialSystemTestType Type, int Ret);

    /****************************************************************************/
    /*!
     *  \brief Signal emitted for refresh heating status
     *  \iparam Type = The pre test module type
     */
    /****************************************************************************/
    void RefreshHeatingStatus(Service::InitialSystemTestType Type);

private:
    /****************************************************************************/
    /*!
     *  \brief To Confirm paraffin melting point.
     */
    /****************************************************************************/
    void ConfirmParaffinBath(void);

    /****************************************************************************/
    /*!
     *  \brief To Confirm retort condition.
     */
    /****************************************************************************/
    void ConfirmRetortCondition(void);

private:
    Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    QWidget                    *mp_Parent;          //!< The parent widget
    int                        m_ParaffinMeltPoint; //!< Store the paraffin melting point.
    bool                       m_IsParaffinInRetort;//!< The flag for retort condition if have paraffin.
};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITIALSYSTEMCHECK_H
