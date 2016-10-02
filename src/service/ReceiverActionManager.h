//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H
#define HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H

#include <com/osteres/automation/action/ActionManagerBase.h>
#include <com/osteres/automation/action/implement/ConsoleDisplayPacket.h>
#include <service/DatabaseManager.h>
#include <string>
#include <iostream>
#include <action/SavePacketAction.h>

using std::string;
using com::osteres::automation::action::ActionManagerBase;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;
using service::DatabaseManager;
using action::SavePacketAction;

namespace service
{
    class ReceiverActionManager : public ActionManagerBase {
    public:

        /**
         * Constructor
         */
        ReceiverActionManager(DatabaseManager * databaseManager)
        {
            this->actionDisplay = new ConsoleDisplayPacket();
            this->serviceDatabaseManager = databaseManager;
        }

        /**
         * Destructor
         */
        ~ReceiverActionManager() {
            // Remove action display
            if (this->actionDisplay != NULL) {
                delete this->actionDisplay;
                this->actionDisplay = NULL;
            }
        }

        /**
         * Process packet
         */
        void processPacket(Packet *packet) {
            // Output packet to console (debug version) TODO: To remove in production
            this->actionDisplay->execute(packet);

            // Save to database
            SavePacketAction action(this->serviceDatabaseManager);
            action.execute(packet);
        }

    protected:

        /**
         * Action to output packet to console
         * TODO: To remove in production? Or edit to print into log
         */
        ConsoleDisplayPacket *actionDisplay = NULL;

        /**
         * Service database manager
         */
        DatabaseManager * serviceDatabaseManager = NULL;
    };
}

#endif //HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H
