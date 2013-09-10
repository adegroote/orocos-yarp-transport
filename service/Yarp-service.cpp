/*
 * Yarp-service.cpp
 *
 *  Created on: 10 sept. 2013
 *      Author: lesire
 */

#include <rtt/Service.hpp>
#include <rtt/plugin/ServicePlugin.hpp>
#include <iostream>

/**
 * A service that offers YARP stream creation operations.
 */
class YarpService : public RTT::Service
{
public:
	YarpService(RTT::TaskContext* c)
	: RTT::Service("yarp", c) {
		this->addOperation("topic", &topic, this);
	}

	void topic() {
		std::cout << "Hello World !" << std::endl;
	}
};

ORO_SERVICE_NAMED_PLUGIN(YarpService, "yarp")
