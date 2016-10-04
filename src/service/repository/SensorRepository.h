//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_REPOSITORY_SENSORREPOSITORY_H
#define HOMECENTER_SERVER_SERVICE_REPOSITORY_SENSORREPOSITORY_H

#include <service/AbstractRepository.h>
#include <entity/Sensor.h>
#include <string>
#include <iostream>
#include <mysql.h>
#include <stdlib.h>
#include <object/SingleResult.h>

using entity::Sensor;
using std::string;
using object::SingleResult;
using service::AbstractRepository;

namespace service
{
    namespace repository
    {
        class SensorRepository : public AbstractRepository
        {
        public:
            /**
             * Constructor
             */
            SensorRepository(DatabaseManager * databaseManager) : AbstractRepository(databaseManager)
            {
            }

            /**
             * Find sensor by id
             *
             * Don't forget to delete sensor object if no used anymore (if not null)
             */
            Sensor * find(unsigned long long id)
            {
                string query = "SELECT `id`, `uid`, `type`, `name` FROM `sensor` WHERE `id` = " + to_string(id) + " LIMIT 1";
                SingleResult result = this->serviceDatabaseManager->selectOne(query);

                Sensor * sensor = NULL;
                if (result.hasResult()) {
                    MYSQL_ROW row = result.getRow();

                    sensor = new Sensor();
                    sensor->setId(static_cast<unsigned long long>(atoll(row[0])));
                    sensor->setUid(static_cast<unsigned char>(atoi(row[1])));
                    sensor->setType(static_cast<unsigned char>(atoi(row[2])));
                    sensor->setName(row[3]);
                }

                return sensor;
            }

            /**
             * Find sensor by uid
             *
             * Don't forget to delete sensor object if no used anymore (if not null)
             */
            Sensor * findByUid(unsigned char uid)
            {
                string query = "SELECT `id`, `uid`, `type`, `name` FROM `sensor` WHERE `uid` = " + to_string(uid) + " LIMIT 1";
                SingleResult result = this->serviceDatabaseManager->selectOne(query);

                Sensor * sensor = NULL;
                if (result.hasResult()) {
                    MYSQL_ROW row = result.getRow();

                    sensor = new Sensor();
                    sensor->setId(static_cast<unsigned long long>(atoll(row[0])));
                    sensor->setUid(static_cast<unsigned char>(atoi(row[1])));
                    sensor->setType(static_cast<unsigned char>(atoi(row[2])));
                    sensor->setName(row[3]);
                }

                return sensor;
            }

            /**
             * Get next sensor uid
             *
             * @TODO: Refactor to get next id not used yet (max 255)
             */
            unsigned char getNextUid()
            {
                string query = "SELECT COUNT(*) FROM `sensor`";
                SingleResult result = this->serviceDatabaseManager->selectOne(query);

                unsigned char uid = 0;
                if (result.hasResult()) {
                    MYSQL_ROW row = result.getRow();
                    uid = static_cast<unsigned char>(atoi(row[0]));
                }
                uid++;

                return uid;
            }

            /**
             * Create new Sensor from sensor type
             */
            Sensor * createFromType(unsigned char type)
            {
                Sensor * sensor = NULL;

                string query = "INSERT INTO `sensor` ";
                query += "(`uid`, `type`, `name`) ";
                query += "VALUES('" + to_string(this->getNextUid()) + "', '" + to_string(type) +
                        "', 'Unknow sensor')";

                // Insert
                unsigned long long id = this->serviceDatabaseManager->insert(query);

                if (id != 0) {
                    // Get sensor
                    sensor = this->find(id);
                }

                return sensor;
            }

        protected:

        };
    }
}


#endif //HOMECENTER_SERVER_SERVICE_REPOSITORY_SENSORREPOSITORY_H
