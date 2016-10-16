//
// Created by Thibault PLET on 04/10/2016.
//

#ifndef HOMECENTER_SERVER_OBJECT_IDENTIFIERVECTOR_H
#define HOMECENTER_SERVER_OBJECT_IDENTIFIERVECTOR_H

#include <vector>
#include <com/osteres/automation/memory/TimedProperty.h>

using std::vector;
using com::osteres::automation::memory::TimedProperty;

namespace object
{
    class IdentifierVector : public vector<TimedProperty<unsigned char>>
    {
    public:
        /**
         * Make iterator available
         */
        using vector<TimedProperty<unsigned char>>::iterator;

        /**
         * Refresh vector by removing outdated item
         */
        void refresh()
        {
            for (IdentifierVector::iterator iterator = this->begin(); iterator != this->end(); ) {
                if (iterator->isOutdated()) {
                    iterator = this->erase(iterator);
                } else {
                    ++iterator;
                }
            }
        }

    protected:
    };
}

#endif //HOMECENTER_SERVER_OBJECT_IDENTIFIERVECTOR_H
