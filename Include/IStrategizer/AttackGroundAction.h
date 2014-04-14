///> [Serializable]
#ifndef ATTACKGROUNDACTION_H
#define ATTACKGROUNDACTION_H

#include "Action.h"
#ifndef AND_H
#include "And.h"
#endif

#include "Vector2.h"

namespace IStrategizer
{
    class CellFeature;
    
    ///> class=AttackGroundAction
    ///> parent=Action
    class AttackGroundAction : public Action
    {
    public:
        OBJECT_SERIALIZABLE(AttackGroundAction);

        AttackGroundAction();
        AttackGroundAction(const PlanStepParameters& p_parameters);
        bool AliveConditionsSatisfied(RtsGame& game);
        bool SuccessConditionsSatisfied(RtsGame& game);

    protected:
        bool ExecuteAux(RtsGame& game, const WorldClock& p_clock);
        void InitializeAddressesAux() ;
        void HandleMessage(RtsGame& game, Message* p_msg, bool& p_consumed);
        void InitializePostConditions();
        void InitializePreConditions();

    private:
        TID _attackerId;
        Vector2 _position;
    };
}

#endif // ATTACKGROUNDACTION_H
