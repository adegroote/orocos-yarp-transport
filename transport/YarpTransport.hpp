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

#ifndef YARP_TRANSPORT_H
#define YARP_TRANSPORT_H

/** 
 * Yarp Protocol ID.
 * To ensure a unique ID, as best as possible, the ID correspond to the ASCII code corresponding to 'Y'.
 * Hence the Yarp Protocol ID is 89.
 */
#define ORO_YARP_PROTOCOL_ID 89

#include <yarp/os/all.h>

extern yarp::os::Network yarpNet;

#endif