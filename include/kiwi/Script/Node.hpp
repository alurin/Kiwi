/*
 *******************************************************************************
 *   Copyright (C) 2012 Vasiliy Sheredeko
 *   MIT license. All Rights Reserved.
 *******************************************************************************
 */

#ifndef KIWI_SCRIPT_NODE_HPP_INCLUDED
#define KIWI_SCRIPT_NODE_HPP_INCLUDED

#include "kiwi/Types.hpp"

namespace kiwi {
    namespace script {
        /// Internal location class
        class location;

        /**
         * The Location class represents node's location in source stream
         */
        class KIWI_API Location {
        public:
            /// Default constructor
            Location(const String& streamName);

            /// Internal constructor
            KIWI_LOCAL Location(const location& loc);

            String getStreamName() const {
                return m_streamName;
            }
        protected:
            String m_streamName;
        };

        /// The Node class is abstract root for all syntaxis nodes
        class Node {
        public:
            /// Virtual node destructor
            virtual ~Node();

            Location getLocation() const {
                return m_location;
            }
        protected:
            /// Node location
            Location m_location;

            /// Protected constructor for children
            Node(const Location& location);
        };
    }
}

#endif