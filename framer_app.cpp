 #include <iostream>
  4 #include <string>
  5 #include <unordered_map>
  6 #include <tuple>
  7
  8 #include "linux_drivers.hpp"
  9
 10 #include "framer_block.hpp"
 11
 12 struct ActionInfo
 13 {
 14     std::string name;
 15     std::string agruments;
 16     std::string description;
 17 };
 18
 19 const std::unordered_map<std::string, ActionInfo> kActionsList = {
 20     {"init", {"init", "", "Initialization"}},
 21     {"ver", {"ver", "", "Read IP block version"}},
 22     {"reg", {"reg", "<reg_idx> [val]", "Read/Write register"}},
 23     {"nbits", {"nbits", "<reg_idx> <start_bit> <n_bits> <val>", "Write N bits"}},
 24     {"set_kill", {"set_kill", "[0/1]", "Get/set kill-register value"}},
 25     {"stat", {"stat", "[reset]", "Print statistics"}},
 26     {"cfg", {"cfg", "", "Get current framer config"}},
 27     {"internal_rtc_period_ns", {"internal_rtc_period_ns", "[float_value]", "Get/set internal_rtc period [ns]"}},
 28     {"use_internal_rtc", {"use_internal_rtc", "[0/1]", "Get/set use_internal_rtc flag"}},
 29     {"pps_sync", {"pps_sync", "[sync_type]", "Get/set pps sync strategy. Supported sync_types: off, each_pps, once_after_restart"}},
 30     {"rtc_frame_period_ns", {"rtc_frame_period_ns", "[val]", "Get/set frame period in ns"}},
 31     {"rtc_slot_period_ns", {"rtc_slot_period_ns", "[val]", "Get/set slot period in ns"}},
 32     {"cls_frame_period_clks", {"cls_frame_period_clks", "[val]", "Get/set frame period in clock cycles"}},
 33     {"cls_slot_period_clks", {"cls_slot_period_clks", "[val]", "Get/set slot period in clock cycles"}},
 34     {"cls_subframe_period_clks", {"cls_subframe_period_clks", "[val]", "Get/set subframe period in clock cycles"}},
 35     {"nf_shift", {"nf_shift", "<shift_type> [val]", "Shift next_frame by N clocks. Supported shift_types: dl_oran, ul_oran, dl_dfe, ul_dfe, dl_beamer, ul    _beamer, fem_ctrl, ext_dev."}},
 36     {"ns_shift", {"ns_shift", "<shift_type> [val]", "Shift next_slot/subframe by N clocks. Supported shift_types: dl_oran, ul_oran, dl_dfe, ul_dfe, dl_be    amer, ul_beamer."}},
 37     {"set_nf_out_en", {"set_nf_out_en", "0/1", "Set next_frame output enable"}},
 38 };
 39
 40 bool isInActionsList(const std::string& actName)
 41 {
                                                               42     for(const auto& a : kActionsList)
 43         if(a.second.name == actName)
 44             return true;
 45     return false;
 46 }
 47
 48 // regmap size (Bytes)
 49 const size_t kMapSize = 4 * (1 << 10);
 50
 51 int main(int argc, char **argv)
 52 {
 53     // read and parse paths to uio device
 54     const uint32_t numUioDevices = 1;
 55     const int actionPosition = numUioDevices + 1;
 56     if(argc < (numUioDevices + 1))
 57     {
 58         std::cout << "Specify <framer-uio> <action>\n";
 59         std::cout << "List of available actions:\n";
 60         for(const auto& a : kActionsList)
 61             std::cout << "\t* " << a.second.name << " " << a.second.agruments << " - " << a.second.description << "\n";
 62         return 0;
 63     }
 64     std::string uioString(argv[1]);
 65
 66     // read and parse actions
 67     if(argc < (numUioDevices + 2))
 68     {
 69         std::cout << "Specify one of the following actions:\n";
 70         for(const auto& a : kActionsList)
 71             std::cout << "\t* " << a.second.name << " " << a.second.agruments << " - " << a.second.description << "\n";
 72         return 0;
 73     }
 74     else
 75     {
 76         std::string actionString(argv[actionPosition]);
 77
 78         if(!isInActionsList(actionString))
 79         {
 80             std::cout << "Specify one of the following actions:\n";
 81             for(const auto& a : kActionsList)
 82                 std::cout << "\t* " << a.second.name << " " << a.second.agruments << " - " << a.second.description << "\n";
 83             return 0;
 84         }
 86         LinuxUioBlockDriver uioDrv(uioString, kMapSize);
 87         if(uioDrv.getStatus() != LinuxUioDriverStatus::success)
 88         {
 89             std::cout << "UIO driver initialization failed with code " << int(uioDrv.getStatus()) << "\n";
 90             return -1;
 91         }
 92         FramerBlock framerBlock(&uioDrv);
 93
 94         if(actionString == kActionsList.at("init").name)
 95         {
 96             std::cout << "Framer v" << framerBlock.getIpVersionString() << "\n";
 97             if(!framerBlock.checkMinRequiredVersion())
 98             {
 99                 uint8_t x, y, z;
100                 std::tie(x, y, z) = framerBlock.getMinRequiredVersion();
101                 std::string reqVerStr = std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
102                 std::cout << "WARNING: a minimal required version is v" << reqVerStr << ".\nA proper block behavior is not guaranteed!\n";
103             }
104
105             framerBlock.init();
106         }
107         else if(actionString == kActionsList.at("ver").name)
108         {
109             std::cout << "Framer v" << framerBlock.getIpVersionString() << "\n";
110             if(!framerBlock.checkMinRequiredVersion())
111             {
112                 uint8_t x, y, z;
113                 std::tie(x, y, z) = framerBlock.getMinRequiredVersion();
114                 std::string reqVerStr = std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
115                 std::cout << "WARNING: a minimal required version is v" << reqVerStr << ".\nA proper block behavior is not guaranteed!\n";
116             }
117         }
118         else if(actionString == kActionsList.at("reg").name)
119         {
120             const auto& act = kActionsList.at("reg");
121
122             if(argc >= (actionPosition + 2))
123             {
124                 uint32_t regIdx = std::stoul(argv[actionPosition + 1]);
125
126                 if(argc >= (actionPosition + 3))
127                 {
128                     uint32_t regValue = std::stoul(argv[actionPosition + 2]);
129                     uioDrv.writeRegister(regIdx, regValue);
130                 }
131
132                 uint32_t readRegValue = uioDrv.readRegister(regIdx);
133                 std::cout << "reg[" << regIdx << "] = " << readRegValue << "\n";
134             }
135             else
136             {
137                 std::cout << "Syntax error, the proper syntax is:" << act.name << " " << act.agruments << "\n";
138             }
139         }
140         else if(actionString == kActionsList.at("nbits").name)
141         {
142             const auto& act = kActionsList.at("nbits");
143
144             if(argc >= (actionPosition + 4))
145             {
146                 uint32_t regIdx = std::stoul(argv[actionPosition + 1]);
147                 uint32_t startBit = std::stoul(argv[actionPosition + 2]);
148                 uint32_t nBits = std::stoul(argv[actionPosition + 3]);
149                 uint32_t value = std::stoul(argv[actionPosition + 4]);
150                 uioDrv.writeNBitsFrom(regIdx, startBit, nBits, value);
151
152                 uint32_t readRegValue = uioDrv.readRegister(regIdx);
153                 std::cout << "reg[" << regIdx << "] = " << readRegValue << "\n";
154             }
155             else
156             {
157                 std::cout << "Syntax error, the proper syntax is:" << act.name << " " << act.agruments << "\n";
158             }
159         }
160         else if(actionString == kActionsList.at("set_kill").name)
161         {
162             if(argc >= (actionPosition + 2))
163             {
164                 uint8_t killValue = std::stoul(argv[actionPosition + 1]);
165                 framerBlock.setKill(killValue != 0);
166             }
167         }
168         else if(actionString == kActionsList.at("stat").name)
169         {
170             if((argc >= (actionPosition + 2))
171                 && (std::string(argv[actionPosition + 1]) == std::string("reset")))
172             {
173                 framerBlock.resetStat();
174                 std::cout << "Stat reset done\n";
175             }
176             else
177             {
178                 std::cout << framerBlock.getStatString();
179             }
180         }
181         else if(actionString == kActionsList.at("cfg").name)
182         {
183             std::cout << framerBlock.getConfigString();
184         }
185         else if(actionString == kActionsList.at("internal_rtc_period_ns").name)
186         {
187             if(argc >= (actionPosition + 2))
188             {
189                 float periodNs = std::stof(argv[actionPosition + 1]);
190                 framerBlock.setInternalRtcPeriodNs(periodNs);
191             }
192             std::cout << "internal_rtc_period [ns]: " << framerBlock.getInternalRtcPeriodNs() << "\n";
193         }
194         else if(actionString == kActionsList.at("use_internal_rtc").name)
195         {
196             if(argc >= (actionPosition + 2))
197             {
198                 bool flag = (std::stoul(argv[actionPosition + 1]) == 1);
199                 framerBlock.setUseInternalRtcFlag(flag);
200             }
201             std::cout << "use_internal_rtc_flag: " << framerBlock.getUseInternalRtcFlag() << "\n";
202         }
203         else if(actionString == kActionsList.at("rtc_frame_period_ns").name)
204         {
205             if((argc >= (actionPosition + 2)))
206             {
207                 uint32_t nsValue = std::stoul(argv[actionPosition + 1]);
208                 framerBlock.setRtcFramePeriodNs(nsValue);
209             }
210
211             std::cout << "rtc_frame_period_ns: " << framerBlock.getRtcFramePeriodNs() << "\n";
212         }213         else if(actionString == kActionsList.at("rtc_slot_period_ns").name)
214         {
215             if((argc >= (actionPosition + 2)))
216             {
217                 uint32_t nsValue = std::stoul(argv[actionPosition + 1]);
218                 framerBlock.setRtcSlotPeriodNs(nsValue);
219             }
220
221             std::cout << "rtc_slot_period_ns: " << framerBlock.getRtcSlotPeriodNs() << "\n";
222         }
223         else if(actionString == kActionsList.at("cls_frame_period_clks").name)
224         {
225             if((argc >= (actionPosition + 2)))
226             {
227                 uint32_t clkValue = std::stoul(argv[actionPosition + 1]);
228                 framerBlock.setClsFramePeriodClks(clkValue);
229             }
230
231             std::cout << "cls_frame_period_clks: " << framerBlock.getClsFramePeriodClks() << "\n";
232         }
233         else if(actionString == kActionsList.at("cls_slot_period_clks").name)
234         {
235             if((argc >= (actionPosition + 2)))
236             {
237                 uint32_t clkValue = std::stoul(argv[actionPosition + 1]);
238                 framerBlock.setClsSlotPeriodClks(clkValue);
239             }
240
241             std::cout << "cls_slot_period_clks: " << framerBlock.getClsSlotPeriodClks() << "\n";
242         }
243         else if(actionString == kActionsList.at("cls_subframe_period_clks").name)
244         {
245             if((argc >= (actionPosition + 2)))
246             {
247                 uint32_t clkValue = std::stoul(argv[actionPosition + 1]);
248                 framerBlock.setClsSubframePeriodClks(clkValue);
249             }
250
251             std::cout << "cls_subframe_period_clks: " << framerBlock.getClsSubframePeriodClks() << "\n";
252         }
253         else if(actionString == kActionsList.at("nf_shift").name)
254         {
255             if((argc >= (actionPosition + 2)))
256             {
257                 std::unordered_map<std::string, FramerBlock::NfShiftType> shiftTypeMap{
258                     {"dl_oran", FramerBlock::NfShiftType::dl_oran},
259                     {"ul_oran", FramerBlock::NfShiftType::ul_oran},
260                     {"dl_dfe", FramerBlock::NfShiftType::dl_dfe},
261                     {"ul_dfe", FramerBlock::NfShiftType::ul_dfe},
262                     {"dl_beamer", FramerBlock::NfShiftType::dl_beamer},
263                     {"ul_beamer", FramerBlock::NfShiftType::ul_beamer},
264                     {"fem_ctrl", FramerBlock::NfShiftType::fem_ctrl},
265                     {"ext_dev", FramerBlock::NfShiftType::ext_dev}
266                 };
267
268                 std::string shiftTypeStr(argv[actionPosition + 1]);
269                 if(shiftTypeMap.count(shiftTypeStr) == 0)
270                 {
271                     std::cout << "Unsupported shift type\n";
272                     return 0;
273                 }
274
275                 if((argc >= (actionPosition + 3)))
276                 {
277                     uint32_t shiftClks = std::stoul(argv[actionPosition + 2]);
278                     framerBlock.setNextFrameShiftClks(shiftTypeMap.at(shiftTypeStr), shiftClks);
279                 }
280
281                 std::cout << shiftTypeStr << "_nf_shift_clks: " << framerBlock.getNextFrameShiftClks(shiftTypeMap.at(shiftTypeStr)) << "\n";
282             }
283             else
284             {
285                 const auto& a = kActionsList.at("nf_shift");
286                 std::cout << "Syntax error.\n";
287                 std::cout << a.name << " " << a.agruments << " - " << a.description << "\n";
288             }
289         }
290         else if(actionString == kActionsList.at("ns_shift").name)
291         {
292             if((argc >= (actionPosition + 2)))
293             {
294                 std::unordered_map<std::string, FramerBlock::NsShiftType> shiftTypeMap{
295                     {"dl_oran", FramerBlock::NsShiftType::dl_oran},
296                     {"ul_oran", FramerBlock::NsShiftType::ul_oran},
297                     {"dl_dfe", FramerBlock::NsShiftType::dl_dfe},
298                     {"ul_dfe", FramerBlock::NsShiftType::ul_dfe},
299                     {"dl_beamer", FramerBlock::NsShiftType::dl_beamer},
300                     {"ul_beamer", FramerBlock::NsShiftType::ul_beamer}
301                 };
302
303                 std::string shiftTypeStr(argv[actionPosition + 1]);
304                 if(shiftTypeMap.count(shiftTypeStr) == 0)
305                 {
306                     std::cout << "Unsupported shift type\n";
307                     return 0;
308                 }
309
310                 if((argc >= (actionPosition + 3)))
311                 {
312                     uint32_t shiftClks = std::stoul(argv[actionPosition + 2]);
313                     framerBlock.setNextSlotShiftClks(shiftTypeMap.at(shiftTypeStr), shiftClks);
314                 }
315
316                 std::cout << shiftTypeStr << "_ns_shift_clks: " << framerBlock.getNextSlotShiftClks(shiftTypeMap.at(shiftTypeStr)) << "\n";
317             }
318             else
319             {
320                 const auto& a = kActionsList.at("ns_shift");
321                 std::cout << "Syntax error.\n";
322                 std::cout << a.name << " " << a.agruments << " - " << a.description << "\n";
323             }
324         }
325         else if(actionString == kActionsList.at("pps_sync").name)
326         {
327             std::unordered_map<std::string, FramerBlock::PpsSyncType> syncStrategyMap{
328                 {"off", FramerBlock::PpsSyncType::sync_off},
329                 {"each_pps", FramerBlock::PpsSyncType::sync_each_pps},
330                 {"once_after_reset", FramerBlock::PpsSyncType::sync_once_after_reset},
331             };
332             if((argc >= (actionPosition + 2)))
333             {
334                 std::string syncTypeStr = argv[actionPosition + 1];
335                 if(syncStrategyMap.count(syncTypeStr))
336                     framerBlock.setPpsSyncType(syncStrategyMap.at(syncTypeStr));
337                 else
338                     std::cout << "Unsupported pps_sync type\n";
339             }
340
341             std::cout << "pps_sync type: " << framerBlock.getPpsSyncTypeString() << "\n";
342         }
343         else if(actionString == kActionsList.at("set_nf_out_en").name)
344         {
345             if(argc >= (actionPosition + 2))
346             {
347                 uint8_t enVal = std::stoul(argv[actionPosition + 1]);
348                 framerBlock.setNextFrameOutputEnable(enVal != 0);
349
350                 std::cout << "nf_out_en = " << (int) enVal << "\n";
351             }
352             else
353             {
354                 std::cout << "Error: specify value 0/1.\n";
355             }
356         }
357     }
358
359     return 0;
360 }

          
		  
#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>

#include "linux_drivers.hpp"
#include "framer_block.hpp"

Thse
/rg/ounas-software/modem-software/libs/modem-ip-ctrl-lib/include 

struct ActionInfo
{
    std::string name;
    std::string arguments;
    std::string description;
};
This defines a struct called ActionInfo. It's used to store information about different actions, including their names, arguments, and descriptions.

const std::unordered_map<std::string, ActionInfo> kActionsList = {
    {"init", {"init", "", "Initialization"}},
    // ...
};
This creates a constant unordered_map called kActionsList. It associates action names (strings) with their corresponding ActionInfo structures.

bool isInActionsList(const std::string& actName)
{
    for(const auto& a : kActionsList)
        if(a.second.name == actName)
            return true;
    return false;
}
This is a function called isInActionsList. It checks if a given action name (actName) is present in the kActionsList map and returns true if it is, otherwise false.

const size_t kMapSize = 4 * (1 << 10);
This defines a constant kMapSize and sets it to a value of 4KB (kilobytes). It's likely used to determine the size of a memory map.

int main(int argc, char **argv)

This is the entry point of the program, the main function, which takes command-line arguments argc (argument count) and argv (argument vector).

const uint32_t numUioDevices = 1;
const int actionPosition = numUioDevices + 1;

These lines declare two constants: numUioDevices (set to 1) and actionPosition (calculated as numUioDevices + 1). These constants are used later to determine the position of the action argument in the command line arguments.

if(argc < (numUioDevices + 1))
{
    // Print usage information if there are not enough arguments.
    // This section provides guidance on how to use the program.
    std::cout << "Specify <framer-uio> <action>\n";
    std::cout << "List of available actions:\n";
    for(const auto& a : kActionsList)
        std::cout << "\t* " << a.second.name << " " << a.second.arguments << " - " << a.second.description << "\n";
    return 0;
}
This block checks if there are not enough command-line arguments and provides usage instructions if the condition is met. It displays the available actions and their descriptions.

std::string uioString(argv[1]);

This line stores the first command-line argument (argv[1]) into a std::string variable called uioString. It appears to be related to a UIO device.

 if(argc < (numUioDevices + 2))
 68     {
 69         std::cout << "Specify one of the following actions:\n";
 70         for(const auto& a : kActionsList)
 71             std::cout << "\t* " << a.second.name << " " << a.second.agruments << " - " << a.second.description << "\n";
 72         return 0;
 73     }
if(argc < (numUioDevices + 2)): This condition checks whether the total number of command-line arguments (argc) is less than numUioDevices + 2. numUioDevices was defined earlier as 1, and 2 is added to it to account for the framer-uio argument and the action argument. This condition checks if there are not enough arguments to specify an action.

If the condition is true, it means there are not enough command-line arguments provided to specify an action. In this case, the program does the following:

std::cout << "Specify one of the following actions:\n";: It outputs a message to the standard output (typically the console) to inform the user that they need to specify one of the available actions.

for(const auto& a : kActionsList): This is a loop that iterates over each entry (action) in the kActionsList map.

std::cout << "\t* " << a.second.name << " " << a.second.agruments << " - " << a.second.description << "\n";: Within the loop, it prints information about each available action, including its name, arguments, and a description. This information is retrieved from the ActionInfo structure associated with each action.

Finally, after listing the available actions and their descriptions, the program returns 0, which typically signifies a successful execution or normal termination of the program.

else
{
    std::string actionString(argv[actionPosition]);

When the if condition in the previous code block (checking for enough arguments) is false, it means the program has received enough arguments to specify an action. In this case, the code proceeds to extract the action string from the command-line arguments. It does this by accessing argv[actionPosition], where actionPosition is previously calculated and points to the location of the action argument in the argv array.

    if(!isInActionsList(actionString))
    {
        std::cout << "Specify one of the following actions:\n";
        for(const auto& a : kActionsList)
            std::cout << "\t* " << a.second.name << " " << a.second.arguments << " - " << a.second.description << "\n";
        return 0;
    }
After extracting the action string, the code checks if it exists in the kActionsList map by calling the isInActionsList function. This function was defined earlier in the code to check if a given action string is valid.

If the action string is not found in the list of valid actions (kActionsList), it means the user provided an invalid action. In this case, the program does the following:

Outputs a message to the console, instructing the user to specify one of the available actions.

Lists the available actions along with their arguments and descriptions using a loop similar to the one explained in the previous code block.

Finally, it returns 0, indicating that the program has completed normally.

In summary, this part of the code ensures that the provided action string is valid and corresponds to one of the available actions. If the action is invalid, it informs the user of the available options and terminates the program with a status code of 0. If the action is valid, the program will continue to execute code related to that specific action.

LinuxUioBlockDriver uioDrv(uioString, kMapSize);
if(uioDrv.getStatus() != LinuxUioDriverStatus::success)
{
    std::cout << "UIO driver initialization failed with code " << int(uioDrv.getStatus()) << "\n";
    return -1;
}

It creates an instance of the LinuxUioBlockDriver class named uioDrv by passing the uioString (presumably representing a UIO device) and kMapSize as constructor arguments. This object is used for interacting with the UIO driver.

It checks the status of the UIO driver by calling uioDrv.getStatus(). If the driver's status is not LinuxUioDriverStatus::success, it means that the UIO driver initialization failed for some reason.

If the UIO driver initialization fails, it prints an error message to the console, indicating the failure and the status code returned by the driver. Then, it returns -1, which is often used to indicate an error condition.

FramerBlock framerBlock(&uioDrv);
Assuming the UIO driver initialization was successful, it creates an instance of the FramerBlock class named framerBlock, passing a pointer to the uioDrv object as a constructor argument. This suggests that the framerBlock will interact with the hardware using the UIO driver.
cpp

if(actionString == kActionsList.at("init").name)
{
    std::cout << "Framer v" << framerBlock.getIpVersionString() << "\n";
    if(!framerBlock.checkMinRequiredVersion())
    {
        uint8_t x, y, z;
        std::tie(x, y, z) = framerBlock.getMinRequiredVersion();
        std::string reqVerStr = std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
        std::cout << "WARNING: a minimal required version is v" << reqVerStr << ".\nA proper block behavior is not guaranteed!\n";
    }
}
If the specified action string is "init" (indicating an initialization action), it performs the following actions:

It prints a message indicating the version of the framer block, obtained by calling framerBlock.getIpVersionString().

It checks if the framer block meets the minimum required version by calling framerBlock.checkMinRequiredVersion(). If the minimum version requirement is not met, it does the following:

Retrieves the minimum required version as three values x, y, and z using std::tie.

Converts these values into a string reqVerStr representing the required version.

Prints a warning message indicating the minimum required version and the potential consequences of not meeting this requirement.

  else if(actionString == kActionsList.at("ver").name)
108         {
109             std::cout << "Framer v" << framerBlock.getIpVersionString() << "\n";
110             if(!framerBlock.checkMinRequiredVersion())
111             {
112                 uint8_t x, y, z;
113                 std::tie(x, y, z) = framerBlock.getMinRequiredVersion();
114                 std::string reqVerStr = std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
115                 std::cout << "WARNING: a minimal required version is v" << reqVerStr << ".\nA proper block behavior is not guaranteed!\n";
116             }
117         }

This line appears to be part of an else if conditional block. It checks whether the value of actionString is equal to the name attribute associated with the key "ver" in the kActionsList map (assuming kActionsList is a map-like data structure).
If the condition in the if statement on line 108 is true, this line will execute. It uses std::cout to print a message to the console. The message is constructed as "Framer v" followed by the result of calling the framerBlock.getIpVersionString() function. This function presumably returns a string representing the IP version of the "Frame

 if(!framerBlock.checkMinRequiredVersion())
This line checks whether the framerBlock object meets the minimum required version by calling the framerBlock.checkMinRequiredVersion() function. The ! operator negates the result, so the block inside the if will execute if the minimum required version check fails (i.e., if the function returns false).

               uint8_t x, y, z;
			   Here, three uint8_t variables (x, y, and z) are declared to store the components of the minimum required version

std::tie(x, y, z) = framerBlock.getMinRequiredVersion();
This line calls framerBlock.getMinRequiredVersion() and uses std::tie to unpack the returned values (presumably version components) into the x, y, and z variables.
       std::string reqVerStr = std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
It constructs a string reqVerStr by concatenating the values of x, y, and z (converted to strings) with dots in between. This creates a string in the format "X.Y.Z," representing the minimum required version.

  std::cout << "WARNING: a minimal required version is v" << reqVerStr << ".\nA proper block behavior is not guaranteed!\n";
  
  This line uses std::cout to print a warning message. It includes the "WARNING: a minimal required version is v" string followed by the reqVerStr (representing the minimum required version) and a message indicating that proper block behavior is not guaranteed if the minimum version is not met.
  
  116             }
117         }

These lines close the if block that handles the case where the minimum required version check fails, and they close the else if block that started on line 108. If the condition on line 108 is not met or if the minimum version check succeeds, these lines are skipped

else if(actionString == kActionsList.at("reg").name)
This line is part of a series of else if statements. It checks whether the value of actionString is equal to the name attribute associated with the key "reg" in the kActionsList map. If this condition is true, the block of code inside this else if branch will execute
120             const auto& act = kActionsList.at("reg");

This line creates a constant reference act to the value associated with the key "reg" in the kActionsList map. It appears to be retrieving information about the "reg" action from the map.
122             if(argc >= (actionPosition + 2))
This line checks if the number of command-line arguments (argc) is greater than or equal to actionPosition + 2. It ensures that there are enough arguments to proceed with the "reg" action. actionPosition seems to be a variable indicating the position of the action in the command-line arguments.
124                 uint32_t regIdx = std::stoul(argv[actionPosition + 1]);
If there are enough arguments, this line extracts the next argument (presumably a numeric index) from argv and converts it to an unsigned 32-bit integer (uint32_t).
128                 uint32_t regValue = std::stoul(argv[actionPosition + 2]);
129                 uioDrv.writeRegister(regIdx, regValue);
These lines extract the argument at actionPosition + 2, which is presumably a numeric value, and convert it to an unsigned 32-bit integer. It then calls uioDrv.writeRegister with regIdx and regValue to write a value to a register.

132                 uint32_t readRegValue = uioDrv.readRegister(regIdx);
133                 std::cout << "reg[" << regIdx << "] = " << readRegValue << "\n";
These lines read the value from the register at index regIdx using uioDrv.readRegister and store it in readRegValue. Then, it prints a message to the console displaying the index and value of the register.
136             else
137             {
138                 std::cout << "Syntax error, the proper syntax is:" << act.name << " " << act.arguments << "\n";
139             }
If there are not enough arguments to perform the "reg" action, this block is executed. It prints an error message to the console, indicating a syntax error and providing the expected syntax for the "reg" action, including act.name and act.arguments.
The code follows a similar structure for other actions ("nbits" in this case), checking the action type, validating arguments, and performing actions or displaying error messages accordingly.

else if(actionString == kActionsList.at("nbits").name)
This is another else if statement. It checks whether actionString is equal to the name attribute associated with the key "nbits" in the kActionsList map. If this condition is true, the block of code inside this else if branch will execute, indicating that the program should perform the "nbits" action.
cpp
142             const auto& act = kActionsList.at("nbits");
This line creates a constant reference act to the value associated with the key "nbits" in the kActionsList map. It appears to be retrieving information about the "nbits" action from the map.
144             if(argc >= (actionPosition + 4))
Similar to the previous block, this line checks whether there are enough command-line arguments (argc) to proceed with the "nbits" action. It ensures that there are at least actionPosition + 4 arguments available.
146                 uint32_t regIdx = std::stoul(argv[actionPosition + 1]);
If there are enough arguments, this line extracts the next argument (presumably a numeric index) from argv and converts it to an unsigned 32-bit integer (uint32_t).

147                 uint32_t startBit = std::stoul(argv[actionPosition + 2]);
148                 uint32_t nBits = std::stoul(argv[actionPosition + 3]);
149                 uint32_t value = std::stoul(argv[actionPosition + 4]);
These lines extract additional arguments, specifically startBit, nBits, and value, converting them to unsigned 32-bit integers. These arguments seem to be related to bit manipulation operations.
150                 uioDrv.writeNBitsFrom(regIdx, startBit, nBits, value);
This line calls uioDrv.writeNBitsFrom with the extracted arguments, presumably to perform a write operation on a specific register with bit manipulation.
cpp
152                 uint32_t readRegValue = uioDrv.readRegister(regIdx);
153                 std::cout << "reg[" << regIdx << "] = " << readRegValue << "\n";
After the operation is performed, these lines read the value from the register at index regIdx using uioDrv.readRegister and store it in readRegValue. Then, it prints a message to the console displaying the index and value of the register.
155             else
156             {
157                 std::cout << "Syntax error, the proper syntax is:" << act.name << " " << act.agruments << "\n";
158             }
If there are not enough arguments to perform the "nbits" action, this block is executed. It prints an error message to the console, indicating a syntax error and providing the expected syntax for the "nbits" action, including act.name and presumably act.arguments.

else if(actionString == kActionsList.at("stat").name)
This is another else if statement. It checks whether actionString is equal to the name attribute associated with the key "stat" in the kActionsList map. If this condition is true, the block of code inside this else if branch will execute, indicating that the program should perform the "stat" action.

170             if((argc >= (actionPosition + 2))
171                 && (std::string(argv[actionPosition + 1]) == std::string("reset")))
This block checks two conditions. First, it verifies that there are at least actionPosition + 2 command-line arguments available. Second, it checks whether the argument immediately after the action is the string "reset." If both conditions are true, it proceeds.
173                 framerBlock.resetStat();
174                 std::cout << "Stat reset done\n";
f the conditions are met, it calls framerBlock.resetStat() to reset some statistics related to the "stat" action. Afterward, it prints a message to the console confirming that the statistics reset is done.
176             else
177             {
178                 std::cout << framerBlock.getStatString();
179             }
If the conditions in lines 170 and 171 are not met, this block is executed. It prints the statistics string using framerBlock.getStatString(). This is likely to display the current statistics related to the "stat" action.
The code continues with more "else if" branches for different actions:
181         else if(actionString == kActionsList.at("cfg").name)
This else if branch checks whether actionString is equal to the name attribute associated with the key "cfg" in the kActionsList map. If true, it prints the configuration string using framerBlock.getConfigString().
cpp
185         else if(actionString == kActionsList.at("internal_rtc_period_ns").name)
This else if branch checks whether actionString is equal to the name attribute associated with the key "internal_rtc_period_ns" in the kActionsList map. If true, it checks for the presence of an additional argument, presumably a floating-point number representing a period in nanoseconds. If the argument is present, it sets the internal RTC period using framerBlock.setInternalRtcPeriodNs(periodNs) and then prints the current internal RTC period.
195         else if(actionString == kActionsList.at("use_internal_rtc").name)
This else if branch checks whether actionString is equal to the name attribute associated with the key "use_internal_rtc" in the kActionsList map. If true, it checks for the presence of an additional argument, which is expected to be either "0" or "1" and sets the use internal RTC flag accordingly using framerBlock.setUseInternalRtcFlag(flag). Then, it prints the current value of the use internal RTC flag.
These "else if" branches follow a similar structure: they check the action type, validate arguments if necessary, perform actions, and print results or error messages accordingly.


203         else if(actionString == kActionsList.at("rtc_frame_period_ns").name)

This else if statement checks whether actionString is equal to the name attribute associated with the key "rtc_frame_period_ns" in the kActionsList map. If true, it indicates that the program should handle the "rtc_frame_period_ns" action.
205             if((argc >= (actionPosition + 2)))
Inside this block, it checks whether there are at least actionPosition + 2 command-line arguments available. It ensures that there is an additional argument provided for this action.

If there are enough arguments, it extracts the next argument (presumably representing a value in nanoseconds) from argv and converts it to an unsigned 32-bit integer (uint32_t). Then, it sets the RTC frame period using framerBlock.setRtcFramePeriodNs(nsValue).

211             std::cout << "rtc_frame_period_ns: " << framerBlock.getRtcFramePeriodNs() << "\n";

Regardless of whether the argument was provided or not, it prints the current value of the RTC frame period in nanoseconds using framerBlock.getRtcFramePeriodNs().
The code follows a similar structure for the next "else if" branches, each handling different actions:

213         else if(actionString == kActionsList.at("rtc_slot_period_ns").name)

This branch checks whether actionString matches the key "rtc_slot_period_ns" and handles a similar structure as the previous branch but for setting and printing the RTC slot period in nanoseconds.

223         else if(actionString == 
kActionsList.at("cls_frame_period_clks").name)

This branch checks whether actionString matches the key "cls_frame_period_clks" and handles setting and printing the clock (clks) value for the clear frame period.

233         else if(actionString == kActionsList.at("cls_slot_period_clks").name)

Similarly, this branch checks whether actionString matches the key "cls_slot_period_clks" and handles setting and printing the clock (clks) value for the clear slot period.
In each of these branches, the code checks for the presence of an additional argument, converts it to the appropriate data type, and then sets or prints the corresponding value in the framerBlock. These branches allow the program to configure various parameters of the framerBlock based on user input.
else if(actionString == kActionsList.at("cls_slot_period_clks").name)
This else if statement checks whether actionString is equal to the name attribute associated with the key "cls_slot_period_clks" in the kActionsList map. If true, it indicates that the program should handle the "cls_slot_period_clks" action.
235             if((argc >= (actionPosition + 2)))
Inside this block, it checks whether there are at least actionPosition + 2 command-line arguments available. It ensures that there is an additional argument provided for this action.
237                 uint32_t clkValue = std::stoul(argv[actionPosition + 1]);
238                 framerBlock.setClsSlotPeriodClks(clkValue);
If there are enough arguments, it extracts the next argument (presumably representing a value in clock cycles) from argv and converts it to an unsigned 32-bit integer (uint32_t). Then, it sets the clear slot period in clock cycles using framerBlock.setClsSlotPeriodClks(clkValue).
241             std::cout << "cls_slot_period_clks: " << framerBlock.getClsSlotPeriodClks() << "\n";
Regardless of whether the argument was provided or not, it prints the current value of the clear slot period in clock cycles using framerBlock.getClsSlotPeriodClks().
The code follows a similar structure for the next "else if" branch:
243         else if(actionString == kActionsList.at("cls_subframe_period_clks").name)
This branch checks whether actionString matches the key "cls_subframe_period_clks" and handles a similar structure as the previous branch but for setting and printing the clear subframe period in clock cycles.
In each of these branches, the code checks for the presence of an additional argument, converts it to the appropriate data type, and then sets or prints the corresponding value in the framerBlock. These branches allow the program to configure various parameters related to clock cycles for the framerBlock based on user input.



else if(actionString == kActionsList.at("nf_shift").name)
This else if statement checks whether actionString is equal to the name attribute associated with the key "nf_shift" in the kActionsList map. If true, it indicates that the program should handle the "nf_shift" action.
255             if((argc >= (actionPosition + 2)))
Inside this block, it checks whether there are at least actionPosition + 2 command-line arguments available. It ensures that there are enough arguments provided for this action.
257                 std::unordered_map<std::string, FramerBlock::NfShiftType> shiftTypeMap{
258                     {"dl_oran", FramerBlock::NfShiftType::dl_oran},
259                     {"ul_oran", FramerBlock::NfShiftType::ul_oran},
260                     {"dl_dfe", FramerBlock::NfShiftType::dl_dfe},
261                     {"ul_dfe", FramerBlock::NfShiftType::ul_dfe},
262                     {"dl_beamer", FramerBlock::NfShiftType::dl_beamer},
263                     {"ul_beamer", FramerBlock::NfShiftType::ul_beamer},
264                     {"fem_ctrl", FramerBlock::NfShiftType::fem_ctrl},
265                     {"ext_dev", FramerBlock::NfShiftType::ext_dev}

This block creates an unordered map called shiftTypeMap, which maps string names of shift types to enum values of FramerBlock::NfShiftType. This map is used to associate string inputs with their corresponding enum values.

268                 std::string shiftTypeStr(argv[actionPosition + 1]);
269                 if(shiftTypeMap.count(shiftTypeStr) == 0)
270                 {
271                     std::cout << "Unsupported shift type\n";
272                     return 0;
273                 }


It extracts the next command-line argument, which is expected to be a string representing a shift type. It then checks if this string exists in the shiftTypeMap. If it doesn't exist (count is zero), it prints an error message and returns from the function with a return code of 0, likely indicating an error.

275                 if((argc >= (actionPosition + 3)))
276                 {
277                     uint32_t shiftClks = std::stoul(argv[actionPosition + 2]);
278                     framerBlock.setNextFrameShiftClks(shiftTypeMap.at(shiftTypeStr), shiftClks);
279                 }
If there are enough arguments, it extracts the next argument (presumably a numeric value) from argv and converts it to an unsigned 32-bit integer (uint32_t). It then calls framerBlock.setNextFrameShiftClks with the shift type (obtained from shiftTypeMap) and the shift value in clock cycles.

281                 std::cout << shiftTypeStr << "_nf_shift_clks: " << framerBlock.getNextFrameShiftClks(shiftTypeMap.at(shiftTypeStr)) << "\n";


Finally, it prints the shift type and the corresponding shift value in clock cycles using framerBlock.getNextFrameShiftClks(shiftTypeMap.at(shiftTypeStr)).
This code allows the program to handle the "nf_shift" action by parsing the shift type and value from command-line arguments and updating the appropriate shift values in the framerBlock. It then prints the updated shift values.





else if(actionString == kActionsList.at("ns_shift").name)
else if(actionString == kActionsList.at("ns_shift").name)
292             if((argc >= (actionPosition + 2)))
Inside this block, it checks whether there are at least actionPosition + 2 command-line arguments available. It ensures that there are enough arguments provided for this action.
294                 std::unordered_map<std::string, FramerBlock::NsShiftType> shiftTypeMap{
295                     {"dl_oran", FramerBlock::NsShiftType::dl_oran},
296                     {"ul_oran", FramerBlock::NsShiftType::ul_oran},
297                     {"dl_dfe", FramerBlock::NsShiftType::dl_dfe},
298                     {"ul_dfe", FramerBlock::NsShiftType::ul_dfe},
299                     {"dl_beamer", FramerBlock::NsShiftType::dl_beamer},
300                     {"ul_beamer", FramerBlock::NsShiftType::ul_beamer}
301                 };
This block creates an unordered map called shiftTypeMap, which maps string names of shift types to enum values of FramerBlock::NsShiftType. This map is used to associate string inputs with their corresponding enum values.
cpp

303                 std::string shiftTypeStr(argv[actionPosition + 1]);
304                 if(shiftTypeMap.count(shiftTypeStr) == 0)
305                 {
306                     std::cout << "Unsupported shift type\n";
307                     return 0;
308                 }

It extracts the next command-line argument, which is expected to be a string representing a shift type. It then checks if this string exists in the shiftTypeMap. If it doesn't exist (count is zero), it prints an error message and returns from the function with a return code of 0, indicating an error.

310                 if((argc >= (actionPosition + 3)))
311                 {
312                     uint32_t shiftClks = std::stoul(argv[actionPosition + 2]);
313                     framerBlock.setNextSlotShiftClks(shiftTypeMap.at(shiftTypeStr), shiftClks);
314                 }


If there are enough arguments, it extracts the next argument (presumably a numeric value) from argv and converts it to an unsigned 32-bit integer (uint32_t). It then calls framerBlock.setNextSlotShiftClks with the shift type (obtained from shiftTypeMap) and the shift value in clock cycles.

316                 std::cout << shiftTypeStr << "_ns_shift_clks: " << framerBlock.getNextSlotShiftClks(shiftTypeMap.at(shiftTypeStr)) << "\n";
Finally, it prints the shift type and the corresponding shift value in clock cycles using framerBlock.getNextSlotShiftClks(shiftTypeMap.at(shiftTypeStr)).



else
This else statement is a catch-all for actions that are not recognized. If none of the previous "else if" branches match, it means the provided actionString doesn't correspond to any known action in kActionsList

319             {
320                 const auto& a = kActionsList.at("ns_shift");
321                 std::cout << "Syntax error.\n";
322                 std::cout << a.name << " " << a.agruments << " - " << a.description << "\n";
323             }

In this block, it assumes that the unrecognized action is "ns_shift" based on the previous "else if" condition. It prints a syntax error message and provides information about the expected syntax and description of the "ns_shift" action.
325         else if(actionString == kActionsList.at("pps_sync").name)

This else if branch checks whether actionString is equal to the name attribute associated with the key "pps_sync" in the kActionsList map. If true, it indicates that the program should handle the "pps_sync" action.
cpp

327             std::unordered_map<std::string, FramerBlock::PpsSyncType> syncStrategyMap{
328                 {"off", FramerBlock::PpsSyncType::sync_off},
329                 {"each_pps", FramerBlock::PpsSyncType::sync_each_pps},
330                 {"once_after_reset", FramerBlock::PpsSyncType::sync_once_after_reset},
331             };

This block creates an unordered map called syncStrategyMap, which maps string names of synchronization strategies to enum values of FramerBlock::PpsSyncType. This map is used to associate string inputs with their corresponding enum values.

332             if((argc >= (actionPosition + 2)))

Inside this block, it checks whether there are at least actionPosition + 2 command-line arguments available. It ensures that there are enough arguments provided for this action.

334                 std::string syncTypeStr = argv[actionPosition + 1];
335                 if(syncStrategyMap.count(syncTypeStr))
336                     framerBlock.setPpsSyncType(syncStrategyMap.at(syncTypeStr));
337                 else
338                     std::cout << "Unsupported pps_sync type\n";

It extracts the next command-line argument, which is expected to be a string representing a synchronization strategy. It then checks if this string exists in the syncStrategyMap. If it does exist, it sets the PPS synchronization type using framerBlock.setPpsSyncType(syncStrategyMap.at(syncTypeStr)). If it doesn't exist, it prints an error message indicating an unsupported synchronization type.

341             std::cout << "pps_sync type: " << framerBlock.getPpsSyncTypeString() << "\n";


Finally, it prints the current PPS synchronization type using framerBlock.getPpsSyncTypeString().
The code then continues with the following "else if" branch:

else if(actionString == kActionsList.at("set_nf_out_en").name)
This else if branch checks whether actionString matches the key "set_nf_out_en" and handles setting the next frame output enable.

345             if(argc >= (actionPosition + 2))
Inside this block, it checks whether there are at least actionPosition + 2 command-line arguments available, ensuring that there is an additional argument provided for this action.

347                 uint8_t enVal = std::stoul(argv[actionPosition + 1]);
348                 framerBlock.setNextFrameOutputEnable(enVal != 0);
349
350                 std::cout << "nf_out_en = " << (int) enVal << "\n";

If there are enough arguments, it extracts the next argument (presumably a numeric value, 0 or 1) from argv and converts it to an unsigned 32-bit integer (uint8_t). It then calls framerBlock.setNextFrameOutputEnable(enVal != 0) to set the next frame output enable based on the provided value. It also prints the value of "nf_out_en."
The code snippet concludes by returning 0, likely indicating successful completion of the program.

In summary, this code handles various actions based on the provided actionString, validates input arguments, and performs actions accordingly while providing appropriate error messages for syntax errors or unsupported actions.




  return 0;
402 360 }


