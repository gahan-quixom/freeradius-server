/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
#ifndef _FR_APPLICATION_H
#define _FR_APPLICATION_H

#include <freeradius-devel/cf_util.h>
#include <freeradius-devel/dl.h>
#include <freeradius-devel/io/io.h>

/**
 * $Id$
 *
 * @file io/application.h
 * @brief Application interfaces.
 *
 * @copyright 2017 The FreeRADIUS project
 */

/*
 *	src/lib/io/schedule.h
 */
typedef struct fr_schedule_t fr_schedule_t;

/*
 *	src/lib/io/io.h
 */
typedef struct fr_io_op_t fr_io_op_t;

typedef int (*fr_app_open_t)(void *instance, fr_schedule_t *sc, CONF_SECTION *cs);
typedef int (*fr_app_instantiate_t)(void *instance, CONF_SECTION *cs);
typedef int (*fr_app_bootstrap_t)( void *instance, CONF_SECTION *cs);
/** Set the next state executed by the request to be one of the application subtype's entry points
 *
 * @param[in] request	To set the next state function for.
 */
typedef void (*fr_app_set_process_t)(REQUEST *request, void const *uctx);

/** Allows submodules to receive uctx data (a structure provided by their parent)
 *
 * @param[in] instance	of #fr_app_subtype_t or #fr_app_io_t.
 * @param[in] uctx	provided by caller.
 */
typedef void (*fr_app_set_uctx_t)(void *instance, void *uctx);

/** Describes a new application (protocol)
 *
 */
typedef struct {
	RAD_MODULE_COMMON;				//!< Common fields to all loadable modules.

	fr_app_bootstrap_t		bootstrap;
	fr_app_instantiate_t		instantiate;
	fr_app_open_t			open;		//!< Open listen sockets.
	fr_app_set_process_t		set_process;
} fr_app_t;

/** Public structure describing an application (protocol) specialisation
 *
 * Some protocols perform multiple distinct functions, and use
 * different state machines to perform those functions.
 */
typedef struct fr_app_subtype_t {
	RAD_MODULE_COMMON;				//!< Common fields to all loadable modules.

	fr_app_bootstrap_t		bootstrap;
	fr_app_instantiate_t		instantiate;
	fr_app_set_uctx_t		set_uctx;	//!< Allow the submodule to receive data from the main module.
	fr_io_process_t			process;	//!< Entry point into the protocol subtype's state machine.
} fr_app_subtype_t;

/** Public structure describing an I/O path for a protocol
 *
 * This structure is exported by I/O modules e.g. proto_radius_udp.
 */
typedef struct fr_app_io_t {
	RAD_MODULE_COMMON;				//!< Common fields to all loadable modules.

	fr_app_bootstrap_t		bootstrap;
	fr_app_instantiate_t		instantiate;
	fr_app_set_uctx_t		set_uctx;	//!< Allow the submodule to receive data from the main module.
	fr_io_op_t			op;		//!< Open/close/read/write functions for sending/receiving
							//!< protocol data.
} fr_app_io_t;
#endif
