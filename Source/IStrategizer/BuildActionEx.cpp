#include "BuildActionEx.h"

#include <cassert>
#include "Vector2.h"
#include "OnlineCaseBasedPlannerEx.h"
#include "AbstractAdapter.h"
#include "CellFeature.h"
#include "CaseBasedReasonerEx.h"
#include "DataMessage.h"
#include "EngineAssist.h"
#include "RtsGame.h"
#include "GamePlayer.h"
#include "GameTechTree.h"
#include "GameType.h"
#include "GameEntity.h"

using namespace OLCBP;
using namespace IStrategizer;
using namespace MetaData;

const unsigned MaxPrepTime = 5000;
const unsigned MaxExecTrialTime = 500;
const unsigned MaxExecTime = 5000;

BuildActionEx::BuildActionEx() :
Action(ACTIONEX_BuildEx, MaxPrepTime, MaxExecTrialTime, MaxExecTime), _buildStarted(false), _buildIssued(false)
{
	_params[PARAM_BuildingClassId]	= ECLASS_START;
}
//////////////////////////////////////////////////////////////////////////
BuildActionEx::BuildActionEx(const PlanStepParameters& p_parameters) :
Action(ACTIONEX_BuildEx, p_parameters, MaxPrepTime, MaxExecTrialTime, MaxExecTime), _buildStarted(false),  _buildIssued(false)
{
}
//////////////////////////////////////////////////////////////////////////
void BuildActionEx::OnSucccess(unsigned p_cycles)
{
	if (_buildIssued)
	{
		assert(!_buildArea.IsNull());
		_buildArea.Unlock(this);

		GameEntity *pEntity = g_Game->Self()->GetEntity(_builderId);

		if (pEntity)
			pEntity->Unlock(this);
	}
}
//////////////////////////////////////////////////////////////////////////
void BuildActionEx::OnFailure(unsigned p_cycles)
{
	if (_buildIssued)
	{
		assert(!_buildArea.IsNull());
		_buildArea.Unlock(this);

		GameEntity *pEntity = g_Game->Self()->GetEntity(_builderId);
		
		if (pEntity)
			pEntity->Unlock(this);
	}
}
//////////////////////////////////////////////////////////////////////////
void BuildActionEx::HandleMessage(Message* p_pMsg, bool& p_consumed)
{
	if(PlanStepEx::State() == ESTATE_Executing && p_pMsg->MessageTypeID() == MSG_EntityCreate) 
	{
		EntityCreateMessage*	pMsg = static_cast<EntityCreateMessage*>(p_pMsg);
		TID						buildingId;
		GameEntity				*pGameBuilding;
		Vector2					msgBuildPosition;

		if (pMsg->Data()->OwnerId != PLAYER_Self)
			return;

		assert(pMsg && pMsg->Data());
		buildingId = pMsg->Data()->EntityId;

		pGameBuilding = g_Game->Self()->GetEntity(buildingId);
		assert(pGameBuilding);

		msgBuildPosition.X = pMsg->Data()->X;
		msgBuildPosition.Y = pMsg->Data()->Y;

		if (msgBuildPosition.X == _buildArea.Pos().X &&
			msgBuildPosition.Y == _buildArea.Pos().Y &&
			pGameBuilding->Type() == _params[PARAM_BuildingClassId])
		{
			_buildingId = pGameBuilding->Id();
			_buildStarted = true;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool BuildActionEx::PreconditionsSatisfied()
{
	EntityClassType	builderType;
	EntityClassType	buildingType;
	int				ret;
	bool			success = false;

	builderType = g_Game->Self()->GetWorkerType();
	ret = g_Assist.EntityClassExist(make_pair(builderType, 1), success);
	assert(ret == ERR_Success);

	if (!success)
		return false;

	buildingType = (EntityClassType)_params[PARAM_BuildingClassId];
	ret = g_Assist.PrerequisitesSatisfied(buildingType, success);

	assert(ret == ERR_Success);

	return success;
}
//////////////////////////////////////////////////////////////////////////
bool BuildActionEx::AliveConditionsSatisfied()
{
	int			ret;
	bool		success = false;
	GameEntity	*pEntity = NULL;

	assert(PlanStepEx::State() == ESTATE_Executing);

	ret = g_Assist.EntityObjectExist(_builderId, success);
	assert(ret == ERR_Success);

	if (success)
	{
		pEntity = g_Game->Self()->GetEntity(_builderId);
		success = (pEntity->Attr(EOATTR_State) == OBJSTATE_Constructing);
	}

	if (success && _buildStarted)
	{
		ret = g_Assist.EntityObjectExist(_buildingId, success);
		assert(ret == ERR_Success);
	}

	return success;
}
//////////////////////////////////////////////////////////////////////////
bool BuildActionEx::SuccessConditionsSatisfied()
{
	assert(PlanStepEx::State() == ESTATE_Executing);

	if (_buildStarted)
	{
		int			entityState;
		GameEntity	*pEntity;

		pEntity = g_Game->Self()->GetEntity(_buildingId);	
		entityState = pEntity->Attr(EOATTR_State);

		return entityState != OBJSTATE_BeingConstructed;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
bool BuildActionEx::ExecuteAux(unsigned long p_cycles)
{
	EntityClassType		builderType;
	EntityClassType		buildingType;
	GameEntity			*pGameBuilder;
	AbstractAdapter		*pAdapter = g_OnlineCaseBasedPlanner->Reasoner()->Adapter();
	bool				bOk;

	builderType = g_Game->Self()->GetWorkerType();
	buildingType = (EntityClassType)_params[PARAM_BuildingClassId];

	// Initialize build state
	_buildStarted = false;

	// Adapt build position
	assert(pAdapter);
	_buildArea = pAdapter->AdaptPositionForBuilding(buildingType);

	// Adapt builder
	_builderId = pAdapter->AdaptWorkerForBuild();

	// Issue build order
	pGameBuilder = g_Game->Self()->GetEntity(_builderId);
	assert(pGameBuilder);

	bOk = pGameBuilder->Build(buildingType, _buildArea.Pos().X, _buildArea.Pos().Y);

	if (bOk)
	{
		_buildIssued = true;
		pGameBuilder->Lock(this);
		_buildArea.Lock(this);
	}

	return bOk;
}
