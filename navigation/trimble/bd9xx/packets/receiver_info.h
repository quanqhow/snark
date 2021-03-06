// This file is part of snark, a generic and flexible library for robotics research
// Copyright (c) 2011 The University of Sydney
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the University of Sydney nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/// @author vsevolod vlaskine

#ifndef SNARK_NAVIGATION_TRIMBLE_BD9XX_PACKETS_RECEIVER_INFO_H_
#define SNARK_NAVIGATION_TRIMBLE_BD9XX_PACKETS_RECEIVER_INFO_H_

#include <comma/base/exception.h>
#include <comma/packed/string.h>
#include <comma/packed/big_endian.h>
#include "../packet.h"

namespace snark { namespace trimble { namespace bd9xx { namespace packets {

struct receiver_info // getserial
{
    struct request { typedef simple_packet< 0x06 > packet; };
    
    struct response
    {
        struct data : public comma::packed::packed_struct< data, 158 >
        {
            comma::packed::string< 8 > receiver_serial_number;
            comma::packed::string< 8 > receiver_type;
            comma::packed::string< 5 > nav_process_version;
            comma::packed::string< 5 > sig_process_version;
            comma::packed::string< 5 > boot_rom_version;
            comma::packed::string< 8 > antenna_serial_number;
            comma::packed::string< 2 > antenna_type;
            comma::packed::string< 2 > number_of_channels;
            comma::packed::string< 2 > number_of_channels_l1;
            comma::packed::string< 10 > long_serial_number;
            comma::packed::string< 31 > local_long_antenna_serial_number;
            comma::packed::string< 31 > base_long_antenna_serial_number;
            comma::packed::string< 31 > base_ngs_antenna_descriptor;
            comma::packed::string< 2 > number_of_usable_channels;
            comma::packed::string< 2 > number_of_physical_channels;
            comma::packed::string< 1 > number_of_simultaneous_channels;
            comma::packed::string< 5 > antenna_ini_version;
        };
    
        typedef bd9xx::fixed_packet< 0x07, data > packet;
    };
};
    
} } } } // namespace snark { namespace trimble { namespace bd9xx { namespace packets {

#endif // SNARK_NAVIGATION_TRIMBLE_BD9XX_PACKETS_RECEIVER_INFO_H_
