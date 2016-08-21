//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H
#define HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H

#include <com/osteres/automation/action/ActionManagerBase.h>
#include <com/osteres/automation/action/implement/ConsoleDisplayPacket.h>

using com::osteres::automation::action::ActionManagerBase;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;

namespace action {
    class ReceiverActionManager : public ActionManagerBase {
    public:

        /**
         * Constructor
         */
        ReceiverActionManager()
        {
            this->actionDisplay = new ConsoleDisplayPacket();
        }

        /**
         * Destructor
         */
        ~ReceiverActionManager()
        {
            // Remove action display
            if (this->actionDisplay != NULL) {
                delete this->actionDisplay;
                this->actionDisplay = NULL;
            }
        }

        /**
         * Process packet
         */
        void processPacket(Packet * packet)
        {
            // Output packet to console (debug version) TODO: To remove in production
            this->actionDisplay->execute(packet);


        }

    protected:

        /**
         * Action to output packet to console
         * TODO: To remove in production? Or edit to print into log
         */
        ConsoleDisplayPacket * actionDisplay = NULL;
    };
}

#endif //HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H
