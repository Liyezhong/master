#ifndef SCHEDULERMACHINE_H
#define SCHEDULERMACHINE_H

#include <QStateMachine>
//#include "SchedulerMainThreadController.h"
#include "ProgramStepStateMachine.h"
#include "RsRvGetOriginalPositionAgain.h"

namespace Scheduler{

class SchedulerMainThreadController;

class CSchedulerStateMachine : public QObject
{
    Q_OBJECT
private:
    QStateMachine* mp_SchedulerMachine;
    QState* mp_InitState;
    QState* mp_IdleState;
    QState* mp_ErrorState;
    QState* mp_BusyState;
    QState* mp_ErrorWaitState;
    CProgramStepStateMachine *mp_ProgramStepStates;
    CRsRvGetOriginalPositionAgain *mp_RSRvGetOriginalPositionAgain;


public:
    CSchedulerStateMachine();
    ~CSchedulerStateMachine();
    void Start();
    void Stop();
    void SendSchedulerInitComplete();
    void SendRunSignal();
    void SendRunComplete();
    void SendErrorSignal();

    void NotifyStInitOK();
    void NotifyStTempOK();
    void NotifyStCurrentOK();
    void NotifyStVoltageOK();
    void NotifyStRVPositionOK();
    void NotifyStPressureOK();
    void NotifyStSealingOK();
    void NotifyStGetStationcheckResult();
    void NotifyStStationLeft();
    void NotifyStStationOK();
    void NotifyStDone();
    void NotifyTempsReady();
    void NotifyLevelSensorTempS1Ready();
    void NotifyLevelSensorTempS2Ready();
    void NotifyHitTubeBefore();
    void NotifyFillFinished();
    void NotifyHitSeal();
    void NotifySoakFinished();
    void NotifyHitTubeAfter();
    void NotifyDrainFinished();
    void NotifyStepFinished();
    void NotifyProgramFinished();
    void NotifyError();
    void NotifyPause(SchedulerStateMachine_t PreviousState);
    void NotifyResume();
    void NotifyResumeDrain();
    void NotifyAbort();
    void NotifyResumeToSelftest();
    void NotifyRsRvMoveToInitPosition();
    void NotifyRsRvMoveToInitPositionFinished();

    SchedulerStateMachine_t GetCurrentState();
    SchedulerStateMachine_t GetPreviousState();

private slots:
    void OnStateChanged();

signals:
    void SchedulerInitComplete();
    void RunSignal();
    void RunComplete();
    void RecoverToPause();
    void ErrorSignal();

    void sigStInitOK();
    void sigStTempOK();
    void sigStCurrentOK();
    void sigStVoltageOK();
    void sigStRVPositionOK();
    void sigStPressureOK();
    void sigStSealingOK();
    void sigStGetStationcheckResult();
    void sigStStationLeft();
    void sigStStationOK();
    void sigStDone();
    void sigTempsReady();
    void sigLevelSensorTempS1Ready();
    void sigLevelSensorTempS2Ready();
    void sigHitTubeBefore();
    void sigFillFinished();
    void sigHitSeal();
    void sigSoakFinished();
    void sigHitTubeAfter();
    void sigDrainFinished();
    void sigStepFinished();
    void sigProgramFinished();
    void sigError();
    void sigPause();
    void sigResumeToSelftest();
    void sigResumeToInit();
    void sigResumeToHeatLevelSensorS1();
    void sigResumeToHeatLevelSensorS2();
    void sigResumeToReadyToFill();
    void sigResumeToSoak();
    void sigResumeToStepFinished();
    void sigResumeToReadyToTubeAfter();
    void sigAbort();
    void sigRsRvMoveToInitPosition();
    void sigRsRvMoveToInitPositionFinished();

    void sigOnInit();
    void sigOnHeatLevelSensorTempS1();
    void sigOnHeatLevelSensorTempS2();
    void sigOnMoveToTubeBefore();
    void sigOnMoveToTubeAfter();
    void sigOnMoveToSeal();
    void sigOnFill();
    void sigOnStopFill();
    void sigOnSoak();
    void sigOnDrain();
    void sigOnStopDrain();
    void sigOnAborting();
    void sigOnAborted();
    void sigOnPause();
    void sigOnPauseDrain();
    void sigOnRsRvMoveToInitPosition();
};
}

#endif // SCHEDULERMACHINE_H
