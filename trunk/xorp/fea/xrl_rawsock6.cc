// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2005 International Computer Science Institute
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

#ident "$XORP: xorp/fea/xrl_rawsock6.cc,v 1.6 2005/08/18 15:45:53 bms Exp $"

#include "fea_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"
#include "libxorp/eventloop.hh"

#include "xrl/interfaces/fea_rawpkt6_client_xif.hh"

#include "iftree.hh"
#include "rawsock6.hh"
#include "xrl_rawsock6.hh"

class XrlFilterRawSocket6 : public FilterRawSocket6::InputFilter {
public:
    XrlFilterRawSocket6(XrlRawSocket6Manager&	rsm,
			const string&		xrl_target_name,
			uint32_t		ip_protocol)
	: _rsm(rsm),
	  _xrl_target_name(xrl_target_name),
	  _ip_protocol(ip_protocol)
    {}

    XrlRawSocket6Manager& manager()		{ return _rsm; }
    const string&	xrl_target_name() const	{ return _xrl_target_name; }
    uint32_t		ip_protocol() const	{ return _ip_protocol; }

protected:
    XrlRawSocket6Manager&	_rsm;
    const string		_xrl_target_name;
    const uint32_t		_ip_protocol;
};

//
// Filter class for checking incoming raw packets and checking whether
// to forward them.
//
class XrlVifInputFilter6 : public XrlFilterRawSocket6 {
public:
    XrlVifInputFilter6(XrlRawSocket6Manager&	rsm,
		       FilterRawSocket6&	rs,
		       const string&		xrl_target_name,
		       const string&		if_name,
		       const string&		vif_name,
		       uint32_t			ip_protocol)
	: XrlFilterRawSocket6(rsm, xrl_target_name, ip_protocol),
	  _rs(rs),
	  _if_name(if_name),
	  _vif_name(vif_name)
    {}

    virtual ~XrlVifInputFilter6() {
	leave_all_multicast_groups();
    }

    void recv(const struct IPv6HeaderInfo& header,
	      const vector<uint8_t>& payload)
    {
	// Check the protocol
	if ((ip_protocol() != 0) && (ip_protocol() != header.ip_protocol)) {
	    debug_msg("Ignore packet with protocol %u (watching for %u)\n",
		      XORP_UINT_CAST(header.ip_protocol),
		      XORP_UINT_CAST(ip_protocol()));
	    return;
	}

	// Check the interface name
	if ((! _if_name.empty()) && (_if_name != header.if_name)) {
	    debug_msg("Ignore packet with interface %s (watching for %s)\n",
		      header.if_name.c_str(),
		      _if_name.c_str());
	    return;
	}

	// Check the vif name
	if ((! _vif_name.empty()) && (_vif_name != header.vif_name)) {
	    debug_msg("Ignore packet with vif %s (watching for %s)\n",
		      header.vif_name.c_str(),
		      _vif_name.c_str());
	    return;
	}

	//
	// Instantiate client sending interface
	//
	XrlRawPacket6ClientV0p1Client cl(&_rsm.router());

	//
	// Send notification, note callback goes to owning
	// XrlRawSocket6Manager instance since send failure to xrl_target
	// is useful for reaping all filters to connected to target.
	//
	cl.send_recv(_xrl_target_name.c_str(),
		     header.if_name,
		     header.vif_name,
		     header.src_address,
		     header.dst_address,
		     header.ip_protocol,
		     header.ip_ttl,
		     header.ip_tos,
		     header.ip_router_alert,
		     payload,
		     callback(&_rsm,
			      &XrlRawSocket6Manager::xrl_send_recv_cb,
			      _xrl_target_name));
    }

    void bye() {}
    const string& if_name() const { return _if_name; }
    const string& vif_name() const { return _vif_name; }

    int join_multicast_group(const IPv6& group_address, string& error_msg) {
	if (_rs.join_multicast_group(if_name(), vif_name(), group_address,
				     xrl_target_name(), error_msg)
	    != XORP_OK) {
	    return (XORP_ERROR);
	}
	_joined_multicast_groups.insert(group_address);
	return (XORP_OK);
    }

    int leave_multicast_group(const IPv6& group_address, string& error_msg) {
	_joined_multicast_groups.erase(group_address);
	if (_rs.leave_multicast_group(if_name(), vif_name(), group_address,
				      xrl_target_name(), error_msg)
	    != XORP_OK) {
	    return (XORP_ERROR);
	}
	return (XORP_OK);
    }

    void leave_all_multicast_groups() {
	string error_msg;
	while (! _joined_multicast_groups.empty()) {
	    const IPv6& group_address = *(_joined_multicast_groups.begin());
	    leave_multicast_group(group_address, error_msg);
	}
    }

protected:
    FilterRawSocket6&	_rs;
    const string	_if_name;
    const string	_vif_name;
    set<IPv6>           _joined_multicast_groups;
};

// ----------------------------------------------------------------------------
// XrlRawSocket6Manager code

XrlRawSocket6Manager::XrlRawSocket6Manager(EventLoop&		eventloop,
					   const IfTree&	iftree,
					   XrlRouter&		xr)
    : _eventloop(eventloop), _iftree(iftree), _xrlrouter(xr)
{
}

XrlRawSocket6Manager::~XrlRawSocket6Manager()
{
    erase_filters(_filters.begin(), _filters.end());
}

void
XrlRawSocket6Manager::erase_filters(const FilterBag6::iterator& begin,
				    const FilterBag6::iterator& end)
{
    FilterBag6::iterator fi(begin);
    while (fi != end) {
	XrlFilterRawSocket6* filter = fi->second;

	SocketTable6::iterator sti = _sockets.find(filter->ip_protocol());
	XLOG_ASSERT(sti != _sockets.end());
	FilterRawSocket6* rs = sti->second;
	XLOG_ASSERT(rs != NULL);

	rs->remove_filter(filter);
	delete filter;

	_filters.erase(fi++);
    }
}

XrlCmdError
XrlRawSocket6Manager::send(const string&	if_name,
			   const string&	vif_name,
			   const IPv6&		src_address,
			   const IPv6&		dst_address,
			   uint32_t		ip_protocol,
			   int32_t		ip_ttl,
			   int32_t		ip_tos,
			   bool			ip_router_alert,
			   const vector<uint8_t>& payload)
{
    string error_msg;

    // Find the socket associated with this protocol
    SocketTable6::iterator sti = _sockets.find(ip_protocol);
    if (sti == _sockets.end()) {
	error_msg = c_format("Protocol %u is not registered",
			     XORP_UINT_CAST(ip_protocol));
	return XrlCmdError::COMMAND_FAILED(error_msg);
    }
    FilterRawSocket6* rs = sti->second;
    XLOG_ASSERT(rs != NULL);

    if (rs->proto_socket_write(if_name,
			       vif_name,
			       src_address,
			       dst_address,
			       ip_ttl,
			       ip_tos,
			       ip_router_alert,
			       payload,
			       error_msg)
	!= XORP_OK) {
	return XrlCmdError::COMMAND_FAILED(error_msg);
    }

    return XrlCmdError::OKAY();
}

XrlCmdError
XrlRawSocket6Manager::register_receiver(const string&	xrl_target_name,
					const string&	if_name,
					const string&	vif_name,
					uint32_t	ip_protocol)
{
    XrlVifInputFilter6* filter;

    //
    // Look in the SocketTable for a socket matching this protocol.
    // If a socket does not yet exist, create one.
    //
    SocketTable6::iterator sti = _sockets.find(ip_protocol);
    FilterRawSocket6* rs = NULL;
    if (sti == _sockets.end()) {
	rs = new FilterRawSocket6(_eventloop, ip_protocol, iftree());
	_sockets[ip_protocol] = rs;
    } else {
	rs = sti->second;
    }
    XLOG_ASSERT(rs != NULL);

    FilterBag6::iterator fi;
    FilterBag6::iterator fi_end = _filters.upper_bound(xrl_target_name);
    for (fi = _filters.lower_bound(xrl_target_name); fi != fi_end; ++fi) {
	filter = dynamic_cast<XrlVifInputFilter6*>(fi->second);
	if (filter == NULL)
	    continue; // Not a vif filter

	//
	// Search if we have already the filter
	//
	if ((filter->ip_protocol() == ip_protocol) &&
	    (filter->if_name() == if_name) &&
	    (filter->vif_name() == vif_name)) {
	    // Already have this filter
	    return XrlCmdError::OKAY();
	}
    }

    //
    // Create the filter
    //
    filter = new XrlVifInputFilter6(*this, *rs, xrl_target_name, if_name,
				    vif_name, ip_protocol);

    // Add the filter to the appropriate raw socket
    rs->add_filter(filter);

    // Add the filter to those associated with xrl_target_name
    _filters.insert(FilterBag6::value_type(xrl_target_name, filter));

    return XrlCmdError::OKAY();
}

XrlCmdError
XrlRawSocket6Manager::unregister_receiver(const string&	xrl_target_name,
					  const string&	if_name,
					  const string&	vif_name,
					  uint32_t	ip_protocol)
{
    string error_msg;

    //
    // Find the socket associated with this protocol
    //
    SocketTable6::iterator sti = _sockets.find(ip_protocol);
    if (sti == _sockets.end()) {
	error_msg = c_format("Protocol %u is not registered",
			     XORP_UINT_CAST(ip_protocol));
	return XrlCmdError::COMMAND_FAILED(error_msg);
    }
    FilterRawSocket6* rs = sti->second;
    XLOG_ASSERT(rs != NULL);

    //
    // Walk through list of filters looking for matching interface and vif
    //
    FilterBag6::iterator fi;
    FilterBag6::iterator fi_end = _filters.upper_bound(xrl_target_name);
    for (fi = _filters.lower_bound(xrl_target_name); fi != fi_end; ++fi) {
	XrlVifInputFilter6* filter;
	filter = dynamic_cast<XrlVifInputFilter6*>(fi->second);

	// If filter found, remove it and delete it
	if ((filter != NULL) &&
	    (filter->ip_protocol() == ip_protocol) &&
	    (filter->if_name() == if_name) &&
	    (filter->vif_name() == vif_name)) {

	    // Remove the filter
	    rs->remove_filter(filter);

	    // Remove the filter from the group associated with this target
	    _filters.erase(fi);

	    // Destruct the filter
	    delete filter;

	    //
	    // Reference counting: if there are now no listeners on
	    // this protocol socket (and hence no filters), remove it
	    // from the table and delete it.
	    //
	    if (rs->empty()) {
		_sockets.erase(ip_protocol);
		delete rs;
	    }

	    return XrlCmdError::OKAY();
	}
    }

    error_msg = c_format("Cannot find registration for target %s interface %s "
			 "and vif %s",
			 xrl_target_name.c_str(),
			 if_name.c_str(),
			 vif_name.c_str());
    return XrlCmdError::COMMAND_FAILED(error_msg);
}

XrlCmdError
XrlRawSocket6Manager::join_multicast_group(const string& xrl_target_name,
					   const string& if_name,
					   const string& vif_name,
					   uint32_t	 ip_protocol,
					   const IPv6&	 group_address)
{
    string error_msg;

    //
    // Search if we have already the filter
    //
    FilterBag6::iterator fi;
    FilterBag6::iterator fi_end = _filters.upper_bound(xrl_target_name);
    for (fi = _filters.lower_bound(xrl_target_name); fi != fi_end; ++fi) {
	XrlVifInputFilter6* filter;
	filter = dynamic_cast<XrlVifInputFilter6*>(fi->second);
	if (filter == NULL)
	    continue; // Not a vif filter

	if ((filter->ip_protocol() == ip_protocol) &&
	    (filter->if_name() == if_name) &&
	    (filter->vif_name() == vif_name)) {
	    // Filter found
	    if (filter->join_multicast_group(group_address, error_msg)
		!= XORP_OK) {
		return XrlCmdError::COMMAND_FAILED(error_msg);
	    }
	    return XrlCmdError::OKAY();
	}
    }

    error_msg = c_format("Cannot join group %s on interface %s vif %s "
			 "protocol %u target %s: the target has not "
			 "registered as a receiver",
			 group_address.str().c_str(),
			 if_name.c_str(),
			 vif_name.c_str(),
			 XORP_UINT_CAST(ip_protocol),
			 xrl_target_name.c_str());

    return XrlCmdError::COMMAND_FAILED(error_msg);
}

XrlCmdError
XrlRawSocket6Manager::leave_multicast_group(const string& xrl_target_name,
					    const string& if_name,
					    const string& vif_name,
					    uint32_t	  ip_protocol,
					    const IPv6&	  group_address)
{
    string error_msg;

    //
    // Search if we have already the filter
    //
    FilterBag6::iterator fi;
    FilterBag6::iterator fi_end = _filters.upper_bound(xrl_target_name);
    for (fi = _filters.lower_bound(xrl_target_name); fi != fi_end; ++fi) {
	XrlVifInputFilter6* filter;
	filter = dynamic_cast<XrlVifInputFilter6*>(fi->second);
	if (filter == NULL)
	    continue; // Not a vif filter

	if ((filter->ip_protocol() == ip_protocol) &&
	    (filter->if_name() == if_name) &&
	    (filter->vif_name() == vif_name)) {
	    // Filter found
	    if (filter->leave_multicast_group(group_address, error_msg)
		!= XORP_OK) {
		return XrlCmdError::COMMAND_FAILED(error_msg);
	    }
	    return XrlCmdError::OKAY();
	}
    }

    error_msg = c_format("Cannot leave group %s on interface %s vif %s "
			 "protocol %u target %s: the target has not "
			 "registered as a receiver",
			 group_address.str().c_str(),
			 if_name.c_str(),
			 vif_name.c_str(),
			 XORP_UINT_CAST(ip_protocol),
			 xrl_target_name.c_str());

    return XrlCmdError::COMMAND_FAILED(error_msg);
}

void
XrlRawSocket6Manager::xrl_send_recv_cb(const XrlError& e,
				       string xrl_target_name)
{
    if (e == XrlError::OKAY())
	return;

    debug_msg("xrl_send_recv_cb: error %s\n", e.str().c_str());

    //
    // Sending Xrl generated an error.
    //
    // Remove all filters associated with Xrl Target that are tied to a raw
    // socket and then erase filters.
    //
    erase_filters(_filters.lower_bound(xrl_target_name),
		  _filters.upper_bound(xrl_target_name));
}
