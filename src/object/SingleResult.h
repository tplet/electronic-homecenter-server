//
// Created by Thibault PLET on 02/10/2016.
//

#ifndef HOMECENTER_SERVER_OBJECT_RESULT_H
#define HOMECENTER_SERVER_OBJECT_RESULT_H

#include <mysql.h>

namespace object
{
    class SingleResult {
    public:
        /**
         * Constructor
         */
        SingleResult()
        {
        }

        /**
         * Set result
         */
        void setResult(bool result)
        {
            this->result = result;
        }

        /**
         * Set row
         */
        void setRow(MYSQL_ROW row)
        {
            this->row = row;
        }

        /**
         * Flag to indicate if result really exists
         */
        bool hasResult()
        {
            return this->result;
        }

        /**
         * Get row
         */
        MYSQL_ROW getRow()
        {
            return this->row;
        }

    protected:
        /**
         * Flag to indicate if result really exists
         */
        bool result;

        /**
         * Row associated
         */
        MYSQL_ROW row;
    };
}

#endif //HOMECENTER_SERVER_OBJECT_RESULT_H
