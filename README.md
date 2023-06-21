<!-- markdownlint-disable MD041 MD034 -->
[![Linux build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_linux.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_linux.yml)
[![Arduino CLI build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_arduino.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_arduino.yml)
[![PlatformIO build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_platformIO.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_platformIO.yml)
[![Pico SDK build](https://github.com/nRF24/RF24Mesh/actions/workflows/build_rp2xxx.yml/badge.svg)](https://github.com/nRF24/RF24Mesh/actions/workflows/build_rp2xxx.yml)
[![Documentation Status](https://readthedocs.org/projects/rf24mesh/badge/?version=latest)](https://rf24mesh.readthedocs.io/en/latest/?badge=latest)

# RF24Mesh

Mesh Networking for RF24Network

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
