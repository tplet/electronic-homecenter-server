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
    HomecenterApplication(RF24 * radio)
    {
        this->initialize(radio);
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

protected:

    /**
     * Initialization (constructor)
     */
    void initialize(RF24 * radio)
    {
        // Create and configure transmitter (as master)
        this->transmitter = new Transmitter(radio, Identity::MASTER, true);

        // Service receiver
        this->receiverActionManager = new ReceiverActionManager();
        this->serviceReceiver = new ServiceReceiver(this->transmitter, this->receiverActionManager);
    }

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

};

#endif //HOMECENTER_SERVER_HOMECENTERAPPLICATION_H
