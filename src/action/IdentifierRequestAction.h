//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H
#define HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H

#include <com/osteres/automation/action/Action.h>
#include <com/osteres/automation/transmission/packet/Packet.h>
#include <service/DatabaseManager.h>
#include <repository/SensorRepository.h>
#include <entity/Sensor.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/transmission/packet/Command.h>

using com::osteres::automation::action::Action;
using com::osteres::automation::transmission::packet::Packet;
using service::DatabaseManager;
using repository::SensorRepository;
using entity::Sensor;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::transmission::packet::Command;


namespace action
{
    class IdentifierRequestAction : public Action {
    public:

        /**
         * Constructor
         */
        IdentifierRequestAction(DatabaseManager * databaseManager, Transmitter * transmitter)
        {
            this->serviceDatabaseManager = databaseManager;
            this->transmitter = transmitter;
        }

        /**
         * Execute action
         *
         * Save packet to database
         */
        bool execute(Packet * packet)
        {
            SensorRepository repositorySensor(this->serviceDatabaseManager);

            // Declare sensor
            Sensor * sensor = repositorySensor.createFromType(packet->getSourceType());

            // Send packet containing identifier
            Packet * p = new Packet();
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
         * Database manager
         */
        DatabaseManager * serviceDatabaseManager = NULL;

        /**
         * Transmitter
         */
        Transmitter * transmitter = NULL;
    };
}

#endif //HOMECENTER_SERVER_ACTION_IDENTIFIERREQUESTACTION_H
