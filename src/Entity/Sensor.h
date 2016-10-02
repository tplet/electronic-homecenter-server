//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_ENTITY_SENSOR_H
#define HOMECENTER_SERVER_ENTITY_SENSOR_H

#include <string>

using std::string;

namespace entity
{
    class Sensor {
    public:

        /**
         * Constructor
         */
        Sensor(unsigned long long id)
        {
            this->id = id;
        }

        /**
         * Constructor
         */
        Sensor()
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
         * Get uid
         */
        unsigned char getUid()
        {
            return this->uid;
        }

        /**
         * Set uid
         */
        void setUid(unsigned char uid)
        {
            this->uid = uid;
        }

        /**
         * Get type
         */
        unsigned char getType()
        {
            return this->type;
        }

        /**
         * Set type
         */
        void setType(unsigned char type)
        {
            this->type = type;
        }

        /**
         * Get name
         */
        string getName()
        {
            return this->name;
        }

        /**
         * Set name
         */
        void setName(string name)
        {
            this->name = name;
        }

    protected:
        unsigned long long id;
        unsigned char uid;
        unsigned char type;
        string name;
    };
}

#endif //HOMECENTER_SERVER_ENTITY_SENSOR_H
