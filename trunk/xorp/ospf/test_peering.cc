// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

// Copyright (c) 2001-2004 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/ospf/test_peering.cc,v 1.46 2005/09/07 08:19:46 atanu Exp $"

#define DEBUG_LOGGING
#define DEBUG_PRINT_FUNCTION_NAME

#include "config.h"
#include <map>
#include <list>
#include <set>
#include <deque>

#include "ospf_module.h"
#include "libxorp/xorp.h"

#include "libxorp/test_main.hh"
#include "libxorp/debug.h"
#include "libxorp/xlog.h"
#include "libxorp/ipv4.hh"
#include "libxorp/ipv6.hh"
#include "libxorp/ipnet.hh"

#include "libxorp/status_codes.h"
#include "libxorp/eventloop.hh"

#include "ospf.hh"
#include "debug_io.hh"

// Make sure that all tests free up any memory that they use. This will
// allow us to use the leak checker program.

/**
 * Bind together a set of IO classes in order to form a virtual subnet
 * for testing, one instance per subnet.
 */
template <typename A>
class EmulateSubnet {
 public:
    EmulateSubnet(TestInfo& info, EventLoop& eventloop)
	: _info(info), _eventloop(eventloop), _queue_add(1), _queue_remove(2)
    {}

    /**
     * Receive frames
     *
     * All frames generated by an OSPF instances arrive here. Note
     * that a frame arriving from one OSPF instance is not sent
     * directly to another. The frames are queued and only when OSPF
     * instance gives back control to the eventloop are the frames
     * forwarded. This ensures that two OSPF instances are not in each
     * others call graphs, which can cause re-entrancy problems.
     */
    void
    receive_frames(const string& interface, const string& vif,
		   A dst, A src,
		   uint8_t* data, uint32_t len, const string instance) {
	DOUT(_info) << "receive(" << instance << "," <<
	    interface << "," << vif << "," 
		    << dst.str() << "," << src.str()
		    << "," << len
		    <<  "...)" << endl;
	
	_queue[_queue_add].
	    push_back(Frame(interface, vif, dst, src, data, len, instance));
	if (_timer.scheduled())
	    return;
	XLOG_ASSERT(_queue[_queue_add].size() == 1);
	 _timer = _eventloop.
	     new_oneoff_after_ms(10, callback(this, &EmulateSubnet::next));
    }

    /**
     * Bind together a set of interfaces.
     */
    void
    bind_interfaces(const string& instance,
		    const string& interface, const string& vif,
		    DebugIO<A>& io) {
	DOUT(_info) << instance << ": " << interface << "/" << vif << endl;
	
	io.register_forward(callback(this,
				     &EmulateSubnet<A>::receive_frames,
				     instance));

	_ios[Multiplex(instance, interface, vif)] = &io;
    }

 private:
    TestInfo& _info;
    EventLoop& _eventloop;
    struct Multiplex {
	Multiplex(const string& instance, const string& interface,
		  const string& vif)
	    : _instance(instance), _interface(interface), _vif(vif)
	{}
	bool
	operator <(const Multiplex& him) const {
	    return him._instance < _instance;
	}
	const string _instance;
	const string _interface;
	const string _vif;
    };

    map<const Multiplex, DebugIO<A> *> _ios;

    struct Frame {
	Frame(string interface, const string vif, A dst, A src,
	      uint8_t* data, uint32_t len, string instance)
	    : _interface(interface), _vif(vif), _dst(dst), _src(src),
	      _instance(instance) {
		_pkt.resize(len);
		memcpy(&_pkt[0], data, len);
	    }

	string _interface;
	string _vif;
	A _dst;
	A _src;
	vector<uint8_t> _pkt;
	string _instance;
    };

    XorpTimer _timer;
    deque<Frame> _queue[2];
    int _queue_add;
    int _queue_remove;
    
    void
    next() {
	if (0 == _queue_add) {
	    _queue_add = 1;
	    _queue_remove = 0;
	} else {
	    _queue_add = 0;
	    _queue_remove = 1;
	}
	while (!_queue[_queue_remove].empty()) {
	    Frame frame = _queue[_queue_remove].front();
	    _queue[_queue_remove].pop_front();
	    forward(frame);
	}
    }

    void
    forward(Frame frame) {
	uint8_t* data = &frame._pkt[0];
	uint32_t len = frame._pkt.size();

	typename map<const Multiplex, DebugIO<A> *>::iterator i;
	for(i = _ios.begin(); i != _ios.end(); i++) {
	    Multiplex m = (*i).first;
	    if (m._instance == frame._instance)
		continue;
	    DOUT(_info) << "Send to: " << m._instance << ": " <<
		m._interface << "/" << m._vif << " " <<	len << endl;
	    (*i).second->receive(m._interface, m._vif,
				 frame._dst, frame._src, data, len);
	}
	
    }
};

// Reduce the hello interval from 10 to 1 second to speed up the test.
uint16_t hello_interval = 1;

// Do not stop a tests allow it to run forever to observe timers.
bool forever = false;

/**
 * Configure a single peering. Nothing is really expected to go wrong
 * but the test is useful to verify the normal path through the code.
 */
template <typename A> 
bool
single_peer(TestInfo& info, OspfTypes::Version version)
{
    EventLoop eventloop;
    DebugIO<A> io(info, version, eventloop);
    io.startup();
    
    Ospf<A> ospf(version, eventloop, &io);
    ospf.set_router_id(set_id("0.0.0.1"));

    OspfTypes::AreaID area = set_id("128.16.64.16");
    const uint16_t interface_prefix_length = 16;
    const uint16_t interface_mtu = 1500;
    const uint16_t interface_cost = 10;
    const uint16_t inftransdelay = 2;

    PeerManager<A>& pm = ospf.get_peer_manager();

    // Create an area
    if (!pm.create_area_router(area, OspfTypes::NORMAL)) {
	DOUT(info) << "Failed to create area\n";
	return false;
    }

    // Create a peer associated with this area.
    const string interface = "eth0";
    const string vif = "vif0";

    A src;
    switch(src.ip_version()) {
    case 4:
	src = "192.150.187.78";
	break;
    case 6:
	src = "2001:468:e21:c800:220:edff:fe61:f033";
	break;
    default:
	XLOG_FATAL("Unknown IP version %d", src.ip_version());
	break;
    }

    PeerID peerid = pm.
	create_peer(interface, vif, src, interface_prefix_length,
		    interface_mtu,
		    OspfTypes::BROADCAST,
		    area);

    if (!ospf.set_hello_interval(interface, vif, area, hello_interval)) {
	DOUT(info) << "Failed to set hello interval\n";
	return false;
    }

    if (!ospf.set_router_dead_interval(interface, vif, area,
				       4 * hello_interval)) {
	DOUT(info) << "Failed to set router dead interval\n";
	return false;
    }

    if (!ospf.set_interface_cost(interface, vif, area, interface_cost)) {
	DOUT(info) << "Failed to set interface cost\n";
	return false;
    }

    if (!ospf.set_inftransdelay(interface, vif,area, inftransdelay)) {
	DOUT(info) << "Failed to set inftransdelay\n";
	return false;
    }

    // Bring the peering up
    if (!pm.set_state_peer(peerid, true)) {
	DOUT(info) << "Failed enable peer\n";
	return false;
    }

    if (forever)
	while (ospf.running())
	    eventloop.run();

    bool timeout = false;
    XorpTimer t = eventloop.set_flag_after(TimeVal(10 * hello_interval ,0),
					   &timeout);
    while (ospf.running() && !timeout) {
	eventloop.run();
	if (2 == io.packets())
	    break;
    }
    if (timeout) {
	DOUT(info) << "No packets sent, test timed out\n";
	return false;
    }

    // Take the peering down
    if (!pm.set_state_peer(peerid, false)) {
	DOUT(info) << "Failed to disable peer\n";
	return false;
    }

    // Delete the peer.
    if (!pm.delete_peer(peerid)) {
	DOUT(info) << "Failed to delete peer\n";
	return false;
    }

    // Delete the area
    if (!pm.destroy_area_router(area)) {
	DOUT(info) << "Failed to delete area\n";
	return false;
    }

    return true;
}

enum Stagger { NOSTAGGER, STAGGER1, STAGGER2};

string suppress;

/**
 * Configure two peerings. Nothing is really expected to go wrong
 * but the test is useful to verify the normal path through the code.
 */
template <typename A> 
bool
two_peers(TestInfo& info, OspfTypes::Version version, 
	  OspfTypes:: LinkType linktype,
	  Stagger stagger)
{
    EventLoop eventloop;

    bool verbose[2];
    verbose[0] = info.verbose();
    verbose[1] = info.verbose();

    if (suppress == "")
	;
    else if (suppress == "ospf1")
	verbose[0] = false;
    else if (suppress == "ospf2")
	verbose[1] = false;
    else {
	info.out() << "illegal value for suppress" << suppress << endl;
	return false;
    }
    
    TestInfo info1(info.test_name() + "(ospf1)" , verbose[0],
		   info.verbose_level(), info.out());
    TestInfo info2(info.test_name() + "(ospf2)" , verbose[1],
		   info.verbose_level(), info.out());

    DebugIO<A> io_1(info1, version, eventloop);
    io_1.startup();
    DebugIO<A> io_2(info2, version, eventloop);
    io_2.startup();
    
    Ospf<A> ospf_1(version, eventloop, &io_1);
    Ospf<A> ospf_2(version, eventloop, &io_2);

    ospf_1.set_router_id(set_id("192.150.187.1"));
    ospf_2.set_router_id(set_id("192.150.187.2"));

    const uint16_t interface_prefix_length = 16;
    const uint16_t interface_mtu = 1500;
    const uint16_t interface_cost = 10;
    const uint16_t inftransdelay = 20;

    OspfTypes::AreaID area = set_id("128.16.64.16");

    PeerManager<A>& pm_1 = ospf_1.get_peer_manager();
    PeerManager<A>& pm_2 = ospf_2.get_peer_manager();

    pm_1.create_area_router(area, OspfTypes::NORMAL);
    pm_2.create_area_router(area, OspfTypes::NORMAL);

    const string interface_1 = "eth1";
    const string interface_2 = "eth2";
    const string vif_1 = "vif1";
    const string vif_2 = "vif2";

    A src_1, src_2;
    switch(src_1.ip_version()) {
    case 4:
	src_1 = "10.10.10.1";
	src_2 = "10.10.10.2";
	break;
    case 6:
	src_1 = "2001::1";
	src_2 = "2001::2";
	break;
    default:
	XLOG_FATAL("Unknown IP version %d", src_1.ip_version());
	break;
    }

    PeerID peerid_1 = pm_1.
	create_peer(interface_1, vif_1, src_1, interface_prefix_length,
		    interface_mtu, linktype, area);
    PeerID peerid_2 = pm_2.
	create_peer(interface_2, vif_2, src_2, interface_prefix_length,
		    interface_mtu, linktype, area);

    ospf_1.set_hello_interval(interface_1, vif_1, area, hello_interval);
    ospf_1.set_router_dead_interval(interface_1, vif_1, area,
				    4 * hello_interval);
    ospf_1.set_interface_cost(interface_1, vif_1, area, interface_cost);
    ospf_1.set_inftransdelay(interface_1, vif_1, area, inftransdelay);

    ospf_2.set_hello_interval(interface_2, vif_2, area, hello_interval);
    ospf_2.set_router_dead_interval(interface_2, vif_2, area,
				    4 * hello_interval);
    ospf_2.set_interface_cost(interface_2, vif_2, area, interface_cost);
    ospf_2.set_inftransdelay(interface_2, vif_2, area, inftransdelay);

    EmulateSubnet<A> emu(info, eventloop);

    emu.bind_interfaces("ospf1", interface_1, vif_1, io_1);
    emu.bind_interfaces("ospf2", interface_2, vif_2, io_2);

    if (STAGGER1 != stagger)
	pm_1.set_state_peer(peerid_1, true);
    if (STAGGER2 != stagger)
	pm_2.set_state_peer(peerid_2, true);

    if (forever)
	while (ospf_1.running() && ospf_2.running())
	    eventloop.run();

    bool timeout = false;
    XorpTimer t = eventloop.set_flag_after(TimeVal(20 * hello_interval, 0),
					   &timeout);
    const int expected = 32;
    while (ospf_1.running() && ospf_2.running() && !timeout) {
	eventloop.run();
	if (expected <= io_1.packets() + io_2.packets())
	    break;
	if (STAGGER1 == stagger && 1 == io_2.packets())
	    pm_1.set_state_peer(peerid_1, true);
	if (STAGGER2 == stagger && 1 == io_1.packets())
	    pm_2.set_state_peer(peerid_2, true);
    }
    if (timeout) {
	DOUT(info) << io_1.packets() << " packets sent " << expected <<
	    " expected test timed out\n";
	return false;
    }

    // Take the peering down
    pm_1.set_state_peer(peerid_1, false);
    pm_2.set_state_peer(peerid_2, false);

    // Delete the peers.
    if (!pm_1.delete_peer(peerid_1)) {
	DOUT(info) << "Failed to delete peer\n";
	return false;
    }

    if (!pm_2.delete_peer(peerid_2)) {
	DOUT(info) << "Failed to delete peer\n";
	return false;
    }

    // Delete the areas.
    if (!pm_1.destroy_area_router(area)) {
	DOUT(info) << "Failed to delete area\n";
	return false;
    }

    // Delete the areas.
    if (!pm_2.destroy_area_router(area)) {
	DOUT(info) << "Failed to delete area\n";
	return false;
    }

    return true;
}

int
main(int argc, char **argv)
{
    XorpUnexpectedHandler x(xorp_unexpected_handler);

    TestMain t(argc, argv);

    string test =
	t.get_optional_args("-t", "--test", "run only the specified test");
    string hello_interval_arg = 
	t.get_optional_args("-h", "--hello", "hello interval");
    suppress = t.get_optional_args("-s", "--suppress", "verbose output");
    forever = t.get_optional_flag("-f", "--forever", "Don't terminate test");
    t.complete_args_parsing();

    if (!hello_interval_arg.empty())
	hello_interval = atoi(hello_interval_arg.c_str());

    struct test {
	string test_name;
	XorpCallback1<bool, TestInfo&>::RefPtr cb;
    } tests[] = {
	{"single_peerV2", callback(single_peer<IPv4>, OspfTypes::V2)},
	{"single_peerV3", callback(single_peer<IPv6>, OspfTypes::V3)},

	{"two_peersV2", callback(two_peers<IPv4>, OspfTypes::V2,
				 OspfTypes::BROADCAST, NOSTAGGER)},
	{"two_peersV3", callback(two_peers<IPv6>, OspfTypes::V3,
				 OspfTypes::BROADCAST, NOSTAGGER)},

	{"two_peersV2s1", callback(two_peers<IPv4>, OspfTypes::V2,
				   OspfTypes::BROADCAST, STAGGER1)},
	{"two_peersV3s1", callback(two_peers<IPv6>, OspfTypes::V3,
				   OspfTypes::BROADCAST, STAGGER1)},

	{"two_peersV2s2", callback(two_peers<IPv4>, OspfTypes::V2,
				   OspfTypes::BROADCAST, STAGGER2)},
	{"two_peersV3s2", callback(two_peers<IPv6>, OspfTypes::V3,
				   OspfTypes::BROADCAST, STAGGER2)},

#if	0
	{"p2pV2", callback(two_peers<IPv4>, OspfTypes::V2,
				 OspfTypes::PointToPoint, NOSTAGGER)},
	{"p2pV3", callback(two_peers<IPv6>, OspfTypes::V3,
				 OspfTypes::PointToPoint, NOSTAGGER)},
#endif
    };

    try {
	if (test.empty()) {
	    for (size_t i = 0; i < sizeof(tests) / sizeof(struct test); i++)
		t.run(tests[i].test_name, tests[i].cb);
	} else {
	    for (size_t i = 0; i < sizeof(tests) / sizeof(struct test); i++)
		if (test == tests[i].test_name) {
		    t.run(tests[i].test_name, tests[i].cb);
		    return t.exit();
		}
	    t.failed("No test with name " + test + " found\n");
	}
    } catch(...) {
	xorp_catch_standard_exceptions();
    }

    return t.exit();
}
