/*
 * Copyright (c) 2021
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Stefano Avallone <stavallo@unina.it>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "wifi-phy-operating-channel.h"

#include "wifi-phy-common.h"

#include "ns3/assert.h"
#include "ns3/log.h"

#include <algorithm>
#include <numeric>
#include <sstream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("WifiPhyOperatingChannel");

const std::set<FrequencyChannelInfo> WifiPhyOperatingChannel::m_frequencyChannels = {{
    // 2.4 GHz channels
    //  802.11b uses width of 22, while OFDM modes use width of 20
    {1, 2412, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {1, 2412, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {2, 2417, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {2, 2417, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {3, 2422, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {3, 2422, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {4, 2427, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {4, 2427, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {5, 2432, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {5, 2432, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {6, 2437, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {6, 2437, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {7, 2442, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {7, 2442, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {8, 2447, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {8, 2447, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {9, 2452, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {9, 2452, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {10, 2457, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {10, 2457, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {11, 2462, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {11, 2462, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {12, 2467, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {12, 2467, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {13, 2472, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    {13, 2472, 20, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    // Only defined for 802.11b
    {14, 2484, 22, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::DSSS},
    // 40 MHz channels
    {3, 2422, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {4, 2427, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {5, 2432, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {6, 2437, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {7, 2442, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {8, 2447, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {9, 2452, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {10, 2457, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},
    {11, 2462, 40, WIFI_PHY_BAND_2_4GHZ, FrequencyChannelType::OFDM},

    // Now the 5 GHz channels used for 802.11a/n/ac/ax/be
    // 20 MHz channels
    {36, 5180, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {40, 5200, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {44, 5220, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {48, 5240, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {52, 5260, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {56, 5280, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {60, 5300, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {64, 5320, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {100, 5500, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {104, 5520, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {108, 5540, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {112, 5560, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {116, 5580, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {120, 5600, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {124, 5620, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {128, 5640, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {132, 5660, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {136, 5680, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {140, 5700, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {144, 5720, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {149, 5745, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {153, 5765, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {157, 5785, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {161, 5805, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {165, 5825, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {169, 5845, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {173, 5865, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {177, 5885, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {181, 5905, 20, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    // 40 MHz channels
    {38, 5190, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {46, 5230, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {54, 5270, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {62, 5310, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {102, 5510, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {110, 5550, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {118, 5590, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {126, 5630, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {134, 5670, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {142, 5710, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {151, 5755, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {159, 5795, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {167, 5835, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {175, 5875, 40, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    // 80 MHz channels
    {42, 5210, 80, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {58, 5290, 80, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {106, 5530, 80, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {122, 5610, 80, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {138, 5690, 80, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {155, 5775, 80, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {171, 5855, 80, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    // 160 MHz channels
    {50, 5250, 160, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {114, 5570, 160, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},
    {163, 5815, 160, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::OFDM},

    // 802.11p 10 MHz channels at the 5.855-5.925 band
    {172, 5860, 10, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {174, 5870, 10, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {176, 5880, 10, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {178, 5890, 10, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {180, 5900, 10, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {182, 5910, 10, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {184, 5920, 10, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},

    // 802.11p 5 MHz channels at the 5.855-5.925 band (for simplification, we consider the same
    // center frequencies as the 10 MHz channels)
    {171, 5860, 5, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {173, 5870, 5, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {175, 5880, 5, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {177, 5890, 5, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {179, 5900, 5, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {181, 5910, 5, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},
    {183, 5920, 5, WIFI_PHY_BAND_5GHZ, FrequencyChannelType::CH_80211P},

    // Now the 6 GHz channels for 802.11ax/be
    // 20 MHz channels
    {1, 5955, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {5, 5975, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {9, 5995, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {13, 6015, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {17, 6035, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {21, 6055, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {25, 6075, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {29, 6095, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {33, 6115, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {37, 6135, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {41, 6155, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {45, 6175, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {49, 6195, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {53, 6215, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {57, 6235, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {61, 6255, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {65, 6275, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {69, 6295, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {73, 6315, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {77, 6335, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {81, 6355, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {85, 6375, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {89, 6395, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {93, 6415, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {97, 6435, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {101, 6455, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {105, 6475, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {109, 6495, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {113, 6515, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {117, 6535, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {121, 6555, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {125, 6575, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {129, 6595, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {133, 6615, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {137, 6635, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {141, 6655, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {145, 6675, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {149, 6695, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {153, 6715, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {157, 6735, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {161, 6755, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {165, 6775, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {169, 6795, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {173, 6815, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {177, 6835, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {181, 6855, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {185, 6875, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {189, 6895, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {193, 6915, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {197, 6935, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {201, 6955, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {205, 6975, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {209, 6995, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {213, 7015, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {217, 7035, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {221, 7055, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {225, 7075, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {229, 7095, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {233, 7115, 20, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    // 40 MHz channels
    {3, 5965, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {11, 6005, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {19, 6045, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {27, 6085, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {35, 6125, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {43, 6165, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {51, 6205, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {59, 6245, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {67, 6285, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {75, 6325, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {83, 6365, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {91, 6405, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {99, 6445, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {107, 6485, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {115, 6525, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {123, 6565, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {131, 6605, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {139, 6645, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {147, 6685, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {155, 6725, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {163, 6765, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {171, 6805, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {179, 6845, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {187, 6885, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {195, 6925, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {203, 6965, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {211, 7005, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {219, 7045, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {227, 7085, 40, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    // 80 MHz channels
    {7, 5985, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {23, 6065, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {39, 6145, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {55, 6225, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {71, 6305, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {87, 6385, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {103, 6465, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {119, 6545, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {135, 6625, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {151, 6705, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {167, 6785, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {183, 6865, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {199, 6945, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {215, 7025, 80, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    // 160 MHz channels
    {15, 6025, 160, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {47, 6185, 160, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {79, 6345, 160, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {111, 6505, 160, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {143, 6665, 160, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {175, 6825, 160, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
    {207, 6985, 160, WIFI_PHY_BAND_6GHZ, FrequencyChannelType::OFDM},
}};

std::ostream&
operator<<(std::ostream& os, const FrequencyChannelInfo& info)
{
    os << "{" << +info.number << " " << info.frequency << " " << info.width << " " << info.band
       << "}";
    return os;
}

bool
WifiPhyOperatingChannel::Compare::operator()(const ConstIterator& first,
                                             const ConstIterator& second) const
{
    return first->frequency < second->frequency;
}

WifiPhyOperatingChannel::WifiPhyOperatingChannel()
    : WifiPhyOperatingChannel(ConstIteratorSet{})
{
}

WifiPhyOperatingChannel::WifiPhyOperatingChannel(ConstIterator it)
    : WifiPhyOperatingChannel(ConstIteratorSet{it})
{
}

WifiPhyOperatingChannel::WifiPhyOperatingChannel(const ConstIteratorSet& channelIts)
    : m_channelIts(channelIts),
      m_primary20Index(0)
{
    NS_LOG_FUNCTION(this);
    NS_ASSERT_MSG(channelIts.size() <= 2,
                  "Operating channel does not support more than 2 segments");
}

WifiPhyOperatingChannel::~WifiPhyOperatingChannel()
{
    NS_LOG_FUNCTION_NOARGS();
}

bool
WifiPhyOperatingChannel::IsSet() const
{
    return !m_channelIts.empty();
}

void
WifiPhyOperatingChannel::Set(const std::vector<FrequencyChannelInfo>& segments,
                             WifiStandard standard)
{
    std::stringstream ss;
    for (const auto& segment : segments)
    {
        ss << segment;
    }
    NS_LOG_FUNCTION(this << ss.str() << standard);

    NS_ASSERT_MSG(!segments.empty(), "At least one frequency segment has to be provided");

    ConstIteratorSet channelIts{};
    for (const auto& segment : segments)
    {
        if (const auto channelIt =
                FindFirst(segment.number, segment.frequency, segment.width, standard, segment.band);
            channelIt != m_frequencyChannels.cend() &&
            FindFirst(segment.number,
                      segment.frequency,
                      segment.width,
                      standard,
                      segment.band,
                      std::next(channelIt)) == m_frequencyChannels.cend())
        {
            // a unique channel matches the specified criteria
            channelIts.insert(channelIt);
        }
    }

    if (channelIts.size() != segments.size())
    {
        // if a unique channel was not found, throw an exception (mainly for unit testing this code)
        throw std::runtime_error(
            "WifiPhyOperatingChannel: No unique channel found given the specified criteria");
    }

    auto it = channelIts.begin();
    for (std::size_t segment = 0; segment < (channelIts.size() - 1); ++segment)
    {
        const auto freq = (*it)->frequency;
        const auto width = (*it)->width;
        const auto band = (*it)->band;
        const auto maxFreq = freq + (width / 2);
        ++it;
        const auto nextFreq = (*it)->frequency;
        const auto nextWidth = (*it)->width;
        const auto nextBand = (*it)->band;
        const auto nextMinFreq = nextFreq - (nextWidth / 2);
        if (maxFreq >= nextMinFreq)
        {
            throw std::runtime_error(
                "WifiPhyOperatingChannel is invalid: segments cannot be adjacent nor overlap");
        }
        if (band != nextBand)
        {
            throw std::runtime_error("WifiPhyOperatingChannel is invalid: all segments shall "
                                     "belong to the same band");
        }
    }

    if ((channelIts.size() > 2) ||
        ((channelIts.size() == 2) &&
         !std::all_of(channelIts.cbegin(), channelIts.cend(), [](const auto& channel) {
             return channel->width == 80;
         })))
    {
        throw std::runtime_error("WifiPhyOperatingChannel is invalid: only 80+80MHz is "
                                 "expected as non-contiguous channel");
    }

    m_channelIts = channelIts;
    m_primary20Index = 0;
}

void
WifiPhyOperatingChannel::SetDefault(ChannelWidthMhz width, WifiStandard standard, WifiPhyBand band)
{
    NS_LOG_FUNCTION(this << width << standard << band);
    Set({{GetDefaultChannelNumber(width, standard, band), 0, width, band}}, standard);
}

uint8_t
WifiPhyOperatingChannel::GetDefaultChannelNumber(ChannelWidthMhz width,
                                                 WifiStandard standard,
                                                 WifiPhyBand band)
{
    auto channelIt = FindFirst(0, 0, width, standard, band);

    if (channelIt != m_frequencyChannels.end())
    {
        // a channel matches the specified criteria
        return channelIt->number;
    }

    // if a default channel was not found, throw an exception (mainly for unit testing this code)
    throw std::runtime_error("WifiPhyOperatingChannel: No default channel found of the given width "
                             "and for the given PHY standard and band");
}

WifiPhyOperatingChannel::ConstIterator
WifiPhyOperatingChannel::FindFirst(uint8_t number,
                                   uint16_t frequency,
                                   ChannelWidthMhz width,
                                   WifiStandard standard,
                                   WifiPhyBand band,
                                   ConstIterator start)
{
    // lambda used to match channels against the specified criteria
    auto predicate = [&](const FrequencyChannelInfo& channel) {
        if (number != 0 && channel.number != number)
        {
            return false;
        }
        if (frequency != 0 && channel.frequency != frequency)
        {
            return false;
        }
        if (width != 0 && channel.width != width)
        {
            return false;
        }
        if (standard != WIFI_STANDARD_UNSPECIFIED &&
            channel.type != GetFrequencyChannelType(standard))
        {
            return false;
        }
        if (band != WIFI_PHY_BAND_UNSPECIFIED && channel.band != band)
        {
            return false;
        }
        return true;
    };

    // Do not search for a channel matching the specified criteria if the given PHY band
    // is not allowed for the given standard (if any) or the given channel width is not
    // allowed for the given standard (if any)
    if (const auto standardIt = wifiStandards.find(standard);
        standardIt != wifiStandards.cend() &&
        (std::find(standardIt->second.cbegin(), standardIt->second.cend(), band) ==
             standardIt->second.cend() ||
         width > GetMaximumChannelWidth(GetModulationClassForStandard(standard))))
    {
        return m_frequencyChannels.cend();
    }

    return std::find_if(start, m_frequencyChannels.cend(), predicate);
}

uint8_t
WifiPhyOperatingChannel::GetNumber(std::size_t segment /* = 0 */) const
{
    NS_ASSERT(IsSet());
    return (*std::next(m_channelIts.begin(), segment))->number;
}

uint16_t
WifiPhyOperatingChannel::GetFrequency(std::size_t segment /* = 0 */) const
{
    NS_ASSERT(IsSet());
    return (*std::next(m_channelIts.begin(), segment))->frequency;
}

ChannelWidthMhz
WifiPhyOperatingChannel::GetWidth(std::size_t /* segment = 0 */) const
{
    NS_ASSERT(IsSet());
    // Current specs only allow all segments to be the same width
    return (*m_channelIts.cbegin())->width;
}

WifiPhyBand
WifiPhyOperatingChannel::GetPhyBand() const
{
    NS_ASSERT(IsSet());
    // Current specs only allow all segments to be the same band
    return (*m_channelIts.cbegin())->band;
}

bool
WifiPhyOperatingChannel::IsOfdm() const
{
    NS_ASSERT(IsSet());
    return ((*m_channelIts.cbegin())->type == FrequencyChannelType::OFDM);
}

bool
WifiPhyOperatingChannel::IsDsss() const
{
    NS_ASSERT(IsSet());
    return ((*m_channelIts.cbegin())->type == FrequencyChannelType::DSSS);
}

bool
WifiPhyOperatingChannel::Is80211p() const
{
    NS_ASSERT(IsSet());
    return ((*m_channelIts.cbegin())->type == FrequencyChannelType::CH_80211P);
}

ChannelWidthMhz
WifiPhyOperatingChannel::GetTotalWidth() const
{
    NS_ASSERT(IsSet());
    return std::accumulate(
        m_channelIts.cbegin(),
        m_channelIts.cend(),
        0,
        [](ChannelWidthMhz sum, const auto& channel) { return sum + channel->width; });
}

WifiChannelWidthType
WifiPhyOperatingChannel::GetWidthType() const
{
    NS_ASSERT(IsSet());
    switch (GetTotalWidth())
    {
    case 20:
        return WifiChannelWidthType::CW_20MHZ;
    case 22:
        return WifiChannelWidthType::CW_22MHZ;
    case 5:
        return WifiChannelWidthType::CW_5MHZ;
    case 10:
        return WifiChannelWidthType::CW_10MHZ;
    case 40:
        return WifiChannelWidthType::CW_40MHZ;
    case 80:
        return WifiChannelWidthType::CW_80MHZ;
    case 160:
        return (m_channelIts.size() == 2) ? WifiChannelWidthType::CW_80_PLUS_80MHZ
                                          : WifiChannelWidthType::CW_160MHZ;
    case 2160:
        return WifiChannelWidthType::CW_2160MHZ;
    case 0:
    default:
        return WifiChannelWidthType::UNKNOWN;
    }
}

uint8_t
WifiPhyOperatingChannel::GetPrimaryChannelIndex(ChannelWidthMhz primaryChannelWidth) const
{
    if (primaryChannelWidth % 20 != 0)
    {
        NS_LOG_DEBUG("The operating channel width is not a multiple of 20 MHz; return 0");
        return 0;
    }

    NS_ASSERT(primaryChannelWidth <= GetTotalWidth());

    // the index of primary40 is half the index of primary20; the index of
    // primary80 is half the index of primary40, ...
    ChannelWidthMhz width = 20;
    uint8_t index = m_primary20Index;

    while (width < primaryChannelWidth)
    {
        index /= 2;
        width *= 2;
    }
    return index;
}

uint8_t
WifiPhyOperatingChannel::GetSecondaryChannelIndex(ChannelWidthMhz secondaryChannelWidth) const
{
    const uint8_t primaryIndex = GetPrimaryChannelIndex(secondaryChannelWidth);
    const uint8_t secondaryIndex =
        (primaryIndex % 2 == 0) ? (primaryIndex + 1) : (primaryIndex - 1);
    return secondaryIndex;
}

void
WifiPhyOperatingChannel::SetPrimary20Index(uint8_t index)
{
    NS_LOG_FUNCTION(this << +index);

    NS_ABORT_MSG_IF(index > 0 && index >= GetTotalWidth() / 20, "Primary20 index out of range");
    m_primary20Index = index;
}

uint8_t
WifiPhyOperatingChannel::GetPrimarySegmentIndex(ChannelWidthMhz primaryChannelWidth) const
{
    if (m_channelIts.size() < 2)
    {
        return 0;
    }
    // Note: this function assumes no more than 2 segments are used
    const auto numIndices = GetTotalWidth() / primaryChannelWidth;
    const auto primaryIndex = GetPrimaryChannelIndex(primaryChannelWidth);
    return (primaryIndex >= (numIndices / 2)) ? 1 : 0;
}

uint8_t
WifiPhyOperatingChannel::GetSecondarySegmentIndex(ChannelWidthMhz primaryChannelWidth) const
{
    NS_ABORT_MSG_IF(primaryChannelWidth > GetWidth(),
                    "Primary channel width cannot be larger than the width of a frequency segment");
    if (m_channelIts.size() < 2)
    {
        return 0;
    }
    // Note: this function assumes no more than 2 segments are used
    const auto numIndices = GetTotalWidth() / primaryChannelWidth;
    const auto secondaryIndex = GetSecondaryChannelIndex(primaryChannelWidth);
    return (secondaryIndex >= (numIndices / 2)) ? 1 : 0;
}

uint16_t
WifiPhyOperatingChannel::GetPrimaryChannelCenterFrequency(ChannelWidthMhz primaryChannelWidth) const
{
    const auto segmentIndex = GetPrimarySegmentIndex(primaryChannelWidth);
    // we assume here that all segments have the same width
    const auto segmentWidth = GetWidth(segmentIndex);
    const auto segmentOffset = (segmentIndex * (segmentWidth / primaryChannelWidth));
    return GetFrequency(segmentIndex) - segmentWidth / 2. +
           (GetPrimaryChannelIndex(primaryChannelWidth) - segmentOffset + 0.5) *
               primaryChannelWidth;
}

uint16_t
WifiPhyOperatingChannel::GetSecondaryChannelCenterFrequency(
    ChannelWidthMhz secondaryChannelWidth) const
{
    const auto segmentIndex = GetSecondarySegmentIndex(secondaryChannelWidth);
    // we assume here that all segments have the same width
    const auto segmentWidth = GetWidth(segmentIndex);
    const auto segmentOffset = (segmentIndex * (segmentWidth / secondaryChannelWidth));
    const auto primaryChannelIndex = GetPrimaryChannelIndex(secondaryChannelWidth);
    const auto primaryCenterFrequency =
        GetFrequency(segmentIndex) - segmentWidth / 2. +
        (primaryChannelIndex - segmentOffset + 0.5) * secondaryChannelWidth;
    return (primaryChannelIndex % 2 == 0) ? (primaryCenterFrequency + secondaryChannelWidth)
                                          : (primaryCenterFrequency - secondaryChannelWidth);
}

uint8_t
WifiPhyOperatingChannel::GetPrimaryChannelNumber(ChannelWidthMhz primaryChannelWidth,
                                                 WifiStandard standard) const
{
    NS_ABORT_MSG_IF(primaryChannelWidth > GetWidth(),
                    "Primary channel width cannot be larger than the width of a frequency segment");
    auto frequency = GetPrimaryChannelCenterFrequency(primaryChannelWidth);
    NS_ASSERT_MSG(IsSet(), "No channel set");
    auto primaryChanIt = FindFirst(0, frequency, primaryChannelWidth, standard, GetPhyBand());
    NS_ASSERT_MSG(primaryChanIt != m_frequencyChannels.end(), "Primary channel number not found");
    return primaryChanIt->number;
}

std::set<uint8_t>
WifiPhyOperatingChannel::GetAll20MHzChannelIndicesInPrimary(ChannelWidthMhz width) const
{
    if (width > GetTotalWidth())
    {
        // a primary channel of the given width does not exist
        return {};
    }

    ChannelWidthMhz currWidth = 20; // MHz
    std::set<uint8_t> indices;
    indices.insert(m_primary20Index);

    while (currWidth < width)
    {
        indices.merge(GetAll20MHzChannelIndicesInSecondary(indices));
        currWidth <<= 1;
    }

    return indices;
}

std::set<uint8_t>
WifiPhyOperatingChannel::GetAll20MHzChannelIndicesInSecondary(ChannelWidthMhz width) const
{
    return GetAll20MHzChannelIndicesInSecondary(GetAll20MHzChannelIndicesInPrimary(width));
}

std::set<uint8_t>
WifiPhyOperatingChannel::GetAll20MHzChannelIndicesInSecondary(
    const std::set<uint8_t>& primaryIndices) const
{
    if (primaryIndices.empty() || GetTotalWidth() == 20)
    {
        return {};
    }

    uint8_t size = 1;
    ChannelWidthMhz primaryWidth = 20; // MHz

    // find the width of the primary channel corresponding to the size of the given set
    while (size != primaryIndices.size())
    {
        size <<= 1;
        primaryWidth <<= 1;

        if (primaryWidth >= GetTotalWidth())
        {
            // the width of the primary channel resulting from the given indices
            // exceeds the width of the operating channel
            return {};
        }
    }

    std::set<uint8_t> secondaryIndices;
    for (const auto& index : primaryIndices)
    {
        secondaryIndices.insert(index ^ size);
    }

    return secondaryIndices;
}

std::set<uint8_t>
WifiPhyOperatingChannel::Get20MHzIndicesCoveringRu(HeRu::RuSpec ru, ChannelWidthMhz width) const
{
    auto ruType = ru.GetRuType();

    NS_ASSERT_MSG(HeRu::GetBandwidth(ruType) <= width,
                  "No RU of type " << ruType << " is contained in a " << width << " MHz channel");
    NS_ASSERT_MSG(width <= GetTotalWidth(),
                  "The given width (" << width << " MHz) exceeds the operational width ("
                                      << GetTotalWidth() << ")");

    // trivial case: 2x996-tone RU
    if (ruType == HeRu::RU_2x996_TONE)
    {
        return {0, 1, 2, 3, 4, 5, 6, 7};
    }

    // handle first the special case of center 26-tone RUs
    if (ruType == HeRu::RU_26_TONE && ru.GetIndex() == 19)
    {
        NS_ASSERT_MSG(width >= 80,
                      "26-tone RU with index 19 is only present in channels of at least 80 MHz");
        // the center 26-tone RU in an 80 MHz channel is not fully covered by
        // any 20 MHz channel, but by the two central 20 MHz channels in the 80 MHz channel
        auto indices = ru.GetPrimary80MHz() ? GetAll20MHzChannelIndicesInPrimary(80)
                                            : GetAll20MHzChannelIndicesInSecondary(80);
        indices.erase(indices.begin());
        indices.erase(std::prev(indices.end()));
        return indices;
    }

    auto ruIndex = ru.GetIndex();

    if (ruType == HeRu::RU_26_TONE && ruIndex > 19)
    {
        // "ignore" the center 26-tone RU in an 80 MHz channel
        ruIndex--;
    }

    // if the RU refers to a 160 MHz channel, we have to update the RU index (which
    // refers to an 80 MHz channel) if the RU is not in the lower 80 MHz channel
    if (width == 160)
    {
        bool primary80IsLower80 = (m_primary20Index < 4);
        if (primary80IsLower80 != ru.GetPrimary80MHz())
        {
            auto nRusIn80MHz = HeRu::GetNRus(80, ruType);
            // "ignore" the center 26-tone RU in an 80 MHz channel
            if (ruType == HeRu::RU_26_TONE)
            {
                nRusIn80MHz--;
            }
            ruIndex += nRusIn80MHz;
        }
    }

    uint8_t n20MHzChannels; // number of 20 MHz channels in the channel covering the RU

    switch (ruType)
    {
    case HeRu::RU_26_TONE:
    case HeRu::RU_52_TONE:
    case HeRu::RU_106_TONE:
    case HeRu::RU_242_TONE:
        n20MHzChannels = 1;
        break;
    case HeRu::RU_484_TONE:
        n20MHzChannels = 2;
        break;
    case HeRu::RU_996_TONE:
        n20MHzChannels = 4;
        break;
    default:
        NS_ABORT_MSG("Unhandled RU type: " << ruType);
    }

    auto nRusInCoveringChannel = HeRu::GetNRus(n20MHzChannels * 20, ruType);
    // compute the index (starting at 0) of the covering channel within the given width
    std::size_t indexOfCoveringChannelInGivenWidth = (ruIndex - 1) / nRusInCoveringChannel;

    // expand the index of the covering channel in the indices of its constituent
    // 20 MHz channels (within the given width)
    NS_ASSERT(indexOfCoveringChannelInGivenWidth < 8); // max number of 20 MHz channels
    std::set<uint8_t> indices({static_cast<uint8_t>(indexOfCoveringChannelInGivenWidth)});

    while (n20MHzChannels > 1)
    {
        std::set<uint8_t> updatedIndices;
        for (const auto& idx : indices)
        {
            updatedIndices.insert(idx * 2);
            updatedIndices.insert(idx * 2 + 1);
        }
        indices.swap(updatedIndices);
        n20MHzChannels /= 2;
    }

    // finally, add the appropriate offset if width is less than the operational channel width
    auto offset = GetPrimaryChannelIndex(width) * width / 20;

    if (offset > 0)
    {
        std::set<uint8_t> updatedIndices;
        for (const auto& idx : indices)
        {
            updatedIndices.insert(idx + offset);
        }
        indices.swap(updatedIndices);
    }

    return indices;
}

std::size_t
WifiPhyOperatingChannel::GetNSegments() const
{
    return m_channelIts.size();
}

bool
WifiPhyOperatingChannel::operator==(const WifiPhyOperatingChannel& other) const
{
    return m_channelIts == other.m_channelIts;
}

bool
WifiPhyOperatingChannel::operator!=(const WifiPhyOperatingChannel& other) const
{
    return !(*this == other);
}

std::ostream&
operator<<(std::ostream& os, const WifiPhyOperatingChannel& channel)
{
    if (channel.IsSet())
    {
        os << "channel " << +channel.GetNumber() << " in band " << channel.GetPhyBand()
           << " frequency " << channel.GetFrequency() << " width " << channel.GetWidth();
    }
    else
    {
        os << "channel not set";
    }
    return os;
}

} // namespace ns3
