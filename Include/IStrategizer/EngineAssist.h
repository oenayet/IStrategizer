#ifndef ENGINEASSIST_H
#define ENGINEASSIST_H

#include "Misc.h"
#include "EngineData.h"
#include "Expression.h"
#include "WorldResources.h"
#include <vector>
#include <map>

#define ENTITY_DEST_ARRIVAL_THRESHOLD_DISTANCE 64

namespace IStrategizer
{
    class Vector2;

    class EngineAssist
    {
    protected:
        EngineAssist() {}

    public:
        static EngineAssist& Instance() { static EngineAssist m_instance; return m_instance; }
        CheckReturn int GetRequiredResources(IN PlayerType p_playerIndex, IN TID p_classId, OUT Cost& p_requiredCost);
        CheckReturn int GetRequiredBuildings(IN PlayerType p_playerIndex, IN TID p_classId, OUT std::vector<EntityClassType>& p_requiredBuildings);
        CheckReturn int GetRequiredResearches(IN PlayerType p_playerIndex, IN TID p_classId, OUT std::vector<ResearchType>& p_requiredResearches);
        CheckReturn int GetEntityClassAttribute(IN PlayerType p_playerIndex, IN EntityClassType p_classId, IN EntityClassAttribute p_attributeId, OUT int& p_value);
        CheckReturn int GetEntityObjectAttribute(IN PlayerType p_playerIndex, IN TID p_entityObjectId, IN EntityObjectAttribute p_attributeId, OUT int& p_value);
        CheckReturn int GetEntityObjectFromEntityClass(IN PlayerType p_playerIndex, IN EntityClassType p_classId, IN const std::vector<TID>& p_execludedIds, OUT TID& p_objectId);
        CheckReturn int GetSourceBuilding(IN PlayerType p_playerIndex, IN TID p_entityClassId, OUT EntityClassType& p_sourceBuildingId);
        CheckReturn int GetFilterCount(IN PlayerType p_playerIndex, IN FilterType p_filterIndex, IN const Vector2& p_cellIndex, OUT int& p_count);
        CheckReturn int GetObject(IN PlayerType p_playerIndex, IN TID p_entityObjectId);
        CheckReturn int ResearchDone(IN PlayerType p_playerIndex, IN ResearchType p_researchId);
        CheckReturn int GetResourceAmount(IN PlayerType p_playerIndex, IN ResourceType p_resourceId, OUT int& p_availableAmount);
        CheckReturn int ExecuteResearch(IN ResearchType p_researchId, IN TID p_sourceBuildingObjectId);
        CheckReturn int ExecuteBuild(IN EntityClassType p_buildingClassId, IN TID p_workerObjectId, IN const Vector2& p_posDescription, OUT TID& p_buildingObjectId);
        CheckReturn int ExecuteTrain(IN TID p_trainerObjectId, IN EntityClassType p_entityClassId);
        CheckReturn int ExecuteAttackGround(IN TID p_entityObjectId, IN const Vector2& p_pos);
        CheckReturn int ExecuteAttackEntity(IN TID p_attackerObjectId, IN PlayerType p_opponentIndex, IN TID p_targetEntityObjectId);
        CheckReturn int GetRequiredBuildingsForBaseType(IN PlayerType p_playerIndex, IN BaseType p_baseTypeId, OUT std::map<EntityClassType, int>& p_requiredBuildings);
        CheckReturn int GetForceDescriptionEntities(IN PlayerType p_playerType, IN const ForceDescriptionEx& p_forceDescription, OUT std::map<EntityClassType, int>& p_entities);
        CheckReturn int GetCurrentPlayers(OUT std::vector<PlayerType>& p_playerIds);
        CheckReturn int GetPlayerAttribute(IN PlayerType p_playerType, IN PlayerAttribute p_attribute, OUT int& p_value);
        CheckReturn int GetTireBaseBuildingId(IN PlayerType p_playerType, IN BaseType p_baseType, OUT EntityClassType& p_entityClassId);
        CheckReturn int GetEntities(IN PlayerType p_playerType, IN const std::vector<EntityClassType>& p_entityTypes, OUT std::vector<TID>& p_entityObjects);
        CheckReturn int PrerequisitesSatisfied(IN int p_entityOrResearchType, OUT bool &p_satisfied, IN PlayerType p_playerType = PLAYER_Self);
        CheckReturn bool DoesEntityClassExist(IN std::pair<EntityClassType, unsigned> p_entityType, IN PlayerType p_playerType = PLAYER_Self);
        CheckReturn bool DoesEntityClassExist(IN const std::map<EntityClassType, unsigned> &p_entityTypes, IN PlayerType p_playerType = PLAYER_Self);
        CheckReturn bool DoesEntityObjectExist(IN TID p_entityObject, IN PlayerType p_playerType = PLAYER_Self);
        CheckReturn bool DoesEntityObjectExist(IN const std::vector<TID> &p_entityObjects, IN PlayerType p_playerType = PLAYER_Self);
        CheckReturn int ResearchesDone(IN const std::vector<ResearchType> &p_researchTypes, OUT bool &p_done, IN PlayerType p_playerType = PLAYER_Self);
        CheckReturn bool IsEntityCloseToPoint(IN const TID p_entityId, IN const Vector2& p_point, IN const unsigned p_maxDistance);
        CheckReturn void GetPrerequisites(IN int p_entityOrResearchType, IN PlayerType p_playerType, OUT std::vector<Expression*>& p_prerequisites);
        CheckReturn void GetPrerequisiteResources(IN int p_entityOrResearchType, IN PlayerType p_playerType, OUT WorldResources& p_resources);

#define g_Assist EngineAssist::Instance()
    };
}

#endif // ENGINEAPI
