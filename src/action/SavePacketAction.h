//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_SAVEPACKETACTION_H
#define HOMECENTER_SERVER_ACTION_SAVEPACKETACTION_H

#include <com/osteres/automation/action/Action.h>
#include <com/osteres/automation/transmission/packet/Packet.h>
#include <service/DatabaseManager.h>
#include <string>
#include <iostream>

using std::string;
using com::osteres::automation::action::Action;
using com::osteres::automation::transmission::packet::Packet;
using service::DatabaseManager;

namespace action
{
    class SavePacketAction : public Action {
    public:

        /**
         * Constructor
         */
        SavePacketAction(DatabaseManager * databaseManager)
        {
            this->serviceDatabaseManager = databaseManager;
        }

        /**
         * Execute action
         *
         * Save packet to database
         */
        bool execute(Packet * packet)
        {
            string query = "INSERT INTO `packet` ";
            query += "(`date`, `source_type`, `source_identifier`, `command`, `target`, `last`, `data_long1`, `data_long2`, `data_long3`, `data_long4`, `data_uchar1`, `data_uchar2`, `data_uchar3`, `data_char1`, `data_char2`, `data_char3`) ";
            query += "VALUES('" +
                     to_string(packet->getDate()) + "', '" +
                     to_string(packet->getSourceType()) + "', '" + to_string(packet->getSourceIdentifier()) + "', '" +
                     to_string(packet->getCommand()) + "', '" +
                     to_string(packet->getTarget()) + "', '" + (packet->isLast() ? "1" : "0") + "', '" +
                     to_string(packet->getDataLong1()) + "', '" + to_string(packet->getDataLong2()) + "', '" +
                     to_string(packet->getDataLong3()) + "', '" + to_string(packet->getDataLong4()) + "', '" +
                     to_string(packet->getDataUChar1()) + "', '" + to_string(packet->getDataUChar2()) + "', '" +
                     to_string(packet->getDataUChar3()) + "', '" +
                     to_string(packet->getDataChar1()) + "', '" + to_string(packet->getDataChar2()) + "', '" +
                     to_string(packet->getDataChar3()) + "')";

            // Check connect
            this->serviceDatabaseManager->checkConnect();
            this->serviceDatabaseManager->insert(query);

            this->executed = true;
            return true;
        }

    protected:

        /**
         * Database manager
         */
        DatabaseManager * serviceDatabaseManager = NULL;
    };
}

#endif //HOMECENTER_SERVER_ACTION_SAVEPACKETACTION_H
