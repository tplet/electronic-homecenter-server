#include <iostream>
#include <HomecenterApplication.h>
#include <DatabaseParameters.h>
#include <com/osteres/automation/proxy/RF24.h>
#include <mysql.h>

#ifdef RF24_MOCK
    #define RPI_BPLUS_GPIO_J8_22 25
    #define RPI_BPLUS_GPIO_J8_24 8
#endif


using namespace std;

// Radio transmitter
RF24 radio(RPI_BPLUS_GPIO_J8_22, RPI_BPLUS_GPIO_J8_24);
// MySQL
MYSQL db;
DatabaseParameters parameters("127.0.0.1", "root", "root", "domotique_homecenter", 8889);
// Application
HomecenterApplication application(&radio, &db, &parameters);

int main() {

    // Check if application is ready
    if (!application.isReady()) {
        cout << "Application initialize failed! Please check configuration (like database credentials access)" << endl;
    }

    cout << "Start server..." << endl;

    // Setup
    application.setup();

    cout << "Server started." << endl;

    // Process
    while (true) {
        cout << "." << flush;
        application.process();
    }

    // Close db connection
    application.closeDatabase();

    return 0;
}

/**
 ** Description du fonctionnement du serveur
 *
 * Opérations effectuées en boucle par le serveur:
 * - Emission: lecture des requêtes en attente dans la base de données. Ces requêtes sont à envoyer
 * - Réception: Ecoute des requêtes en provenance des clients (capteurs)
 *
 ** Emission:
 *
 * Le serveur fonctionnera en mode asynchrone. Les requêtes seront générées par la partie web et consignée
 * dans la base de données.
 * Ce mode de fonctionnement permettra de faciliter l'écriture du serveur en se limitant à la maîtrise de la
 * lecture/écriture avec la base de données. Une autre solution, plus complexe, consisterai à écouter sur un port
 * des requêtes qui proviendrait de la partie web. Cette solution semble plus intéréssante car elle permet d'envoyer
 * une requête en temps réel.
 * Cependant, les clients (les capteurs) ne sont pas continuellement à l'écoute du serveur (par soucis d'économie
 * d'énergie par exemple) et par conséquent, on ne peut garantir que la requête sera bien envoyée et traitée par le
 * client en quelques secondes.
 *
 * D'où le mode de fonctionnement asynchrone. La requête est préparée, mise en attente, et une réponse sera apportée
 * lorsqu'elle aura été traitée (c'est à dire, envoyée au client, et que le client aura apporté une réponse).
 *
 * L'inconvéniant du fonctionnement asynchrone est qu'il ne permet pas une exécution très rapide des requêtes.
 * La requête peux être traitée en quelques secondes, comme cela peux être fait en plusieurs minutes (selon le mode de
 * fonctionnement du client et de la charge du réseau - s'il y a beaucoup d'autres requêtes à traiter).
 *
 * Côté web, on ne pourra donc que confirmer que la demande a bien été prise en compte, puis demander à interval
 * régulier si la demande a bien été traitée (idée: état à consigner sur la requête ?).
 *
 *
 ** Réception:
 *
 * La réception en continue du serveur (excepté lors de la vérification de requête à émettre) permet de rester à
 * l'écoute des informations et/ou demandes envoyées par les clients.
 * Les données (DATA) transmisent par les clients sont ainsi consignées sur un serveur central (du fait notamment de
 * l'impossibilité de stocker l'historique sur les clients - il s'agit la plupart du temps d'Arduino, donc avec peu
 *  de mémoire de stockage).
 *
 *  De plus, les clients peuvent demander des informations de configuration au serveur, tel que l'heure courante pour
 *  se réajuste (1 fois par jour par exemple).
 *
 *
 ** Répartition entre émission/réception:
 *
 * Elle suit un cycle régulier (que l'on pourra rendre intelligent ultérieurement en s'adaptant à la fréquence
 * d'émission de requête).
 *
 * Les émissions se veulent plus rare que les réceptions. Par conséquant, on peut établir le cycle suivant:
 * - reception, reception, reception, emission
 * On obtient un cycle en 4 temps, avec 3 temps de réception et 1 temps d'émission.
 *
 * Un temps de réception, sans requête reçue est estimé à 1 seconde.
 * Un temps d'émission, sans requête à envoyer est estimé à moins d'une seconde (= vérification de la pile de requête
 * en base)
 *
 * Par conséquent, le serveur sera quasiment en permanence entre train d'écouter des requêtes.
 * Les clients doivent cependant, et systématiquement, intégrer un mécanisme de relance d'une requête (soit la même,
 * soit une requête actualisée) dans le cas où elle n'aurait pas pu être réceptionnée par le serveur (problème de
 * connexion, interférence ou bien parce que le serveur serait dans une phase d'émission).
 *
 ** Multi-tâche:
 *
 * A étudier.
 * Permettrait de travailler en mode émission ET réception en même temps.
 * Attention cependant, le module de transmission RF24 ne peut pas lire et écrire en même temps.
 * - Soit on place un verou permettant d'attendre la fin du cycle de réception pour émettre des requêtes s'il en
 *   existent
 * - Soit on utilise 2 modules RF24, 1 en lecture et 1 en écriture.
 */