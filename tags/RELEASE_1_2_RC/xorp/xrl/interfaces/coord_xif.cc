/*
 * Copyright (c) 2001-2005 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by 'clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/coord_xif.cc,v 1.13 2004/12/16 01:56:46 pavlin Exp $"

#include "coord_xif.hh"

bool
XrlCoordV0p1Client::send_command(
	const char*	the_tgt,
	const string&	command,
	const CommandCB&	cb
)
{
    Xrl x(the_tgt, "coord/0.1/command");
    x.args().add("command", command);
    return _sender->send(x, callback(this, &XrlCoordV0p1Client::unmarshall_command, cb));
}


/* Unmarshall command */
void
XrlCoordV0p1Client::unmarshall_command(
	const XrlError&	e,
	XrlArgs*	a,
	CommandCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(0));
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlCoordV0p1Client::send_status(
	const char*	the_tgt,
	const string&	peer,
	const StatusCB&	cb
)
{
    Xrl x(the_tgt, "coord/0.1/status");
    x.args().add("peer", peer);
    return _sender->send(x, callback(this, &XrlCoordV0p1Client::unmarshall_status, cb));
}


/* Unmarshall status */
void
XrlCoordV0p1Client::unmarshall_status(
	const XrlError&	e,
	XrlArgs*	a,
	StatusCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0);
	return;
    } else if (a && a->size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(1));
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    string status;
    try {
	a->get("status", status);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    cb->dispatch(e, &status);
}

bool
XrlCoordV0p1Client::send_pending(
	const char*	the_tgt,
	const PendingCB&	cb
)
{
    Xrl x(the_tgt, "coord/0.1/pending");
    return _sender->send(x, callback(this, &XrlCoordV0p1Client::unmarshall_pending, cb));
}


/* Unmarshall pending */
void
XrlCoordV0p1Client::unmarshall_pending(
	const XrlError&	e,
	XrlArgs*	a,
	PendingCB		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0);
	return;
    } else if (a && a->size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != %u)", XORP_UINT_CAST(a->size()), XORP_UINT_CAST(1));
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    bool pending;
    try {
	a->get("pending", pending);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    cb->dispatch(e, &pending);
}