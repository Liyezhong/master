/****************************************************************************/
/*! \file CSelect110v220vDialog.h
 *
 *  \brief Header file for class CSelect110v220vDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-18
 *   $Author:  $ Dixiong Li
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


#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "DiagnosticsManufacturing/Include/Select110v220vDialog.h"
#include "ui_Select110v220vDialog.h"
#include <QDebug>
#include "ServiceDataManager/Include/TestCaseGuide.h"

namespace DiagnosticsManufacturing {

CSelect110v220vDialog::CSelect110v220vDialog(bool Is110V, QWidget *p_Parent):
    m_Is110V(Is110V),
    mp_Ui(new Ui::CSelect110v220vDialog),
    MainMenu::CDialogFrame(p_Parent)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    mp_ButtonGroup = new QButtonGroup();
    mp_ButtonGroup->addButton(mp_Ui->radioButton110, 0);
    mp_ButtonGroup->addButton(mp_Ui->radioButton220, 1);

    mp_Ui->radioButton110->setChecked(m_Is110V);
    mp_Ui->radioButton220->setDisabled(!m_Is110V);
    mp_Ui->radioButton220->setChecked(!m_Is110V);
    CONNECTSIGNALSLOTGUI(mp_ButtonGroup, buttonClicked(int), this, OnRadioBtnSelected(int));
}

CSelect110v220vDialog::~CSelect110v220vDialog()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

void CSelect110v220vDialog::OnRadioBtnSelected(int radioBtnIndex)
{
    if (radioBtnIndex == 0) {
        m_Is110V = true;
    }
    else {
        m_Is110V = false;
    }
}

void CSelect110v220vDialog::RetranslateUI()
{

}

bool CSelect110v220vDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
    return false;
}

} // end namespace DiagnosticsManufacturing
