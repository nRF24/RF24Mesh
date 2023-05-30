/**
 * @file RF24Mesh.cpp
 *
 * Class definitions for ESBMesh
 */

#include "RF24Mesh.h"
#include "RF24Mesh_config.h"
#if defined(__linux) && !defined(__ARDUINO_X86__)
    #include <fstream>
#endif

template<class network_t, class radio_t>
ESBMesh<network_t, radio_t>::ESBMesh(radio_t& _radio, network_t& _network) : radio(_radio), network(_network)
{
    setCallback(NULL);
    meshStarted = false;
#if !defined(MESH_NOMASTER)
    addrMemAllocated = false;
#endif
}

/*****************************************************/

template<class network_t, class radio_t>
bool ESBMesh<network_t, radio_t>::begin(uint8_t channel, rf24_datarate_e data_rate, uint32_t timeout)
{
    //delay(1); // Found problems w/SPIDEV & ncurses. Without this, getch() returns a stream of garbage
    if (meshStarted) {
        radio.stopListening();
    }
    meshStarted = true;
    if (!radio.begin())
        return 0;
    radio.setChannel(channel);
    radio.setDataRate(data_rate);
    network.returnSysMsgs = true;

    if (getNodeID() > 0) { //Not master node
        if (renewAddress(timeout) == MESH_DEFAULT_ADDRESS) {
            return false;
        }
    }
    else {
#if !defined(MESH_NOMASTER)
        if (!addrMemAllocated) {
            addrMemAllocated = true;
            addrList = (addrListStruct*)malloc((MESH_MEM_ALLOC_SIZE * sizeof(addrListStruct)));
            addrListTop = 0;
            loadDHCP();
        }
#endif
        mesh_address = 0;
        network.begin(mesh_address);
    }

    return true;
}

/*****************************************************/

template<class network_t, class radio_t>
uint8_t ESBMesh<network_t, radio_t>::update()
{
    uint8_t type = network.update();
    if (mesh_address == MESH_DEFAULT_ADDRESS) return type;

#if !defined(MESH_NOMASTER)
    if (type == NETWORK_REQ_ADDRESS) {
        doDHCP = 1;
    }

    if (!getNodeID()) {
        if ((type == MESH_ADDR_LOOKUP || type == MESH_ID_LOOKUP)) {
            RF24NetworkHeader* header = (RF24NetworkHeader*)(&network.frame_buffer);
            header->to_node = header->from_node;

            int16_t returnAddr = 0;
            if (type == MESH_ADDR_LOOKUP) {
                returnAddr = getAddress(network.frame_buffer[sizeof(RF24NetworkHeader)]);
                network.write(*header, &returnAddr, sizeof(returnAddr));
            }
            else {
                int16_t addr = 0;
                memcpy(&addr, &network.frame_buffer[sizeof(RF24NetworkHeader)], sizeof(addr));
                returnAddr = getNodeID(addr);
                network.write(*header, &returnAddr, sizeof(returnAddr));
            }
        }
        else if (type == MESH_ADDR_RELEASE) {
            uint16_t* fromAddr = (uint16_t*)network.frame_buffer;
            for (uint8_t i = 0; i < addrListTop; i++) {
                if (addrList[i].address == *fromAddr) {
                    addrList[i].address = 0;
                }
            }
        }
    }
#endif //!NO_MASTER

    return type;
}

/*****************************************************/

template<class network_t, class radio_t>
bool ESBMesh<network_t, radio_t>::write(uint16_t to_node, const void* data, uint8_t msg_type, size_t size)
{
    if (mesh_address == MESH_DEFAULT_ADDRESS) return 0;

    RF24NetworkHeader header(to_node, msg_type);
    return network.write(header, data, size);
}

/*****************************************************/

template<class network_t, class radio_t>
bool ESBMesh<network_t, radio_t>::write(const void* data, uint8_t msg_type, size_t size, uint8_t nodeID)
{
    if (mesh_address == MESH_DEFAULT_ADDRESS) return 0;

    int16_t toNode = 0;
    uint32_t lookupTimeout = millis() + MESH_WRITE_TIMEOUT;
    uint32_t retryDelay = 5;

    if (nodeID) {
        while ((toNode = getAddress(nodeID)) < 0) {
            if (millis() > lookupTimeout || toNode == -2) {
                return 0;
            }
            retryDelay += 10;
            delay(retryDelay);
        }
    }
    return write(toNode, data, msg_type, size);
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::setChannel(uint8_t _channel)
{
    radio.stopListening();
    radio.setChannel(_channel);
    radio.startListening();
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::setChild(bool allow)
{
    network.networkFlags = allow ? network.networkFlags & ~FLAG_NO_POLL : network.networkFlags | FLAG_NO_POLL;
}

/*****************************************************/

template<class network_t, class radio_t>
bool ESBMesh<network_t, radio_t>::checkConnection()
{
    // getAddress() doesn't use auto-ack; do a double-check to manually retry 1 more time
    if (getAddress(_nodeID) < 1) {
        if (getAddress(_nodeID) < 1) {
            return false;
        }
    }
    return true;
}

/*****************************************************/

template<class network_t, class radio_t>
int16_t ESBMesh<network_t, radio_t>::getAddress(uint8_t nodeID)
{ // Master will return and send 00 address for a nodeID with address 0, -1 if not found

    //if (nodeID == _nodeID) return mesh_address;
    if (!nodeID) return 0;
    if (mesh_address == MESH_DEFAULT_ADDRESS) return -2;

// Lets say 0 if nodeID 0,  -1 if write failed or timed out, -2 if not found in list or address is default,
#if !defined(MESH_NOMASTER)
    if (!getNodeID()) { //Master Node
        for (uint8_t i = 0; i < addrListTop; i++) {
            if (addrList[i].nodeID == nodeID) {
                return addrList[i].address;
            }
        }
        return -2;
    }
#endif

    RF24NetworkHeader header(00, MESH_ADDR_LOOKUP);
    if (network.write(header, &nodeID, sizeof(nodeID))) {
        uint32_t timer = millis();
        while (network.update() != MESH_ADDR_LOOKUP) {
            MESH_CALLBACK
            if (millis() - timer > MESH_LOOKUP_TIMEOUT) {
                return -1;
            }
        }
        int16_t address = 0;
        memcpy(&address, network.frame_buffer + sizeof(RF24NetworkHeader), sizeof(address));
        return address;
    }
    return -1;
}

/*****************************************************/

template<class network_t, class radio_t>
int16_t ESBMesh<network_t, radio_t>::getNodeID(uint16_t address)
{
    if (address == MESH_BLANK_ID) return _nodeID;
    if (address == 0) return 0;
    if (mesh_address == MESH_DEFAULT_ADDRESS) return -2;

#if !defined(MESH_NOMASTER)
    if (!mesh_address) { //Master Node
        for (uint8_t i = 0; i < addrListTop; i++) {
            if (addrList[i].address == address) {
                return addrList[i].nodeID;
            }
        }
        return -2;
    }
#endif

    RF24NetworkHeader header(00, MESH_ID_LOOKUP);
    if (network.write(header, &address, sizeof(address))) {
        uint32_t timer = millis();
        while (network.update() != MESH_ID_LOOKUP) {
            MESH_CALLBACK
            if (millis() - timer > MESH_LOOKUP_TIMEOUT) return -1;
        }
        int16_t ID = 0;
        memcpy(&ID, &network.frame_buffer[sizeof(RF24NetworkHeader)], sizeof(ID));
        return ID;
    }
    return -1;
}

/*****************************************************/

template<class network_t, class radio_t>
uint8_t ESBMesh<network_t, radio_t>::getLevel(uint16_t address)
{
    uint8_t count = 0;
    while (address) {
        address >>= 3;
        count++;
    }
    return count;
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::beginDefault()
{
    radio.stopListening();
    network.begin(MESH_DEFAULT_ADDRESS);
    mesh_address = MESH_DEFAULT_ADDRESS;
}

/*****************************************************/

template<class network_t, class radio_t>
bool ESBMesh<network_t, radio_t>::releaseAddress()
{
    if (mesh_address == MESH_DEFAULT_ADDRESS) return 0;

    RF24NetworkHeader header(00, MESH_ADDR_RELEASE);
    if (network.write(header, 0, 0)) {
        beginDefault();
        return 1;
    }
    return 0;
}

/*****************************************************/

template<class network_t, class radio_t>
uint16_t ESBMesh<network_t, radio_t>::renewAddress(uint32_t timeout)
{
    if (radio.available()) network.update();

    uint8_t reqCounter = 0;
    uint8_t totalReqs = 0;

    beginDefault();

    uint32_t start = millis();
    while (!requestAddress(reqCounter)) {
        if (millis() - start > timeout) break;

        uint32_t timeoutInternal = 50 + ((totalReqs + 1) * (reqCounter + 1)) * 2;
        uint32_t startInternal = millis();
        while (millis() - startInternal < timeoutInternal) {
            MESH_CALLBACK
            delay(1);
        }
        reqCounter++;
        reqCounter = reqCounter % 4;
        totalReqs++;
        totalReqs = totalReqs % 10;
    }
    return mesh_address;
}

/*****************************************************/

template<class network_t, class radio_t>
bool ESBMesh<network_t, radio_t>::requestAddress(uint8_t level)
{
    RF24NetworkHeader header(MESH_MULTICAST_ADDRESS, NETWORK_POLL);
    //Find another radio, starting with level 0 multicast
    IF_MESH_DEBUG(printf_P(PSTR("%u: MSH Poll\n"), millis()));
    network.multicast(header, 0, 0, level);

    uint32_t timr = millis();
#define MESH_MAXPOLLS 4
    uint16_t contactNode[MESH_MAXPOLLS];
    uint8_t pollCount = 0;

    while (1) {
#if defined(MESH_DEBUG)
        bool goodSignal = radio.testRPD();
#endif

        if (network.update() == NETWORK_POLL) {

            memcpy(&contactNode[pollCount], &network.frame_buffer[0], sizeof(uint16_t));
            if (pollCount > 0 && contactNode[pollCount] != contactNode[pollCount - 1]) { //Drop duplicate polls to help prevent duplicate requests
                ++pollCount;
            }
            else {
                ++pollCount;
            }

            IF_MESH_DEBUG(printf_P(PSTR("%u: MSH Poll %c -64dbm\n"), millis(), (goodSignal ? '>' : '<')));
        } // end if

        if (millis() - timr > 55 || pollCount >= MESH_MAXPOLLS) {
            if (!pollCount) {
                IF_MESH_DEBUG(printf_P(PSTR("%u: MSH No poll from level %d\n"), millis(), level));
                return 0;
            }
            else {
                IF_MESH_DEBUG(printf_P(PSTR("%u: MSH Poll OK\n"), millis()));
                break;
            }
        }
        MESH_CALLBACK
    } // end while

    IF_MESH_DEBUG(printf_P(PSTR("%u: MSH Got poll from level %d count %d\n"), millis(), level, pollCount));

    bool gotResponse = 0;
    for (uint8_t i = 0; i < pollCount; i++) {
        // Request an address via the contact node
        header.type = NETWORK_REQ_ADDRESS;
        header.reserved = _nodeID;
        header.to_node = contactNode[i];

        // Do a direct write (no ack) to the contact node. Include the nodeId and address.
        network.write(header, 0, 0, contactNode[i]);

        IF_MESH_DEBUG(printf_P(PSTR("%u: MSH Request address from: 0%o\n"), millis(), contactNode[i]));

        timr = millis();

        while (millis() - timr < 225) {
            if (network.update() == NETWORK_ADDR_RESPONSE) {
                if (network.frame_buffer[7] == _nodeID) {
                    uint16_t newAddy = 0;
                    memcpy(&newAddy, &network.frame_buffer[sizeof(RF24NetworkHeader)], sizeof(newAddy));
                    uint16_t mask = 0xFFFF;
                    newAddy &= ~(mask << (3 * getLevel(contactNode[i]))); // Get the level of contact node. Multiply by 3 to get the number of bits to shift (3 per digit)
                    if (newAddy == contactNode[i]) {                      // Then shift the mask by this much, and invert it bitwise. Apply the mask to the newly received
                        i = pollCount;                                    // address to evalute whether 'subnet' of the assigned address matches the contact node address.
                        gotResponse = 1;
                        break;
                    }
                }
            }
            MESH_CALLBACK
        }
    } // end for

    if (!gotResponse) {
        return 0;
    }

    uint16_t newAddress = 0;
    memcpy(&newAddress, network.frame_buffer + sizeof(RF24NetworkHeader), sizeof(newAddress));

    IF_MESH_DEBUG(printf_P(PSTR("Set address 0%o rcvd 0%o\n"), mesh_address, newAddress));
    mesh_address = newAddress;

    radio.stopListening();
    network.begin(mesh_address);

    // getNodeID() doesn't use auto-ack; do a double-check to manually retry 1 more time
    if (getNodeID(mesh_address) != _nodeID) {
        if (getNodeID(mesh_address) != _nodeID) {
            beginDefault();
            return 0;
        }
    }
    return 1;
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::setNodeID(uint8_t nodeID)
{
    _nodeID = nodeID;
}

/*****************************************************/
#if !defined(MESH_NOMASTER)

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::setStaticAddress(uint8_t nodeID, uint16_t address)
{
    setAddress(nodeID, address);
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::setAddress(uint8_t nodeID, uint16_t address, bool searchBy)
{
    //Look for the node in the list
    for (uint8_t i = 0; i < addrListTop; i++) {
        if (searchBy == false) {
            if (addrList[i].nodeID == nodeID) {
                addrList[i].address = address;
    #if defined(__linux) && !defined(__ARDUINO_X86__)
                saveDHCP();
    #endif
                return; //Found & set, complete
            }
        }
        else { // Search by address, set the nodeID
            if (addrList[i].address == address) {
                //printf("*** Addr 0%o Found, reassign fr ID %d to ID %d ***\n", addrList[i].address, addrList[i].nodeID, nodeID);
                addrList[i].nodeID = nodeID;
    #if defined(__linux) && !defined(__ARDUINO_X86__)
                saveDHCP();
    #endif
                return;
            }
        }
    }

    if (addrListTop > 0 && addrListTop % MESH_MEM_ALLOC_SIZE == 0) {
        addrList = (addrListStruct*)realloc(addrList, (addrListTop + MESH_MEM_ALLOC_SIZE) * sizeof(addrListStruct));
    }
    addrList[addrListTop].address = address;
    addrList[addrListTop++].nodeID = nodeID; //Set the value AND increment Top without another line of code
    #if defined(__linux) && !defined(__ARDUINO_X86__)
    saveDHCP();
    #endif
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::loadDHCP()
{

    #if defined(__linux) && !defined(__ARDUINO_X86__)
    std::ifstream infile("dhcplist.txt", std::ifstream::binary);
    if (!infile) return;

    infile.seekg(0, infile.end);
    int length = infile.tellg();
    infile.seekg(0, infile.beg);

    addrListStruct tmpNode;

    for (uint8_t i = 0; i < (length / sizeof(addrListStruct)); i++) {
        infile.read((char*)&tmpNode, sizeof(addrListStruct));
        setAddress(tmpNode.nodeID, tmpNode.address);
    }
    infile.close();
    #endif
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::saveDHCP()
{
    #if defined(__linux) && !defined(__ARDUINO_X86__)
    std::ofstream outfile("dhcplist.txt", std::ofstream::binary | std::ofstream::trunc);

    for (int i = 0; i < addrListTop; i++) {
        outfile.write((char*)&addrList[i], sizeof(addrListStruct));
    }
    outfile.close();
    #endif // __linux & not X86
}

/*****************************************************/

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::DHCP()
{
    if (doDHCP)
        doDHCP = false;
    else
        return;

    RF24NetworkHeader header;
    memcpy(&header, network.frame_buffer, sizeof(RF24NetworkHeader));

    uint16_t newAddress;

    // Get the unique id of the requester (ID is in header.reserved)
    if (!header.reserved || header.type != NETWORK_REQ_ADDRESS) {
        IF_MESH_DEBUG(printf_P(PSTR("%u: MSH Invalid id or type rcvd\n"), millis()));
        return;
    }

    uint16_t fwd_by = 0;
    uint8_t shiftVal = 0;
    bool extraChild = false;

    if (header.from_node != MESH_DEFAULT_ADDRESS) {
        fwd_by = header.from_node;
        uint16_t m = fwd_by;
        uint8_t count = 0;

        while (m) {  //Octal addresses convert nicely to binary in threes. Address 03 = B011  Address 033 = B011011
            m >>= 3; //Find out how many digits are in the octal address
            count += 3;
        }
        shiftVal = count; //Now we know how many bits to shift when adding a child node 1-5 (B001 to B101) to any address
    }
    else {
        //If request is coming from level 1, add an extra child to the master
        extraChild = 1;
    }

    // IF_MESH_DEBUG(printf_P(PSTR("%u: MSH Rcv addr req from_id %d\n"), millis(), header.reserved));

    for (int i = MESH_MAX_CHILDREN + extraChild; i > 0; i--) { // For each of the possible addresses (5 max)

        bool found = false;
        newAddress = fwd_by | (i << shiftVal);

        if (newAddress == MESH_DEFAULT_ADDRESS) continue;

        for (uint8_t i = 0; i < addrListTop; i++) {
            IF_MESH_DEBUG_MINIMAL(printf_P(PSTR("ID: %d ADDR: 0%o\n"), addrList[i].nodeID, addrList[i].address));
            if (addrList[i].address == newAddress && addrList[i].nodeID != header.reserved) {
                found = true;
                break;
            }
        } // 3 conditions: 1. address in list = assigned to somebody else (bad); 2. address in list = assigned to this nodeID (ok); 3. address not in list (ok)

        if (!found) {
            header.type = NETWORK_ADDR_RESPONSE;
            header.to_node = header.from_node;
            //This is a routed request to 00

            setAddress(header.reserved, newAddress);
    // without this delay, address renewal fails for children with slower execution speed
    #if defined(SLOW_ADDR_POLL_RESPONSE)
            delay(SLOW_ADDR_POLL_RESPONSE);
    #endif // defined (SLOW_ADDR_POLL_RESPONSE)

            if (header.from_node != MESH_DEFAULT_ADDRESS) { //Is NOT node 01 to 05
                //delay(2);
                if (!network.write(header, &newAddress, sizeof(newAddress))) {
                    network.write(header, &newAddress, sizeof(newAddress));
                }
            }
            else {
                //delay(2);
                network.write(header, &newAddress, sizeof(newAddress), header.to_node);
            }

            IF_MESH_DEBUG(printf_P(PSTR("Sent to 0%o phys: 0%o new: 0%o id: %d\n"), header.to_node, MESH_DEFAULT_ADDRESS, newAddress, header.reserved));
            break;
        }
        else {
            IF_MESH_DEBUG(printf_P(PSTR("not allocated\n")));
        }
    } // end for
}

/*****************************************************/

#endif // !MESH_NOMASTER

template<class network_t, class radio_t>
void ESBMesh<network_t, radio_t>::setCallback(void (*meshCallback)(void))
{

    this->meshCallback = meshCallback;
}

/*****************************************************/

// ensure the compiler is aware of the possible datatype for the template class
template class ESBMesh<ESBNetwork<RF24>, RF24>;
#if defined(ARDUINO_ARCH_NRF52) || defined(ARDUINO_ARCH_NRF52840)
template class ESBMesh<ESBNetwork<nrf_to_nrf>, nrf_to_nrf>;
#endif
