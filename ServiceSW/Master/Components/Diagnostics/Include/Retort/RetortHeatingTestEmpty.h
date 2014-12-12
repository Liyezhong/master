/****************************************************************************/
/*! \file HeatingTestEmpty.h
 *
 *  \brief Declaration of Retort Heating test empty.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-21
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_RETORT_HEATINGTESTEMPTY_H
#define DIAGNOSTICS_RETORT_HEATINGTESTEMPTY_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "Diagnostics/Include/Retort/RetortTimingDialog.h"

namespace Diagnostics {

namespace Retort {

#ifndef _RETORTHEATINGTESTSTATUS_
#define _RETORTHEATINGTESTSTATUS_

/****************************************************************************/
/**
 * \brief Struct for retort heating status data.
 */
/****************************************************************************/
struct HeatingStatus {
   int UsedTime;        //!< number of used time
   int EDTime;          //!< number of end time

   qreal RetortTempSide;    //!< value of retort side temperature
   qreal RetortTempSensor1; //!< value of retort sensor 1 temperature
   qreal RetortTempSensor2; //!< value of retort sensor 2 temperature

   QString RetortTempTarget;    //!< string of retort target temperature

   qreal TempOffset;        //!< value of temperature offset
   qreal TempOffsetRange;   //!< value of temperature offset range
};
#endif

/****************************************************************************/
/*!
*   \brief This class implements the functionality to retort heating test empty test
*/
/****************************************************************************/
class CHeatingTestEmpty : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam _dlg = message dialog
     */
    /****************************************************************************/
    CHeatingTestEmpty(CDiagnosticMessageDlg *_dlg);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CHeatingTestEmpty(void);
    /****************************************************************************/
    /*!
     *  \brief To run test case.
     */
    /****************************************************************************/
    int Run(void);

private:
    /****************************************************************************/
    /*!
     *  \brief To show waiting dialog for test case.
     */
    /****************************************************************************/
    void RefreshWaitingDialog(struct HeatingStatus *buf, bool isShow = false);

private:
    CDiagnosticMessageDlg *dlg; //!< For show message dialog
    RetortTimingDialog *timingDialog;  //!< For show timming message dialog
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_HEATINGTESTEMPTY_H
