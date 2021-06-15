/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <LibAudio/Buffer.h>
#include <LibAudio/ClientConnection.h>

namespace Audio {

ClientConnection::ClientConnection()
    : IPC::ServerConnection<AudioClientEndpoint, AudioServerEndpoint>(*this, "/tmp/portal/audio")
{
}



}