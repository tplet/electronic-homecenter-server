//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H
#define HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H

#include <action/AbstractPacketAction.h>
#include <com/osteres/automation/transmission/packet/Packet.h>
#include <service/repository/SensorRepository.h>
#include <entity/Sensor.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/transmission/packet/Command.h>

using com::osteres::automation::transmission::packet::Packet;
using service::repository::SensorRepository;
using action::AbstractPacketAction;
using entity::Sensor;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::transmission::packet::Command;


namespace action
{
    namespace packet
    {
        class IdentifierAssignAction : public AbstractPacketAction
        {
        public:

            /**
             * Constructor
             */
            IdentifierAssignAction(RepositoryContainer * repositoryContainer, Transmitter *transmitter) : AbstractPacketAction(repositoryContainer)
            {
                this->transmitter = transmitter;
            }

            /**
             * Execute action
             *
             * Save packet to database
             */
            bool execute(Packet *packet)
            {
                SensorRepository * repositorySensor = this->getServiceRepositoryContainer()->getRepositorySensor();

                // Declare sensor
                Sensor * sensor = repositorySensor->createFromType(packet->getSourceType());

                // Send packet containing identifier
                Packet *p = new Packet();
                p->setTarget(packet->getSourceType()); // Use type for the moment
                p->setSourceType(packet->getTarget()); // Master hasn't identifier
                p->setSourceIdentifier(packet->getTarget());
                p->setCommand(Command::IDENTIFIER_RESPONSE);
                p->setDataUChar1(sensor->getUid());
                p->setLast(true);
                this->transmitter->send(p);

                // Free memory
                delete sensor;
                delete p;

                this->executed = true;
                return true;
            }

        protected:

            /**
             * Transmitter
             */
            Transmitter * transmitter = NULL;
        };
    }
}

#endif //HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H
