// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2006 International Computer Science Institute
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

#ident "$XORP: xorp/libfeaclient/ifmgr_atoms.cc,v 1.13 2005/11/02 02:27:22 pavlin Exp $"

#include "ifmgr_atoms.hh"

// ----------------------------------------------------------------------------
// Find method helpers

static inline const IfMgrIfAtom*
find_interface(const IfMgrIfTree* tree, const string& ifname)
{
    const IfMgrIfTree::IfMap& ifs = tree->ifs();
    IfMgrIfTree::IfMap::const_iterator ii = ifs.find(ifname);
    if (ifs.end() == ii)
	return 0;
    return &ii->second;
}

static inline IfMgrIfAtom*
find_interface(IfMgrIfTree* tree, const string& ifname)
{
    IfMgrIfTree::IfMap& ifs = tree->ifs();
    IfMgrIfTree::IfMap::iterator ii = ifs.find(ifname);
    if (ifs.end() == ii)
	return 0;
    return &ii->second;
}


static inline const IfMgrVifAtom*
find_virtual_interface(const IfMgrIfAtom* ifa, const string& vifname)
{
    const IfMgrIfAtom::VifMap& vifs = ifa->vifs();
    IfMgrIfAtom::VifMap::const_iterator vi = vifs.find(vifname);
    if (vifs.end() == vi)
	return 0;
    return &vi->second;
}

static inline IfMgrVifAtom*
find_virtual_interface(IfMgrIfAtom* ifa, const string& vifname)
{
    IfMgrIfAtom::VifMap& vifs = ifa->vifs();
    IfMgrIfAtom::VifMap::iterator vi = vifs.find(vifname);
    if (vifs.end() == vi)
	return 0;
    return &vi->second;
}


static inline const IfMgrIPv4Atom*
find_virtual_interface_addr(const IfMgrVifAtom* vifa, const IPv4& addr)
{
    const IfMgrVifAtom::V4Map& addrs = vifa->ipv4addrs();
    IfMgrVifAtom::V4Map::const_iterator ai = addrs.find(addr);
    if (addrs.end() == ai)
	return 0;
    return &ai->second;
}

static inline IfMgrIPv4Atom*
find_virtual_interface_addr(IfMgrVifAtom* vifa, const IPv4& addr)
{
    IfMgrVifAtom::V4Map& addrs = vifa->ipv4addrs();
    IfMgrVifAtom::V4Map::iterator ai = addrs.find(addr);
    if (addrs.end() == ai)
	return 0;
    return &ai->second;
}

static inline const IfMgrIPv6Atom*
find_virtual_interface_addr(const IfMgrVifAtom* vifa, const IPv6& addr)
{
    const IfMgrVifAtom::V6Map& addrs = vifa->ipv6addrs();
    IfMgrVifAtom::V6Map::const_iterator ai = addrs.find(addr);
    if (addrs.end() == ai)
	return 0;
    return &ai->second;
}

static inline IfMgrIPv6Atom*
find_virtual_interface_addr(IfMgrVifAtom* vifa, const IPv6& addr)
{
    IfMgrVifAtom::V6Map& addrs = vifa->ipv6addrs();
    IfMgrVifAtom::V6Map::iterator ai = addrs.find(addr);
    if (addrs.end() == ai)
	return 0;
    return &ai->second;
}

// ----------------------------------------------------------------------------
// IfMgrIfTree Methods

const IfMgrIfAtom*
IfMgrIfTree::find_if(const string& ifname) const
{
    return find_interface(this, ifname);
}

IfMgrIfAtom*
IfMgrIfTree::find_if(const string& ifname)
{
    return find_interface(this, ifname);
}

const IfMgrVifAtom*
IfMgrIfTree::find_vif(const string& ifname, const string& vifname) const
{
    const IfMgrIfAtom* ifa = find_interface(this, ifname);
    if (ifa == 0)
	return 0;
    return find_virtual_interface(ifa, vifname);
}

IfMgrVifAtom*
IfMgrIfTree::find_vif(const string& ifname, const string& vifname)
{
    IfMgrIfAtom* ifa = find_interface(this, ifname);
    if (ifa == 0)
	return 0;
    return find_virtual_interface(ifa, vifname);
}

const IfMgrIPv4Atom*
IfMgrIfTree::find_addr(const string& ifname,
		       const string& vifname,
		       const IPv4&   addr) const
{
    const IfMgrIfAtom* ifa = find_interface(this, ifname);
    if (ifa == 0)
	return 0;
    const IfMgrVifAtom* vifa = find_virtual_interface(ifa, vifname);
    if (vifa == 0)
	return 0;
    return find_virtual_interface_addr(vifa, addr);
}

IfMgrIPv4Atom*
IfMgrIfTree::find_addr(const string& ifname,
		       const string& vifname,
		       const IPv4&   addr)
{
    IfMgrIfAtom* ifa = find_interface(this, ifname);
    if (ifa == 0)
	return 0;
    IfMgrVifAtom* vifa = find_virtual_interface(ifa, vifname);
    if (vifa == 0)
	return 0;
    return find_virtual_interface_addr(vifa, addr);
}

const IfMgrIPv6Atom*
IfMgrIfTree::find_addr(const string& ifname,
		       const string& vifname,
		       const IPv6&   addr) const
{
    const IfMgrIfAtom* ifa = find_interface(this, ifname);
    if (ifa == 0)
	return 0;
    const IfMgrVifAtom* vifa = find_virtual_interface(ifa, vifname);
    if (vifa == 0)
	return 0;
    return find_virtual_interface_addr(vifa, addr);
}

IfMgrIPv6Atom*
IfMgrIfTree::find_addr(const string& ifname,
		       const string& vifname,
		       const IPv6&   addr)
{
    IfMgrIfAtom* ifa = find_interface(this, ifname);
    if (ifa == 0)
	return 0;
    IfMgrVifAtom* vifa = find_virtual_interface(ifa, vifname);
    if (vifa == 0)
	return 0;
    return find_virtual_interface_addr(vifa, addr);
}

bool
IfMgrIfTree::operator==(const IfMgrIfTree& o) const
{
    return o.ifs() == ifs();
}

bool
IfMgrIfTree::is_directly_connected(const IPv4& addr, string& ifname,
				   string& vifname) const
{
    IfMgrIfTree::IfMap::const_iterator if_iter;

    for (if_iter = ifs().begin(); if_iter != ifs().end(); ++if_iter) {
	const IfMgrIfAtom& iface = if_iter->second;

	// Test if interface is enabled and the link state is up
	if ((! iface.enabled()) || iface.no_carrier())
	    continue;

	IfMgrIfAtom::VifMap::const_iterator vif_iter;
	for (vif_iter = iface.vifs().begin();
	     vif_iter != iface.vifs().end();
	     ++vif_iter) {
	    const IfMgrVifAtom& vif = vif_iter->second;

	    // Test if vif is enabled
	    if (! vif.enabled())
		continue;

	    // Test if there is matching IPv4 address
	    IfMgrVifAtom::V4Map::const_iterator a4_iter;

	    for (a4_iter = vif.ipv4addrs().begin();
		 a4_iter != vif.ipv4addrs().end();
		 ++a4_iter) {
		const IfMgrIPv4Atom& a4 = a4_iter->second;

		if (! a4.enabled())
		    continue;

		// Test if my own address
		if (a4.addr() == addr) {
		    ifname = iface.name();
		    vifname = vif.name();
		    return (true);
		}

		// Test if p2p address
		if (a4.has_endpoint()) {
		    if (a4.endpoint_addr() == addr) {
			ifname = iface.name();
			vifname = vif.name();
			return (true);
		    }
		}

		// Test if same subnet
		if (IPv4Net(addr, a4.prefix_len())
		    == IPv4Net(a4.addr(), a4.prefix_len())) {
		    ifname = iface.name();
		    vifname = vif.name();
		    return (true);
		}
	    }
	}
    }

    ifname = "";
    vifname = "";
    return (false);
}

bool
IfMgrIfTree::is_directly_connected(const IPv6& addr, string& ifname,
				   string& vifname) const
{
    IfMgrIfTree::IfMap::const_iterator if_iter;

    for (if_iter = ifs().begin(); if_iter != ifs().end(); ++if_iter) {
	const IfMgrIfAtom& iface = if_iter->second;

	// Test if interface is enabled and the link state is up
	if ((! iface.enabled()) || iface.no_carrier())
	    continue;

	IfMgrIfAtom::VifMap::const_iterator vif_iter;
	for (vif_iter = iface.vifs().begin();
	     vif_iter != iface.vifs().end();
	     ++vif_iter) {
	    const IfMgrVifAtom& vif = vif_iter->second;

	    // Test if vif is enabled
	    if (! vif.enabled())
		continue;

	    // Test if there is matching IPv6 address
	    IfMgrVifAtom::V6Map::const_iterator a6_iter;

	    for (a6_iter = vif.ipv6addrs().begin();
		 a6_iter != vif.ipv6addrs().end();
		 ++a6_iter) {
		const IfMgrIPv6Atom& a6 = a6_iter->second;

		if (! a6.enabled())
		    continue;

		// Test if my own address
		if (a6.addr() == addr) {
		    ifname = iface.name();
		    vifname = vif.name();
		    return (true);
		}

		// Test if p2p address
		if (a6.has_endpoint()) {
		    if (a6.endpoint_addr() == addr) {
			ifname = iface.name();
			vifname = vif.name();
			return (true);
		    }
		}

		// Test if same subnet
		if (IPv6Net(addr, a6.prefix_len())
		    == IPv6Net(a6.addr(), a6.prefix_len())) {
		    ifname = iface.name();
		    vifname = vif.name();
		    return (true);
		}
	    }
	}
    }

    ifname = "";
    vifname = "";
    return (false);
}

bool
IfMgrIfTree::is_directly_connected(const IPvX& addr, string& ifname,
				   string& vifname) const
{
    if (addr.is_ipv4()) {
	IPv4 addr4 = addr.get_ipv4();
	return (is_directly_connected(addr4, ifname, vifname));
    }

    if (addr.is_ipv6()) {
	IPv6 addr6 = addr.get_ipv6();
	return (is_directly_connected(addr6, ifname, vifname));
    }

    return (false);
}


// ----------------------------------------------------------------------------
// IfMgrIfAtom methods

bool
IfMgrIfAtom::operator==(const IfMgrIfAtom& o) const
{
    return (
	    name()			== o.name()			&&
	    enabled()			== o.enabled()			&&
	    mtu_bytes()			== o.mtu_bytes()		&&
	    mac()			== o.mac()			&&
	    pif_index()			== o.pif_index()		&&
	    no_carrier()		== o.no_carrier()		&&
	    vifs()			== o.vifs()
	    );
}

const IfMgrVifAtom*
IfMgrIfAtom::find_vif(const string& vifname) const
{
    return find_virtual_interface(this, vifname);
}

IfMgrVifAtom*
IfMgrIfAtom::find_vif(const string& vifname)
{
    return find_virtual_interface(this, vifname);
}


// ----------------------------------------------------------------------------
// IfMgrVifAtom methods

bool
IfMgrVifAtom::operator==(const IfMgrVifAtom& o) const
{
    return (
	    name()			== o.name()			&&
	    enabled()			== o.enabled()			&&
	    multicast_capable()		== o.multicast_capable()	&&
	    broadcast_capable()	  	== o.broadcast_capable()	&&
	    p2p_capable()		== o.p2p_capable()		&&
	    loopback()			== o.loopback()			&&
	    pif_index()			== o.pif_index()		&&
	    ipv4addrs()			== o.ipv4addrs()		&&
	    ipv6addrs()			== o.ipv6addrs()
	    );
}

const IfMgrIPv4Atom*
IfMgrVifAtom::find_addr(const IPv4& addr) const
{
    return find_virtual_interface_addr(this, addr);
}

IfMgrIPv4Atom*
IfMgrVifAtom::find_addr(const IPv4& addr)
{
    return find_virtual_interface_addr(this, addr);
}

const IfMgrIPv6Atom*
IfMgrVifAtom::find_addr(const IPv6& addr) const
{
    return find_virtual_interface_addr(this, addr);
}

IfMgrIPv6Atom*
IfMgrVifAtom::find_addr(const IPv6& addr)
{
    return find_virtual_interface_addr(this, addr);
}


// ----------------------------------------------------------------------------
// IfMgrIPv4Atom methods

bool
IfMgrIPv4Atom::operator==(const IfMgrIPv4Atom& o) const
{
    return (
	    addr()			== o.addr()			&&
	    prefix_len()		== o.prefix_len()		&&
	    enabled()			== o.enabled()			&&
	    multicast_capable()		== o.multicast_capable()	&&
	    loopback()			== o.loopback()			&&
	    has_broadcast()		== o.has_broadcast()		&&
	    broadcast_addr()		== o.broadcast_addr()		&&
	    has_endpoint()		== o.has_endpoint()		&&
	    endpoint_addr()		== o.endpoint_addr()
	    );
}


// ----------------------------------------------------------------------------
// IfMgrIfIPv6Atom methods

bool
IfMgrIPv6Atom::operator==(const IfMgrIPv6Atom& o) const
{
    return (
	    addr()			== o.addr()			&&
	    prefix_len()		== o.prefix_len()		&&
	    enabled()			== o.enabled()			&&
	    multicast_capable()		== o.multicast_capable()	&&
	    loopback()			== o.loopback()			&&
	    has_endpoint()		== o.has_endpoint()		&&
	    endpoint_addr()		== o.endpoint_addr()
	    );
}