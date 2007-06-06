// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2007 International Computer Science Institute
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

// $XORP: xorp/fea/data_plane/ifconfig/ifconfig_get_getifaddrs.hh,v 1.1 2007/06/05 09:48:52 greenhal Exp $

#ifndef __FEA_DATA_PLANE_IFCONFIG_IFCONFIG_GET_GETIFADDRS_HH__
#define __FEA_DATA_PLANE_IFCONFIG_IFCONFIG_GET_GETIFADDRS_HH__


#include "fea/ifconfig_get.hh"

class IfConfigGetGetifaddrs : public IfConfigGet {
public:
    IfConfigGetGetifaddrs(IfConfig& ifconfig);
    virtual ~IfConfigGetGetifaddrs();

    /**
     * Start operation.
     * 
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg);
    
    /**
     * Stop operation.
     * 
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg);

    /**
     * Pull the network interface information from the underlying system.
     * 
     * @param config the IfTree storage to store the pulled information.
     * @return true on success, otherwise false.
     */
    virtual bool pull_config(IfTree& config);

    /**
     * Parse information about network interface configuration change from
     * the underlying system.
     * 
     * The information to parse is in "struct ifaddrs" format
     * (e.g., obtained by getifaddrs(3) mechanism).
     * 
     * @param ifconfig the IfConfig instance.
     * @param it the IfTree storage to store the parsed information.
     * @param ifap a linked list of the network interfaces on the
     * local machine.
     * @return true on success, otherwise false.
     * @see IfTree.
     */
    static bool parse_buffer_getifaddrs(IfConfig& ifconfig, IfTree& it,
					const struct ifaddrs* ifap);

private:
    bool read_config(IfTree& it);
};

#endif // __FEA_DATA_PLANE_IFCONFIG_IFCONFIG_GET_GETIFADDRS_HH__