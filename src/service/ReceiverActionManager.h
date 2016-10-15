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
#include <service/RepositoryContainer.h>
#include <string>
#include <iostream>
#include <action/SavePacketAction.h>
#include <action/packet/IdentifierAssignAction.h>
#include <service/manager/IdentifierManager.h>
#include <entity/Sensor.h>

using std::string;
using com::osteres::automation::action::ActionManagerBase;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::transmission::packet::Command;
using service::DatabaseManager;
using service::RepositoryContainer;
using action::SavePacketAction;
using action::packet::IdentifierAssignAction;
using service::manager::IdentifierManager;
using entity::Sensor;

namespace service
{
    class ReceiverActionManager : public ActionManagerBase {
    public:

        /**
         * Constructor
         */
        ReceiverActionManager(RepositoryContainer * repositoryContainer, Transmitter * transmitter)
        {
            // Vars
            this->serviceRepositoryContainer = repositoryContainer;
            this->transmitter = transmitter;

            // Action
            this->actionDisplay = new ConsoleDisplayPacket();
            this->managerIdentifier = new IdentifierManager(this->serviceRepositoryContainer);
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
            // Remove manager identifier
            if (this->managerIdentifier != NULL) {
                delete this->managerIdentifier;
                this->managerIdentifier = NULL;
            }
        }

        /**
         * Process packet
         */
        void processPacket(Packet *packet)
        {
            // Output packet to console (debug version) TODO: To remove in production
            this->actionDisplay->execute(packet);

            // Is packet has identifier ?
            // No: Ignore this packet and send a generated identifier
            if (packet->getSourceIdentifier() == 0) {
                IdentifierAssignAction action(
                    this->serviceRepositoryContainer,
                    this->transmitter,
                    this->managerIdentifier
                );
                action.execute(packet);
            }
            // Yes: Potentially allowed to communicate with server
            else {
                Sensor * sensor = this->managerIdentifier->getByUid(packet->getSourceIdentifier());
                // Is identifier found
                if (sensor == NULL) {
                    // No: Create them before processing packet
                    sensor = this->managerIdentifier->create(packet->getSourceType(), packet->getSourceIdentifier());
                }

                // Save packet to database (for archive)
                SavePacketAction action(this->serviceRepositoryContainer->getServiceDatabaseManager());
                action.execute(packet);

                // Analyse command

                // End: free memory
                delete sensor;
            }
        }



    protected:

        /**
         * Action to output packet to console
         * TODO: To remove in production? Or edit to print into log
         */
        ConsoleDisplayPacket *actionDisplay = NULL;

        /**
         * Transmitter
         */
        Transmitter * transmitter = NULL;

        /**
         * Service repository container
         */
        RepositoryContainer * serviceRepositoryContainer = NULL;

        /**
         * Manager identifier
         */
        IdentifierManager * managerIdentifier = NULL;
    };
}

#endif //HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H
