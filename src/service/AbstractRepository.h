//
// Created by Thibault PLET on 04/10/2016.
//

#ifndef HOMECENTER_SERVER_SERVICE_ABSTRACTREPOSITORY_H
#define HOMECENTER_SERVER_SERVICE_ABSTRACTREPOSITORY_H

#include <service/DatabaseManager.h>

using service::DatabaseManager;

namespace service
{
    class AbstractRepository
    {
    public:
        /**
         * Constructor
         */
        AbstractRepository(DatabaseManager * databaseManager)
        {
            this->serviceDatabaseManager = databaseManager;
        }

        /**
         * Get service database manager
         */
        DatabaseManager * getServiceDatabaseManager()
        {
            return this->serviceDatabaseManager;
        }

    protected:

        /**
         * Service database manager
         */
        DatabaseManager * serviceDatabaseManager = NULL;
    };
}

#endif //HOMECENTER_SERVER_SERVICE_ABSTRACTREPOSITORY_H
