//
// Created by Thibault PLET on 01/10/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_DATABASEMANAGER_H
#define HOMECENTER_SERVER_SERVICE_DATABASEMANAGER_H

#include <mysql.h>
#include <string>
#include <DatabaseParameters.h>

using std::string;

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
         * Insert query
         * Return number of rows affected
         */
        unsigned long long insert(string query)
        {
            unsigned long long affected = 0;

            if (this->isConnected()) {
                // @see http://dev.mysql.com/doc/refman/5.7/en/mysql-query.html
                if (mysql_query(this->db, query.c_str())) {
                    printf("MySQL query error : %s\n", mysql_error(this->db));
                }

                affected = mysql_affected_rows(this->db);

                // Check insert
                if (affected == 0) {
                    printf("Error: No data correctly inserted into database!");
                }
            }

            return affected;
        }

    protected:

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
    };
}

#endif //HOMECENTER_SERVER_SERVICE_DATABASEMANAGER_H
