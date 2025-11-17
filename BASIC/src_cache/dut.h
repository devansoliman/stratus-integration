#ifndef _DUT_H_
#define _DUT_H_
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2022 Cadence Design Systems, Inc. All rights reserved worldwide.
//
// The code contained herein is the proprietary and confidential information
// of Cadence or its licensors, and is supplied subject to a previously
// executed license and maintenance agreement between Cadence and customer.
// This code is intended for use with Cadence high-level synthesis tools and
// may not be used with other high-level synthesis tools. Permission is only
// granted to distribute the code as indicated. Cadence grants permission for
// customer to distribute a copy of this code to any partner to aid in designing
// or verifying the customer's intellectual property, as long as such
// distribution includes a restriction of no additional distributions from the
// partner, unless the partner receives permission directly from Cadence.
//
// ALL CODE FURNISHED BY CADENCE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, AND CADENCE SPECIFICALLY DISCLAIMS ANY WARRANTY OF NONINFRINGEMENT,
// FITNESS FOR A PARTICULAR PURPOSE OR MERCHANTABILITY. CADENCE SHALL NOT BE
// LIABLE FOR ANY COSTS OF PROCUREMENT OF SUBSTITUTES, LOSS OF PROFITS,
// INTERRUPTION OF BUSINESS, OR FOR ANY OTHER SPECIAL, CONSEQUENTIAL OR
// INCIDENTAL DAMAGES, HOWEVER CAUSED, WHETHER FOR BREACH OF WARRANTY,
// CONTRACT, TORT, NEGLIGENCE, STRICT LIABILITY OR OTHERWISE.
//
////////////////////////////////////////////////////////////////////////////////

#include <cynw_p2p.h>
#include "defines.h"

SC_MODULE( dut )
{
    // Declare the clock and reset ports
    sc_in_clk clk {"clk"};
    sc_in < bool > rst {"rst"};

    // Declare the input port and the output port.
    // The template specializations <DT_*> configure the
    // modular interfaces to carry the desired datatypes.
    cynw_p2p < DT_1, ioConfig >::in din {"din"};    // The input port
    cynw_p2p < DT_2, ioConfig >::out dout {"dout"};  // The output port

    SC_CTOR( dut )
    {
        SC_CTHREAD( thread_function, clk.pos() );
        reset_signal_is( rst, 0 );

        // Give the interfaces access to the clock and reset ports
        din.clk_rst( clk, rst );
        dout.clk_rst( clk, rst );
    }

    void thread_function();                   // the thread function

    DT_2 f( DT_1 );             // the computation function
};

#endif // _DUT_H_
