RF24Mesh class
~~~~~~~~~~~~~~

.. cpp:class:: RF24Mesh

    .. doxygenfunction:: RF24Mesh::RF24Mesh

    .. seealso::
        - :cpp:class:`RF24` for the ``radio`` object
        - :cpp:class:`RF24Network` for the ``network`` object.

Basic API
============

.. doxygenfunction:: RF24Mesh::begin

.. seealso::
    :cpp:enum:`rf24_datarate_e`, :external:cpp:func:`RF24::setChannel()`, :c:macro:`MESH_DEFAULT_CHANNEL`, :c:macro:`MESH_RENEWAL_TIMEOUT`

.. doxygenfunction:: RF24Mesh::update

.. seealso::
    Review :cpp:func:`RF24Network::update()` for more details.

.. doxygenfunction:: RF24Mesh::write (const void *data, uint8_t msg_type, size_t size, uint8_t nodeID=0)

.. seealso::
    Review :cpp:var:`RF24NetworkHeader::type` for more details about available message types.

.. doxygenfunction:: RF24Mesh::renewAddress
.. doxygenfunction:: RF24Mesh::setNodeID
.. doxygenvariable:: RF24Mesh::_nodeID

Advanced API
============

.. doxygenvariable:: RF24Mesh::mesh_address
.. doxygenfunction:: RF24Mesh::getNodeID
.. doxygenfunction:: RF24Mesh::checkConnection
.. doxygenfunction:: RF24Mesh::releaseAddress
.. doxygenfunction:: RF24Mesh::getAddress
.. doxygenfunction:: RF24Mesh::write (uint16_t to_node, const void *data, uint8_t msg_type, size_t size)
.. doxygenfunction:: RF24Mesh::setChannel

.. seealso:: :external:cpp:func:`RF24::setChannel()`

.. doxygenfunction:: RF24Mesh::setChild
.. doxygenfunction:: RF24Mesh::setCallback
.. doxygenfunction:: RF24Mesh::setAddress
.. doxygenfunction:: RF24Mesh::setStaticAddress
.. doxygenfunction:: RF24Mesh::DHCP
.. doxygenfunction:: RF24Mesh::saveDHCP
.. doxygenfunction:: RF24Mesh::loadDHCP

Address List Struct
===================

.. doxygenvariable:: RF24Mesh::addrList
.. doxygenvariable:: RF24Mesh::addrListTop

.. doxygenstruct:: RF24Mesh::addrListStruct
    :members:
