//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_RECEIVERACTION_H
#define HOMECENTER_SERVER_ACTION_RECEIVERACTION_H

#include <com/osteres/automation/action/Action.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/action/ActionManagerBase.h>

using com::osteres::automation::action::Action;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::action::ActionManagerBase;

/**
 * USELESS?
 */
namespace action
{
    class ReceiverAction : public Action {
    public:

        /**
         * Constructor
         */
        ReceiverAction(Transmitter * transmitter, ActionManagerBase * actionManager) {
            this->transmitter = transmitter;
            this->actionManager = actionManager;
        }

        /**
         * Execute service
         */
        bool execute() {
            // Ensure that transmitter use right action manager
            this->transmitter->setActionManager(this->actionManager);

            // Listen
            //this->transmitter->listen();

            this->executed = true;
            return true;
        }

        /**
         * Get transmitter
         */
        Transmitter * getTransmitter()
        {
            return this->transmitter;
        }

    protected:

        /**
         * Transmitter
         */
        Transmitter *transmitter = NULL;

        /**
         * Action manager
         */
        ActionManagerBase * actionManager = NULL;
    };
}

#endif //HOMECENTER_SERVER_ACTION_RECEIVERACTION_H
