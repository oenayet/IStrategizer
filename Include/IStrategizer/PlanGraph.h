///> [Serializable]
#ifndef PLANGRAPH_H
#define PLANGRAPH_H

#include <vector>
#include <map>
#include <algorithm>

#ifndef DIAGRAPH_H
#include "Diagraph.h"
#endif

#ifndef USEROBJECT_H
#include "UserObject.h"
#endif

#ifndef EXPRESSION_H
#include "Expression.h"
#endif

namespace IStrategizer
{
    class PlanStepEx;
    class ConditionEx;

    ///> alias=PlanGraphNodeValue(PlanStepEx*)
    typedef PlanStepEx* PlanGraphNodeValue;
    ///> alias=EdgeAnnotation(vector(Expression*))
    typedef Serialization::SVector<Expression*> EdgeAnnotation;

    ///> class=PlanGraph
    ///> parent=Diagraph(PlanGraphNodeValue,EdgeAnnotation)
    class PlanGraph : public Diagraph<PlanGraphNodeValue, EdgeAnnotation>
    {
        OBJECT_SERIALIZABLE(PlanGraph);

    public:
        PlanGraph() {}
        //----------------------------------------------------------------------------------------------
        PlanGraph(std::vector<PlanStepEx*> p_sPlan) : Diagraph(p_sPlan.size())
        {
            for (unsigned i = 0; i < p_sPlan.size(); ++i)
            {
                AddNode(p_sPlan[i], i);
            }
        }
        //----------------------------------------------------------------------------------------------
        PlanGraph(PlanStepEx* p_planStep) : Diagraph(1)
        {
            AddNode(p_planStep, 0);
        }
        //----------------------------------------------------------------------------------------------
        void SubGraphSubstitution(std::vector<int> p_subGraphIndexes, PlanGraphNodeValue p_substitute)
        {
            std::map<int, EdgeAnnotation> m_parents;
            std::map<int, EdgeAnnotation> m_children;
            std::vector<int> m_temp;
            unsigned i, j;
            EdgeAnnotation m_eAnn;

            std::sort(p_subGraphIndexes.begin(), p_subGraphIndexes.end(), std::less<int>());
            int m_sgNodeIndex;

            for (i = 0; i < p_subGraphIndexes.size(); ++i)
            {
                m_sgNodeIndex = p_subGraphIndexes[i];
                m_temp.clear();
                m_temp = _adjacencyMatrix[m_sgNodeIndex]->GetChildren(p_subGraphIndexes);

                for (j = 0; j < m_temp.size(); ++j)
                {
                    m_eAnn = GetConnectionAnnotation(m_sgNodeIndex, m_temp[j]);
                    if(!m_eAnn.empty())
                        m_children[m_temp[j]].insert(m_children[m_temp[j]].end(), m_eAnn.begin(), m_eAnn.end());
                }

                m_temp.clear();
                m_temp = _adjacencyMatrix[m_sgNodeIndex]->GetParents(p_subGraphIndexes);

                for (j = 0; j < m_temp.size(); ++j)
                {
                    m_eAnn = GetConnectionAnnotation(m_temp[j], m_sgNodeIndex);
                    if(!m_eAnn.empty())
                        m_parents[m_temp[j]].insert(m_parents[m_temp[j]].end(), m_eAnn.begin(), m_eAnn.end());
                }
            }

            AddNode(p_substitute, Size());

            for (std::map<int, EdgeAnnotation>::iterator i = m_parents.begin(); i != m_parents.end(); ++i) 
                Connect((*i).first, Size() - 1, (*i).second);

            for (std::map<int, EdgeAnnotation>::iterator i = m_children.begin(); i != m_children.end(); ++i)
                Connect(Size() - 1, (*i).first, (*i).second);

            for (unsigned i = 0; i < p_subGraphIndexes.size(); ++i)
            {
                RemoveNode( p_subGraphIndexes[i] - i );
            }
        }
    };
}

#endif // PLANGRAPH_H
