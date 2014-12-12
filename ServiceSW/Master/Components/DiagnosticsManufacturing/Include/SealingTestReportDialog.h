/****************************************************************************/
/*! \file SealingTestReportDialog.h
 *
 *  \brief Header file for class CSealingTestReportDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-30
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
#ifndef SEALINGTESTREPORTDIALOG_H
#define SEALINGTESTREPORTDIALOG_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceDefines.h"
#include <QTimer>
#include <QList>
#include <QLabel>

namespace DiagnosticsManufacturing {

namespace Ui {
    class CSealingTestReportDialog;
}

/****************************************************************************/
/*!
 *  \brief This is the class of the diagnostics Seaing test report dialog
 */
/****************************************************************************/
class CSealingTestReportDialog : public MainMenu::CDialogFrame
{
    Q_OBJECT

    /****************************************************************************/
    /**
     * \brief Struct for Label group data.
     */
    /****************************************************************************/
    struct LabelGroup {
        LabelGroup(QLabel* label1, QLabel*label2, QLabel*label3) {
            positionLabel = label1;
            pressureLabel = label2;
            resultLabel   = label3;
        }
        QLabel* positionLabel;      //!< Position text label
        QLabel* pressureLabel;      //!< Pressure text label
        QLabel* resultLabel;        //!< result text label
    };
public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    explicit CSealingTestReportDialog(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CSealingTestReportDialog();

    /****************************************************************************/
    /*!
     *  \brief Update label status
     *
     *  \iparam Status = Label test status
     */
    /****************************************************************************/
    void UpdateLabel(const Service::ModuleTestStatus &Status);

private slots:

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void RetranslateUI();

    /****************************************************************************/
    /*!
     *  \brief OnButtonClicked
     */
    /****************************************************************************/
    void OnButtonClicked();
signals:
    /****************************************************************************/
    /**
       * \brief Signal emitted to perform manufacturing tests
       * \iparam Test = Test name
       */
    /****************************************************************************/
    void PerformManufacturingTest(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortTestCaseId);

    /****************************************************************************/
    /**
       * \brief Signal emitted to perform manufacturing tests
       * \iparam Result = true of false
       */
    /****************************************************************************/
    void RefreshResultToMain(bool Result);

protected:
    /****************************************************************************/
    /*!
     *  \brief Event filter
     *  \iparam p_Object = QObject
     *  \iparam p_Event = event object
     */
    /****************************************************************************/
    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    Ui::CSealingTestReportDialog *mp_Ui;     //!< User interface
    QList<LabelGroup*> m_LabelGroups;     //!< label group
    int m_Index;                        //!< label index
    bool m_IsOkButton;                  //!< flag of Ok button
    bool m_IsAbort;                     //!< flag of Abort button

    /****************************************************************************/
    /*!
     *  \brief label init
     */
    /****************************************************************************/
    void InitLableGroup();
    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CSealingTestReportDialog)

};

} // end namespace DiagnosticsManufacturing

#endif // SEALINGTESTREPORTDIALOG_H
