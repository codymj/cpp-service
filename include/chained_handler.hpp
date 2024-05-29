#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

using Poco::Net::HTTPRequestHandler;

/**
 * ChainedHandler extends Poco::Net::HTTPRequestHandler but allows for chaining
 * handlers together for implementing middleware.
 */
class ChainedHandler
: public HTTPRequestHandler
{
    /**
     * For setting next handler in the chain.
     * @param handler
     */
    virtual void setNextHandler(ChainedHandler* handler) = 0;
};
