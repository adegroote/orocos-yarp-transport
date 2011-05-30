/**
 * Copyright (C) ONERA 2010
 * Version: 1.0
 * Author: Charles Lesire <charles.lesire@onera.fr>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _YARP_BOTTLE_TRANSPORTER_HPP_
#define _YARP_BOTTLE_TRANSPORTER_HPP_

#include "yarp_bottle_archive.hpp"
#include <rtt/types/TypeTransporter.hpp>
#include <rtt/Port.hpp>
#include <rtt/TaskContext.hpp>
#include <yarp/os/all.h>

template<typename T>
class YarpChannelElement: public RTT::base::ChannelElement<T>, public yarp::os::TypedReaderCallback<yarp::os::Bottle>
{
	char hostname[1024];
	yarp::os::BufferedPort<yarp::os::Bottle> yarp_port;
	bool newdata, send;
	yarp::os::Bottle m_b;
public:
	YarpChannelElement(RTT::base::PortInterface* port, const RTT::ConnPolicy& policy, bool is_sender) :
		newdata(false), send(is_sender)
	{
		// Check Network connection
		if (! yarpNet.checkNetwork()) {
			std::stringstream error;
			error << "Yarp network not available. You could check your Yarp configuration with:\n";
			error << "\t'yarp check' to check whether a yarp name server is reachable\n";
			error << "\t'yarp detect' to detect a yarp name server on the network\n";
			throw std::runtime_error(error.str());
		}
		// Create the Yarp port name
		std::stringstream namestr;
		gethostname(hostname, sizeof(hostname));
		namestr << '/' << hostname << '/' << port->getInterface()->getOwner()->getName() << '/' << port->getName();
		// Open the Yarp port
		if (! yarp_port.open(namestr.str().c_str())) {
			std::stringstream error;
			error << "Unable to open Yarp port " << namestr.str() << ". You could check your Yarp configuration with:\n";
			error << "\t'yarp check' to check whether a yarp name server is reachable\n";
			error << "\t'yarp detect' to detect a yarp name server on the network\n";
			throw std::runtime_error(error.str());
		}
		// Attach callback if receiver
		if (! is_sender)
			yarp_port.useCallback(*this);
		// Connect the port to the topic
		if (! policy.name_id.empty()) {
			std::stringstream topic;
			topic << "topic://" << policy.name_id;
			if (is_sender) yarpNet.connect(yarp_port.getName(), topic.str().c_str());
			else yarpNet.connect(topic.str().c_str(), yarp_port.getName());
		}
	}

	~YarpChannelElement() {}
	virtual bool inputReady() { return true; }
	virtual bool data_sample(typename RTT::base::ChannelElement<T>::param_t sample) { return true; }

	bool signal()
	{
		typename RTT::internal::ValueDataSource<T>::shared_ptr sample;
		if (send) {
			// this read should always succeed since signal() means 'data available in a data element'.
        	typename RTT::base::ChannelElement<T>::shared_ptr input =
				boost::static_pointer_cast< RTT::base::ChannelElement<T> >(this->getInput());
			if (send && input->read(sample->set(), false) == RTT::NewData)
				return this->write(sample->rvalue());
		}
		else {
			typename RTT::base::ChannelElement<T>::shared_ptr output = 
				boost::static_pointer_cast< RTT::base::ChannelElement<T> >(this->getOutput());
			if (this->read(sample->set(), false) == RTT::NewData && output)
				return output->write(sample->rvalue());
		}
		return false;
	}
    
	bool write(typename RTT::base::ChannelElement<T>::param_t sample)
	{
		yarp::os::Bottle& m_b = yarp_port.prepare();
		m_b.clear();
		yarp_bottle_oarchive arch( m_b );
		try {
			arch << sample;
		}
		catch (boost::archive::archive_exception e) {
			RTT::log(RTT::Error) << e.what() << RTT::endlog();
			return false;
		}
		yarp_port.write();
		return true;
	}  

	virtual void onRead(yarp::os::Bottle& b){
		m_b = b;
		newdata = true;
		this->signal();
	}

	RTT::FlowStatus read(typename RTT::base::ChannelElement<T>::reference_t sample, bool copy_old_date)
	{
		try {
			yarp_bottle_iarchive arch(m_b);
			arch >> sample;
			if (newdata) {
				newdata = false;
				return RTT::NewData;
			}
			return RTT::OldData;
		} 
		catch (boost::archive::archive_exception e) {
			RTT::log(RTT::Error) << e.what() << RTT::endlog();
		}
		catch (...) {}
		return RTT::NoData;
	}

};

#endif
