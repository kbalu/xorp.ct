/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'tgt-gen'.
 *
 * $XORP$
 */


#ifndef __XRL_INTERFACES_BGP_BASE_HH__
#define __XRL_INTERFACES_BGP_BASE_HH__

#undef XORP_LIBRARY_NAME
#define XORP_LIBRARY_NAME "XrlBgpTarget"

#include "libxorp/xlog.h"
#include "libxipc/xrl_cmd_map.hh"

class XrlBgpTargetBase {
protected:
    XrlCmdMap* _cmds;

public:
    /**
     * Constructor.
     *
     * @param cmds an XrlCmdMap that the commands associated with the target
     *		   should be added to.  This is typically the XrlRouter
     *		   associated with the target.
     */
    XrlBgpTargetBase(XrlCmdMap* cmds = 0);

    /**
     * Destructor.
     *
     * Dissociates instance commands from command map.
     */
    virtual ~XrlBgpTargetBase();

    /**
     * Set command map.
     *
     * @param cmds pointer to command map to associate commands with.  This
     * argument is typically a pointer to the XrlRouter associated with the
     * target.
     *
     * @return true on success, false if cmds is null or a command map has
     * already been supplied.
     */
    bool set_command_map(XrlCmdMap* cmds);

    /**
     * Get Xrl instance name associated with command map.
     */
    inline const string& name() const { return _cmds->name(); }

    /**
     * Get version string of instance.
     */
    inline const char* version() const { return "bgp/0.0"; }

protected:

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get name of Xrl Target
     */
    virtual XrlCmdError common_0_1_get_target_name(
	// Output values,
	string&	name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get version string from Xrl Target
     */
    virtual XrlCmdError common_0_1_get_version(
	// Output values,
	string&	version) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get status of Xrl Target
     */
    virtual XrlCmdError common_0_1_get_status(
	// Output values,
	uint32_t&	status,
	string&	reason) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Request clean shutdown of Xrl Target
     */
    virtual XrlCmdError common_0_1_shutdown() = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the BGP version currently running.
     */
    virtual XrlCmdError bgp_0_2_get_bgp_version(
	// Output values,
	uint32_t&	version) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get local config
     *
     *  @param id our BGP ID.
     */
    virtual XrlCmdError bgp_0_2_local_config(
	// Input values,
	const uint32_t&	as,
	const IPv4&	id) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Set the local AS number.
     *
     *  @param as our AS number.
     */
    virtual XrlCmdError bgp_0_2_set_local_as(
	// Input values,
	const uint32_t&	as) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the local AS number.
     */
    virtual XrlCmdError bgp_0_2_get_local_as(
	// Output values,
	uint32_t&	as) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Set the BGP id.
     *
     *  @param id our BGP ID.
     */
    virtual XrlCmdError bgp_0_2_set_bgp_id(
	// Input values,
	const IPv4&	id) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the BGP id.
     */
    virtual XrlCmdError bgp_0_2_get_bgp_id(
	// Output values,
	IPv4&	id) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Add peer.
     *
     *  @param as the peer's AS number.
     *
     *  @param next_hop the local next hop.
     *
     *  @param holdtime expects traffic in this time interval.
     */
    virtual XrlCmdError bgp_0_2_add_peer(
	// Input values,
	const string&	local_ip,
	const uint32_t&	local_port,
	const string&	peer_ip,
	const uint32_t&	peer_port,
	const uint32_t&	as,
	const IPv4&	next_hop,
	const uint32_t&	holdtime) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Delete peer.
     */
    virtual XrlCmdError bgp_0_2_delete_peer(
	// Input values,
	const string&	local_ip,
	const uint32_t&	local_port,
	const string&	peer_ip,
	const uint32_t&	peer_port) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Enable this peer.
     */
    virtual XrlCmdError bgp_0_2_enable_peer(
	// Input values,
	const string&	local_ip,
	const uint32_t&	local_port,
	const string&	peer_ip,
	const uint32_t&	peer_port) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Disable this peer.
     */
    virtual XrlCmdError bgp_0_2_disable_peer(
	// Input values,
	const string&	local_ip,
	const uint32_t&	local_port,
	const string&	peer_ip,
	const uint32_t&	peer_port) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Set which parameters we support per peer
     */
    virtual XrlCmdError bgp_0_2_set_parameter(
	// Input values,
	const string&	local_ip,
	const uint32_t&	local_port,
	const string&	peer_ip,
	const uint32_t&	peer_port,
	const string&	parameter) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Set next hop rewrite filter. XXX - This is a temporary hack until we
     *  get programmable filters.
     */
    virtual XrlCmdError bgp_0_2_next_hop_rewrite_filter(
	// Input values,
	const string&	local_ip,
	const uint32_t&	local_port,
	const string&	peer_ip,
	const uint32_t&	peer_port,
	const IPv4&	next_hop) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Set the peer state enabled or disabled.
     *
     *  @param toggle enabled or disabled.
     */
    virtual XrlCmdError bgp_0_2_set_peer_state(
	// Input values,
	const string&	local_ip,
	const uint32_t&	local_port,
	const string&	peer_ip,
	const uint32_t&	peer_port,
	const bool&	toggle) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Originate route IPv4
     *
     *  @param nlri subnet to announce
     *
     *  @param next_hop to forward to
     *
     *  @param unicast if true install in unicast routing table
     *
     *  @param multicast if true install in multicast routing table
     */
    virtual XrlCmdError bgp_0_2_originate_route4(
	// Input values,
	const IPv4Net&	nlri,
	const IPv4&	next_hop,
	const bool&	unicast,
	const bool&	multicast) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Originate route IPv6
     *
     *  @param nlri subnet to announce
     *
     *  @param next_hop to forward to
     *
     *  @param unicast if true install in unicast routing table
     *
     *  @param multicast if true install in multicast routing table
     */
    virtual XrlCmdError bgp_0_2_originate_route6(
	// Input values,
	const IPv6Net&	nlri,
	const IPv6&	next_hop,
	const bool&	unicast,
	const bool&	multicast) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Withdraw route IPv4
     *
     *  @param nlri subnet to withdraw
     *
     *  @param unicast if true withdraw from unicast routing table
     *
     *  @param multicast if true withdraw from multicast routing table
     */
    virtual XrlCmdError bgp_0_2_withdraw_route4(
	// Input values,
	const IPv4Net&	nlri,
	const bool&	unicast,
	const bool&	multicast) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Withdraw route IPv6
     *
     *  @param nlri subnet to withdraw
     *
     *  @param unicast if true withdraw from unicast routing table
     *
     *  @param multicast if true withdraw from multicast routing table
     */
    virtual XrlCmdError bgp_0_2_withdraw_route6(
	// Input values,
	const IPv6Net&	nlri,
	const bool&	unicast,
	const bool&	multicast) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the first item of a list of BGP peers See RFC 1657 (BGP MIB) for
     *  full definitions of return values.
     *
     *  @param token returned token to be provided when calling
     *  get_peer_list_next.
     *
     *  @param more returned to indicate whether there are more list items
     *  remaining.
     */
    virtual XrlCmdError bgp_0_2_get_peer_list_start(
	// Output values,
	uint32_t&	token,
	bool&	more) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the next item of a list of BGP peers
     *
     *  @param token token from prior call to get_peer_list
     *
     *  @param more returned to indicate whether there are more list items
     *  remaining.
     */
    virtual XrlCmdError bgp_0_2_get_peer_list_next(
	// Input values,
	const uint32_t&	token,
	// Output values,
	IPv4&	local_ip,
	uint32_t&	local_port,
	IPv4&	peer_ip,
	uint32_t&	peer_port,
	bool&	more) = 0;

    virtual XrlCmdError bgp_0_2_get_peer_id(
	// Input values,
	const IPv4&	local_ip,
	const uint32_t&	local_port,
	const IPv4&	peer_ip,
	const uint32_t&	peer_port,
	// Output values,
	IPv4&	peer_id) = 0;

    virtual XrlCmdError bgp_0_2_get_peer_status(
	// Input values,
	const IPv4&	local_ip,
	const uint32_t&	local_port,
	const IPv4&	peer_ip,
	const uint32_t&	peer_port,
	// Output values,
	uint32_t&	peer_state,
	uint32_t&	admin_status) = 0;

    virtual XrlCmdError bgp_0_2_get_peer_negotiated_version(
	// Input values,
	const IPv4&	local_ip,
	const uint32_t&	local_port,
	const IPv4&	peer_ip,
	const uint32_t&	peer_port,
	// Output values,
	int32_t&	neg_version) = 0;

    virtual XrlCmdError bgp_0_2_get_peer_as(
	// Input values,
	const IPv4&	local_ip,
	const uint32_t&	local_port,
	const IPv4&	peer_ip,
	const uint32_t&	peer_port,
	// Output values,
	uint32_t&	peer_as) = 0;

    virtual XrlCmdError bgp_0_2_get_peer_msg_stats(
	// Input values,
	const IPv4&	local_ip,
	const uint32_t&	local_port,
	const IPv4&	peer_ip,
	const uint32_t&	peer_port,
	// Output values,
	uint32_t&	in_updates,
	uint32_t&	out_updates,
	uint32_t&	in_msgs,
	uint32_t&	out_msgs,
	uint32_t&	last_error,
	uint32_t&	in_update_elapsed) = 0;

    virtual XrlCmdError bgp_0_2_get_peer_established_stats(
	// Input values,
	const IPv4&	local_ip,
	const uint32_t&	local_port,
	const IPv4&	peer_ip,
	const uint32_t&	peer_port,
	// Output values,
	uint32_t&	transitions,
	uint32_t&	established_time) = 0;

    virtual XrlCmdError bgp_0_2_get_peer_timer_config(
	// Input values,
	const IPv4&	local_ip,
	const uint32_t&	local_port,
	const IPv4&	peer_ip,
	const uint32_t&	peer_port,
	// Output values,
	uint32_t&	retry_interval,
	uint32_t&	hold_time,
	uint32_t&	keep_alive,
	uint32_t&	hold_time_conf,
	uint32_t&	keep_alive_conf,
	uint32_t&	min_as_orgination_interval,
	uint32_t&	min_route_adv_interval) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Register rib.
     *
     *  @param name rib name.
     */
    virtual XrlCmdError bgp_0_2_register_rib(
	// Input values,
	const string&	name) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  add route
     *
     *  @param origin the origin of the path information.
     *
     *  @param next_hop the border router that should be used as a destination
     *  for the nlri.
     *
     *  @param nlri network level reachability information.
     */
    virtual XrlCmdError bgp_0_2_add_route(
	// Input values,
	const int32_t&	origin,
	const int32_t&	asnum,
	const IPv4&	next_hop,
	const IPv4Net&	nlri) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  delete route
     *
     *  @param nlri network level reachability information.
     */
    virtual XrlCmdError bgp_0_2_delete_route(
	// Input values,
	const IPv4Net&	nlri) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the first item of a list of BGP routes See RFC 1657 (BGP MIB) for
     *  full definitions of return values.
     *
     *  @param token returned token to be provided when calling
     *  get_route_list_next.
     */
    virtual XrlCmdError bgp_0_2_get_v4_route_list_start(
	// Output values,
	uint32_t&	token) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the first item of a list of BGP routes See RFC 1657 (BGP MIB) for
     *  full definitions of return values.
     *
     *  @param token returned token to be provided when calling
     *  get_route_list_next.
     */
    virtual XrlCmdError bgp_0_2_get_v6_route_list_start(
	// Output values,
	uint32_t&	token) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the next route in the list See RFC 1657 (BGP MIB) for full
     *  definitions of return values.
     */
    virtual XrlCmdError bgp_0_2_get_v4_route_list_next(
	// Input values,
	const uint32_t&	token,
	// Output values,
	IPv4&	peer_id,
	IPv4Net&	net,
	uint32_t&	best_and_origin,
	vector<uint8_t>&	aspath,
	IPv4&	nexthop,
	int32_t&	med,
	int32_t&	localpref,
	int32_t&	atomic_agg,
	vector<uint8_t>&	aggregator,
	int32_t&	calc_localpref,
	vector<uint8_t>&	attr_unknown,
	bool&	valid) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Get the next route in the list See RFC 1657 (BGP MIB) for full
     *  definitions of return values.
     */
    virtual XrlCmdError bgp_0_2_get_v6_route_list_next(
	// Input values,
	const uint32_t&	token,
	// Output values,
	IPv4&	peer_id,
	IPv6Net&	net,
	uint32_t&	best_and_origin,
	vector<uint8_t>&	aspath,
	IPv6&	nexthop,
	int32_t&	med,
	int32_t&	localpref,
	int32_t&	atomic_agg,
	vector<uint8_t>&	aggregator,
	int32_t&	calc_localpref,
	vector<uint8_t>&	attr_unknown,
	bool&	valid) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Route Info Changed route_info_changed is called by the RIB on the RIB
     *  client (typically a routing protocol) that had registered an interest
     *  in the routing of an address. This can be because the metric and/or
     *  nexthop changed.
     *
     *  @param addr base address of the subnet that was registered
     *
     *  @param prefix_len prefix length of the subnet that was registered
     *
     *  @param metric the routing metric toward the address.
     *
     *  @param admin_distance the administratively defined distance toward the
     *  address.
     *
     *  @param protocol_origin the name of the protocol that originated this
     *  entry.
     */
    virtual XrlCmdError rib_client_0_1_route_info_changed4(
	// Input values,
	const IPv4&	addr,
	const uint32_t&	prefix_len,
	const IPv4&	nexthop,
	const uint32_t&	metric,
	const uint32_t&	admin_distance,
	const string&	protocol_origin) = 0;

    virtual XrlCmdError rib_client_0_1_route_info_changed6(
	// Input values,
	const IPv6&	addr,
	const uint32_t&	prefix_len,
	const IPv6&	nexthop,
	const uint32_t&	metric,
	const uint32_t&	admin_distance,
	const string&	protocol_origin) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Route Info Invalid route_info_invalid is called by the RIB on the RIB
     *  client (typically a routing protocol) that had registere d an interest
     *  in the routing of an address. This can be because the information
     *  previously reported as applying no longer applies for any number of
     *  reasons. When the RIB sends this message, it has automatically
     *  de-registered interest in the route, and the client will normally need
     *  to send a register_interest request again.
     */
    virtual XrlCmdError rib_client_0_1_route_info_invalid4(
	// Input values,
	const IPv4&	addr,
	const uint32_t&	prefix_len) = 0;

    virtual XrlCmdError rib_client_0_1_route_info_invalid6(
	// Input values,
	const IPv6&	addr,
	const uint32_t&	prefix_len) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Announce target birth.
     */
    virtual XrlCmdError finder_event_observer_0_1_xrl_target_birth(
	// Input values,
	const string&	target_class,
	const string&	target_instance) = 0;

    /**
     *  Pure-virtual function that needs to be implemented to:
     *
     *  Announce target death.
     */
    virtual XrlCmdError finder_event_observer_0_1_xrl_target_death(
	// Input values,
	const string&	target_class,
	const string&	target_instance) = 0;

private:
    const XrlCmdError handle_common_0_1_get_target_name(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_common_0_1_get_version(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_common_0_1_get_status(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_common_0_1_shutdown(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_bgp_version(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_local_config(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_set_local_as(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_local_as(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_set_bgp_id(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_bgp_id(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_add_peer(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_delete_peer(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_enable_peer(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_disable_peer(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_set_parameter(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_next_hop_rewrite_filter(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_set_peer_state(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_originate_route4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_originate_route6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_withdraw_route4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_withdraw_route6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_list_start(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_list_next(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_id(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_status(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_negotiated_version(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_as(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_msg_stats(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_established_stats(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_peer_timer_config(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_register_rib(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_add_route(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_delete_route(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_v4_route_list_start(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_v6_route_list_start(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_v4_route_list_next(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_bgp_0_2_get_v6_route_list_next(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_rib_client_0_1_route_info_changed4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_rib_client_0_1_route_info_changed6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_rib_client_0_1_route_info_invalid4(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_rib_client_0_1_route_info_invalid6(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_finder_event_observer_0_1_xrl_target_birth(const XrlArgs& in, XrlArgs* out);

    const XrlCmdError handle_finder_event_observer_0_1_xrl_target_death(const XrlArgs& in, XrlArgs* out);

    void add_handlers()
    {
	if (_cmds->add_handler("common/0.1/get_target_name",
	    callback(this, &XrlBgpTargetBase::handle_common_0_1_get_target_name)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/common/0.1/get_target_name");
	}
	if (_cmds->add_handler("common/0.1/get_version",
	    callback(this, &XrlBgpTargetBase::handle_common_0_1_get_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/common/0.1/get_version");
	}
	if (_cmds->add_handler("common/0.1/get_status",
	    callback(this, &XrlBgpTargetBase::handle_common_0_1_get_status)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/common/0.1/get_status");
	}
	if (_cmds->add_handler("common/0.1/shutdown",
	    callback(this, &XrlBgpTargetBase::handle_common_0_1_shutdown)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/common/0.1/shutdown");
	}
	if (_cmds->add_handler("bgp/0.2/get_bgp_version",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_bgp_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_bgp_version");
	}
	if (_cmds->add_handler("bgp/0.2/local_config",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_local_config)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/local_config");
	}
	if (_cmds->add_handler("bgp/0.2/set_local_as",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_set_local_as)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/set_local_as");
	}
	if (_cmds->add_handler("bgp/0.2/get_local_as",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_local_as)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_local_as");
	}
	if (_cmds->add_handler("bgp/0.2/set_bgp_id",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_set_bgp_id)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/set_bgp_id");
	}
	if (_cmds->add_handler("bgp/0.2/get_bgp_id",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_bgp_id)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_bgp_id");
	}
	if (_cmds->add_handler("bgp/0.2/add_peer",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_add_peer)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/add_peer");
	}
	if (_cmds->add_handler("bgp/0.2/delete_peer",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_delete_peer)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/delete_peer");
	}
	if (_cmds->add_handler("bgp/0.2/enable_peer",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_enable_peer)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/enable_peer");
	}
	if (_cmds->add_handler("bgp/0.2/disable_peer",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_disable_peer)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/disable_peer");
	}
	if (_cmds->add_handler("bgp/0.2/set_parameter",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_set_parameter)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/set_parameter");
	}
	if (_cmds->add_handler("bgp/0.2/next_hop_rewrite_filter",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_next_hop_rewrite_filter)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/next_hop_rewrite_filter");
	}
	if (_cmds->add_handler("bgp/0.2/set_peer_state",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_set_peer_state)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/set_peer_state");
	}
	if (_cmds->add_handler("bgp/0.2/originate_route4",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_originate_route4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/originate_route4");
	}
	if (_cmds->add_handler("bgp/0.2/originate_route6",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_originate_route6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/originate_route6");
	}
	if (_cmds->add_handler("bgp/0.2/withdraw_route4",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_withdraw_route4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/withdraw_route4");
	}
	if (_cmds->add_handler("bgp/0.2/withdraw_route6",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_withdraw_route6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/withdraw_route6");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_list_start",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_list_start)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_list_start");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_list_next",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_list_next)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_list_next");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_id",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_id)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_id");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_status",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_status)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_status");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_negotiated_version",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_negotiated_version)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_negotiated_version");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_as",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_as)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_as");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_msg_stats",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_msg_stats)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_msg_stats");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_established_stats",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_established_stats)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_established_stats");
	}
	if (_cmds->add_handler("bgp/0.2/get_peer_timer_config",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_peer_timer_config)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_peer_timer_config");
	}
	if (_cmds->add_handler("bgp/0.2/register_rib",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_register_rib)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/register_rib");
	}
	if (_cmds->add_handler("bgp/0.2/add_route",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_add_route)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/add_route");
	}
	if (_cmds->add_handler("bgp/0.2/delete_route",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_delete_route)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/delete_route");
	}
	if (_cmds->add_handler("bgp/0.2/get_v4_route_list_start",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_v4_route_list_start)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_v4_route_list_start");
	}
	if (_cmds->add_handler("bgp/0.2/get_v6_route_list_start",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_v6_route_list_start)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_v6_route_list_start");
	}
	if (_cmds->add_handler("bgp/0.2/get_v4_route_list_next",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_v4_route_list_next)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_v4_route_list_next");
	}
	if (_cmds->add_handler("bgp/0.2/get_v6_route_list_next",
	    callback(this, &XrlBgpTargetBase::handle_bgp_0_2_get_v6_route_list_next)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/bgp/0.2/get_v6_route_list_next");
	}
	if (_cmds->add_handler("rib_client/0.1/route_info_changed4",
	    callback(this, &XrlBgpTargetBase::handle_rib_client_0_1_route_info_changed4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/rib_client/0.1/route_info_changed4");
	}
	if (_cmds->add_handler("rib_client/0.1/route_info_changed6",
	    callback(this, &XrlBgpTargetBase::handle_rib_client_0_1_route_info_changed6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/rib_client/0.1/route_info_changed6");
	}
	if (_cmds->add_handler("rib_client/0.1/route_info_invalid4",
	    callback(this, &XrlBgpTargetBase::handle_rib_client_0_1_route_info_invalid4)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/rib_client/0.1/route_info_invalid4");
	}
	if (_cmds->add_handler("rib_client/0.1/route_info_invalid6",
	    callback(this, &XrlBgpTargetBase::handle_rib_client_0_1_route_info_invalid6)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/rib_client/0.1/route_info_invalid6");
	}
	if (_cmds->add_handler("finder_event_observer/0.1/xrl_target_birth",
	    callback(this, &XrlBgpTargetBase::handle_finder_event_observer_0_1_xrl_target_birth)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/finder_event_observer/0.1/xrl_target_birth");
	}
	if (_cmds->add_handler("finder_event_observer/0.1/xrl_target_death",
	    callback(this, &XrlBgpTargetBase::handle_finder_event_observer_0_1_xrl_target_death)) == false) {
	    XLOG_ERROR("Failed to xrl handler finder://bgp/finder_event_observer/0.1/xrl_target_death");
	}
	_cmds->finalize();
    }

    void remove_handlers()
    {
	_cmds->remove_handler("common/0.1/get_target_name");
	_cmds->remove_handler("common/0.1/get_version");
	_cmds->remove_handler("common/0.1/get_status");
	_cmds->remove_handler("common/0.1/shutdown");
	_cmds->remove_handler("bgp/0.2/get_bgp_version");
	_cmds->remove_handler("bgp/0.2/local_config");
	_cmds->remove_handler("bgp/0.2/set_local_as");
	_cmds->remove_handler("bgp/0.2/get_local_as");
	_cmds->remove_handler("bgp/0.2/set_bgp_id");
	_cmds->remove_handler("bgp/0.2/get_bgp_id");
	_cmds->remove_handler("bgp/0.2/add_peer");
	_cmds->remove_handler("bgp/0.2/delete_peer");
	_cmds->remove_handler("bgp/0.2/enable_peer");
	_cmds->remove_handler("bgp/0.2/disable_peer");
	_cmds->remove_handler("bgp/0.2/set_parameter");
	_cmds->remove_handler("bgp/0.2/next_hop_rewrite_filter");
	_cmds->remove_handler("bgp/0.2/set_peer_state");
	_cmds->remove_handler("bgp/0.2/originate_route4");
	_cmds->remove_handler("bgp/0.2/originate_route6");
	_cmds->remove_handler("bgp/0.2/withdraw_route4");
	_cmds->remove_handler("bgp/0.2/withdraw_route6");
	_cmds->remove_handler("bgp/0.2/get_peer_list_start");
	_cmds->remove_handler("bgp/0.2/get_peer_list_next");
	_cmds->remove_handler("bgp/0.2/get_peer_id");
	_cmds->remove_handler("bgp/0.2/get_peer_status");
	_cmds->remove_handler("bgp/0.2/get_peer_negotiated_version");
	_cmds->remove_handler("bgp/0.2/get_peer_as");
	_cmds->remove_handler("bgp/0.2/get_peer_msg_stats");
	_cmds->remove_handler("bgp/0.2/get_peer_established_stats");
	_cmds->remove_handler("bgp/0.2/get_peer_timer_config");
	_cmds->remove_handler("bgp/0.2/register_rib");
	_cmds->remove_handler("bgp/0.2/add_route");
	_cmds->remove_handler("bgp/0.2/delete_route");
	_cmds->remove_handler("bgp/0.2/get_v4_route_list_start");
	_cmds->remove_handler("bgp/0.2/get_v6_route_list_start");
	_cmds->remove_handler("bgp/0.2/get_v4_route_list_next");
	_cmds->remove_handler("bgp/0.2/get_v6_route_list_next");
	_cmds->remove_handler("rib_client/0.1/route_info_changed4");
	_cmds->remove_handler("rib_client/0.1/route_info_changed6");
	_cmds->remove_handler("rib_client/0.1/route_info_invalid4");
	_cmds->remove_handler("rib_client/0.1/route_info_invalid6");
	_cmds->remove_handler("finder_event_observer/0.1/xrl_target_birth");
	_cmds->remove_handler("finder_event_observer/0.1/xrl_target_death");
    }
};

#endif /* __XRL_INTERFACES_BGP_BASE_HH__ */
