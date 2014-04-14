#ifndef WARGUSPLAYERRESOURCES_H
#include "StarCraftPlayerResources.h"
#endif

#include "BWAPI.h"

using namespace StarCraftModel;
using namespace BWAPI;

int StarCraftPlayerResources::Supply() 
{ 
    return m_player->supplyTotal() - m_player->supplyUsed();
}
//////////////////////////////////////////////////////////////////////////
int StarCraftPlayerResources::Secondary()
{
    return m_player->gas();
}
//////////////////////////////////////////////////////////////////////////
int StarCraftPlayerResources::Primary()
{
    return m_player->minerals();
}
