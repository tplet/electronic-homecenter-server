//
// Created by Thibault PLET on 04/10/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_MANAGER_IDENTIFIERMANAGER_H
#define HOMECENTER_SERVER_SERVICE_MANAGER_IDENTIFIERMANAGER_H

#define PROPERTY_DURATION 10000 // milliseconds

#include <service/RepositoryContainer.h>
#include <vector>
#include <object/IdentifierVector.h>
#include <memory/RTimedProperty.h>
#include <entity/Sensor.h>

using service::RepositoryContainer;
using std::vector;
using object::IdentifierVector;
using memory::RTimedProperty;
using entity::Sensor;

namespace service
{
    namespace manager
    {
        class IdentifierManager
        {
        public:
            /**
             * Constructor
             */
            IdentifierManager(RepositoryContainer * repositoryContainer)
            {
                this->serviceRepositoryContainer = repositoryContainer;
            }

            /**
             * Destructor
             */
            ~IdentifierManager()
            {
                // Remove identifier vector
                if (this->identifierVector != NULL) {
                    delete this->identifierVector;
                    this->identifierVector = NULL;
                }
            }

            /**
             * Find sensor by uid
             *
             * Don't forget to delete sensor object if no used anymore (if not null)
             */
            Sensor * getByUid(unsigned char uid)
            {
                return this->serviceRepositoryContainer->getRepositorySensor()->findByUid(uid);
            }

            /**
             * Generate new sensor object and hydrate properties
             *
             * Don't forget to delete sensor object if no used anymore (if not null)
             */
            Sensor * create(unsigned char type, unsigned char uid)
            {
                return this->serviceRepositoryContainer->getRepositorySensor()->createFromTypeAndUid(type, uid);
            }

            /**
             * Get first free uid
             */
            unsigned char generateFreeUid()
            {
                SensorRepository * repositorySensor = this->serviceRepositoryContainer->getRepositorySensor();

                //
                // Search next uid
                //
                // Merge array of uid: from db and temporary generated
                vector<unsigned char> identifiers;
                vector<unsigned char> fromDb = repositorySensor->getUidUsed();
                identifiers.reserve(fromDb.size() + this->getIdentifierVector()->size());
                identifiers.insert(identifiers.end(), fromDb.begin(), fromDb.end());
                for (IdentifierVector::iterator it = this->getIdentifierVector()->begin(); it != this->getIdentifierVector()->end(); it++) {
                    identifiers.push_back(it->get());
                }
                // Prepare vars
                unsigned char uid = 0;
                unsigned char limit = 255;
                unsigned char i = 0;
                bool free = false;
                long index;
                while (!free && i++ < limit) {
                    index = find(identifiers.begin(), identifiers.end(), ++uid) - identifiers.begin();
                    // Available!
                    if (index >= identifiers.size()) {
                        free = true;
                    }
                }
                identifiers.clear();
                fromDb.clear();

                return uid;
            }

            /**
             * Get first free uid and save temporary
             */
            unsigned char generateAndStoreFreeUid()
            {
                unsigned char uid = this->generateFreeUid();

                // Create TimedProperty to store uid temporary
                RTimedProperty<unsigned char> timedProperty(PROPERTY_DURATION, uid);
                this->getIdentifierVector()->push_back(timedProperty);

                return uid;
            }

            /**
             * Get identifier vector
             */
            IdentifierVector * getIdentifierVector()
            {
                if (this->identifierVector == NULL) {
                    this->identifierVector = new IdentifierVector();
                }

                return this->identifierVector;
            }

        protected:
            /**
             * Service repository container
             */
            RepositoryContainer * serviceRepositoryContainer = NULL;

            /**
             * Identifier vector
             */
            IdentifierVector * identifierVector = NULL;
        };
    }
}

#endif //HOMECENTER_SERVER_SERVICE_MANAGER_IDENTIFIERMANAGER_H
