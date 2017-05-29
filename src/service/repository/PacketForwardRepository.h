//
// Created by Thibault PLET on 29/05/2017.
//

#ifndef HOMECENTER_SERVER_SERVICE_REPOSITORY_PACKETFORWARDREPOSITORY_H
#define HOMECENTER_SERVER_SERVICE_REPOSITORY_PACKETFORWARDREPOSITORY_H

#include <string>
#include <iostream>
#include <mysql.h>
#include <stdlib.h>
#include <object/SingleResult.h>
#include <vector>
#include <service/AbstractRepository.h>
#include <entity/PacketForward.h>

using std::string;
using object::SingleResult;
using std::vector;
using service::AbstractRepository;
using entity::PacketForward;

namespace service
{
    namespace repository
    {
        class PacketForwardRepository : public AbstractRepository
        {
        public:
            /**
             * Constructor
             */
            PacketForwardRepository(DatabaseManager * databaseManager) : AbstractRepository(databaseManager)
            {
            }

            /**
             * Fetch packet forward by id
             *
             * Don't forget to delete packet queue object if no used anymore (if not null)
             */
            PacketForward * fetchOne(unsigned long long id)
            {
                string query = "SELECT " + this->getSelectPart() + " FROM `packet_forward` WHERE `id` = " + to_string(id) + " LIMIT 1";
                SingleResult result = this->serviceDatabaseManager->selectOne(query);

                PacketForward * packetForward = NULL;
                if (result.hasResult()) {
                    packetForward = new PacketForward();
                    this->hydrate(packetForward, result.getRow());
                }

                return packetForward;
            }

            /**
             * Fetch all packet forward
             *
             * Don't forget to delete packet queue object if no used anymore (if not null)
             */
            vector<PacketForward *> fetchAll()
            {
                vector<PacketForward *> list;

                // Query
                string query = "SELECT " + this->getSelectPart() + " FROM `packet_forward` ORDER BY `id_sensor_source` ASC";
                vector<MYSQL_ROW> rows = this->serviceDatabaseManager->select(query);

                // Hydrate list
                PacketForward * packetForward;
                for (auto & row : rows) {

                    packetForward = new PacketForward();
                    this->hydrate(packetForward, row);

                    list.push_back(packetForward);
                }

                return list;
            }

            /**
             * Fetch packet forward list by source
             *
             * Don't forget to delete result object if no used anymore (if not null)
             */
            vector<PacketForward *> fetchBySource(unsigned long long sensorSource)
            {
                vector<PacketForward *> list;

                string query = "SELECT " + this->getSelectPart() + " FROM `packet_forward` WHERE `id_sensor_source` = " + to_string(sensorSource);
                vector<MYSQL_ROW> rows = this->serviceDatabaseManager->select(query);

                // Hydrate list
                PacketForward * packetForward;
                for (auto & row : rows) {

                    packetForward = new PacketForward();
                    this->hydrate(packetForward, row);

                    list.push_back(packetForward);
                }

                return list;
            }

            /**
             * Fetch packet forward list by source and command
             *
             * Don't forget to delete result object if no used anymore (if not null)
             */
            vector<PacketForward *> fetchBySourceAndCommand(unsigned long long sensorSource, unsigned char command)
            {
                vector<PacketForward *> list;

                string query = "SELECT " + this->getSelectPart() + " FROM `packet_forward` WHERE ";
                query += "`id_sensor_source` = " + to_string(sensorSource) + " ";
                query += "AND `commandFrom` = '" + to_string(command) + "'";
                vector<MYSQL_ROW> rows = this->serviceDatabaseManager->select(query);

                // Hydrate list
                PacketForward * packetForward;
                for (auto & row : rows) {

                    packetForward = new PacketForward();
                    this->hydrate(packetForward, row);

                    list.push_back(packetForward);
                }

                return list;
            }

            /**
             * Save packet forward
             */
            void save(PacketForward * packetForward)
            {
                if (packetForward != NULL) {

                    // Insert
                    if (packetForward->getId() == 0) {

                        string query = "INSERT INTO `packet_forward` ";
                        query += "(" + this->getInsertPart() + ") ";
                        query += "VALUES('" + packetForward->getType() + "', '" + to_string(packetForward->getCommandFrom())
                            + "', '" + to_string(packetForward->getCommandTo()) + "', '" + to_string(packetForward->getSensorSource())
                            + "', '" + to_string(packetForward->getSensorTarget()) + "')";

                        // Insert
                        unsigned long long id = this->serviceDatabaseManager->insert(query);

                        // Set new identifier
                        packetForward->setId(id);
                    }
                    // Update
                    else {

                        string query = "UPDATE `packet_forward` SET ";
                        query += "`type` = '" + packetForward->getType() + "', ";
                        query += "`commandFrom` = '" + to_string(packetForward->getCommandFrom()) + "', ";
                        query += "`commandTo` = '" + to_string(packetForward->getCommandTo()) + "', ";
                        query += "`id_sensor_source` = '" + to_string(packetForward->getSensorSource()) + "', ";
                        query += "`id_sensor_target` = '" + to_string(packetForward->getSensorTarget()) + "' ";
                        query += "WHERE `id` = " + packetForward->getId();

                        this->serviceDatabaseManager->update(query);
                    }
                }
            }

        protected:

            /**
             * Hydrate entity with data
             */
            PacketForward * hydrate(PacketForward * packetForward, MYSQL_ROW row)
            {
                if (packetForward != NULL) {
                    packetForward->setId(static_cast<unsigned long long>(atoll(row[0])));
                    packetForward->setType(row[1]);
                    packetForward->setCommandFrom(static_cast<unsigned char>(atoll(row[2])));
                    packetForward->setCommandTo(static_cast<unsigned char>(atoll(row[3])));
                    packetForward->setSensorSource(static_cast<unsigned long long>(atoll(row[4])));
                    packetForward->setSensorTarget(static_cast<unsigned long long>(atoll(row[5])));
                }

                return packetForward;
            }

            /**
             * Get select part
             */
            string getSelectPart()
            {
                return "`id`, `type`, `commandFrom`, `commandTo`, `id_sensor_source`, `id_sensor_target`";
            }

            /**
             * Get insert part
             */
            string getInsertPart()
            {
                return "`type`, `commandFrom`, `commandTo`, `id_sensor_source`, `id_sensor_target`";
            }

        };
    }
}


#endif //HOMECENTER_SERVER_SERVICE_REPOSITORY_PACKETFORWARDREPOSITORY_H
