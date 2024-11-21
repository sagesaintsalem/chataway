#pragma once
#include "allheader.h"
extern int connected_peers;
void loadPeerCount();
void savePeerCount();
void addPeer();
void subtractPeer();
void clearPeer();