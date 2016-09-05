//
// Created by Thibault PLET on 05/09/2016.
//

#ifndef HOMECENTER_SERVER_DATABASEPARAMETERS_H
#define HOMECENTER_SERVER_DATABASEPARAMETERS_H

class DatabaseParameters
{
public:
    /**
     * Constructor
     */
    DatabaseParameters(const char *host, const char *user, const char *passwd, const char *db,
                       unsigned int port = 3306, const char *unix_socket = NULL, unsigned long client_flag = 0)
    {
        this->host = host;
        this->user = user;
        this->passwd = passwd;
        this->db = db;
        this->port = port;
        this->unix_socket = unix_socket;
        this->client_flag = client_flag;
    }

    const char * getHost() { return this->host; }
    const char * getUser() { return this->user; }
    const char * getPasswd() { return this->passwd; }
    const char * getDb() { return this->db; }
    unsigned int getPort() { return this->port; }
    const char * getUnixSocket() { return this->unix_socket; }
    unsigned long getClientFlag() { return this->client_flag; }

protected:

    const char *host = "127.0.0.1";
    const char *user;
    const char *passwd;
    const char *db;
    unsigned int port = 3306;
    const char *unix_socket = NULL;
    unsigned long client_flag = 0;
};


#endif //HOMECENTER_SERVER_DATABASEPARAMETERS_H
