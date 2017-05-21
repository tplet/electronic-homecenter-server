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
#include <com/osteres/automation/action/implement/ConsoleDisplayPacket.h>

using service::RepositoryContainer;
using com::osteres::automation::transmission::Transmitter;
using service::repository::PacketQueueRepository;
using std::vector;
using entity::PacketQueue;
using service::converter::PacketConverter;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;

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
            this->actionDisplay = new ConsoleDisplayPacket();
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

            // Remove action display
            if (this->actionDisplay != NULL) {
                delete this->actionDisplay;
                this->actionDisplay = NULL;
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

            if (list.size() > 0) {
                Packet * packet = NULL;
                // For each packet queue found
                vector<unsigned long long> ids;
                for (auto &packetQueue : list) {
                    // Convert PacketQueue to Packet
                    packet = this->getPacketConverter()->convert(packetQueue);

                    // Add to transmitter for send
                    this->getTransmitter()->add(packet);

                    // Console display
                    this->getActionDisplay()->execute(packet, false);

                    // Push PacketQueue id to remove after
                    ids.push_back(packetQueue->getId());

                    // Free memory
                    delete packetQueue;
                }

                // Remove packet queue sended
                packetQueueRepository->deleteById(ids);
            }
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

        /**
         * Get action to output packet to console
         */
        ConsoleDisplayPacket * getActionDisplay()
        {
            return this->actionDisplay;
        }

    protected:

        /**
         * Action to output packet to console
         * TODO: To remove in production? Or edit to print into log
         */
        ConsoleDisplayPacket * actionDisplay = NULL;

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
