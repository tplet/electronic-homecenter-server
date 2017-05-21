//
// Created by Thibault PLET on 01/10/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_DATABASEMANAGER_H
#define HOMECENTER_SERVER_SERVICE_DATABASEMANAGER_H

#include <mysql.h>
#include <string>
#include <vector>
#include <iostream>
#include <DatabaseParameters.h>
#include <object/SingleResult.h>

using std::string;
using std::vector;
using object::SingleResult;

namespace service {
    class DatabaseManager {
    public:

        /**
         * Constructor
         */
        DatabaseManager(DatabaseParameters * parameters)
        {
            this->parameters = parameters;
        }

        /**
         * Destructor
         */
        ~DatabaseManager()
        {
            // Disconnect from db
            this->disconnect();
        }

        /**
         * Flag to indicate if connection to database is active
         */
        bool isConnected()
        {
            return this->connected;
        }

        /**
         * Check db connect and reconnect if needed
         */
        bool checkConnect()
        {
            if (!this->isConnected()) {
                this->connect();
            }

            return this->isConnected();
        }

        /**
         * Connect to db
         *
         * Return bool True if connection successfull, false otherwise
         */
        bool connect()
        {
            // Ensure that no connected to optional previous db object
            this->disconnect();

            DatabaseParameters * parameters = this->parameters;

            this->db = new MYSQL();
            mysql_init(this->db);
            mysql_options(this->db, MYSQL_READ_DEFAULT_GROUP, "option");
            this->connected = mysql_real_connect(
                this->db,
                parameters->getHost(),
                parameters->getUser(),
                parameters->getPasswd(),
                parameters->getDb(),
                parameters->getPort(),
                parameters->getUnixSocket(),
                parameters->getClientFlag()
            ) != NULL;

            if(!connected) {
                printf("Connection error : %s\n", mysql_error(this->db));
                delete this->db;
                this->db = NULL;
            }

            return this->connected;
        }

        /**
         * Disconnect database
         */
        void disconnect()
        {
            if (this->db != NULL) {
                mysql_close(this->db);

                delete this->db;
                this->db = NULL;
            }

            this->connected = false;
        }

        /**
         * Flag to indicate if auto-connect is allowed when needed
         */
        bool isAutoConnect()
        {
            return this->autoConnect;
        }

        /**
         * Set flag for auto-connect
         */
        void setAutoConnect(bool autoConnect)
        {
            this->autoConnect = autoConnect;
        }

        /**
         * Insert query
         * Return primary value of inserted row
         */
        unsigned long long insert(string query)
        {
            unsigned long long id = 0;

            // Try to reconnect if needed
            if (!this->isConnected() && this->isAutoConnect()) {
                this->connect();
            }

            if (this->isConnected() && this->query(query)) {
                id = mysql_insert_id(this->db);

                // Check insert
                if (id == 0) {
                    printf("Error: No data correctly inserted into database!");
                }
            }

            return id;
        }

        /**
         * Update query
         *
         * @return bool True if success, false otherwise
         */
        bool update(string query)
        {
            // Try to reconnect if needed
            if (!this->isConnected() && this->isAutoConnect()) {
                this->connect();
            }

            return this->isConnected() && this->query(query);
        }

        /**
         * Select multiple
         */
        vector<MYSQL_ROW> select(string query)
        {
            vector<MYSQL_ROW> list;

            // Try to reconnect if needed
            if (!this->isConnected() && this->isAutoConnect()) {
                this->connect();
            }

            if (this->isConnected() && this->query(query)) {

                MYSQL_ROW row;
                MYSQL_RES * results = mysql_store_result(this->db);
                unsigned long long count = mysql_num_rows(results);

                list.reserve(count);
                while ((row = mysql_fetch_row(results))) {
                    list.push_back(row);
                }

                mysql_free_result(results);

                return list;
            }

            return list;
        }

        /**
         * Select single
         */
        SingleResult selectOne(string query)
        {
            // Try to reconnect if needed
            if (!this->isConnected() && this->isAutoConnect()) {
                this->connect();
            }

            SingleResult result;
            if (this->isConnected() && this->query(query)) {

                MYSQL_RES * results = mysql_store_result(this->db);
                unsigned long long count = mysql_num_rows(results);
                result.setResult(count > 0);

                if (result.hasResult()) {
                    result.setRow(mysql_fetch_row(results));
                }

                mysql_free_result(results);
            }

            return result;
        }

    protected:
        /**
         * Query
         *
         * @return bool True if success, false otherwise
         */
        bool query(string query)
        {
            // @see http://dev.mysql.com/doc/refman/5.7/en/mysql-query.html
            bool success = !mysql_query(this->db, query.c_str());
            if (!success) {
                printf("MySQL query error : %s\n", mysql_error(this->db));
            }

            return success;
        }

        /**
         * Parameters for database connection
         */
        DatabaseParameters * parameters = NULL;

        /**
         * Db instance
         */
        MYSQL *db = NULL;

        /**
         * Flag to indicate if connection to database is active
         */
        bool connected = false;

        /**
         * Flag to allow auto-connect or re-connect when needed
         */
        bool autoConnect = true;
    };
}

#endif //HOMECENTER_SERVER_SERVICE_DATABASEMANAGER_H
