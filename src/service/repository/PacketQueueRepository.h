//
// Created by Thibault PLET on 21/05/2017.
//

#ifndef HOMECENTER_SERVER_SERVICE_REPOSITORY_PACKETQUEUEREPOSITORY_H
#define HOMECENTER_SERVER_SERVICE_REPOSITORY_PACKETQUEUEREPOSITORY_H

#include <string>
#include <iostream>
#include <mysql.h>
#include <stdlib.h>
#include <object/SingleResult.h>
#include <vector>
#include <service/AbstractRepository.h>
#include <entity/PacketQueue.h>

using std::string;
using object::SingleResult;
using std::vector;
using service::AbstractRepository;
using entity::PacketQueue;

namespace service
{
    namespace repository
    {
        class PacketQueueRepository : public AbstractRepository
        {
        public:
            /**
             * Constructor
             */
            PacketQueueRepository(DatabaseManager * databaseManager) : AbstractRepository(databaseManager)
            {
            }

            /**
             * Fetch packet queue by id
             *
             * Don't forget to delete packet queue object if no used anymore (if not null)
             */
            PacketQueue * fetchOne(unsigned long long id)
            {
                string query = "SELECT " + this->getSelectPart() + " FROM `packet_queue` WHERE `id` = " + to_string(id) + " LIMIT 1";
                SingleResult result = this->serviceDatabaseManager->selectOne(query);

                PacketQueue * packetQueue = NULL;
                if (result.hasResult()) {
                    packetQueue = new PacketQueue();
                    this->hydrate(packetQueue, result.getRow());
                }

                return packetQueue;
            }

            /**
             * Fetch all packet queue
             *
             * Don't forget to delete packet queue object if no used anymore (if not null)
             */
            vector<PacketQueue *> fetchAll()
            {
                vector<PacketQueue *> list;

                // Query
                string query = "SELECT " + this->getSelectPart() + " FROM `packet_queue` ORDER BY `target` ASC";
                vector<MYSQL_ROW> rows = this->serviceDatabaseManager->select(query);

                // Hydrate list
                PacketQueue * packetQueue;
                for (auto & row : rows) {

                    packetQueue = new PacketQueue();
                    this->hydrate(packetQueue, row);

                    list.push_back(packetQueue);
                }

                return list;
            }

            /**
             * Fetch packet queue list by target
             *
             * Don't forget to delete sensor object if no used anymore (if not null)
             */
            vector<PacketQueue *> fetchByTarget(unsigned char target)
            {
                vector<PacketQueue *> list;

                string query = "SELECT " + this->getSelectPart() + " FROM `packet_queue` WHERE `target` = " + to_string(target);
                vector<MYSQL_ROW> rows = this->serviceDatabaseManager->select(query);

                // Hydrate list
                PacketQueue * packetQueue;
                for (auto & row : rows) {

                    packetQueue = new PacketQueue();
                    this->hydrate(packetQueue, row);

                    list.push_back(packetQueue);
                }

                return list;
            }

            /**
             * Save packet queue
             */
            void save(PacketQueue * packetQueue)
            {
                if (packetQueue != NULL) {

                    // Insert
                    if (packetQueue->getId() == 0) {

                        string query = "INSERT INTO `packet_queue` ";
                        query += "(" + this->getInsertPart() + ") ";
                        query += "VALUES('" + to_string(packetQueue->getDate()) + "', '" + to_string(packetQueue->getSourceType())
                            + "', '" + to_string(packetQueue->getSourceIdentifier()) + "', '" + to_string(packetQueue->getCommand())
                            + "', '" + to_string(packetQueue->getTarget()) + "', '" + to_string(packetQueue->getDataLong1())
                            + "', '" + to_string(packetQueue->getDataLong2()) + "', '" + to_string(packetQueue->getDataLong3())
                            + "', '" + to_string(packetQueue->getDataLong4()) + "', '" + to_string(packetQueue->getDataUChar1())
                            + "', '" + to_string(packetQueue->getDataUChar2()) + "', '" + to_string(packetQueue->getDataUChar3())
                            + "', '" + to_string(packetQueue->getDataChar1()) + "', '" + to_string(packetQueue->getDataChar2())
                            + "', '" + to_string(packetQueue->getDataChar3()) + "')";

                        // Insert
                        unsigned long long id = this->serviceDatabaseManager->insert(query);

                        // Set new identifier
                        packetQueue->setId(id);
                    }
                    // Update
                    else {

                        string query = "UPDATE `packet_queue` SET ";
                        query += "`date` = '" + to_string(packetQueue->getDate()) + "', ";
                        query += "`source_type` = '" + to_string(packetQueue->getSourceType()) + "', ";
                        query += "`source_identifier` = '" + to_string(packetQueue->getSourceIdentifier()) + "', ";
                        query += "`command` = '" + to_string(packetQueue->getCommand()) + "', ";
                        query += "`target` = '" + to_string(packetQueue->getTarget()) + "', ";
                        query += "`data_long1` = '" + to_string(packetQueue->getDataLong1()) + "', ";
                        query += "`data_long2` = '" + to_string(packetQueue->getDataLong2()) + "', ";
                        query += "`data_long3` = '" + to_string(packetQueue->getDataLong3()) + "', ";
                        query += "`data_long4` = '" + to_string(packetQueue->getDataLong4()) + "', ";
                        query += "`data_uchar1` = '" + to_string(packetQueue->getDataUChar1()) + "', ";
                        query += "`data_uchar2` = '" + to_string(packetQueue->getDataUChar2()) + "', ";
                        query += "`data_uchar3` = '" + to_string(packetQueue->getDataUChar3()) + "', ";
                        query += "`data_char1` = '" + to_string(packetQueue->getDataChar1()) + "', ";
                        query += "`data_char2` = '" + to_string(packetQueue->getDataChar2()) + "', ";
                        query += "`data_char3` = '" + to_string(packetQueue->getDataChar3()) + "'";

                        this->serviceDatabaseManager->update(query);
                    }
                }
            }

        protected:

            /**
             * Hydrate entity with data
             */
            PacketQueue * hydrate(PacketQueue * packetQueue, MYSQL_ROW row)
            {
                if (packetQueue != NULL) {
                    packetQueue->setId(static_cast<unsigned long long>(atoll(row[0])));
                    packetQueue->setDate(static_cast<unsigned long>(atoll(row[1])));
                    packetQueue->setSourceType(static_cast<unsigned char>(atoll(row[2])));
                    packetQueue->setSourceIdentifier(static_cast<unsigned char>(atoll(row[3])));
                    packetQueue->setCommand(static_cast<unsigned char>(atoll(row[4])));
                    packetQueue->setTarget(static_cast<unsigned char>(atoll(row[5])));
                    packetQueue->setDataLong1(static_cast<long>(atoll(row[6])));
                    packetQueue->setDataLong2(static_cast<long>(atoll(row[7])));
                    packetQueue->setDataLong3(static_cast<long>(atoll(row[8])));
                    packetQueue->setDataLong4(static_cast<long>(atoll(row[9])));
                    packetQueue->setDataUChar1(static_cast<unsigned char>(atoll(row[10])));
                    packetQueue->setDataUChar2(static_cast<unsigned char>(atoll(row[11])));
                    packetQueue->setDataUChar3(static_cast<unsigned char>(atoll(row[12])));
                    packetQueue->setDataChar1(static_cast<char>(atoll(row[13])));
                    packetQueue->setDataChar1(static_cast<char>(atoll(row[14])));
                    packetQueue->setDataChar1(static_cast<char>(atoll(row[15])));
                }

                return packetQueue;
            }

            /**
             * Get select part
             */
            string getSelectPart()
            {
                return "`id`, `date`, `source_type`, `source_identifier`, `command`, `target`, `data_long1`, `data_long2`, `data_long3`, `data_long4`, `data_uchar1`, `data_uchar2`, `data_uchar3`, `data_char1`, `data_char2`, `data_char3`";
            }

            /**
             * Get insert part
             */
            string getInsertPart()
            {
                return "`date`, `source_type`, `source_identifier`, `command`, `target`, `data_long1`, `data_long2`, `data_long3`, `data_long4`, `data_uchar1`, `data_uchar2`, `data_uchar3`, `data_char1`, `data_char2`, `data_char3`";
            }

        };
    }
}


#endif //HOMECENTER_SERVER_SERVICE_REPOSITORY_PACKETQUEUEREPOSITORY_H
