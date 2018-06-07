#include "Scheduler/Include/Actions/FillAction.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/Session.h"
#include <QDebug>


using namespace DeviceControl;
namespace Scheduler{

FillAction::FillAction(SchedulerCommandProcessorBase* commandProcessor, Session* session):
    IAction(session),
    ActionHelper(commandProcessor),
    m_currentState(STATE_FILLING_RVROD_HEATING),
    m_stateWaitResult(false)
{

}

FillAction::~FillAction()
{

}

void FillAction::Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    RVPosition_t position = RV_UNDEF;
    switch (m_currentState)
    {
    case STATE_FILLING_RVROD_HEATING:
        //to do
        position = GetRVPosition(m_stationID, true);
        if (m_stateWaitResult)
        {
            if (IsRVRightPosition(position, mp_session->GetRetortID()))
            {
                m_currentState = STATE_FILLING_LEVELSENSOR_HEATING;
                m_stateWaitResult = false;
            }
        }
        else
        {
            CmdRVReqMoveToRVPosition* moveRVcmd = new CmdRVReqMoveToRVPosition(500, mp_session->GetRetortID());
            moveRVcmd->SetRVPosition(position);
            mp_SchedulerCommandProcessor->pushCmd(moveRVcmd);
            m_stateWaitResult = true;
        }
        break;
    case STATE_FILLING_LEVELSENSOR_HEATING:
        //to do
        m_stateWaitResult = true;
        if (m_stateWaitResult)
        {
            m_currentState = STATE_FILLING;
            m_stateWaitResult = false;
        }
        break;
    case STATE_FILLING:
        if (m_stateWaitResult)
        {
            if( "Scheduler::ALFilling" == cmdName)
            {
                qDebug()<<"************************ fill result:"<<retCode;
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_currentState = STATE_RV_MOVE_TO_SEAL;
                    m_stateWaitResult = false;
                }
                else
                {
                    //retry
                    return;
                }
            }
        }
        else
        {
            CmdALFilling* fillCmd  = new CmdALFilling(500, mp_session->GetRetortID());
            mp_SchedulerCommandProcessor->pushCmd(fillCmd);
            m_stateWaitResult = true;
        }
        break;
    case STATE_RV_MOVE_TO_SEAL:
        position = GetRVPosition(m_stationID, false);
        if (m_stateWaitResult)
        {
            if (IsRVRightPosition(position, mp_session->GetRetortID()))
            {
                m_currentState = STATE_FILLING_RVROD_HEATING;
                m_stateWaitResult = false;
                m_finished = true;
            }
        }
        else
        {
            CmdRVReqMoveToRVPosition* moveRVcmd = new CmdRVReqMoveToRVPosition(500, mp_session->GetRetortID());
            moveRVcmd->SetRVPosition(position);
            mp_SchedulerCommandProcessor->pushCmd(moveRVcmd);
            m_stateWaitResult = true;
        }
        break;
    }
}


}