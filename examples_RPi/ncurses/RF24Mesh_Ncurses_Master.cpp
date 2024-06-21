
/*
 * RF24Mesh Master Node Monitoring Tool
 * This is a generic tool for master nodes running RF24Mesh that will display address
 * assignments, and information regarding incoming data, regardless of the specific
 * configuration details.
 *
 * Requirements: NCurses
 * Install NCurses: apt-get install libncurses5-dev
 * Setup:
 * 1. cd ~/RF24Mesh/examples_RPi
 * 2. mkdir build && cd build
 * 3. cmake ..
 * 4. make
 * 5. ./ncurses/RF24Mesh_Ncurses_Master
 *
 * NOTE: RF24MESH_DEBUG MUST BE DISABLED IN RF24Mesh_config.h
 *
 * Once configured and running, the interface will display the header information, data rate,
 * and address assignments for all connected nodes.*
 * The master node will also continuously ping each of the child nodes, one per second, while indicating
 * the results.
 *
 */

#include <ncurses.h>
#include "RF24Mesh/RF24Mesh.h"
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

RF24 radio(22, 0);

RF24Network network(radio);
RF24Mesh mesh(radio, network);

void printNodes(uint8_t boldID);
void pingNode(uint8_t listNo);

uint8_t nodeCounter;

uint16_t failID = 0;
WINDOW* win;
WINDOW* topoPad;
void drawTopology();
int nodeY, nodeX = 0;
int maxY, maxX = 0;

int main()
{

    printf("Establishing mesh...\n");
    mesh.setNodeID(0);
    if (!mesh.begin()) {
        // if mesh.begin() returns false for a master node, then radio.begin() returned false.
        printf("Radio hardware not responding.\n");
        return 0;
    }
    radio.printDetails();
    win = initscr(); /* Start curses mode 		  */
    getmaxyx(win, maxX, maxY);
    start_color();
    curs_set(0);
    //keypad(stdscr, TRUE); //Enable user interaction
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_GREEN);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    topoPad = newpad(31, 75);
    attron(COLOR_PAIR(1));
    printw("RF24Mesh Master Node Monitoring Interface by TMRh20 - 2014-2022\n");
    attroff(COLOR_PAIR(1));
    refresh(); /* Print it on to the real screen */

    uint32_t kbTimer = 0, kbCount = 0, pingTimer = millis();
    unsigned long totalPayloads = 0;
    uint8_t boldID = 0;

    while (1)
    {

        // Call mesh.update to keep the network updated
        mesh.update();
        // In addition, keep the 'DHCP service' running on the master node so addresses will
        // be assigned to the sensor nodes
        mesh.DHCP();

        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(2, 0, "[Last Payload Info]\n");
        attroff(A_BOLD | COLOR_PAIR(1));

        // Check for incoming data from the sensors
        while (network.available()) {
            RF24NetworkHeader header;
            network.peek(header);

            // Print the total number of received payloads
            mvprintw(9, 0, " Total: %lu\n", totalPayloads++);

            kbCount++;

            // Read the network payload
            network.read(header, 0, 0);

            // Display the header info
            mvprintw(3, 0, " HeaderID: %u  \n Type: %d  \n From: 0%o  \n ", header.id, header.type, header.from_node);

            for (uint8_t i = 0; i < mesh.addrListTop; i++) {
                if (header.from_node == mesh.addrList[i].address) {
                    boldID = mesh.addrList[i].nodeID;
                }
            }
        }
        printNodes(boldID);

        if (millis() - kbTimer > 1000) {
            kbTimer = millis();
            attron(A_BOLD | COLOR_PAIR(1));
            mvprintw(7, 0, "[Data Rate (In)]");
            attroff(A_BOLD | COLOR_PAIR(1));
            mvprintw(8, 0, " Kbps: %.2f", (kbCount * 32 * 8) / 1000.00);
            kbCount = 0;
        }

        // Ping each connected node, one per second
        if (millis() - pingTimer > 1003 && mesh.addrListTop > 0) {
            pingTimer = millis();
            if (nodeCounter == mesh.addrListTop) {
                nodeCounter = 0;
            }
            pingNode(nodeCounter);
            nodeCounter++;
            drawTopology();
        }

        /*
        uint32_t nOK, nFails;
        network.failures(&nFails, &nOK);
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(2, 24, "[Transmit Results] ");
        attroff(A_BOLD | COLOR_PAIR(1));
        mvprintw(3, 25, " #OK: %u   ", nOK);
        mvprintw(4, 25, " #Fail: %u   ", nFails);
        */

        prefresh(topoPad, 0, 0, 18, 1, maxX - 1, maxY - 2);
        refresh();

        delay(2);
    } //while 1

    endwin(); /* End curses mode		  */
    return 0;
}

void printNodes(uint8_t boldID)
{

    uint8_t xCoord = 2;
    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(xCoord++, 27, "[Address Assignments]\n");
    attroff(A_BOLD | COLOR_PAIR(1));
    for (uint8_t i = 0; i < mesh.addrListTop; i++) {
        if (failID == mesh.addrList[i].nodeID) {
            attron(COLOR_PAIR(2));
        }
        else if (boldID == mesh.addrList[i].nodeID) {
            attron(A_BOLD | COLOR_PAIR(1));
        }
        mvprintw(xCoord++, 28, "ID: %d  Network: 0%o   ", mesh.addrList[i].nodeID, mesh.addrList[i].address);
        attroff(A_BOLD | COLOR_PAIR(1));
        attroff(COLOR_PAIR(2));
    }
    //mvprintw(xCoord++, 28, "                   ");
    //mvprintw(xCoord++, 28, "                   ");
    //mvprintw(xCoord++, 28, "                   ");
    getyx(win, nodeY, nodeX);
}

void pingNode(uint8_t listNo)
{

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(11, 0, "[Ping Test]\n");
    attroff(A_BOLD | COLOR_PAIR(1));

    RF24NetworkHeader headers(mesh.addrList[listNo].address, NETWORK_PING);
    uint32_t pingtime = millis();
    bool ok = false;
    if (headers.to_node) {
        ok = network.write(headers, 0, 0);
        if (ok && failID == mesh.addrList[listNo].nodeID) {
            failID = 0;
        }
        if (!ok) {
            failID = mesh.addrList[listNo].nodeID;
        }
    }
    pingtime = millis() - pingtime;
    mvprintw(12, 0, " ID:%d    ", mesh.addrList[listNo].nodeID);
    mvprintw(13, 0, " Net:0%o    ", mesh.addrList[listNo].address);
    mvprintw(14, 0, " Time:%ums       ", pingtime);

    if (ok || !headers.to_node) {
        mvprintw(15, 0, " OK  ");
    }
    else {
        attron(A_BOLD);
        mvprintw(15, 0, " FAIL");
        attroff(A_BOLD);
    }
}

/****************************************************************************************/

void drawTopology()
{
    wclear(topoPad);
    wattroff(topoPad, COLOR_PAIR(1));
    mvprintw(17, 10, "Mesh Topology");
    mvwaddch(topoPad, nodeY > 15 ? nodeY - 16 : 0, 0, 0);
    wattron(topoPad, COLOR_PAIR(1));
    int topoPadmaxX;
    topoPadmaxX = getmaxx(topoPad);

    for (int i = 01; i < 06; i++) {
        for (int j = 0; j < mesh.addrListTop; j++) {

            if (mesh.addrList[j].address == i) {
                wprintw(topoPad, "0%o[%d]   |    ", mesh.addrList[j].address, mesh.addrList[j].nodeID);
            }
        }
    }
    wprintw(topoPad, "\n");
    wattron(topoPad, COLOR_PAIR(4));
    uint16_t g = 051;
    for (int h = 011; h <= 015; h++) {

        for (int i = h; i <= g; i += 010) {

            for (int j = 0; j < mesh.addrListTop; j++) {
                if (mesh.addrList[j].address == i) {
                    int x = getcurx(topoPad);
                    if (x >= topoPadmaxX) wprintw(topoPad, "\n");
                    wprintw(topoPad, "0%o[%d] ", mesh.addrList[j].address, mesh.addrList[j].nodeID);
                }
            }
        }
        g++;
        wprintw(topoPad, "| ");
    }
    wprintw(topoPad, "\n");
    wattron(topoPad, COLOR_PAIR(5));
    g = 0411;
    for (int h = 0111; h <= 0145; h++) {

        for (int i = h; i <= g; i += 0100) {

            for (int j = 0; j < mesh.addrListTop; j++) {
                if (mesh.addrList[j].address == i) {
                    int x = getcurx(topoPad);
                    if (x >= topoPadmaxX) wprintw(topoPad, "\n");
                    wprintw(topoPad, "0%o[%d] ", mesh.addrList[j].address, mesh.addrList[j].nodeID);
                }
            }
        }
        g++;
    }
    wprintw(topoPad, "\n");
    wattron(topoPad, COLOR_PAIR(3));
    g = 04111;

    for (int h = 01111; h <= 01445; h++) {

        for (int i = h; i <= g; i += 01000) {

            for (int j = 0; j < mesh.addrListTop; j++) {
                if (mesh.addrList[j].address == i) {
                    int x = getcurx(topoPad);
                    if (x >= topoPadmaxX) wprintw(topoPad, "\n");
                    wprintw(topoPad, "0%o[%d] ", mesh.addrList[j].address, mesh.addrList[j].nodeID);
                }
            }
        }
        g++;
    }
    wattroff(topoPad, COLOR_PAIR(2));
}