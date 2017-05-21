//
// Created by Thibault PLET on 21/05/2017.
//

#ifndef HOMECENTER_SERVER_ENTITY_PACKETQUEUE_H
#define HOMECENTER_SERVER_ENTITY_PACKETQUEUE_H

#include <string>

using std::string;

namespace entity
{
    class PacketQueue {
    public:

        /**
         * Constructor
         */
        PacketQueue(unsigned long long id)
        {
            this->id = id;
        }

        /**
         * Constructor
         */
        PacketQueue()
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
         * Get sensor type identifier
         */
        unsigned char getSourceType()
        {
            return this->sourceType;
        }

        /**
         * Set sensor type identifier
         */
        void setSourceType(unsigned char type)
        {
            this->sourceType = type;
        }

        /**
         * Get sensor identifier (uniq for all automation park)
         */
        unsigned char getSourceIdentifier()
        {
            return this->sourceIdentifier;
        }

        /**
         * Set sensor identifier (uniq for all automation park)
         */
        void setSourceIdentifier(unsigned char identifier)
        {
            this->sourceIdentifier = identifier;
        }

        /**
         * Get command identifier
         */
        unsigned char getCommand()
        {
            return this->command;
        }

        /**
         * Set command identifier
         */
        void setCommand(unsigned char command)
        {
            this->command = command;
        }

        /**
         * Get target of request
         */
        unsigned char getTarget()
        {
            return this->target;
        }

        /**
         * Set target of request
         */
        void setTarget(unsigned char target)
        {
            this->target = target;
        }

        /**
         * Get timestamp associated to data
         */
        unsigned long getDate()
        {
            return this->date;
        }

        /**
         * Set timestamp associated to data
         */
        void setDate(unsigned long date)
        {
            this->date = date;
        }

        unsigned char getDataUChar1()
        {
            return this->dataUChar1;
        }

        unsigned char getDataUChar2()
        {
            return this->dataUChar2;
        }

        unsigned char getDataUChar3()
        {
            return this->dataUChar3;
        }

        void setDataUChar1(unsigned char data)
        {
            this->dataUChar1 = data;
        }

        void setDataUChar2(unsigned char data)
        {
            this->dataUChar2 = data;
        }

        void setDataUChar3(unsigned char data)
        {
            this->dataUChar3 = data;
        }

        long getDataLong1()
        {
            return dataLong1;
        }

        long getDataLong2()
        {
            return dataLong2;
        }

        long getDataLong3()
        {
            return dataLong3;
        }

        long getDataLong4()
        {
            return dataLong4;
        }

        void setDataLong1(long data)
        {
            this->dataLong1 = data;
        }

        void setDataLong2(long data)
        {
            this->dataLong2 = data;
        }

        void setDataLong3(long data)
        {
            this->dataLong3 = data;
        }

        void setDataLong4(long data)
        {
            this->dataLong4 = data;
        }

        char getDataChar1()
        {
            return dataChar1;
        }

        char getDataChar2()
        {
            return dataChar2;
        }

        char getDataChar3()
        {
            return dataChar3;
        }

        void setDataChar1(char data)
        {
            this->dataChar1 = data;
        }

        void setDataChar2(char data)
        {
            this->dataChar2 = data;
        }

        void setDataChar3(char data)
        {
            this->dataChar3 = data;
        }

    protected:

        /**
         * Identifier
         */
        unsigned long long id;

        /**
         * Packet date
         */
        unsigned long date;

        /**
         * Data long containers
         */
        long dataLong1;
        long dataLong2;
        long dataLong3;
        long dataLong4;

        /**
         * Data byte containers
         */
        unsigned char dataUChar1;
        unsigned char dataUChar2;
        unsigned char dataUChar3;

        /**
         * Data char containers
         */
        char dataChar1;
        char dataChar2;
        char dataChar3;

        /**
         * Sensor type identifier
         */
        unsigned char sourceType;

        /**
         * Sensor identifier (uniq for all automation park)
         */
        unsigned char sourceIdentifier;

        /**
         * Command identifier
         */
        unsigned char command;

        /**
         * Target of request
         */
        unsigned char target;
    };
}

#endif //HOMECENTER_SERVER_ENTITY_PACKETQUEUE_H
