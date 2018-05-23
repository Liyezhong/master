#include "Scheduler/Include/States/Instrument/Busy.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"

namespace Scheduler{
namespace Instrument{

Busy::Busy(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller)
{
    setObjectName("Instrument_Busy_State");
}

Busy::~Busy()
{

}

void Busy::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
}

bool Busy::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{
    auto pSelectedcmd = dynamic_cast<MsgClasses::CmdProgramSelected*>(event->Data().GetPointerToUserData());
    if(pSelectedcmd != nullptr)
    {
        event->SetHandled();
        pTransition = TPTransition_t::Load;
        return true;
    }

    auto actionCmd = dynamic_cast<MsgClasses::CmdProgramAction*>(event->Data().GetPointerToUserData());
    if(actionCmd != nullptr)
    {
        if(actionCmd->ProgramActionType() == DataManager::ProgramActionType_t::PROGRAM_START)
        {
            event->SetHandled();
            // Start a protocol running
            pTransition = TPTransition_t::Start;
        }
    }

    return false;
}

void Busy::RepeatAction(TPTransition_t &pTransition)
{
    // check if session is done
    pTransition = TPTransition_t::Done;
}

}
}
