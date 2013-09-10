/*
 * Yarp-service.cpp
 *
 *  Created on: 10 sept. 2013
 *      Author: lesire
 */

#include <iostream>

#include <rtt/Service.hpp>
#include <rtt/ConnPolicy.hpp>
#include <rtt/plugin/ServicePlugin.hpp>

#include "../transport/YarpTransport.hpp"

using namespace RTT;

/**
 * A service that offers YARP stream creation operations.
 */
class YarpService : public Service
{
public:
	YarpService(TaskContext* c) : Service("yarp", c) {
		this->addOperation("topic", &YarpService::topic, this)
			.doc("create a ConnPolicy configured with a YARP topic");
		this->addOperation("add_topic", &YarpService::add_topic, this)
			.doc("configure an existing ConnPolicy for YARP");
	}

	ConnPolicy topic(const std::string& topic) {
		ConnPolicy conn = ConnPolicy::data();
		return add_topic(conn, topic);
	}

	ConnPolicy& add_topic(ConnPolicy& conn, const std::string& topic) {
		conn.transport = ORO_YARP_PROTOCOL_ID;
		conn.name_id = topic;
		return conn;
	}
};

ORO_SERVICE_NAMED_PLUGIN(YarpService, "yarp")
