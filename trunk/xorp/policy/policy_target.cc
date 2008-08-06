// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

// Copyright (c) 2001-2008 XORP, Inc.
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

#ident "$XORP: xorp/policy/policy_target.cc,v 1.18 2008/07/23 05:11:20 pavlin Exp $"

#include "policy_module.h"

#include "libxorp/xorp.h"

#include "policy/common/policy_utils.hh"
#include "policy/common/varrw.hh"

#include "policy_target.hh"


// static members
string PolicyTarget::policy_target_name = "policy";

PolicyTarget::PolicyTarget(XrlStdRouter& rtr) :
    _running(true), _commit_delay(2000), 
    _process_watch(rtr, _pmap),
    _conf(_process_watch),
    _filter_manager(_conf.import_filters(),
		    _conf.sourcematch_filters(),
		    _conf.export_filters(),
		    _conf.sets(),
		    _conf.tagmap(),
		    rtr,
		    _process_watch,
		    _pmap)

{
    _conf.set_filter_manager(_filter_manager);
    _process_watch.set_notifier(_filter_manager);
}

bool
PolicyTarget::running()
{
    return _running;
}

void
PolicyTarget::shutdown()
{
    _running = false;
}


void
PolicyTarget::create_term(const string& policy, const ConfigNodeId& order,
			  const string& term)
{
    _conf.create_term(policy, order, term);
}

void
PolicyTarget::delete_term(const string& policy, const string& term)
{
    _conf.delete_term(policy,term);
}

void
PolicyTarget::update_term_block(const string& policy,
				const string& term,
				const uint32_t& block,
				const ConfigNodeId& order,
				const string& statement)
{
    _conf.update_term_block(policy, term, block, order, statement);
}

void
PolicyTarget::create_policy(const string& policy)
{
    _conf.create_policy(policy);
}

void
PolicyTarget::delete_policy(const string& policy)
{
    _conf.delete_policy(policy);
}

void
PolicyTarget::create_set(const string& name)
{
    _conf.create_set(name);
}

void
PolicyTarget::update_set(const string& type, const string& name, 
			 const string& element)
{
    _conf.update_set(type, name, element);
}

void
PolicyTarget::delete_set(const string& name)
{
    _conf.delete_set(name);
}

void
PolicyTarget::add_to_set(const string& type, const string& name, 
			 const string& element)
{
    _conf.add_to_set(type, name, element);
}

void
PolicyTarget::delete_from_set(const string& type, const string& name, 
			      const string& element)
{
    _conf.delete_from_set(type, name, element);
}

void
PolicyTarget::parse_policies(const string& polin, POLICIES& polout, string& mod)
{
    if (polin.length() == 0)
	return;

    string p; // policies

    if (polin.at(0) == '#') {
	int idx = polin.find_last_of('#');

	p   = polin.substr(idx+1);
	mod = polin.substr(1, idx-1);
    } else
	p = polin;

    policy_utils::str_to_list(p, polout);
}

void
PolicyTarget::update_import(const string& protocol,
			    const string& policies)
{
    string   mod;
    POLICIES p;

    parse_policies(policies, p, mod);

    _conf.update_imports(protocol, p, mod);

    // commit after a bit, as we may get conf changes... especially on "global
    // conf change" or at startup
    _conf.commit(_commit_delay);
}

void
PolicyTarget::update_export(const string& protocol,
			    const string& policies)
{
    string   mod;
    POLICIES p;

    parse_policies(policies, p, mod);

    _conf.update_exports(protocol, p, mod);

    // try to aggregate commits by delaying them
    _conf.commit(_commit_delay);
}

void
PolicyTarget::add_varmap(const string& protocol, const string& variable,
			 const string& type, const string& access,
			 const VarRW::Id& id)
{
    _conf.add_varmap(protocol, variable, type, access, id);
}

void
PolicyTarget::commit(uint32_t msec)
{
    _conf.commit(msec);
}

string
PolicyTarget::dump_state(uint32_t id)
{
    return _conf.dump_state(id);
}

void
PolicyTarget::birth(const string& tclass, const string& /* tinstance */)
{
    _process_watch.birth(tclass);
}

void
PolicyTarget::death(const string& tclass, const string& /* tinstance */)
{
    // Remove the "import" and "export" dependencies for the protocol
    string protocol = _pmap.protocol(tclass);

    _conf.clear_imports(protocol);
    _conf.clear_exports(protocol);

    _process_watch.death(tclass);
}

void
PolicyTarget::set_proto_target(const string& protocol, const string& target)
{
    _pmap.set_xrl_target(protocol, target);
}
