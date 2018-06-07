#include "Scheduler/Include/Actions/ActionHelper.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"

using namespace DeviceControl;
namespace Scheduler{

ActionHelper::ActionHelper(SchedulerCommandProcessorBase* commandProcessor):
    mp_SchedulerCommandProcessor(commandProcessor)
{

}

ActionHelper::~ActionHelper()
{

}

DeviceControl::RVPosition_t ActionHelper::GetRVPosition(const QString& stationID, bool isTube)
{
    RVPosition_t ret = RV_UNDEF;
    bool ok = false;
    if (!stationID.isEmpty())
    {
        int position = stationID.right(stationID.length() - 1).toInt(&ok, 10);
        if (ok)
        {
            int offset = 0;
            if (isTube)
            {
                offset = 1;
            }

            if(stationID.left(1) == "S")
            {
                ret = (RVPosition_t)(position * 2 - offset);
            }
            else if(stationID.left(1) == "P")
            {
                ret = (RVPosition_t)((position + 13) * 2 - offset);
            }
        }
    }
    return ret;
}

bool ActionHelper::IsRVRightPosition(DeviceControl::RVPosition_t position, const QString& retortID)
{
    HardwareMonitor_t strctHWMonitor = mp_SchedulerCommandProcessor->HardwareMonitor(retortID);

    if (strctHWMonitor.PositionRV == position)
    {
        return true;
    }
    else
    {
        return false;
    }
}


}
