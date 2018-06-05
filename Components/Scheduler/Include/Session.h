#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QSharedPointer>

#include "HimalayaDataContainer/Containers/Programs/Include/ProgramStep.h"
#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"
#include "Scheduler/Include/IAction.h"

using namespace DataManager;

namespace Scheduler{


typedef struct {
    QString stationID;       ///<  Definition/Declaration of variable stationID
    QString nextStationID;   ///< Definition/Declaration of variable stationID
    qint64 durationInSeconds;       ///<  Definition/Declaration of variable durationInSeconds
    int temperature;       ///<  Definition/Declaration of variable temperature
    bool isPressure;       ///<  Definition/Declaration of variable isPressure
    bool isVacuum;       ///<  Definition/Declaration of variable isVacuum
    QString reagentGroup;       ///<  Definition/Declaration of variable reagentGroup
} ProgramStepInfor;


typedef struct Schedule
{
public:
    Schedule(QDateTime startTime,  int actionNum)
    {
        StartTime = startTime;
        ActionNum = actionNum;
    }

    int GetNextAction() {return ActionIndex++; }

private:
    int ActionIndex;
    int ActionNum;
    QDateTime StartTime;
}Schedule_t;

class Session
{
public:
    Session(const QString& retortId, CProgram* pProgram);
    ~Session();

    const QString& GetRetortID() {return m_RetortId;}
    CProgram* GetProgram() {return m_pProgram;}

    QList<QSharedPointer<const CProgramStep>> GetProgramStep() {return m_pProgramSteps;}

    ProgramStepInfor* GetProgramStepInfo(){return m_pProgramStepInfo;}

    QList<QSharedPointer<IAction>>& GetActionList(){return m_ActionList;}
    
    Schedule* GetSchedule() const {return m_pSchedule;}

    void SetSchedule(Schedule* pSchedule) {m_pSchedule = pSchedule;}

    typedef enum Status
    {
        Initial,
        Ready,
        Running,
        Completed,
        Error
    }SessionStatus_t;
        
    void SetStatus(const SessionStatus_t& status){m_Status = status;}
    SessionStatus_t GetStatus() const { return m_Status;}

    Q_DISABLE_COPY(Session)
private:
    QString m_RetortId;
    CProgram* m_pProgram;
    QList<QSharedPointer<const CProgramStep>> m_pProgramSteps;
    QList<QSharedPointer<IAction>> m_ActionList;
    ProgramStepInfor* m_pProgramStepInfo;
    SessionStatus_t m_Status;
    Schedule* m_pSchedule;
};
}
#endif // SESSION_H
