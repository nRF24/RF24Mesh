#include "boost/python.hpp"
#include "RF24/RF24.h"
#include "RF24Network/RF24Network.h"
#include "RF24Mesh/RF24Mesh.h"

namespace bp = boost::python;

// ******************** overload wrappers **************************
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(beginWrapper, RF24Mesh::begin, 0, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(writeWrapper, RF24Mesh::write, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getNodeIDWrapper, RF24Mesh::getNodeID, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(renewAddressWrapper, RF24Mesh::renewAddress, 0, 1)

// **************** RF24Mesh exposed  *****************
//
BOOST_PYTHON_MODULE(RF24Mesh){
    { //::RF24Mesh
        bp::class_<RF24Mesh>("RF24Mesh", bp::init<RF24&, RF24Network&>((bp::arg("_radio"), bp::arg("_network"))))
        //bool begin(uint8_t channel = MESH_DEFAULT_CHANNEL, rf24_datarate_e data_rate = RF24_1MBPS, uint32_t timeout=MESH_RENEWAL_TIMEOUT );
        .def("begin", static_cast<void(RF24Mesh::*)
                (uint8_t, rf24_datarate_e, uint32_t)>
                (&RF24Mesh::begin), beginWrapper())
        //uint8_t update();
        .def("update", &RF24Mesh::update)
        //bool write(const void* data, uint8_t msg_type, size_t size, uint8_t nodeID=0);
        .def("write", static_cast<void(RF24Mesh::*)
                (const void*, uint8_t, size_t, uint8_t)>
                (&RF24Mesh::write), writeWrapper())
        //void setNodeID(uint8_t nodeID);
        .def("setNodeId", &RF24Mesh::setNodeID, (bp::arg("nodeID")))
        //void DHCP();
        .def("DHCP", &RF24Mesh::DHCP)
        //int16_t getNodeID(uint16_t address=MESH_BLANK_ID);
        .def("getNodeID", static_cast<void(RF24Mesh::*)
                (uint16_t)>
                (&RF24Mesh::write), getNodeIDWrapper())
        //bool checkConnection();
        .def("checkConnection", &RF24Mesh::checkConnection)
        //uint16_t renewAddress(uint32_t timeout=MESH_RENEWAL_TIMEOUT);
        .def("renewAddress", static_cast<void(RF24Mesh::*)
                (uint32_t)>
                (&RF24Mesh::renewAddress), renewAddressWrapper())
        //bool releaseAddress();
        .def("releaseAddress", &RF24Mesh::releaseAddress)
        //int16_t getAddress(uint8_t nodeID);
        .def("getAddress", &RF24Mesh::getAddress, (bp::arg("nodeID")))
        //bool write(uint16_t to_node, const void* data, uint8_t msg_type, size_t size );
        .def("write", &RF24Mesh::write, (bp::arg("to_node"), bp::arg("data"), bp::arg("msg_type"), bp::arg("size")))
        //void setChannel(uint8_t _channel);
        .def("setChannel", &RF24Mesh::setChannel, (bp::arg("_channel")))
        //void setChild(bool allow);
        .def("setChild", &RF24Mesh::setChild, (bp::arg("allow")))
        //void setAddress(uint8_t nodeID, uint16_t address);
        .def("setAddress", &RF24Mesh::setAddress, (bp::arg("nodeID"), bp::arg("address")))
        //void saveDHCP();
        .def("saveDHCP", &RF24Mesh::saveDHCP)
        //void loadDHCP();
        .def("loadDHCP", &RF24Mesh::loadDHCP);
    }
}