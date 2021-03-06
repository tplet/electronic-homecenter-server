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
#include <service/RepositoryContainer.h>
#include <service/QueueService.h>
#include <DatabaseParameters.h>
#include <iostream>
#include <string>

using com::osteres::automation::Application;
using com::osteres::automation::transmission::Transmitter;
using com::osteres::automation::sensor::Identity;
using action::ReceiverAction;
using service::DatabaseManager;
using service::ReceiverActionManager;
using service::RepositoryContainer;
using service::QueueService;
using com::osteres::automation::memory::Property;
using std::string;

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

        // Remove service repository container
        if (this->serviceRepositoryContainer != NULL) {
            delete this->serviceRepositoryContainer;
            this->serviceRepositoryContainer = NULL;
        }

        // Remove service queue
        if (this->serviceQueue != NULL) {
            delete this->serviceQueue;
            this->serviceQueue = NULL;
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
         * Step 1: Get all packet to send
         * and push to transmitter queue. They will be send during process
         */
        this->serviceQueue->sendEnqueuedPacket();

        /*
         * Step 2: Launch Receive - Send - Receive process
         * Listen radio transmission, to process packet send by sensor
         * Send queued packet (from db and generated during receiving step)
         * Listen again for response (OK confirmation for example)
         */
        this->transmitter->rsr(3000); // 3s for receiving
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
        // Property identifier
        this->propertyIdentifier = new Property<unsigned char>();
        this->propertyIdentifier->set(Identity::MASTER);

        // Create and configure transmitter (as master)
        this->transmitter = new Transmitter(radio, true);
        this->transmitter->setPropertySensorType(this->propertyType);
        this->transmitter->setPropertySensorIdentifier(this->propertyIdentifier);

        // Service
        this->serviceRepositoryContainer = new RepositoryContainer(this->serviceDatabaseManager);
        this->serviceReceiverActionManager = new ReceiverActionManager(this->serviceRepositoryContainer, this->transmitter);
        this->serviceQueue = new QueueService(this->serviceRepositoryContainer, this->transmitter);

        // Receiver service
        //this->receiverAction = new ReceiverAction(this->transmitter, this->serviceReceiverActionManager);
        this->transmitter->setActionManager(this->serviceReceiverActionManager);

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
     * Queue service
     */
    QueueService * serviceQueue = NULL;

    /**
     * Property for application (sensor) type
     */
    Property<unsigned char> * propertyType = NULL;

    /**
     * Property for application (sensor) identifier
     */
    Property<unsigned char> * propertyIdentifier = NULL;

    /**
     * Service repository container
     */
    RepositoryContainer * serviceRepositoryContainer = NULL;

};

#endif //HOMECENTER_SERVER_HOMECENTERAPPLICATION_H
