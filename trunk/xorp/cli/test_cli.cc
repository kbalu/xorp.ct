// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2003 International Computer Science Institute
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

#ident "$XORP: xorp/cli/test_cli.cc,v 1.25 2004/03/02 05:27:20 pavlin Exp $"


//
// CLI (Command-Line Interface) test program.
//


#include "cli_module.h"
#include "libxorp/xorp.h"

#include <netdb.h>

#include "libxorp/xlog.h"
#include "libxorp/eventloop.hh"
#include "libxorp/exceptions.hh"
#include "libxorp/xlog.h"

#include "libxipc/finder_server.hh"
#include "libxipc/xrl_std_router.hh"

#include "cli_client.hh"
#include "xrl_cli_node.hh"


//
// Local variables
//
CliNode *global_cli_node = NULL;

//
// Local functions prototypes
//
static	bool wakeup_hook();
static	bool wakeup_hook2(int, int);
static	CliNode& cli_node();
static	void usage(const char *argv0, int exit_value);

/**
 * usage:
 * @argv0: Argument 0 when the program was called (the program name itself).
 * @exit_value: The exit value of the program.
 *
 * Print the program usage.
 * If @exit_value is 0, the usage will be printed to the standart output,
 * otherwise to the standart error.
 **/
static void
usage(const char *argv0, int exit_value)
{
    FILE *output;
    const char *progname = strrchr(argv0, '/');

    if (progname != NULL)
	progname++;		// Skip the last '/'
    if (progname == NULL)
	progname = argv0;

    //
    // If the usage is printed because of error, output to stderr, otherwise
    // output to stdout.
    //
    if (exit_value == 0)
	output = stdout;
    else
	output = stderr;

    fprintf(output, "Usage: %s [-F <finder_hostname>[:<finder_port>]]\n",
	    progname);
    fprintf(output, "           -F <finder_hostname>[:<finder_port>]  : finder hostname and port\n");
    fprintf(output, "           -h                                    : usage (this message)\n");
    fprintf(output, "\n");
    fprintf(output, "Program name:   %s\n", progname);
    fprintf(output, "Module name:    %s\n", XORP_MODULE_NAME);
    fprintf(output, "Module version: %s\n", XORP_MODULE_VERSION);

    exit (exit_value);

    // NOTREACHED
}

//
// Wait until the XrlRouter becomes ready
//
static void
wait_until_xrl_router_is_ready(EventLoop& eventloop, XrlRouter& xrl_router)
{
    bool timed_out = false;

    XorpTimer t = eventloop.set_flag_after_ms(10000, &timed_out);
    while (xrl_router.ready() == false && timed_out == false) {
	eventloop.run();
    }

    if (xrl_router.ready() == false) {
	XLOG_FATAL("XrlRouter did not become ready.  No Finder?");
    }
}

class Foo {
public:
    Foo() {}
    bool print() { printf("Hello %p\n", this); return (1); }
};

int
add_my_cli_commands(CliNode& cli_node);

static CliNode&
cli_node()
{
    return (*global_cli_node);
}

int
main(int argc, char *argv[])
{
    int ch;
    const char *argv0 = argv[0];
    char *finder_hostname_port = NULL;
    IPv4 finder_addr = FINDER_DEFAULT_HOST;
    uint16_t finder_port = FINDER_DEFAULT_PORT;	// XXX: host order

    //
    // Initialize and start xlog
    //
    xlog_init(argv[0], NULL);
    xlog_set_verbose(XLOG_VERBOSE_LOW);	// Least verbose messages
    // XXX: verbosity of the error messages temporary increased
    xlog_level_set_verbose(XLOG_LEVEL_ERROR, XLOG_VERBOSE_HIGH);
    xlog_add_default_output();
    xlog_start();

    //
    // Get the program options
    //
    while ((ch = getopt(argc, argv, "F:h")) != -1) {
	switch (ch) {
	case 'F':
	    // Finder hostname and port
	    finder_hostname_port = optarg;
	    break;
	case 'h':
	case '?':
	    // Help
	    usage(argv0, 0);
	    break;
	default:
	    usage(argv0, 1);
	    break;
	}
    }
    argc -= optind;
    argv += optind;
    if (argc != 0) {
	usage(argv0, 1);
	// NOTREACHED
    }

    //
    // Get the finder hostname and port
    //
    if (finder_hostname_port != NULL) {
	char buf[MAXHOSTNAMELEN + 1];
	char *p;
	struct hostent *h;

	// Get the finder address
	strcpy(buf, finder_hostname_port);
	p = strrchr(buf, ':');
	if (p != NULL)
	    *p = '\0';
	h = gethostbyname(buf);
	if (h == NULL) {
	    fprintf(stderr, "Can't resolve IP address for %s: %s\n",
		    buf, hstrerror(h_errno));
	    usage(argv0, 1);
	}

	try {
	    IPvX addr(h->h_addrtype, (uint8_t *)h->h_addr_list[0]);
	    finder_addr = addr.get_ipv4();
	} catch (const InvalidFamily&) {
	    fprintf(stderr, "Invalid finder address family: %d\n",
		    h->h_addrtype);
	    usage(argv0, 1);
	} catch (const InvalidCast&) {
	    fprintf(stderr,
		    "Invalid finder address family: %d (expected IPv4)\n",
		    h->h_addrtype);
	    usage(argv0, 1);
	}

	// Get the finder port
	strcpy(buf, finder_hostname_port);
	p = strrchr(buf, ':');
	if (p != NULL) {
	    p++;
	    finder_port = static_cast<uint16_t>(atoi(p));
	    if (finder_port == 0) {
		fprintf(stderr, "Invalid finder port: %d\n", finder_port);
		usage(argv0, 1);
	    }
	}
    }

    //
    // The main body
    //
    try {
	Foo f;
	EventLoop eventloop;

	//
	// Finder
	//
	FinderServer *finder = NULL;
	if (finder_hostname_port == NULL) {
	    // Start our own finder
	    try {
		finder = new FinderServer(eventloop, finder_port, finder_addr);
	    } catch (const InvalidPort&) {
		XLOG_FATAL("Could not start in-process Finder");
	    }
	    finder_addr = finder->addr();
	    finder_port = finder->port();
	}

	//
	// CLI
	//
	// XXX: we use a single CLI node to handle both IPv4 and IPv6
	CliNode cli_node(AF_INET, XORP_MODULE_CLI, eventloop);
	cli_node.set_cli_port(12000);

	//
	// CLI access
	//
	IPvXNet enable_ipvxnet1("127.0.0.1/32");
	// IPvXNet enable_ipvxnet2("192.150.187.0/25");
	IPvXNet disable_ipvxnet1("0.0.0.0/0");	// Disable everything else
	//
	cli_node.add_enable_cli_access_from_subnet(enable_ipvxnet1);
	// cli_node.add_enable_cli_access_from_subnet(enable_ipvxnet2);
	cli_node.add_disable_cli_access_from_subnet(disable_ipvxnet1);

	//
	// Create and configure the CLI XRL interface
	//
	XrlStdRouter xrl_std_router_cli(eventloop, cli_node.module_name(),
					finder_addr, finder_port);
	XrlCliNode xrl_cli_node(&xrl_std_router_cli, cli_node);
	wait_until_xrl_router_is_ready(eventloop, xrl_std_router_cli);

#if 0
#ifdef HAVE_IPV6
	CliNode cli_node6(AF_INET6, XORP_MODULE_CLI, eventloop);
	cli_node6.set_cli_port(12000);
	XrlStdRouter xrl_std_router_cli6(eventloop, cli_node6.module_name(),
					 finder_addr, finder_port);
	XrlCliNode xrl_cli_node(&xrl_std_router_cli6, cli_node6);
	wait_until_xrl_router_is_ready(eventloop, xrl_std_router_cli6);
#endif // HAVE_IPV6
#endif // 0


	//
	// XXX: CLI test-specific setup
	//
	global_cli_node = &cli_node;
	add_my_cli_commands(cli_node);
	// add_my_cli_commands(cli_node6);

	//
	// Start the nodes
	//
	cli_node.enable();
	cli_node.start();
	// cli_node6.enable();
	// cli_node6.start();

	// Test timer
	XorpTimer wakeywakey = eventloop.new_periodic(1000, callback(wakeup_hook));
	XorpTimer wakeywakey2 = eventloop.new_periodic(5000, callback(wakeup_hook2, 3, 5));
	XorpTimer wakeywakey3 = eventloop.new_periodic(2000, callback(f, &Foo::print));

	//
	// Main loop
	//
	for (;;) {
	    eventloop.run();
	}

    } catch(...) {
	xorp_catch_standard_exceptions();
    }

    //
    // Gracefully stop and exit xlog
    //
    xlog_stop();
    xlog_exit();

    exit (0);
}

static bool
wakeup_hook()
{
    fprintf(stdout, "%s\n", xlog_localtime2string());
    fflush(stdout);

    return (true);
}

static bool
wakeup_hook2(int a, int b)
{
    fprintf(stdout, "%s ARGS = %d %d\n", xlog_localtime2string(), a, b);
    fflush(stdout);

    return (true);
}

int
cli_myset_func(const string& ,		// server_name
	       const string& cli_term_name,
	       uint32_t ,		// cli_session_id
	       const string& command_global_name,
	       const vector<string>& argv)
{
    CliClient *cli_client = cli_node().find_cli_by_term_name(cli_term_name);
    if (cli_client == NULL)
	return (XORP_ERROR);

    if (command_global_name.size() > 0)
	cli_client->cli_print(c_format("MYSET_FUNC command_global_name = %s\n",
				       command_global_name.c_str()));
    for (size_t i = 0; i < argv.size(); i++)
	cli_client->cli_print(c_format("MYSET_FUNC arg = %s\n",
				       argv[i].c_str()));

    return (XORP_OK);
}

int
cli_print(const string& ,		// server_name
	  const string& cli_term_name,
	  uint32_t ,			// cli_session_id
	  const string& ,		// command_global_name,
	  const vector<string>& argv)
{
    CliClient *cli_client = cli_node().find_cli_by_term_name(cli_term_name);
    if (cli_client == NULL)
	return (XORP_ERROR);

    if (argv.size() > 0) {
	cli_client->cli_print("Error: unexpected arguments\n");
	return (XORP_ERROR);
    }

    //
    // Test to print a number of lines at once
    //
    string my_string = "";
    for (uint32_t i = 0; i < 100; i++)
	my_string += c_format("This is my multi-line number %u\n", i);
    cli_client->cli_print(my_string);

    //
    // Test to print a number of lines one-by-one
    //
    for (uint32_t i = 0; i < 100; i++)
	cli_client->cli_print(c_format("This is my line number %u\n", i));


    return (XORP_OK);
}

int
cli_print2(const string& ,		// server_name
	   const string& cli_term_name,
	   uint32_t ,			// cli_session_id
	   const string& ,		// command_global_name,
	   const vector<string>& argv)
{
    CliClient *cli_client = cli_node().find_cli_by_term_name(cli_term_name);
    if (cli_client == NULL)
	return (XORP_ERROR);

    if (argv.size() > 0) {
	cli_client->cli_print("Error: unexpected arguments\n");
	return (XORP_ERROR);
    }

    for (int i = 0; i < 10; i++)
	cli_client->cli_print(c_format("This is my line number %d\n", i));

    return (XORP_OK);
}

int
cli_print_wide(const string& ,		// server_name
	       const string& cli_term_name,
	       uint32_t ,		// cli_session_id
	       const string& ,		// command_global_name,
	       const vector<string>& argv)
{
    CliClient *cli_client = cli_node().find_cli_by_term_name(cli_term_name);
    if (cli_client == NULL)
	return (XORP_ERROR);

    if (argv.size() > 0) {
	cli_client->cli_print("Error: unexpected arguments\n");
	return (XORP_ERROR);
    }

    string trail = "111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999";
#if 1
    for (int i = 0; i < 200; i++)
	cli_client->cli_print(c_format("This is my line number %d %s %d\n",
				       i, trail.c_str(), i));
#endif

    return (XORP_OK);
}

int
add_my_cli_commands(CliNode& cli_node)
{
    CliCommand *com0, *com1, *com2, *com3;

    com0 = cli_node.cli_command_root();
#if 0
    com2 = com0->add_command("myset", "Set my variable", cli_myset_func);
    com1 = com0->add_command("myshow", "Show my information", "Myshow> ");
    com2 = com1->add_command("version", "Show my information about system");
    com3 = com2->add_command("pim", "Show my information about PIM");
    com3 = com2->add_command("igmp", "Show my information about IGMP");

    com1 = com0->add_command("myset2", "Set my variable2", cli_myset_func);
    com1 = com0->add_command("myshow2", "Show my information2", "Myshow2> ");

    com1 = com0->add_command("print", "Print numbers", cli_print);
    com1 = com0->add_command("print2", "Print few numbers", cli_print2);
    com1 = com0->add_command("print_wide", "Print wide lines", cli_print_wide);
#endif
    com2 = com0->add_command("myset", "Set my variable", cli_myset_func);
    com1 = com0->add_command("myshow", "Show my information", "Myshow> ");
    com2 = com0->add_command("myshow version",
			     "Show my information about system");
    com3 = com0->add_command("myshow version pim",
			     "Show my information about PIM");
    com3 = com0->add_command("myshow version igmp",
			     "Show my information about IGMP");

    com1 = com0->add_command("myset2", "Set my variable2", cli_myset_func);
    com1 = com0->add_command("myshow2", "Show my information2", "Myshow2> ");

    com1 = com0->add_command("print", "Print numbers", cli_print);
    com1 = com0->add_command("print2", "Print few numbers", cli_print2);
    com1 = com0->add_command("print_wide", "Print wide lines", cli_print_wide);

    return (XORP_OK);
}

