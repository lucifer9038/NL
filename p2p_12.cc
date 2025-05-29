/*.*-Mode:C++;c-file-style:"gnu";indent-tabs-mode:nil;-*-*/

//add required headers file

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

//adding namespace
using namespace ns3;

//define log component where messages will be saved
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

//Main Function
int main (int argc,char *argv[])
{
	CommandLine cmd (__FILE__); //read command line argument
	cmd.Parse (argc,argv); //process command line argument
	
	//Set Time Resolution to 1 nanaosecond
	Time::SetResolution (Time::NS);
	
	//Enable Logging for applications
	LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
	
	//create nodes
	NodeContainer nodes; // create object of NodeConatiner class
	nodes.Create(2);
	
	//create and configure  netDevices and channel
	PointToPointHelper pointToPoint; //create object of PointToPointHelper class
	
	//configuring the attributes of netDevice an channel
	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute("Delay", StringValue ("2ms"));
	
	//install the netDevices on nodes and connect them to channel
	NetDeviceContainer devices;
	devices=pointToPoint.Install (nodes);
	
	//Installing Protocol stack on nodes
	InternetStackHelper stack;
	stack.Install (nodes);
	
	//configuring IP Address
	Ipv4AddressHelper address; // create object of Ipv4Helper class
	address.SetBase("10.0.0.0","255.0.0.0");
	
	//create a interfaces of netdevices and assign Ipaddress
	Ipv4InterfaceContainer interfaces = address.Assign (devices);
	
	//configure a server application
	UdpEchoServerHelper echoServer(9); //port address
	
	//install server application on node one
	ApplicationContainer serverApps = echoServer.Install (nodes.Get(1));
	
	//configuring start and stop time of server application
	serverApps.Start(Seconds(1.0));
	serverApps.Stop(Seconds(10.0));
	
	//configuring the clinet application
	UdpEchoClientHelper echoClient (interfaces.GetAddress (1),9);
	
	//Configuring Client applications attributes
	echoClient.SetAttribute ("MaxPackets",UintegerValue (1));
	echoClient.SetAttribute ("Interval",TimeValue (Seconds(1)));
	echoClient.SetAttribute ("PacketSize",UintegerValue (1024));
	
	
	//install client application on nodes 0
	ApplicationContainer clientApps=echoClient.Install(nodes.Get(0));
	
	//configuring start and stop time of Client application
	clientApps.Start(Seconds(2.0));
	clientApps.Stop(Seconds(10.0));
	
	
	//animate p2p topology
	AnimationInterface anim("p2P_animation.xml");
	
	//set postion of nodes in bus topology
	anim.SetConstantPosition(nodes.Get(0),10.0,15.0);
	anim.SetConstantPosition(nodes.Get(1),30.0,15.0);
	
	//to run the simulation
	Simulator::Run ();
	Simulator::Destroy ();
	
	return 0;
}
