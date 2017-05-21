//
// Created by Thibault PLET on 04/10/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_REPOSITORYCONTAINER_H
#define HOMECENTER_SERVER_SERVICE_REPOSITORYCONTAINER_H

#include <service/repository/SensorRepository.h>
#include <service/repository/PacketQueueRepository.h>
#include <service/DatabaseManager.h>

using service::repository::SensorRepository;
using service::repository::PacketQueueRepository;
using service::DatabaseManager;

namespace service
{
    class RepositoryContainer
    {
    public:
        /**
         * Constructor
         */
        RepositoryContainer(DatabaseManager * databaseManager)
        {
            this->serviceDatabaseManager = databaseManager;
        }

        /**
         * Destructor
         */
        ~RepositoryContainer()
        {
            // Sensor
            if (this->repositorySensor != NULL) {
                delete this->repositorySensor;
                this->repositorySensor = NULL;
            }
        }

        /**
         * Get service database manager
         */
        DatabaseManager * getServiceDatabaseManager()
        {
            return this->serviceDatabaseManager;
        }

        /**
         * Get repository sensor
         */
        SensorRepository * getRepositorySensor()
        {
            if (this->repositorySensor == NULL) {
                this->repositorySensor = new SensorRepository(this->serviceDatabaseManager);
            }

            return this->repositorySensor;
        }

        /**
         * Get repository packet queue
         */
        PacketQueueRepository * getRepositoryPacketQueue()
        {
            if (this->repositoryPacketQueue == NULL) {
                this->repositoryPacketQueue = new PacketQueueRepository(this->serviceDatabaseManager);
            }

            return this->repositoryPacketQueue;
        }

    protected:

        /**
         * Service database manager
         */
        DatabaseManager * serviceDatabaseManager = NULL;

        /**
         * Sensor repository
         */
        SensorRepository * repositorySensor = NULL;

        /**
         * Packet queue repository
         */
        PacketQueueRepository * repositoryPacketQueue = NULL;
    };
}

#endif //HOMECENTER_SERVER_SERVICE_REPOSITORYCONTAINER_H
