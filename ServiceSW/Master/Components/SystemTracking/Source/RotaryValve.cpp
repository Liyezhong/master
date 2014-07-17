/****************************************************************************/
/*! \file RotaryValve.cpp
 *
 *  \brief Implementation of Rotary Valve configuration class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
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

#include "SystemTracking/Include/RotaryValve.h"

#include <QDebug>

#include "SystemTracking/Include/DlgModifyModule.h"
#include "SystemTracking/Include/DlgModifySubModule.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "ui_RotaryValve.h"

namespace SystemTracking {

const QString MODULE_ROTARYVALVE("Rotary Valve");
const QString SUBMODULE_HEATER("Heater");
const QString SUBMODULE_MOTOR("Motor");

CRotaryValve::CRotaryValve(Core::CServiceGUIConnector &DataConnector,
             QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , mp_Ui(new Ui::CRotaryValve)
    , mp_ModuleList(NULL)
{
    mp_Ui->setupUi(this);

    mp_Ui->finalizeConfigBtn->setEnabled(false);

    mp_MessageDlg = new MainMenu::CMessageDlg(this);
    mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CRotaryValve",
                                                    "Finalize Configuration", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->setModal(true);

    (void)connect(mp_Ui->modifyRotaryValve,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyRotaryValve()) );

    (void)connect(mp_Ui->modifyHeater,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeater()) );


    (void)connect(mp_Ui->modifyMotor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyMotor()) );
    (void)connect(mp_Ui->finalizeConfigBtn,
                  SIGNAL(clicked()),
                  this,
                  SLOT(OnFinalizeConfiguration()) );

}

CRotaryValve::~CRotaryValve()
{
    try
    {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) { }
}

void CRotaryValve::UpdateModule(ServiceDataManager::CModule &Module)
{
    qDebug() << "CRotaryValve::UpdateModule !"
             << Module.GetModuleName();

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::UpdateModule(): Invalid module list!";
        return;
    }

    pModuleList->UpdateModule(&Module);

    emit ModuleListChanged();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void CRotaryValve::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "CRotaryValve::UpdateSubModule !"
             << SubModule.GetSubModuleName();

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::UpdateSubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_ROTARYVALVE);
    if (0 == pModule)
    {
        qDebug() << "CRotaryValve::UpdateSubModule(): Invalid module : "
                 << MODULE_ROTARYVALVE;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    emit ModuleListChanged();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void CRotaryValve::ModifyRotaryValve(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_ROTARYVALVE_MODULE);
    qDebug() << "CRotaryValve::ModifyRotaryValve !";

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::ModifyRotaryValve(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_ROTARYVALVE);
    if (0 == pModule)
    {
        qDebug() << "CRotaryValve::ModifyRotaryValve(): Invalid module : "
                 << MODULE_ROTARYVALVE;
        return;
    }

    ServiceDataManager::CModule Module = *pModule;

    CDlgModifyModule *dlg = new CDlgModifyModule(Module);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateModule(ServiceDataManager::CModule&)),
                  this,
                  SLOT(UpdateModule(ServiceDataManager::CModule&)));

    dlg->exec();

    delete dlg;
}

void CRotaryValve::ModifyHeater(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_ROTARYVALVE_HEATER_MODULE);
    qDebug() << "CRotaryValve::ModifyHeater !";

    this->ModifySubModule(MODULE_ROTARYVALVE, SUBMODULE_HEATER);
}

void CRotaryValve::ModifyMotor(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_ROTARYVALVE_MOTOR_MODULE);
    qDebug() << "CRotaryValve::ModifyMotor !";

    this->ModifySubModule(MODULE_ROTARYVALVE, SUBMODULE_MOTOR);
}

void CRotaryValve::OnFinalizeConfiguration(void)
{
    QString Text = QApplication::translate("SystemTracking::CRotaryValve",
                                           "Do you want to finalize the configuration for the Rotary valve?",
                                                       0, QApplication::UnicodeUTF8);
    ConfirmModuleConfiguration(Text);
}

void CRotaryValve::CurrentTabChanged(int Index)
{
    if (Index != 0) {
        ConfirmModuleConfiguration();
    }
}

void CRotaryValve::ConfirmModuleConfiguration()
{
    QString Text = QApplication::translate("SystemTracking::CRotaryValve",
                                           "Rotary valve Module has been modified. Do you want to finalize the configuration?",
                                           0, QApplication::UnicodeUTF8);

    if (mp_Ui->finalizeConfigBtn->isEnabled()) {
        ConfirmModuleConfiguration(Text);
    }
}

void CRotaryValve::ConfirmModuleConfiguration(QString& Text)
{
    mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRotaryValve",
                                                            "Ok", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetButtonText(3,QApplication::translate("SystemTracking::CRotaryValve",
                                                           "Cancel", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->HideCenterButton();

    mp_MessageDlg->SetText(Text);
    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->show();

    int Result = mp_MessageDlg->exec();

    ResetMessageBox();
    if (Result)
    {
        if(mp_ModuleList && mp_ModuleList->Write())
        {
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRotaryValve",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CRotaryValve",
                                          "Configuration file updated successfully.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
        else
        {
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRotaryValve",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CRotaryValve",
                                          "Configuration file updation failed.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
    }
    else
    {
        //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_ADDMODIFY_SAVE_AND_OVERWRITE_CONFIGURATION_FAILURE);
        mp_DateConnector->ReloadModuleList();
        mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRotaryValve",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CRotaryValve",
                                             "Finalize Configuration Cancelled.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    mp_Ui->finalizeConfigBtn->setEnabled(false);
}

void CRotaryValve::ModifySubModule(const QString &ModuleName,
                                   const QString &SubModuleName)
{
    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::ModifySubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CRotaryValve::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    ServiceDataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CRotaryValve::ModifySubModule(): Invalid sub module : "
                 << SubModuleName;
        return;
    }

    ServiceDataManager::CSubModule SubModule = *pSubModule;

    CDlgModifySubModule *dlg = new CDlgModifySubModule(SubModule);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(ServiceDataManager::CSubModule&)));

    dlg->exec();

    delete dlg;
}

void CRotaryValve::ResetMessageBox()
{
    if (mp_MessageDlg) {
        delete mp_MessageDlg;
        mp_MessageDlg = new MainMenu::CMessageDlg(this);
    }
}

} // namespace SystemTracking
