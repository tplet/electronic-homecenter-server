//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H
#define HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H

#include <com/osteres/automation/action/ActionManagerBase.h>
#include <com/osteres/automation/action/implement/ConsoleDisplayPacket.h>
#include <string>
#include <iostream>
#include <mysql.h>

using std::string;
using com::osteres::automation::action::ActionManagerBase;
using com::osteres::automation::action::implement::ConsoleDisplayPacket;

namespace action {
    class ReceiverActionManager : public ActionManagerBase {
    public:

        /**
         * Constructor
         */
        ReceiverActionManager(MYSQL *db) {
            this->actionDisplay = new ConsoleDisplayPacket();
            this->db = db;
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
            query += "(`date`, `sensor`, `command`, `target`, `last`, `data_long1`, `data_long2`, `data_long3`, `data_long4`, `data_uchar1`, `data_uchar2`, `data_uchar3`, `data_uchar4`, `data_char1`, `data_char2`, `data_char3`) ";
            query += "VALUES('" +
                     to_string(packet->getDate()) + "', '" +
                     to_string(packet->getSensor()) + "', '" + to_string(packet->getCommand()) + "', '" +
                     to_string(packet->getTarget()) + "', '" + (packet->isLast() ? "1" : "0") + "', '" +
                     to_string(packet->getDataLong1()) + "', '" + to_string(packet->getDataLong2()) + "', '" +
                     to_string(packet->getDataLong3()) + "', '" + to_string(packet->getDataLong4()) + "', '" +
                     to_string(packet->getDataUChar1()) + "', '" + to_string(packet->getDataUChar2()) + "', '" +
                     to_string(packet->getDataUChar3()) + "', '" + to_string(packet->getDataUChar4()) + "', '" +
                     to_string(packet->getDataChar1()) + "', '" + to_string(packet->getDataChar2()) + "', '" +
                     to_string(packet->getDataChar3()) + "')";

            // Insert packet into database
            if (mysql_query(this->db, query.c_str()))
            {
                printf("MySQL query error : %s\n", mysql_error(this->db));
            }

            // Check insert
            if (mysql_affected_rows(this->db) == 0) {
                cout << "Error: Packet not correctly inserted into database!" << endl;
            }
        }

    protected:

        /**
         * Action to output packet to console
         * TODO: To remove in production? Or edit to print into log
         */
        ConsoleDisplayPacket *actionDisplay = NULL;

        /**
         * Db instance
         */
        MYSQL *db = NULL;
    };
}

#endif //HOMECENTER_SERVER_ACTION_RECEIVERACTIONMANAGER_H
