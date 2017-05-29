//
// Created by Thibault PLET on 29/05/2017.
//

#ifndef HOMECENTER_SERVER_ENTITY_PACKETFORWARD_H
#define HOMECENTER_SERVER_ENTITY_PACKETFORWARD_H

#include <string>

using std::string;

namespace entity
{
    class PacketForward {
    public:

        /**
         * Constructor
         */
        PacketForward(unsigned long long id)
        {
            this->id = id;
        }

        /**
         * Constructor
         */
        PacketForward()
        {

        }

        /**
         * Get id
         */
        unsigned long long getId()
        {
            return this->id;
        }

        /**
         * Set id
         */
        void setId(unsigned long long id)
        {
            this->id = id;
        }

        /**
         * Get type
         */
        string getType()
        {
            return this->type;
        }

        /**
         * Set type
         */
        void setType(string type)
        {
            this->type = type;
        }

        /**
         * Get command to watch
         */
        unsigned char getCommandFrom()
        {
            return this->commandFrom;
        }

        /**
         * Set command to watch
         */
        void setCommandFrom(unsigned char value)
        {
            this->commandFrom = value;
        }

        /**
         * Get command to target
         */
        unsigned char getCommandTo()
        {
            return this->commandTo;
        }

        /**
         * Set command to target
         */
        void setCommandTo(unsigned char value)
        {
            this->commandTo = value;
        }

        /**
         * Get sensor identifier to watch
         */
        unsigned long long getSensorSource()
        {
            return this->sensorSource;
        }

        /**
         * Set sensor identifier to watch
         */
        void setSensorSource(unsigned long long value)
        {
            this->sensorSource = value;
        }

        /**
         * Get sensor identifier to forward packet
         */
        unsigned long long getSensorTarget()
        {
            return this->sensorTarget;
        }

        /**
         * Set sensor identifier to forward packet
         */
        void setSensorTarget(unsigned long long value)
        {
            this->sensorTarget = value;
        }

    protected:
        /**
         * Identifier
         */
        unsigned long long id;

        /**
         * Forward type (copy or ghost)
         */
        string type;

        /**
         * Command to watch
         */
        unsigned char commandFrom;

        /**
         * Command for target
         */
        unsigned char commandTo;

        /**
         * Sensor to watch
         */
        unsigned long long sensorSource;

        /**
         * Sensor to forward packet
         */
        unsigned long long sensorTarget;
    };
}

#endif //HOMECENTER_SERVER_ENTITY_PACKETFORWARD_H
