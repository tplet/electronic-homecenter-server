//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H
#define HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H

#include <com/osteres/automation/action/ActionManagerBase.h>
#include <com/osteres/automation/action/implement/ConsoleDisplayPacket.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/transmission/packet/Command.h>
#include <service/DatabaseManager.h>
#include <string>
#include <iostream>
#include <action/SavePacketAction.h>
#include <action/IdentifierRequestAction.h>

using std::string;
using com::osteres::automation::action::ActionManagerBase;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::transmission::packet::Command;
using service::DatabaseManager;
using action::SavePacketAction;
using action::IdentifierRequestAction;

namespace service
{
    class ReceiverActionManager : public ActionManagerBase {
    public:

        /**
         * Constructor
         */
        ReceiverActionManager(DatabaseManager * databaseManager, Transmitter * transmitter)
        {
            this->actionDisplay = new ConsoleDisplayPacket();
            this->serviceDatabaseManager = databaseManager;
            this->transmitter = transmitter;
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

            // Save to database (for archive)
            SavePacketAction action(this->serviceDatabaseManager);
            action.execute(packet);

            // Analyse some command
            if (Command::IDENTIFIER_REQUEST == packet->getCommand()) {
                IdentifierRequestAction action(this->serviceDatabaseManager, this->transmitter);
                action.execute(packet);
            }
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

        /**
         * Transmitter
         */
        Transmitter * transmitter = NULL;
    };
}

#endif //HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H
