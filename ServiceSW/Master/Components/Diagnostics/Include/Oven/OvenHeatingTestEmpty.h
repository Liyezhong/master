/****************************************************************************/
/*! \file HeatingTestEmpty.h
 *
 *  \brief Declaration of Heating test empty.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
 *   $Author:  $ R.Wu
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

#ifndef DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H
#define DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Oven {

class CHeatingTestEmpty : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CHeatingTestEmpty(CDiagnosticMessageDlg *dlg);
    ~CHeatingTestEmpty(void);

    int Run(void);

public:
    int StartHeating(void);

private Q_SLOTS:
    void Succeed(void);
    void Fail(void);

private:
    CDiagnosticMessageDlg *dlg;
};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H
