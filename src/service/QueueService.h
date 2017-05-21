//
// Created by Thibault PLET on 21/05/2017.
//

#ifndef HOMECENTER_SERVER_SERVICE_QUEUESERVICE_H
#define HOMECENTER_SERVER_SERVICE_QUEUESERVICE_H

#include <service/RepositoryContainer.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <service/repository/PacketQueueRepository.h>
#include <vector>
#include <entity/PacketQueue.h>
#include <service/converter/PacketConverter.h>

using service::RepositoryContainer;
using com::osteres::automation::transmission::Transmitter;
using service::repository::PacketQueueRepository;
using std::vector;
using entity::PacketQueue;
using service::converter::PacketConverter;

namespace service
{
    class QueueService
    {
    public:
        /**
         * Constructor
         */
        QueueService(RepositoryContainer * repositoryContainer, Transmitter * transmitter)
        {
            this->repositoryContainer = repositoryContainer;
            this->transmitter = transmitter;

            this->packetConverter = new PacketConverter();
        }

        /**
         * Destructor
         */
        ~QueueService()
        {
            // Packet converter service
            if (this->packetConverter != NULL) {
                delete this->packetConverter;
                this->packetConverter = NULL;
            }
        }

        /**
         * Send packet enqueued
         */
        void sendEnqueuedPacket()
        {
            PacketQueueRepository * packetQueueRepository = this->getRepositoryContainer()->getRepositoryPacketQueue();

            // Get list of packet to send
            vector<PacketQueue *> list = packetQueueRepository->fetchAll();

            // For each packet queue found
            vector<unsigned long long> ids;
            for (auto & packetQueue : list) {
                // Add to transmitter
                this->getTransmitter()->add(this->getPacketConverter()->convert(packetQueue));

                ids.push_back(packetQueue->getId());

                // Free memory
                delete packetQueue;
            }

            // Remove packet queue sended
            packetQueueRepository->deleteById(ids);
        }

        /**
         * Get service repository container
         */
        RepositoryContainer * getRepositoryContainer()
        {
            return this->repositoryContainer;
        }

        /**
         * Get transmitter
         */
        Transmitter * getTransmitter()
        {
            return this->transmitter;
        }

        /**
         * Get packet converter service
         */
        PacketConverter * getPacketConverter()
        {
            return this->packetConverter;
        }

    protected:

        /**
         * Service repository container
         */
        RepositoryContainer * repositoryContainer = NULL;

        /**
         * Transmitter
         */
        Transmitter * transmitter = NULL;

        /**
         * Packet converter service
         */
        PacketConverter * packetConverter = NULL;
    };
}

#endif //HOMECENTER_SERVER_SERVICE_QUEUESERVICE_H
