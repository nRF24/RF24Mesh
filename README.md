<!-- markdownlint-disable MD041 MD034 -->
[![Linux build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_linux.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_linux.yml)
[![Arduino CLI build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_arduino.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_arduino.yml)
[![PlatformIO build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_platformIO.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_platformIO.yml)
[![Pico SDK build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_rp2xxx.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_rp2xxx.yml)
[![Documentation Status](https://readthedocs.org/projects/rf24mesh/badge/?version=latest)](https://rf24mesh.readthedocs.io/en/latest/?badge=latest)

# RF24Mesh

Mesh Networking for RF24Network

RF24Mesh lets a group of radios form a wireless network that largely runs itself. Nodes get their
addresses automatically, pass traffic along for each other, and reconnect if they move or lose their
connection. It works with nRF24L01(+) radios on Arduino and Linux devices such as the Raspberry Pi,
and with nRF52x and nRF54x devices using the [nrf_to_nrf](https://github.com/TMRh20/nrf_to_nrf) library.

<img width="2560" height="1280" alt="Image" src="https://github.com/user-attachments/assets/4a60ca90-fea2-43ba-9079-f6b11e54d5aa" />

## The basics

**The master node** runs the network. It always has NodeID 0, assigns addresses to the other nodes, and
answers address lookups, much like DHCP and DNS on a home network. Every network has exactly one master,
and it needs to stay online.

**Each node has two identifiers:**

| | NodeID | RF24Network address |
| :--- | :--- | :--- |
| **Set by** | You, in each node's code | The master, automatically |
| **Changes?** | No | Yes, when a node moves or reconnects |
| **Used for** | Identifying a node | Routing data through the network |
| **Comparable to** | A static IP address | A MAC address |

**NodeIDs** can be from 1 to 254, and each node needs its own. If you use RF24Gateway, the gateway's IP
address also uses up a NodeID, typically 1 or 2, so it's a good habit to start numbering your nodes from 3.

**Nodes don't need to be in range of the master.** They can connect through other nodes, up to 4 hops away.
For more detail, see the [network topology guide](https://nrf24.github.io/pyRF24/topology.html).

<img width="1208" height="926" alt="Image" src="https://github.com/user-attachments/assets/edb76517-5679-44c7-83a9-6e9840345e66" />

## Installation

**Arduino:** In the Library Manager, install **RF24Mesh** and accept the prompt to also install RF24 and
RF24Network.

**Linux:** Run the installer, answer `y` for RF24, RF24Network and RF24Mesh, and let it build the examples.
It creates a `rf24libs` folder in the current directory, which the commands below assume is in your home
directory.

```shell
wget https://raw.githubusercontent.com/nRF24/.github/main/installer/install.sh
chmod +x install.sh
./install.sh
```

If you build the libraries manually, build and install them in this order: RF24, RF24Network, RF24Mesh. See
the [Linux installation guide](https://nrf24.github.io/RF24/md_docs_2linux__install.html) for wiring and other
options.

## Get a test network running

Test one layer at a time, so that if something fails, you know where to look. On Linux, set your radio's pins
in each example's `.cpp` file, run `make` in the `build` folder next to it, then start the program with `sudo`.

| Step | Run this on two devices | Linux folder under `~/rf24libs` | It's working when |
| :--- | :--- | :--- | :--- |
| 1. Radio | RF24 `GettingStarted` (Linux: `gettingstarted`) | `RF24/examples_linux` | The transmitter prints `Transmission successful!` |
| 2. Network | RF24Network `helloworld_rx` and `helloworld_tx` | `RF24Network/examples_RPi` | The receiver prints a packet every 2 seconds |
| 3. Mesh | RF24Mesh `RF24Mesh_Example_Master` and `RF24Mesh_Example` | `RF24Mesh/examples_RPi` | The node prints `Send OK:` every second |

For step 3, change the NodeID in the node's example to 2 (`#define nodeID 2` on Arduino,
`mesh.setNodeID(2);` on Linux). The Arduino master also prints its address table every 5 seconds:

```text
********Assigned Addresses********
NodeID: 2 RF24Network Address: 05
**********************************
```

The examples use the radio's lowest power setting, so keep the devices close together while testing.

To see the mesh in action, add another node with NodeID 3, then power it off and on again. It should
reconnect by itself. If you place a node out of range of the master but in range of another node, it gets a
longer address such as `045`. Each extra digit is one more hop from the master.

## Writing your own code

Start from the example closest to what you need. The code below shows the main loop only, and the setup is
the same as in the examples.

### Master node

```cpp
void loop() {
  mesh.update();  // keeps the network running
  mesh.DHCP();    // assigns addresses; call immediately after mesh.update()

  while (network.available()) {
    RF24NetworkHeader header;
    uint32_t value = 0;
    network.read(header, &value, sizeof(value));

    // Find out which node sent this
    int16_t fromNodeID = mesh.getNodeID(header.from_node);
  }
}
```

### Regular node

Nodes need to notice when they lose their connection, and recover from it:

```cpp
uint32_t lastSend = 0;

void reconnectIfNeeded() {
  if (!mesh.checkConnection()) {
    // Blocks for up to 7.5 seconds by default
    if (mesh.renewAddress() == MESH_DEFAULT_ADDRESS) {
      mesh.begin();  // resets the radio, which recovers from most radio errors
    }
  }
}

void loop() {
  mesh.update();  // also routes traffic for other nodes

  if (millis() - lastSend >= 1000) {
    lastSend = millis();
    uint32_t value = millis();

    if (!mesh.write(&value, 'M', sizeof(value))) {
      reconnectIfNeeded();
    }
  }
}
```

### Good habits

- **Check the connection on a timer if a node rarely sends.** A node that mostly listens has no failed
  write to warn it. Call `reconnectIfNeeded()` about every 30 seconds.
- **Avoid long delays.** A node only routes traffic for others while `mesh.update()` is being called, so use
  `millis()` timers instead of `delay()`.
- **Choose message types deliberately.** The network acknowledges types 65-127 but not types 1-64. `'M'` is 77,
  so it's acknowledged. Without acknowledgements, a write can't tell you if the connection is working.
- **Sending to another node costs more.** Add the NodeID to send to a node other than the master. Each of these
  writes asks the master for the current address first:

  ```cpp
  mesh.write(&value, 'M', sizeof(value), 3);  // send to NodeID 3
  ```

- **Release the address before sleeping.** Call `mesh.releaseAddress()` before powering down, then
  `mesh.renewAddress()` after waking.

## Linux or Arduino for the master?

Linux is the better choice if the master handles many nodes or large payloads.

| | Linux master | Arduino master |
| :--- | :--- | :--- |
| **Address list after a restart** | Restored from `dhcplist.txt` | Empty; all nodes must re-join |
| **Reassembling large payloads** | From many nodes at the same time | From one node at a time |
| **Maximum payload (default)** | 1514 bytes | 1514 bytes at 45 MHz or faster, otherwise 144 bytes |

**Restarting a Linux master with an empty list:** stop the master, delete `dhcplist.txt`, then start it again.
The file is in the directory the master was run from. Running the master from a different directory gives it a
separate address list.

**Restarting an Arduino master:** keep it off for longer than the interval at which nodes check their connection,
so they all renew their addresses. By default nodes only check the link to their parent, so allow extra time on
networks with several levels.

## Configuration

These settings are in `RF24Mesh_config.h`. On Linux, rebuild and reinstall the library after changing them, then
rebuild your program.

| Setting | Default | What it does |
| :--- | :--- | :--- |
| `MESH_MEM_ALLOC_SIZE` | 10 | How many entries the master's address list adds at a time. This isn't a limit on nodes. On a non-Linux master, set it to the number of nodes you expect, to avoid repeated re-allocation. |
| `RF24MESH_CONN_CHECK_TYPE` | `RF24MESH_CONN_CHECK_PARENT` | What `checkConnection()` contacts. See below. |
| `MESH_MAX_CHILDREN` | 4 | Maximum child nodes per node, from 1 to 4. |
| `MESH_NOMASTER` | Off | Saves memory on any node that isn't the master. |

**Connection checks:** by default, a node only checks the link to its parent. Setting the check type to
`RF24MESH_CONN_CHECK_MASTER` makes it check with the master instead. This suits nodes that mostly receive data
and rarely check their connection, and it prevents some temporary denial-of-service scenarios. However, it adds
traffic and load on the master, and can make large or busy networks less reliable.

## Optional extras

### Static nodes

RF24Network nodes with a fixed address can share a network with mesh nodes, and other nodes can connect through
them. This suits routing nodes that won't move. Static nodes must stay in place, because they don't reconnect by
themselves.

```cpp
// On the master: reserve address 02 for NodeID 23
mesh.setNodeID(0);
mesh.setAddress(23, 02);

// On the static node: use RF24Network directly, on the mesh's channel
radio.begin();
radio.setChannel(MESH_DEFAULT_CHANNEL);  // 97 unless changed
network.begin(02);
```

### TCP/IP

<img width="1600" height="1028" alt="Image" src="https://github.com/user-attachments/assets/cd797bc5-9667-4109-b189-74cd208cbb88" />

[RF24Ethernet](https://nrf24.github.io/RF24Ethernet/) and [RF24Gateway](https://nrf24.github.io/RF24Gateway/) run
protocols such as HTTP, MQTT and SSH over the mesh. The mesh itself works the same way underneath, and small
RF24Mesh messages can travel alongside large TCP/IP packets. It's worth getting comfortable with plain RF24Mesh
first.

A non-master gateway started without a NodeID uses NodeID 253, so avoid using 253 for another node on those
networks.

### Encryption

- **nRF52840 and nRF54x:** the [nrf_to_nrf](https://github.com/TMRh20/nrf_to_nrf) library provides AES-CCM encryption.
- **Linux with nRF24L01(+):** build RF24Network, RF24Mesh and RF24Gateway with the `RF24NETWORK_CCM` option, which
  the installer offers. These nodes can communicate with encrypted nRF52840 and nRF54x nodes.
- **Arduino with nRF24L01(+):** encryption isn't available at the radio layer. With RF24Ethernet, use TLS or a similar
  protocol at the application layer.

## Troubleshooting

Most mesh problems start lower in the stack, so first make sure the RF24 and RF24Network tests above work. Also
see the [RF24 Common Issues](https://nrf24.github.io/RF24/md_COMMON__ISSUES.html) page.

| Problem | What to check |
| :--- | :--- |
| `Radio hardware not responding` | The device can't talk to its radio. Check the wiring and the CE and CS pins in your code. |
| `Could not connect to network` repeats | Is the master running and calling `mesh.update()` and `mesh.DHCP()`? Is the node in range of the master or a connected node? Do all nodes use the same channel and data rate? |
| Two nodes take turns connecting | They probably have the same NodeID. |
| Unreliable connections | Check the radio's power supply, particularly with PA/LNA modules. Look for `delay()` or other slow code on routing nodes. See [Tuning the Network](https://nrf24.github.io/RF24Network/md_docs_2tuning.html). |
| A node stays offline after the network changes | Make sure it checks its connection regularly and renews its address when the check fails. |
| Writes to the master work, but not to another node | Is the other node connected? On large networks, try raising `MESH_LOOKUP_TIMEOUT` and `MESH_WRITE_TIMEOUT`. |
| Large payloads fail | Check the maximum payload size for your device (ATTiny is limited to 24 bytes). An Arduino receiving large payloads from several nodes at once can lose some, so use a Linux master or stagger the transmissions. |
| Lookups fail after restarting the master | See "Linux or Arduino for the master?" above. |
| Build errors after upgrading | RF24Network and RF24Mesh must both be v2 or newer. The Linux installer won't overwrite existing folders, so delete them from `rf24libs` first, then reinstall. |

To see more detail, uncomment `RF24MESH_DEBUG_MINIMAL` or `RF24MESH_DEBUG` in `RF24Mesh_config.h` and rebuild. If you're
still stuck, search or open an issue on [GitHub](https://github.com/nRF24/RF24Mesh/issues).

## More information

- [RF24Mesh documentation](https://nrf24.github.io/RF24Mesh/)
- [RF24Network documentation](https://nrf24.github.io/RF24Network/)
- [RF24 documentation](https://nrf24.github.io/RF24/)
- [pyRF24](https://nrf24.github.io/pyRF24/), for using the mesh from Python


# RF24Mesh v2.0

Introducing **RF24Network & RF24Mesh v2.0** with some *significant API changes*, adding the use of [C++ Templates](https://cplusplus.com/doc/oldtutorial/templates/)
in order to support a range of ESB enabled radios, most recently NRF52x radios.

**Important Notes:**
- Any network layer that uses v2 needs to have RF24Network/RF24Mesh dependencies of v2 or newer. RF24 v1.x is an exception here.
- General usage should remain backward compatible, see the included examples of the related libraries for more info
- Any third party libs that extend the network/mesh layer may also need to be updated to incorporate the new templated class prototypes:
```cpp
template<class radio_t>
class ESBNetwork;
  
template<class network_t, class radio_t>
class ESBMesh;
```
- Third party libs should also be able to use the backward-compatible typedef in their template:
  - ESBGateway.h:
  ```cpp
  template<typename network_t, typename mesh_t>
  class ESBGateway
  ```
  and inform the compiler what types they intend to support:
  - ESBGateway.cpp:
  ```cpp
  template class ESBGateway<RF24Network, RF24Mesh>;
  ```  
- The auto installers do not perform a version check like package managers, so having the correct versions of the software is important.
- We *will* be maintaining the v1.x versions with bugfixes etc for those who cannot or do not wish to migrate to the newer template approach.

https://nRF24.github.io/RF24Mesh
