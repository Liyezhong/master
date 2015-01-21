/****************************************************************************/
/*! \file LevelSensor.h
 *
 *  \brief LevelSensor definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-11
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *      Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef CALIBRATION_LEVELSENSOR_H
#define CALIBRATION_LEVELSENSOR_H

#include <QWidget>

namespace Calibration {

namespace Ui {
    class CLevelSensor;
}

/****************************************************************************/
/**
 * \brief This class handles the calibration of the liquid level sensor.
 */
/****************************************************************************/
class CLevelSensor : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = parent windows
     */
    /****************************************************************************/
    explicit CLevelSensor(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CLevelSensor();

private:
    Ui::CLevelSensor *mp_Ui;    //!< User interface
    QStringList m_Instructions; //!< List of calibration instructions
    qint32 m_Step;              //!< Current step in the calibration process

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emitted to device configure
     */
    /****************************************************************************/
    void deviceConfigure();

private slots:

    /****************************************************************************/
    /*!
     *  \brief slot for next step
     */
    /****************************************************************************/
    void NextStep();

    /****************************************************************************/
    /*!
     *  \brief slot for test device init
     */
    /****************************************************************************/
    void TestForDeviceInit();
};

} // end namespace Calibration

#endif // CALIBRATION_LEVELSENSOR_H
