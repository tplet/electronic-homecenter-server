//
// Created by Thibault PLET on 04/10/2016.
//

#ifndef HOMECENTER_SERVER_MEMORY_RTIMEDPROPERTY_H
#define HOMECENTER_SERVER_MEMORY_RTIMEDPROPERTY_H

#include <com/osteres/automation/memory/TimedProperty.h>
#include <ctime>

using com::osteres::automation::memory::TimedProperty;
using std::time;

namespace memory
{
    template<typename T>
    class RTimedProperty : public TimedProperty<T>
    {
    public:
        /**
         * Constructor
         */
        RTimedProperty(unsigned long duration) : TimedProperty<T>(duration)
        {
            this->initialize();
        }

        /**
         * Constructor
         */
        RTimedProperty(unsigned long duration, T initValue) : TimedProperty<T>(duration, initValue)
        {
            this->initialize();
        }

        /**
         * Flag to indicate if property is outdated
         */
        bool isOutdated()
        {
            return this->getTimestamp() - this->dateCreated > this->getDuration();
        }


    protected:
        /**
         * Common constructor
         */
        void initialize()
        {
            this->dateCreated = this->getTimestamp();

        }

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
         * Timestamp when property has been created
         */
        long int dateCreated;
    };
}

#endif //HOMECENTER_SERVER_MEMORY_RTIMEDPROPERTY_H
