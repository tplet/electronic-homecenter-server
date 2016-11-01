//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_DATETIMERESPONSEACTION_H
#define HOMECENTER_SERVER_ACTION_DATETIMERESPONSEACTION_H

#include <action/AbstractPacketAction.h>
#include <com/osteres/automation/transmission/packet/Packet.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/transmission/packet/Command.h>
#include <iostream>
#include <ctime>

using com::osteres::automation::transmission::packet::Packet;
using action::AbstractPacketAction;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::transmission::packet::Command;
using std::time;


namespace action
{
    namespace packet
    {
        class DateTimeResponseAction : public AbstractPacketAction
        {
        public:

            /**
             * Constructor
             */
            DateTimeResponseAction(
                RepositoryContainer * repositoryContainer,
                Transmitter * transmitter
            ) : AbstractPacketAction(repositoryContainer)
            {
                this->transmitter = transmitter;
            }

            /**
             * Execute action
             *
             * Send datetime to sensor
             */
            bool execute(Packet *packet)
            {
                cout << "Request for datetime." << endl;
                long int timestamp = this->getTimestamp();

                // Send packet containing datetime
                Packet *p = new Packet();
                p->setTarget(packet->getSourceIdentifier());
                p->setSourceType(packet->getTarget()); // Master hasn't identifier
                p->setSourceIdentifier(packet->getTarget());
                p->setCommand(Command::DATETIME_RESPONSE);
                p->setDataLong1(timestamp);
                this->transmitter->add(p);
                cout << "Datetime prepared for sending to sensor" << endl;

                this->executed = true;
                return true;
            }

        protected:

            /**
             * Get current timestamp
             */
            long int getTimestamp()
            {
                time_t t = time(0);
                long int now = static_cast<long int> (t);
                return now;
            }

            /**
             * Transmitter
             */
            Transmitter * transmitter = NULL;
        };
    }
}

#endif //HOMECENTER_SERVER_ACTION_DATETIMERESPONSEACTION_H
