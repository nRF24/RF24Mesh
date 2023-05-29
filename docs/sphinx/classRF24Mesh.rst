ESB_Mesh class
~~~~~~~~~~~~~~

.. doxygentypedef:: RF24Mesh

.. cpp:class:: template<network_t = ESB_Network<RF24>, radio_t = RF24> ESB_Mesh

    :tparam network_t: The ``network`` object's type. Defaults to the :class:`RF24` specialization
        for legacy behavior. This new abstraction is really meant for using the nRF52840 SoC as a
        drop-in replacement for the nRF24L01 radio. For more detail, see the
        `nrf_to_nrf Arduino library <https://github.com/TMRh20/nrf_to_nrf>`_.
    :tparam radio_t: The ``radio`` object's type. Defaults to :class:`RF24` for legacy behavior.
        This new abstraction is really meant for using the nRF52840 SoC as a drop-in replacement
        for the nRF24L01 radio. For more detail, see the
        `nrf_to_nrf Arduino library <https://github.com/TMRh20/nrf_to_nrf>`_.

    .. doxygenfunction:: ESB_Mesh::ESB_Mesh

    .. seealso::
        - :cpp:class:`RF24` for the ``radio`` object
        - :cpp:class:`RF24Network` for the ``network`` object.

Basic API
============

.. doxygenfunction:: ESB_Mesh::begin

.. seealso::
    :cpp:enum:`rf24_datarate_e`, :external:cpp:func:`RF24::setChannel()`, :c:macro:`MESH_DEFAULT_CHANNEL`, :c:macro:`MESH_RENEWAL_TIMEOUT`

.. doxygenfunction:: ESB_Mesh::update

.. seealso::
    Review :cpp:func:`RF24Network::update()` for more details.

.. doxygenfunction:: ESB_Mesh::write (const void *data, uint8_t msg_type, size_t size, uint8_t nodeID=0)

.. seealso::
    Review :cpp:var:`RF24NetworkHeader::type` for more details about available message types.

.. doxygenfunction:: ESB_Mesh::renewAddress
.. doxygenfunction:: ESB_Mesh::setNodeID
.. doxygenvariable:: ESB_Mesh::_nodeID

Advanced API
============

.. doxygenvariable:: ESB_Mesh::mesh_address
.. doxygenfunction:: ESB_Mesh::getNodeID
.. doxygenfunction:: ESB_Mesh::checkConnection
.. doxygenfunction:: ESB_Mesh::releaseAddress
.. doxygenfunction:: ESB_Mesh::getAddress
.. doxygenfunction:: ESB_Mesh::write (uint16_t to_node, const void *data, uint8_t msg_type, size_t size)
.. doxygenfunction:: ESB_Mesh::setChannel

.. seealso:: :external:cpp:func:`RF24::setChannel()`

.. doxygenfunction:: ESB_Mesh::setChild
.. doxygenfunction:: ESB_Mesh::setCallback
.. doxygenfunction:: ESB_Mesh::setAddress
.. doxygenfunction:: ESB_Mesh::setStaticAddress
.. doxygenfunction:: ESB_Mesh::DHCP
.. doxygenfunction:: ESB_Mesh::saveDHCP
.. doxygenfunction:: ESB_Mesh::loadDHCP

Address List Struct
===================

.. doxygenvariable:: ESB_Mesh::addrList
.. doxygenvariable:: ESB_Mesh::addrListTop

.. doxygenstruct:: ESB_Mesh::addrListStruct
    :members:
