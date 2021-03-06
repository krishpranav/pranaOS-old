
#pragma once

#include <Kernel/Net/NetworkAdapter.h>
#include <Kernel/Thread.h>

namespace Kernel {

struct RoutingDecision {
    RefPtr<NetworkAdapter> adapter;
    MACAddress next_hop;

    bool is_zero() const;
};

void update_arp_table(const IPv4Address&, const MACAddress&);
RoutingDecision route_to(const IPv4Address& target, const IPv4Address& source, const RefPtr<NetworkAdapter> through = nullptr);

Lockable<HashMap<IPv4Address, MACAddress>>& arp_table();

}
