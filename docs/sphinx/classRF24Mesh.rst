ESBMesh class
~~~~~~~~~~~~~~

.. doxygentypedef:: RF24Mesh

.. cpp:class:: template<network_t = ESBNetwork<RF24>, radio_t = RF24> ESBMesh

    :tparam network_t: The ``network`` object's type. Defaults to the :class:`RF24Network` specialization
        for legacy behavior. This new abstraction is really meant for using the nRF52840 SoC as a
        drop-in replacement for the nRF24L01 radio. For more detail, see the
        `nrf_to_nrf Arduino library <https://github.com/TMRh20/nrf_to_nrf>`_.
    :tparam radio_t: The ``radio`` object's type. Defaults to :class:`RF24` for legacy behavior.
        This new abstraction is really meant for using the nRF52840 SoC as a drop-in replacement
        for the nRF24L01 radio. For more detail, see the
        `nrf_to_nrf Arduino library <https://github.com/TMRh20/nrf_to_nrf>`_.

    .. doxygenfunction:: ESBMesh::ESBMesh

    .. seealso::
        - :cpp:class:`RF24` for the ``radio`` object
        - :cpp:class:`RF24Network` for the ``network`` object.

Basic API
============

.. doxygenfunction:: ESBMesh::begin

.. seealso::
    :cpp:enum:`rf24_datarate_e`, :external:cpp:func:`RF24::setChannel()`, :c:macro:`MESH_DEFAULT_CHANNEL`, :c:macro:`MESH_RENEWAL_TIMEOUT`

.. doxygenfunction:: ESBMesh::update

.. seealso::
    Review :cpp:func:`RF24Network::update()` for more details.

.. doxygenfunction:: ESBMesh::write (const void *data, uint8_t msg_type, size_t size, uint8_t nodeID=0)

.. seealso::
    Review :cpp:var:`RF24NetworkHeader::type` for more details about available message types.

.. doxygenfunction:: ESBMesh::renewAddress
.. doxygenfunction:: ESBMesh::setNodeID
.. doxygenvariable:: ESBMesh::_nodeID

Advanced API
============

.. doxygenvariable:: ESBMesh::mesh_address
.. doxygenfunction:: ESBMesh::getNodeID
.. doxygenfunction:: ESBMesh::checkConnection
.. doxygenfunction:: ESBMesh::releaseAddress
.. doxygenfunction:: ESBMesh::getAddress
.. doxygenfunction:: ESBMesh::write (uint16_t to_node, const void *data, uint8_t msg_type, size_t size)
.. doxygenfunction:: ESBMesh::setChannel

.. seealso:: :external:cpp:func:`RF24::setChannel()`

.. doxygenfunction:: ESBMesh::setChild
.. doxygenfunction:: ESBMesh::setCallback
.. doxygenfunction:: ESBMesh::setAddress
.. doxygenfunction:: ESBMesh::setStaticAddress
.. doxygenfunction:: ESBMesh::DHCP
.. doxygenfunction:: ESBMesh::saveDHCP
.. doxygenfunction:: ESBMesh::loadDHCP

Address List Struct
===================

.. doxygenvariable:: ESBMesh::addrList
.. doxygenvariable:: ESBMesh::addrListTop

.. doxygenstruct:: ESBMesh::addrListStruct
    :members:
