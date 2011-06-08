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

#include <rtt/types/TransportPlugin.hpp>
#include <rtt/types/TypekitPlugin.hpp>

#include "YarpTransport.hpp"
#include "YarpChannelElement.hpp"
#include "YarpTemplateProtocol.hpp"

struct YarpLibPlugin : public RTT::types::TransportPlugin {

	bool registerTransport(std::string name, RTT::types::TypeInfo* ti) {
		if ( name == "array" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol< std::vector<double> >() );
		if ( name == "bool" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<bool>() );
		//if ( name == "bools" )
		//	return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<std::vector<bool> >() );
		if ( name == "char" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<char>() );
		if ( name == "double" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<double>() );
		if ( name == "float" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<float>() );
		if ( name == "int" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<int>() );
		if ( name == "ints" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<std::vector<int> >() );
		if ( name == "string" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<std::string>() );
		if ( name == "strings" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<std::vector<std::string> >() );
		if ( name == "uint" )
			return ti->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<unsigned int>() );
		return false;
	}
      
	std::string getTransportName() const {
		return "yarp";
	}
      
	std::string getTypekitName() const {
		return "rtt-types";
	}

	std::string getName() const {
		return "yarp-transport";
	}

};

yarp::os::Network yarpNet;
	
ORO_TYPEKIT_PLUGIN(YarpLibPlugin)
