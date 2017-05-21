//
// Created by Thibault PLET on 21/05/2017.
//

#ifndef HOMECENTER_SERVER_SERVICE_CONVERTER_PACKETCONVERTER_H
#define HOMECENTER_SERVER_SERVICE_CONVERTER_PACKETCONVERTER_H

#include <com/osteres/automation/transmission/packet/Packet.h>
#include <entity/PacketQueue.h>

using com::osteres::automation::transmission::packet::Packet;
using entity::PacketQueue;

namespace service
{
    namespace converter
    {
        class PacketConverter
        {
        public:
            /**
             * Constructor
             */
            PacketConverter()
            {

            }

            /**
             * Convert PacketQueue to Packet
             *
             * Don't forget to delete packet object if no used anymore (if not null)
             */
            Packet * convert(PacketQueue * packetQueue)
            {
                Packet * packet = new Packet();

                packet->setDate(packetQueue->getDate());
                packet->setSourceType(packetQueue->getSourceType());
                packet->setSourceIdentifier(packetQueue->getSourceIdentifier());
                packet->setCommand(packetQueue->getCommand());
                packet->setTarget(packetQueue->getTarget());
                packet->setDataLong1(packetQueue->getDataLong1());
                packet->setDataLong2(packetQueue->getDataLong2());
                packet->setDataLong3(packetQueue->getDataLong3());
                packet->setDataLong4(packetQueue->getDataLong4());
                packet->setDataUChar1(packetQueue->getDataUChar1());
                packet->setDataUChar2(packetQueue->getDataUChar2());
                packet->setDataUChar3(packetQueue->getDataUChar3());
                packet->setDataChar1(packetQueue->getDataChar1());
                packet->setDataChar2(packetQueue->getDataChar2());
                packet->setDataChar3(packetQueue->getDataChar3());

                return packet;
            }


        protected:

        };
    }
}

#endif //HOMECENTER_SERVER_SERVICE_CONVERTER_PACKETCONVERTER_H
