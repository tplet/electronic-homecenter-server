//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_HOMECENTERAPPLICATION_H
#define HOMECENTER_SERVER_HOMECENTERAPPLICATION_H

#include <com/osteres/automation/Application.h>
#include <com/osteres/automation/proxy/RF24.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/sensor/Identity.h>
#include <service/Receiver.h>
#include <action/ReceiverActionManager.h>
#include <DatabaseParameters.h>
#include <mysql.h>

using com::osteres::automation::Application;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::sensor::Identity;
using ServiceReceiver = service::Receiver;
using action::ReceiverActionManager;

class HomecenterApplication : public Application
{
public:

    /**
     * Constructor
     */
    HomecenterApplication(RF24 * radio, MYSQL * db, DatabaseParameters * parameters)
    {
        this->initialize(radio, db, parameters);
    }

    /**
     * Destructor
     */
    ~HomecenterApplication()
    {
        // Remove transmitter
        if (this->transmitter != NULL) {
            delete this->transmitter;
            this->transmitter = NULL;
        }
        // Remove receiver action manager
        if (this->receiverActionManager != NULL) {
            delete this->receiverActionManager;
            this->receiverActionManager = NULL;
        }
        // Remove service receiver
        if (this->serviceReceiver != NULL) {
            delete this->serviceReceiver;
            this->serviceReceiver = NULL;
        }
    }

    /**
     * Setup application
     */
    void setup()
    {
        // Setup transmitter
        this->transmitter->setup();
    }

    /**
     * Process
     */
    void process()
    {
        /*
         * Step 1: Receive
         * Listen radio transmission, to process packet send by sensor
         */
        // Configure maximum waiting time to receive packet
        this->serviceReceiver->getTransmitter()->getReceiver()->setTimeout(3000); // 3 s
        // Execute service (listen)
        this->serviceReceiver->execute();

        /*
         * Step 2: Send
         * Read packet queue to send to sensor and send packet one by one
         */
        // TODO
    }

    /**
     * Flag to indicate if application is ready to process
     */
    bool isReady()
    {
        return this->ready;
    }

    /**
     * Close connection to database
     */
    void closeDatabase()
    {
        if (this->db != NULL) {
            mysql_close(this->db);
        }
    }

protected:

    /**
     * Initialization (constructor)
     */
    void initialize(RF24 * radio, MYSQL * db, DatabaseParameters * parameters)
    {
        // Db
        this->db = db;
        mysql_init(this->db);
        mysql_options(this->db, MYSQL_READ_DEFAULT_GROUP, "option");
        if(!mysql_real_connect(this->db, parameters->getHost(), parameters->getUser(), parameters->getPasswd(),
                              parameters->getDb(), parameters->getPort(), parameters->getUnixSocket(),
                              parameters->getClientFlag()))
        {
            printf("Connection error : %s\n", mysql_error(this->db));
            this->db = NULL;
        }

        // Create and configure transmitter (as master)
        this->transmitter = new Transmitter(radio, Identity::MASTER, true);

        // Service receiver
        this->receiverActionManager = new ReceiverActionManager(this->db);
        this->serviceReceiver = new ServiceReceiver(this->transmitter, this->receiverActionManager);

        // Ready flag
        this->ready = this->db != NULL;
    }

    /**
     * Flag to indicate if application is ready to process
     */
    bool ready = false;

    /**
     * Radio transmitter
     */
    Transmitter * transmitter = NULL;

    /**
     * Service receiver
     */
    ServiceReceiver * serviceReceiver = NULL;

    /**
     * Action manager for receiver
     */
    ReceiverActionManager * receiverActionManager = NULL;

    /**
     * Database connection instance
     */
    MYSQL * db = NULL;

};

#endif //HOMECENTER_SERVER_HOMECENTERAPPLICATION_H
