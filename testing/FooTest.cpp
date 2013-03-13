#include <rtt/types/StructTypeInfo.hpp>
#include <rtt/TaskContext.hpp>
#include <rtt/Port.hpp>
#include <rtt/ConnPolicy.hpp>
#include <rtt/os/main.h>

#include "Foo.hpp"
#include "../YarpTemplateProtocol.hpp"
#include "../YarpTransport.hpp"

using namespace RTT;
using namespace RTT::types;

int ORO_main(int argc, char** argv)
{
	// Add your type to the Orocos type system (see: Writing plugins). Same as simple case.
	Types()->addType( new StructTypeInfo<Foo>("Foo") );
	// New: Install the Serialization template protocol for your data type.
	Types()->type("Foo")->addProtocol(ORO_YARP_PROTOCOL_ID, new YarpTemplateProtocol<Foo>() );

	TaskContext tc("FooTask");
	InputPort<Foo> in_foo("in_foo"); tc.addPort(in_foo);
	OutputPort<Foo> out_foo("out_foo"); tc.addPort(out_foo);
	ConnPolicy yarp_foopol = ConnPolicy::data();
	yarp_foopol.transport = ORO_YARP_PROTOCOL_ID;
	yarp_foopol.name_id = "FooTopic";

	assert(out_foo.connectTo(&in_foo, yarp_foopol));

	assert(tc.start());

	// Wait for Yarp connection to be effective
	sleep(3);
	
	Foo f; f.i = 1; f.d = 2.3; f.s = "Foo"; f.v = std::vector<double>(4, 5.6);
	out_foo.write(f);
	
	// Wait for Yarp to carry the data
	sleep(1);

	Foo b;
	assert(in_foo.read(b) == NewData);
	assert(f.i == b.i);
	assert(f.d == b.d);
	assert(f.s == b.s);
	assert(f.v.size() == b.v.size());
	for (int k = 0; k < f.v.size(); k++) assert(f.v[k] == b.v[k]);
	
	return 0;
}
