//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H
#define HOMECENTER_SERVER_SERVICE_RECEIVERACTIONMANAGER_H

#include <com/osteres/automation/action/ActionManagerBase.h>
#include <com/osteres/automation/action/implement/packet/ResponseOkAction.h>
#include <com/osteres/automation/action/implement/ConsoleDisplayPacket.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/transmission/packet/Command.h>
#include <service/DatabaseManager.h>
#include <service/RepositoryContainer.h>
#include <string>
#include <iostream>
#include <inttypes.h>
#include <action/SavePacketAction.h>
#include <action/packet/IdentifierAssignAction.h>
#include <action/packet/DateTimeResponseAction.h>
#include <service/manager/IdentifierManager.h>
#include <entity/Sensor.h>

using std::string;
using com::osteres::automation::action::ActionManagerBase;
using com::osteres::automation::action::implement::packet::ResponseOkAction;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::transmission::packet::Command;
using service::DatabaseManager;
using service::RepositoryContainer;
using action::SavePacketAction;
using action::packet::IdentifierAssignAction;
using action::packet::DateTimeResponseAction;
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
            this->actionDisplay->execute(packet, true);

            // Ignore OK command (useless for treatment)
            if (packet->getCommand() != Command::OK) {

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
                    Sensor *sensor = this->managerIdentifier->getByUid(packet->getSourceIdentifier());
                    // Is identifier found
                    if (sensor == NULL) {
                        // No: Create them before processing packet
                        sensor = this->managerIdentifier->create(packet->getSourceType(),
                                                                 packet->getSourceIdentifier());
                    }

                    //
                    // Forward feature
                    //
                    bool savePacket = true;
                    Packet * packetClone = NULL;
                    string sGhost = "ghost";
                    vector<PacketForward *> forwards = this->serviceRepositoryContainer
                        ->getRepositoryPacketForward()
                        ->fetchBySourceAndCommand(sensor->getId(), packet->getCommand());
                    if (forwards.size() > 0) {
                        for (auto &packetForward : forwards) {

                            // If ghost type, packet will not be copied
                            if (sGhost.compare(packetForward->getType()) == 0) {
                                savePacket = false;
                            }

                            // Forward to destination
                            packetClone = new Packet(packet);
                            packetClone->setTarget(this->serviceRepositoryContainer->getRepositorySensor()->find(packetForward->getSensorTarget())->getUid());
                            packetClone->setCommand(packetForward->getCommandTo());
                            this->transmitter->add(packetClone);

                            // End
                            delete packetForward;

                        }

                        forwards.clear();
                    }


                    // Save packet to database (for archive)
                    if (savePacket) {
                        SavePacketAction action(this->serviceRepositoryContainer->getServiceDatabaseManager());
                        action.execute(packet);
                    }

                    //
                    // Analyse command
                    //

                    // DateTime request
                    if (packet->getCommand() == Command::DATETIME_REQUEST) {
                        DateTimeResponseAction action(this->serviceRepositoryContainer, this->transmitter);
                        action.execute(packet);
                    }


                    // End: free memory
                    if (sensor != NULL) {
                        delete sensor;
                    }
                }
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
