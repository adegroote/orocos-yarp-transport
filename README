                       Orocos RTT Yarp transport
                       =========================

Author: Chales Lesire (charles.lesire@onera.fr)
Date: 2011-01-27

Table of Contents
=================
1 Overview 
2 Setup 
    2.1 Compilation 
    2.2 Runtime 
3 Transporting simple data with Yarp
	 3.1 Using the deployer
4 Links 

1 Overview 
~~~~~~~~~~~

  The Yarp transport provides a new way to connect Orocos data ports.
  When creating a stream or connecting ports using Yarp as the connection transport, this plugin open a port on the Yarp server.
  If a connection name is specified, it is used as a Yarp topic, hence allows to automatically connect open ports.

2 Setup 
~~~~~~~~

2.1 Compilation 
================

  Requires Yarp 2.3

2.2 Runtime 
============

  The transport library is installed to your Orocos install path, in orocos/types folder.
  Hence it will be automatically loaded despite you have modified your RTT_COMPONENT_PATH.

3 Transporting simple data with Yarp
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Before trying to transport data with the Yarp transport, a Yarp name server must be active.
  When compiling Yarp 2.3, you have the possibility to build the yarpserver3. It will allow you to use topics to connect ports, 
  otherwise the connection must be done by hand.
  
  Launch the Yarp name server ($ yarpserver3 or $ yarpserver if you have not installed yarpserver3).
  You can check that the name server is available by using command $ yarp check
  If you do not succeed in reaching the server, please go to the Yarp wiki/tutorials [Yarp website].

3.1 Using the deployer
======================

  In the deployer, you can specify to use the Yarp transport by setting a connection policy transport to 89.
  If you specify a name_id for the connection policy, it will be used as a Yarp topic (only working with yarpserver3).
  
  ConnPolicy yarp_pol
  yarp_pol.transport = 89
  yarp_pol.name_id =  YarpTopic
  
  Then connecting ports using this policy will result in creating associated Yarp ports, and connecting them to the topic.
  Default connection is tcp.
  Following the Yarp documentation, you can specifiy, outside the deployer, the connection support (tcp, udp, shared memory, ros...)
  
  The testing/helloworld_yarp.xml file is a simple deployment file allowing to test this behaviour.
  While the Yarp server is active, launch the test file with:
  $ deployer-gnulinux -s helloworld_yarp.xml -linfo
  
  Then Hello.the_results and World.the_buffer_port are streamed to Yarp.
  If you use yarpserver3, they are directly connected via the YarpString topic.
  Else, you have to open a new terminal and write:
  $ yarp connect <hello port name> <world port name>
  to connect the ports.
  
  Then you can play with Hello and World, for instance by launching the test program from the deployer:
  > test.start

4 Links 
~~~~~~~~
  [Yarp website]: http://eris.liralab.it/yarp/
  