//
// Created by Thibault PLET on 21/08/2016.
//

#ifndef HOMECENTER_SERVER_HOMECENTERAPPLICATION_H
#define HOMECENTER_SERVER_HOMECENTERAPPLICATION_H

#include <com/osteres/automation/Application.h>
#include <com/osteres/automation/proxy/RF24.h>
#include <com/osteres/automation/transmission/Transmitter.h>
#include <com/osteres/automation/sensor/Identity.h>
#include <com/osteres/automation/memory/Property.h>
#include <action/ReceiverAction.h>
#include <service/DatabaseManager.h>
#include <service/ReceiverActionManager.h>
#include <DatabaseParameters.h>

using com::osteres::automation::Application;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::sensor::Identity;
using action::ReceiverAction;
using service::DatabaseManager;
using service::ReceiverActionManager;
using com::osteres::automation::memory::Property;

class HomecenterApplication : public Application
{
public:

    /**
     * Constructor
     */
    HomecenterApplication(RF24 * radio, DatabaseParameters * parameters)
    {
        this->initialize(radio, parameters);
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
        if (this->serviceReceiverActionManager != NULL) {
            delete this->serviceReceiverActionManager;
            this->serviceReceiverActionManager = NULL;
        }

        // Remove receiver action
        if (this->receiverAction != NULL) {
            delete this->receiverAction;
            this->receiverAction = NULL;
        }

        // Remove service database
        if (this->serviceDatabaseManager != NULL) {
            delete this->serviceDatabaseManager;
            this->serviceDatabaseManager = NULL;
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
        this->receiverAction->getTransmitter()->getReceiver()->setTimeout(3000); // 3 s
        // Execute service (listen)
        this->receiverAction->execute();

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

protected:

    /**
     * Initialization (constructor)
     */
    void initialize(RF24 * radio, DatabaseParameters * parameters)
    {
        // Db
        this->serviceDatabaseManager = new DatabaseManager(parameters);
        this->serviceDatabaseManager->connect();

        // Property type
        this->propertyType = new Property<unsigned char>();
        this->propertyType->set(Identity::MASTER);

        // Create and configure transmitter (as master)
        this->transmitter = new Transmitter(radio, true);
        this->transmitter->setPropertySensorType(this->propertyType);

        // Receiver service
        this->serviceReceiverActionManager = new ReceiverActionManager(this->serviceDatabaseManager);
        this->receiverAction = new ReceiverAction(this->transmitter, this->serviceReceiverActionManager);

        // Ready flag
        this->ready = this->serviceDatabaseManager->isConnected();
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
     * Receiver action
     */
    ReceiverAction * receiverAction = NULL;

    /**
     * Service database manager
     */
    DatabaseManager * serviceDatabaseManager = NULL;

    /**
     * Service action manager for receiver
     */
    ReceiverActionManager * serviceReceiverActionManager = NULL;

    /**
     * Property for application (sensor) type
     */
    Property<unsigned char> * propertyType = NULL;

};

#endif //HOMECENTER_SERVER_HOMECENTERAPPLICATION_H
