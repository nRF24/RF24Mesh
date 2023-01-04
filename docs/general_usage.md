# General Usage

@tableofcontents

## Network Design Options

1. **Static Network** (No Mesh)

   RF24Network can be configured manually, with a static design. RF24Mesh is not used at all. See [Network addressing](http://nRF24.github.io/RF24Network/md_docs_addressing.html)
2. **Static Network w/Dynamic Assignment**

   RF24Mesh is only used to acquire an address on startup. Nodes are generally expected to remain stationary. Changes to
   the network would be addressed manually, by adding, removing, or resetting nodes. Users can choose to use RF24Network functions directly, or use RF24Mesh.
3. **Dynamic Network & Assignment**

   Nodes join the mesh automatically and re-attach as required. This is the default and how the examples work.
4. **Hybrid Network**

   Utilizes a combination of static & dynamic nodes. Requires initial planning and deployment, but can result in a more stable network, easing
   the use of sleeping nodes.

## Network Management

RF24Network addresses can be viewed as MAC addresses, and RF24Mesh nodeIDs
viewed as static IP addresses. When joining or re-attaching to the network,
nodes will request a RF24Network address, and are identified via nodeID.

### Raspberry Pi/Linux

On Linux devices, the RF24Gateway will save address assignments to file
(dhcplist.txt) so they will be restored, even if the gateway is restarted.
To force network re-convergence, delete the dhcplist.txt file and restart the
gateway. If nodes are configured to verify their connection at a set interval,
they will come back online in time.

### Arduino/AVR

On all other devices, the address list is not saved. To force network re-convergence,
restart the gateway. If nodes are configured to verify their connection at a set
interval, they will come back online in time.

If a node/nodeID is removed from the network permanently, the address should be
released prior to removal. If it is not, the assigned RF24Network address can be
written to 0 in the RF24Mesh address list.

## Mesh Communication

RF24Mesh nodeIDs are unique identifiers, while RF24Network addresses change
dynamically within a statically defined structure. Due to this structure, it is
simple for any node to communicate with the master node, since the RF24Network
address is always known (00). Conversely, the master node maintains a list of
every node on the network, so address 'lookups' return immediately.

Communication from node-to-node requires address queries to be sent to the master
node, since individual nodes may change RF24Network & radio address at any time.
Due to the extra data transmissions, node-to-node communication is less efficient.

## Tricks of Trade

One thing to keep in mind is the dynamic nature of RF24Mesh, and the need to
verify connectivity to the network. For nodes that are constantly transmitting,
(every few seconds at most) it is suitable to check the connection, and/or renew
the address when connectivity fails. Since data is not saved by the master
node, if the master node goes down, all child nodes must renew their address.
In this case, as long as the master node is down for a few seconds, the nodes
will all begin requesting an address.

Nodes that are not actively transmitting, should be configured to test their
connection at predefined intervals, to allow them to reconnect as necessary.

In the case of sleeping nodes, or nodes that will only be online temporarily,
it is generally suitable to release the address prior to going offline, and
requesting an address upon waking. Keep in mind, address requests can generally
take anywhere from 10 - 15ms, up to few seconds in most cases.

One of the recently introduced features is the ability to transmit payloads without the network returning a network-ack response. If solely using this method
of transmission, the node should also be configured to verify its connection via `RF24Mesh::checkConnection()` periodically, to ensure connectivity.

## RF24Network

Beyond requesting and releasing addresses, usage is outlined in the RF24Mesh class documentation, and further information regarding RF24Network is available at
[RF24Network documentation](http://nRF24.github.io/RF24Network).
