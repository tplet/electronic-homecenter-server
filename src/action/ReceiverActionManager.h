//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H
#define HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H

#include <com/osteres/automation/action/ActionManagerBase.h>
#include <com/osteres/automation/action/implement/ConsoleDisplayPacket.h>
#include <service/DatabaseManager.h>
#include <string>
#include <iostream>

using std::string;
using com::osteres::automation::action::ActionManagerBase;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;
using service::DatabaseManager;

namespace action {
    class ReceiverActionManager : public ActionManagerBase {
    public:

        /**
         * Constructor
         */
        ReceiverActionManager(DatabaseManager * databaseManager)
        {
            this->actionDisplay = new ConsoleDisplayPacket();
            this->serviceDatabaseManager = databaseManager;
        }

        /**
         * Destructor
         */
        ~ReceiverActionManager() {
            // Remove action display
            if (this->actionDisplay != NULL) {
                delete this->actionDisplay;
                this->actionDisplay = NULL;
            }
        }

        /**
         * Process packet
         */
        void processPacket(Packet *packet) {
            // Output packet to console (debug version) TODO: To remove in production
            this->actionDisplay->execute(packet);

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
        }

    protected:

        /**
         * Action to output packet to console
         * TODO: To remove in production? Or edit to print into log
         */
        ConsoleDisplayPacket *actionDisplay = NULL;

        /**
         * Service database manager
         */
        DatabaseManager * serviceDatabaseManager = NULL;
    };
}

#endif //HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H
