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

#ifndef ORO_YARP_TEMPATE_PROTOCOL_HPP
#define ORO_YARP_TEMPATE_PROTOCOL_HPP

#include "YarpTransport.hpp"
#include "YarpChannelElement.hpp"
#include <rtt/types/TypeTransporter.hpp>

template<class T>
class YarpTemplateProtocol : public RTT::types::TypeTransporter {
public:
	virtual RTT::base::ChannelElementBase::shared_ptr createStream(RTT::base::PortInterface* port, const RTT::ConnPolicy& policy, bool is_sender) const {
		try {
			RTT::base::ChannelElementBase* yarp = new YarpChannelElement<T>(port, policy, is_sender);
			if (!yarp) throw std::runtime_error("Yarp channel element is void :(");
			if (!is_sender) {
				// the receiver needs a buffer to store his messages in.
				RTT::base::ChannelElementBase::shared_ptr buf = RTT::detail::DataSourceTypeInfo<T>::getTypeInfo()->buildDataStorage(policy);
				yarp->setOutput(buf);
			}
			return yarp;
		} catch(std::exception& e) {
			log(RTT::Error) << "Failed to create Yarp Channel element: " << e.what() << RTT::endlog();
		}
		return 0;
	};
};

#endif
