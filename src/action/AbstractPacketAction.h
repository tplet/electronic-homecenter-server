//
// Created by Thibault PLET on 04/10/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_PACKET_ABSTRACTPACKETACTION_H
#define HOMECENTER_SERVER_ACTION_PACKET_ABSTRACTPACKETACTION_H

#include <com/osteres/automation/action/Action.h>
#include <service/RepositoryContainer.h>

using com::osteres::automation::action::Action;
using service::RepositoryContainer;

namespace action
{
    class AbstractPacketAction : public Action
    {
    public:
        /**
         * Constructor
         */
        AbstractPacketAction(RepositoryContainer * repositoryContainer) : Action()
        {
            this->serviceRepositoryContainer = repositoryContainer;
        }

        /**
         * Get service repository container
         */
        RepositoryContainer * getServiceRepositoryContainer()
        {
            return this->serviceRepositoryContainer;
        }

    protected:
        /**
         * Service repository container
         */
        RepositoryContainer * serviceRepositoryContainer = NULL;
    };
}

#endif //HOMECENTER_SERVER_ACTION_PACKET_ABSTRACTPACKETACTION_H
