/****************************************************************************/
/*! \file LaSystem.cpp
 *
 *  \brief Implementation of Liquid and Air system configuration class.
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

#include "SystemTracking/Include/LaSystem.h"

#include <QDebug>

#include "SystemTracking/Include/DlgModifySubModule.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "ui_LaSystem.h"

namespace SystemTracking {

const QString MODULE_LASYSTEM("L&A System");
const QString SUBMODULE_PUMP("Pump");
const QString SUBMODULE_VALVE1("Valve 1");
const QString SUBMODULE_VALVE2("Valve 2");
const QString SUBMODULE_HEATINGBELT1("Liquid Heating Tube");
const QString SUBMODULE_HEATINGBELT2("Air Heating Tube");
const QString SUBMODULE_PRESSURESENSOR("Pressure Sensor");
const QString SUBMODULE_CARBONFILTER("Carbon Filter");
const QString SUBMODULE_FAN("Exhaust Fan");

CLaSystem::CLaSystem(Core::CServiceGUIConnector &DataConnector,
                           QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , mp_Ui(new Ui::CLaSystem)
    , mp_ModuleList(NULL)
{
    mp_Ui->setupUi(this);

    mp_Ui->finalizeConfigBtn->setEnabled(false);

    mp_MessageDlg = new MainMenu::CMessageDlg(this);
    mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CMainControl",
                                                    "Finalize Configuration", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->setModal(true);
    (void)connect(mp_Ui->modifyPump,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyPump()) );

    (void)connect(mp_Ui->modifyValve1,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyValve1()) );

    (void)connect(mp_Ui->modifyValve2,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyValve2()) );

    (void)connect(mp_Ui->modifyHeatingBelt1,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeatingBelt1()) );

    (void)connect(mp_Ui->modifyHeatingBelt2,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeatingBelt2()) );

    (void)connect(mp_Ui->modifyPressureSensor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyPressureSensor()) );

    (void)connect(mp_Ui->modifyCarbonFilter,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyCarbonFilter()) );

    (void)connect(mp_Ui->modifyFan,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyFan()) );

    (void)connect(mp_Ui->finalizeConfigBtn,
                  SIGNAL(clicked()),
                  this,
                  SLOT(OnFinalizeConfiguration()));
}

CLaSystem::~CLaSystem()
{
    try
    {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) { }
}

void CLaSystem::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "CLaSystem::UpdateSubModule : "
             << SubModule.GetSubModuleName();

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CLaSystem::UpdateSubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_LASYSTEM);
    if (0 == pModule)
    {
        qDebug() << "CLaSystem::UpdateSubModule(): Invalid module : "
                 << MODULE_LASYSTEM;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    emit ModuleListChanged();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void CLaSystem::ModifyPump(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_PUMP_MODULE);
    qDebug() << "CLaSystem::ModifyPump !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_PUMP);
}

void CLaSystem::ModifyValve1(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_VALVE1_MODULE);
    qDebug() << "CLaSystem::ModifyValve1 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_VALVE1);
}

void CLaSystem::ModifyValve2(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_VALVE2_MODULE);
    qDebug() << "CLaSystem::ModifyValve2 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_VALVE2);
}

void CLaSystem::ModifyHeatingBelt1(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_HEATING_LIQUID_MODULE);
    qDebug() << "CLaSystem::ModifyHeatingBelt1 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_HEATINGBELT1);
}

void CLaSystem::ModifyHeatingBelt2(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_HEATING_AIR_MODULE);
    qDebug() << "CLaSystem::ModifyHeatingBelt2 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_HEATINGBELT2);
}

void CLaSystem::ModifyPressureSensor(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_PRESSURE_SENSOR_MODULE);
    qDebug() << "CLaSystem::ModifyPressureSensor !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_PRESSURESENSOR);
}

void CLaSystem::ModifyCarbonFilter(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_CARBON_FILTER_MODULE);
    qDebug() << "CLaSystem::ModifyCarbonFilter !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_CARBONFILTER);
}

void CLaSystem::ModifyFan(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_FAN_MODULE);
    qDebug() << "CLaSystem::ModifyFan !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_FAN);
}

void CLaSystem::OnFinalizeConfiguration(void)
{
    QString Text = QApplication::translate("SystemTracking::CLaSystem",
                                           "Do you want to finalize the configuration for the L&A System?",
                                                       0, QApplication::UnicodeUTF8);
    ConfirmModuleConfiguration(Text);
}

void CLaSystem::CurrentTabChanged(int Index)
{
    if (Index != 0) {
        ConfirmModuleConfiguration();
    }
}

void CLaSystem::ConfirmModuleConfiguration()
{
    QString Text = QApplication::translate("SystemTracking::CMainControl",
                                           "L&A System Module has been modified. Do you want to finalize the configuration?",
                                           0, QApplication::UnicodeUTF8);

    if (mp_Ui->finalizeConfigBtn->isEnabled()) {
        ConfirmModuleConfiguration(Text);
    }
}

void CLaSystem::ConfirmModuleConfiguration(QString& Text)
{
    mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CLaSystem",
                                                            "Ok", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetButtonText(3,QApplication::translate("SystemTracking::CLaSystem",
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
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CLaSystem",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CLaSystem",
                                          "Configuration file updated successfully.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
        else
        {
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CLaSystem",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CLaSystem",
                                          "Configuration file updation failed.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
    }
    else
    {
        //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_ADDMODIFY_SAVE_AND_OVERWRITE_CONFIGURATION_FAILURE);
        mp_DateConnector->ReloadModuleList();
        mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CLaSystem",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CLaSystem",
                                             "Finalize Configuration Cancelled.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    mp_Ui->finalizeConfigBtn->setEnabled(false);
}

void CLaSystem::ModifySubModule(const QString &ModuleName,
                                const QString &SubModuleName)
{
    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CLaSystem::ModifySubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CLaSystem::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    ServiceDataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CLaSystem::ModifySubModule(): Invalid sub module : "
                 << SubModuleName;
        return;
    }

    ServiceDataManager::CSubModule SubModule = *pSubModule;

    CDlgModifySubModule *dlg = new CDlgModifySubModule(SubModule);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(ServiceDataManager::CSubModule&)) );

    dlg->exec();

    delete dlg;
}

void CLaSystem::ResetMessageBox()
{
    if (mp_MessageDlg) {
        delete mp_MessageDlg;
        mp_MessageDlg = new MainMenu::CMessageDlg(this);
    }
}

} // namespace SystemTracking
