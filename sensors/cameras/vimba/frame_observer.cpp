// This file is part of snark, a generic and flexible library for robotics research
// Copyright (c) 2016 The University of Sydney
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

#include <iostream>
#include "frame.h"
#include "frame_observer.h"

namespace snark { namespace vimba {

frame_observer::frame_observer( AVT::VmbAPI::CameraPtr camera
                              , std::vector< snark::cv_mat::filter > filters
                              , boost::shared_ptr< snark::cv_mat::serialization > serialization )
    : IFrameObserver( camera )
    , filters_( filters )
    , serialization_( serialization )
    , last_frame_id_( 0 )
    , output_null_( false )
{
    if( !filters_.empty() )
    {
        // Check if the last filter is "null"
        if( !filters_.back().filter_function )
        {
            // In which case remove it and disable output
            output_null_ = true;
            filters_.pop_back();
        }
    }
}

void frame_observer::FrameReceived( const AVT::VmbAPI::FramePtr frame_ptr )
{
    // Take the timestamp immediately
    boost::posix_time::ptime timestamp( boost::posix_time::microsec_clock::universal_time() );

    frame frame( frame_ptr );

    if( frame.status() == VmbFrameStatusComplete )
    {
        if( last_frame_id_ != 0 )
        {
            VmbUint64_t missing_frames = frame.id() - last_frame_id_ - 1;
            if( missing_frames > 0 )
            {
                std::cerr << "Warning: " << missing_frames << " missing frame"
                          << ( missing_frames == 1 ? "" : "s" )
                          << " detected" << std::endl;
            }
        }
        last_frame_id_ = frame.id();

        frame::pixel_format_desc fd = frame.format_desc();

        cv::Mat cv_mat( frame.height()
                      , frame.width() * fd.width_adjustment
                      , fd.type
                      , frame.image_buffer() );

        snark::cv_mat::filters::value_type timestamped_data( std::make_pair( timestamp, cv_mat ));
        snark::cv_mat::filters::value_type filtered_data = snark::cv_mat::filters::apply( filters_, timestamped_data );
        if( !output_null_ ) serialization_->write( std::cout, filtered_data );
    }
    else
    {
        std::cerr << "Warning: frame " << frame.id() << " status " << frame.status_as_string() << std::endl;
    }

    m_pCamera->QueueFrame( frame_ptr );
}

} } // namespace snark { namespace vimba {
