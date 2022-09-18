#include "systeminfo.h"



#include "../gfx/gui/gui_s.h"

#include <algorithm>

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <ntddscsi.h>
#include <Windows.h>

#pragma comment(lib, "wbemuuid.lib")



#define _WIN32_WINNT 0x0500  //0x0400 & 0x0500:Flag that this routine is NOT for Win 95 - NT.

#include <Windows.h>

// Use to change the divisor from Kb to Mb.

#if 0
typedef struct _MEMORYSTATUSEX {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORDLONG ullTotalPhys;
    DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile;
    DWORDLONG ullAvailPageFile;
    DWORDLONG ullTotalVirtual;
    DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, * LPMEMORYSTATUSEX;

WINBASEAPI
BOOL
WINAPI
GlobalMemoryStatusEx(
    IN OUT LPMEMORYSTATUSEX lpBuffer
);
#endif

#define DIV 1024
// #define DIV 1

const char* divisor = "K";
// char *divisor = "";

// Handle the width of the field in which to print numbers this way to
// make changes easier. The asterisk in the print format specifier
// "%*I64d" takes an int from the argument list, and uses it to pad
// and right-justify the number being formatted.
#define WIDTH 7

void getMemoryMetrics(MEMORYSTATUSEX& statex) {

    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);

    printf("%ld percent of memory is in use.\n",
           statex.dwMemoryLoad);
    printf("There are %*I64d total %sbytes of physical memory.\n",
           WIDTH, statex.ullTotalPhys / DIV, divisor);
    printf("There are %*I64d free %sbytes of physical memory.\n",
           WIDTH, statex.ullAvailPhys / DIV, divisor);
    printf("There are %*I64d total %sbytes of paging file.\n",
           WIDTH, statex.ullTotalPageFile / DIV, divisor);
    printf("There are %*I64d free %sbytes of paging file.\n",
           WIDTH, statex.ullAvailPageFile / DIV, divisor);
    printf("There are %*I64x total %sbytes of virtual memory.\n",
           WIDTH, statex.ullTotalVirtual / DIV, divisor);
    printf("There are %*I64x free %sbytes of virtual memory.\n",
           WIDTH, statex.ullAvailVirtual / DIV, divisor);

    // Show the amount of extended memory available.

    printf("There are %*I64x free %sbytes of extended memory.\n",
           WIDTH, statex.ullAvailExtendedVirtual / DIV, divisor);
}


#pragma region HWID

class HardwareId {

public:


	static std::wstring SafeString(const wchar_t* pString) {
		return std::wstring((pString == nullptr ? L"(null)" : pString));
	}


	static void RemoveWhitespaces(std::wstring& String) {
		String.erase(std::remove(String.begin(), String.end(), L' '), String.end());
	}


private:


	std::wstring GetHKLM(std::wstring SubKey, std::wstring Value) {
		DWORD Size{};
		std::wstring Ret{};

		RegGetValueW(HKEY_LOCAL_MACHINE,
					 SubKey.c_str(),
					 Value.c_str(),
					 RRF_RT_REG_SZ,
					 nullptr,
					 nullptr,
					 &Size);

		Ret.resize(Size);

		RegGetValueW(HKEY_LOCAL_MACHINE,
					 SubKey.c_str(),
					 Value.c_str(),
					 RRF_RT_REG_SZ,
					 nullptr,
					 &Ret[0],
					 &Size);

		return Ret.c_str();
	}


	template <typename T = const wchar_t*>
	void QueryWMI(std::wstring WMIClass, std::wstring Field, std::vector <T>& Value, const wchar_t* ServerName = L"ROOT\\CIMV2") {
		std::wstring Query(L"SELECT ");
		Query.append(Field.c_str()).append(L" FROM ").append(WMIClass.c_str());

		IWbemLocator* Locator{};
		IWbemServices* Services{};
		IEnumWbemClassObject* Enumerator{};
		IWbemClassObject* ClassObject{};
		VARIANT Variant{};
		DWORD Returned{};


		HRESULT hResult{ CoInitializeEx(nullptr, COINIT_MULTITHREADED) };


		if (FAILED(hResult)) {
			return;
		}


		hResult = CoInitializeSecurity(nullptr,
									   -1,
									   nullptr,
									   nullptr,
									   RPC_C_AUTHN_LEVEL_DEFAULT,
									   RPC_C_IMP_LEVEL_IMPERSONATE,
									   nullptr,
									   EOAC_NONE,
									   nullptr);



		if (FAILED(hResult)) {
			CoUninitialize();
			return;
		}


		hResult = CoCreateInstance(CLSID_WbemLocator,
								   NULL,
								   CLSCTX_INPROC_SERVER,
								   IID_IWbemLocator,
								   reinterpret_cast<PVOID*>(&Locator));


		if (FAILED(hResult)) {
			CoUninitialize();
			return;
		}


		hResult = Locator->ConnectServer(_bstr_t(ServerName),
										 nullptr,
										 nullptr,
										 nullptr,
										 NULL,
										 nullptr,
										 nullptr,
										 &Services);


		if (FAILED(hResult)) {
			Locator->Release();
			CoUninitialize();
			return;
		}


		hResult = CoSetProxyBlanket(Services,
									RPC_C_AUTHN_WINNT,
									RPC_C_AUTHZ_NONE,
									nullptr,
									RPC_C_AUTHN_LEVEL_CALL,
									RPC_C_IMP_LEVEL_IMPERSONATE,
									nullptr,
									EOAC_NONE);


		if (FAILED(hResult)) {
			Services->Release();
			Locator->Release();
			CoUninitialize();
			return;
		}


		hResult = Services->ExecQuery(bstr_t(L"WQL"),
									  bstr_t(Query.c_str()),
									  WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
									  nullptr,
									  &Enumerator);


		if (FAILED(hResult)) {
			Services->Release();
			Locator->Release();
			CoUninitialize();
			return;
		}

		while (Enumerator) {

			HRESULT Res = Enumerator->Next(WBEM_INFINITE,
										   1,
										   &ClassObject,
										   &Returned);

			if (!Returned) {
				break;
			}

			Res = ClassObject->Get(Field.c_str(),
								   0,
								   &Variant,
								   nullptr,
								   nullptr);

			if (typeid(T) == typeid(long) || typeid(T) == typeid(int)) {
				Value.push_back((T) Variant.intVal);
			}
			else if (typeid(T) == typeid(bool)) {
				Value.push_back((T) Variant.boolVal);
			}
			else if (typeid(T) == typeid(unsigned int)) {
				Value.push_back((T) Variant.uintVal);
			}
			else if (typeid(T) == typeid(unsigned short)) {
				Value.push_back((T) Variant.uiVal);
			}
			else if (typeid(T) == typeid(long long)) {
				Value.push_back((T) Variant.llVal);
			}
			else if (typeid(T) == typeid(unsigned long long)) {
				Value.push_back((T) Variant.ullVal);
			}
			else {
				Value.push_back((T) ((bstr_t) Variant.bstrVal).copy());
			}

			VariantClear(&Variant);
			ClassObject->Release();
		}

		if (!Value.size()) {
			Value.resize(1);
		}

		Services->Release();
		Locator->Release();
		Enumerator->Release();
		CoUninitialize();
	}


	void QueryDisk() {

		std::wstring DrivePath{ L"\\\\.\\PhysicalDrive" };
		std::wstring VolumePath{ L"\\\\.\\" };
		HANDLE hVolume{ nullptr };
		VOLUME_DISK_EXTENTS DiskExtents{ NULL };
		DWORD IoBytes{ NULL };
		ULARGE_INTEGER FreeBytesAvailable{};
		ULARGE_INTEGER TotalBytes{};
		int DriveCount{ 0 };


		std::vector <const wchar_t*> SerialNumber{};
		std::vector <const wchar_t*> Model{};
		std::vector <const wchar_t*> Interface{};
		std::vector <const wchar_t*> Name{};
		std::vector <const wchar_t*> DeviceId{};
		std::vector <const wchar_t*> SortedDeviceId{};
		std::vector <const wchar_t*> FriendlyName{};
		std::vector <unsigned int> MediaType{};
		std::vector <bool> IsBoot{};


		// We need a count of the actual physical disks active on the system
		// The simplest way is to attempt to open a handle to PhysicalDriveX (where X is 0, 1, 2, etc.)
		// If we can get a valid handle, we increment the drive count, else we break

		HANDLE Handle{ nullptr };

		for (;; DriveCount++) {
			if ((Handle = CreateFileW((DrivePath + std::to_wstring(DriveCount)).c_str(),
				NULL,
				NULL,
				nullptr,
				OPEN_EXISTING,
				NULL,
				nullptr)) == INVALID_HANDLE_VALUE) {
				break;
			}

			CloseHandle(Handle);
		}


		this->Disk.resize(DriveCount);
		SortedDeviceId.resize(DriveCount);


		// To get most of the data we want, we make several queries to the Windows Management Instrumentation (WMI) service
		// Queries to MSFT_PhysicalDisk and MSFT_Disk require a connection to the ROOT\\microsoft\\windows\\storage namespace

		QueryWMI(L"Win32_DiskDrive", L"SerialNumber", SerialNumber);
		QueryWMI(L"Win32_DiskDrive", L"Model", Model);
		QueryWMI(L"Win32_DiskDrive", L"InterfaceType", Interface);
		QueryWMI(L"Win32_DiskDrive", L"Name", Name);
		QueryWMI(L"Win32_LogicalDisk", L"DeviceId", DeviceId);
		QueryWMI(L"MSFT_PhysicalDisk", L"MediaType", MediaType, L"ROOT\\microsoft\\windows\\storage");
		QueryWMI(L"MSFT_PhysicalDisk", L"FriendlyName", FriendlyName, L"ROOT\\microsoft\\windows\\storage");
		QueryWMI(L"MSFT_Disk", L"IsBoot", IsBoot, L"ROOT\\microsoft\\windows\\storage");


		for (int i = 0; i < DriveCount; i++) {
			for (int j = 0; j < DriveCount; j++) {

				// Win32_LogicalDisk is relied on to get the drive letter (DeviceId)
				// However, the data it returns will not be in the same order as the results we get from Win32_DiskDrive
				// The drive letter is what we rely on to get the total size and free space of the drive, so we must map the data accordingly
				// Win32_DiskDrive will return data in the order we want. It'll always be PhysicalDrive0, then PhysicalDrive1, then 2, etc.

				// Armed with that knowledge, we can open handles directly to the drive letters we have, and use DeviceIoControl to call into the volume

				hVolume = CreateFileW((VolumePath + DeviceId.at(j)).c_str(),
									  NULL,
									  NULL,
									  nullptr,
									  OPEN_EXISTING,
									  NULL,
									  nullptr);


				// IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS will fill our buffer with a VOLUME_DISK_EXTENTS structure
				// First, we must get the number of disk extents

				DeviceIoControl(hVolume,
								IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
								nullptr,
								NULL,
								&DiskExtents,
								sizeof(DiskExtents),
								&IoBytes,
								nullptr);


				// VOLUME_DISK_EXTENTS contains an array of DISK_EXTENT structures. DISK_EXTENT contains a DWORD member, DiskNumber
				// DiskNumber will be the same number used to construct the name of the disk, which is PhysicalDriveX, where X is the DiskNumber

				DeviceIoControl(hVolume,
								IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
								nullptr,
								NULL,
								&DiskExtents,
								offsetof(VOLUME_DISK_EXTENTS, Extents[DiskExtents.NumberOfDiskExtents]),
								&IoBytes,
								nullptr);

				CloseHandle(hVolume);


				// To map the drive letter from Win32_LogicalDisk to the data returned by Win32_DiskDrive
				// We compare the drive letter's DiskNumber to the number at the end of the "Name" we recieve from Win32_DiskDrive
				// We then reorder the drive letters accordingly

				if (DiskExtents.Extents->DiskNumber == std::stoi(&SafeString(Name.at(i)).back())) {
					SortedDeviceId.at(i) = DeviceId.at(j);
					break;
				}
			}

		}


		this->Disk.resize(DriveCount);


		for (int i = 0; i < DriveCount; i++) {

			// GetDiskFreeSpaceEx() will give us the size and free space available corresponding to the drive letters we have

			GetDiskFreeSpaceEx(SafeString(SortedDeviceId.at(i)).c_str(),
							   &FreeBytesAvailable,
							   &TotalBytes,
							   nullptr);

			RemoveWhitespaces(this->Disk.at(i).SerialNumber = SafeString(SerialNumber.at(i)));
			this->Disk.at(i).Model = SafeString(Model.at(i));
			this->Disk.at(i).Interface = SafeString(Interface.at(i));
			this->Disk.at(i).DriveLetter = SafeString(SortedDeviceId.at(i));
			this->Disk.at(i).Size = TotalBytes.QuadPart / pow(1024, 3);
			this->Disk.at(i).FreeSpace = FreeBytesAvailable.QuadPart / pow(1024, 3);
			this->Disk.at(i).IsBootDrive = IsBoot.at(i);


			// Data from MSFT_PhysicalDisk will not be in the same order as Win32_DiskDrive
			// So we compare the "FriendlyName" from MSFT_PhysicalDisk with the "Model" from Win32_DiskDrive
			// We then reorder the data accordingly

			for (int j = 0; j < DriveCount; j++) {
				if (!this->Disk.at(i).Model.compare(FriendlyName.at(j))) {
					this->Disk.at(i).MediaType = MediaType.at(j);
				}
			}
		}
	}


	void QuerySMBIOS() {
		std::vector <const wchar_t*> Manufacturer{};
		std::vector <const wchar_t*> Product{};
		std::vector <const wchar_t*> Version{};
		std::vector <const wchar_t*> SerialNumber{};

		QueryWMI(L"Win32_BaseBoard", L"Manufacturer", Manufacturer);
		QueryWMI(L"Win32_BaseBoard", L"Product", Product);
		QueryWMI(L"Win32_BaseBoard", L"Version", Version);
		QueryWMI(L"Win32_BaseBoard", L"SerialNumber", SerialNumber);


		this->SMBIOS.Manufacturer = SafeString(Manufacturer.at(0));
		this->SMBIOS.Product = SafeString(Product.at(0));
		this->SMBIOS.Version = SafeString(Version.at(0));
		this->SMBIOS.SerialNumber = SafeString(SerialNumber.at(0));
	}


	void QueryProcessor() {
		std::vector <const wchar_t*> ProcessorId{};
		std::vector <const wchar_t*> Manufacturer{};
		std::vector <const wchar_t*> Name{};
		std::vector <int> Cores{};
		std::vector <int> Threads{};

		QueryWMI(L"Win32_Processor", L"ProcessorId", ProcessorId);
		QueryWMI(L"Win32_Processor", L"Manufacturer", Manufacturer);
		QueryWMI(L"Win32_Processor", L"Name", Name);
		QueryWMI<int>(L"Win32_Processor", L"NumberOfCores", Cores);
		QueryWMI<int>(L"Win32_Processor", L"NumberOfLogicalProcessors", Threads);

		this->CPU.ProcessorId = SafeString(ProcessorId.at(0));
		this->CPU.Manufacturer = SafeString(Manufacturer.at(0));
		this->CPU.Name = SafeString(Name.at(0));
		this->CPU.Cores = Cores.at(0);
		this->CPU.Threads = Threads.at(0);
	}


	void QueryGPU() {
		std::vector <const wchar_t*> Name{};
		std::vector <const wchar_t*> DriverVersion{};
		std::vector <unsigned long long> Memory{};
		std::vector <int> XRes{};
		std::vector <int> YRes{};
		std::vector <int> RefreshRate{};

		QueryWMI(L"Win32_VideoController", L"Name", Name);
		QueryWMI(L"Win32_VideoController", L"DriverVersion", DriverVersion);
		QueryWMI(L"Win32_VideoController", L"AdapterRam", Memory);
		QueryWMI(L"Win32_VideoController", L"CurrentHorizontalResolution", XRes);
		QueryWMI(L"Win32_VideoController", L"CurrentVerticalResolution", YRes);
		QueryWMI(L"Win32_VideoController", L"CurrentRefreshRate", RefreshRate);

		this->GPU.resize(Name.size());

		for (int i = 0; i < Name.size(); i++) {
			this->GPU.at(i).Name = SafeString(Name.at(i));
			this->GPU.at(i).DriverVersion = SafeString(DriverVersion.at(i));
			this->GPU.at(i).Memory = (Memory.at(i) * 2 / (1024 * 1024) / 1000);
			this->GPU.at(i).XResolution = XRes.at(i);
			this->GPU.at(i).YResolution = YRes.at(i);
			this->GPU.at(i).RefreshRate = RefreshRate.at(i);
		}
	}


	void QuerySystem() {
		std::vector <const wchar_t*> SystemName{};
		std::vector <const wchar_t*> OSVersion{};
		std::vector <const wchar_t*> OSName{};
		std::vector <const wchar_t*> OSArchitecture{};
		std::vector <const wchar_t*> OSSerialNumber{};
		std::vector <bool> IsHypervisorPresent{};

		QueryWMI(L"Win32_ComputerSystem", L"Name", SystemName);
		QueryWMI(L"Win32_ComputerSystem", L"Model", IsHypervisorPresent);
		QueryWMI(L"Win32_OperatingSystem", L"Version", OSVersion);
		QueryWMI(L"Win32_OperatingSystem", L"Name", OSName);
		QueryWMI(L"Win32_OperatingSystem", L"OSArchitecture", OSArchitecture);
		QueryWMI(L"Win32_OperatingSystem", L"SerialNumber", OSSerialNumber);

		std::wstring wOSName{ SafeString(OSName.at(0)) };

		if (wOSName.find('|') != std::wstring::npos) {
			wOSName.resize(wOSName.find('|'));
		}

		this->System.Name = SafeString(SystemName.at(0));
		this->System.IsHypervisorPresent = IsHypervisorPresent.at(0);
		this->System.OSName = wOSName;
		this->System.OSVersion = SafeString(OSVersion.at(0));
		this->System.OSSerialNumber = SafeString(OSSerialNumber.at(0));
		this->System.OSArchitecture = SafeString(OSArchitecture.at(0));
	}


	void QueryNetwork() {
		std::vector <const wchar_t*> Name{};
		std::vector <const wchar_t*> MAC{};

		QueryWMI(L"Win32_NetworkAdapter", L"Name", Name);
		QueryWMI(L"Win32_NetworkAdapter", L"MACAddress", MAC);

		this->NetworkAdapter.resize(Name.size());

		for (int i = 0; i < Name.size(); i++) {
			this->NetworkAdapter.at(i).Name = SafeString(Name.at(i));
			this->NetworkAdapter.at(i).MAC = SafeString(MAC.at(i));
		}
	}


	void QueryPhysicalMemory() {
		std::vector<const wchar_t*> PartNumber{};

		QueryWMI(L"Win32_PhysicalMemory", L"PartNumber", PartNumber);
		this->PhysicalMemory.PartNumber = SafeString(PartNumber.at(0));
	}


	void QueryRegistry() {
		this->Registry.ComputerHardwareId = SafeString(GetHKLM(L"SYSTEM\\CurrentControlSet\\Control\\SystemInformation", L"ComputerHardwareId").c_str());
	}


	void GetHardwareId() {
		QueryDisk();
		QuerySMBIOS();
		QueryProcessor();
		QueryGPU();
		QuerySystem();
		QueryNetwork();
		QueryPhysicalMemory();
		QueryRegistry();
	}


public:


	struct DiskObject {

		std::wstring SerialNumber{};
		std::wstring Model{};		 // e.g. ADATA SU740
		std::wstring Interface{};	 // e.g. SCSI
		std::wstring DriveLetter{};
		long long Size{};		 // In GB
		long long FreeSpace{};		 // In GB
		unsigned int MediaType{};	 // 4 = SSD, 3 = HDD, 0 = Unspecified, 5 = SCM
		bool IsBootDrive{};

	}; std::vector <DiskObject> Disk{};


	struct {

		std::wstring Manufacturer{};
		std::wstring Product{};
		std::wstring Version{};
		std::wstring SerialNumber{};

	} SMBIOS;


	struct {

		std::wstring ProcessorId{};
		std::wstring Manufacturer{};
		std::wstring Name{};
		int Cores{};
		int Threads{};

	} CPU;


	struct GPUObject {

		std::wstring Name{};
		std::wstring DriverVersion{};
		unsigned long long Memory{};
		int XResolution{};
		int YResolution{};
		int RefreshRate{};

	}; std::vector <GPUObject> GPU{};


	struct NetworkAdapterObject {

		std::wstring Name{};
		std::wstring MAC{};

	}; std::vector <NetworkAdapterObject> NetworkAdapter{};



	struct {

		std::wstring Name{};
		bool IsHypervisorPresent{};
		std::wstring OSVersion{};
		std::wstring OSName{};
		std::wstring OSArchitecture{};
		std::wstring OSSerialNumber{};

	} System;


	struct {

		std::wstring PartNumber{};

	} PhysicalMemory;


	struct {

		std::wstring ComputerHardwareId{};

	} Registry;


	std::unique_ptr<HardwareId> Pointer() {
		return std::make_unique<HardwareId>(*this);
	}


	HardwareId() {
		GetHardwareId();
	}
};


#pragma endregion HWID

#pragma region CMD



namespace CommandLine {


	HardwareId HWID{};
	std::wstring CurCmd{ L"" };
	std::wstring CmdName{ L"HWInfo>" };


	enum {
		eInvalid,
		eDisk,
		eSMBIOS,
		eGPU,
		eCPU,
		eNetwork,
		eSystem,
		ePhysicalMemory,
		eRegistry,
		eAll,
		eHelp,
		eExit
	};


	std::vector <std::wstring> ValidCommands{
		L"invalid command",
		L"disk",
		L"smbios",
		L"gpu",
		L"cpu",
		L"network",
		L"system",
		L"physicalmemory",
		L"registry",
		L"all",
		L"help",
		L"exit"
	};


	std::vector <std::wstring> DiskSubCommands{
		L"invalid command",
		L"serialnumber",
		L"model",
		L"interface",
		L"driveletter",
		L"size",
		L"freespace",
		L"mediatype",
		L"isbootdrive"
	};

	std::vector <std::wstring> SMBIOSSubCommands{
		L"invalid command",
		L"serialnumber",
		L"manufacturer",
		L"product",
		L"version"
	};

	std::vector <std::wstring> GPUSubCommands{
		L"invalid command",
		L"name",
		L"driverversion",
		L"resolution",
		L"refreshrate",
		L"memory"
	};

	std::vector <std::wstring> CPUSubCommands{
		L"invalid command",
		L"processorid",
		L"manufacturer",
		L"name",
		L"cores",
		L"threads"
	};

	std::vector <std::wstring> NetworkSubCommands{
		L"invalid command",
		L"name",
		L"mac"
	};

	std::vector <std::wstring> SystemSubCommands{
		L"invalid command",
		L"name",
		L"ishypervisorpresent",
		L"osversion",
		L"ostitle",
		L"osarchitecture",
		L"osserialnumber"
	};

	std::vector <std::wstring> PhysicalMemorySubCommands{
		L"invalid command",
		L"partnumber"
	};

	std::vector <std::wstring> RegistrySubCommands{
		L"invalid command",
		L"computerhardwareid"
	};


	struct {

		int CommandIndex{};
		std::vector <int> SubCommandIndex{};

	} ParsedCommand;


	template <typename T>
	void FmtPrint(T ToPrint, int HWType, int Iterator, const wchar_t* AdditionalText = nullptr) {
		std::wcout << ToPrint;

		if (AdditionalText != nullptr) {
			std::wcout << AdditionalText;
		}


		switch (HWType) {
			case eDisk: {
				std::wcout << (Iterator && Iterator + 1 == HWID.Disk.size() ? L"\n\n" : L"\n");
			} break;

			case eGPU: {
				std::wcout << (Iterator && Iterator + 1 == HWID.GPU.size() ? L"\n\n" : L"\n");
			} break;

			case eNetwork: {
				std::wcout << (Iterator && Iterator + 1 == HWID.NetworkAdapter.size() ? L"\n\n" : L"\n");
			} break;
		}
	}


	void PrintDisks() {
		for (int i = 0; i < HWID.Disk.size(); i++) {

			std::wcout << L"Model:\t\t\t" << HWID.Disk.at(i).Model << std::endl;
			std::wcout << L"Serial Number:\t\t" << HWID.Disk.at(i).SerialNumber << std::endl;
			std::wcout << L"Interface Type:\t\t" << HWID.Disk.at(i).Interface << std::endl;
			std::wcout << L"Drive Letter:\t\t" << HWID.Disk.at(i).DriveLetter << std::endl;
			std::wcout << L"Size:\t\t\t" << HWID.Disk.at(i).Size << L" GB" << std::endl;
			std::wcout << L"Free Space:\t\t" << HWID.Disk.at(i).FreeSpace << L" GB" << std::endl;

			std::wcout << L"Media Type:\t\t" <<
				(HWID.Disk.at(i).MediaType == 4 ? L"SSD" :
				 (HWID.Disk.at(i).MediaType == 3 ? L"HDD" : L"")) << std::endl;

			std::wcout << L"Boot Drive:\t\t" << (HWID.Disk.at(i).IsBootDrive ? L"Yes" : L"No") << std::endl;

			if (i + 1 < HWID.Disk.size()) { std::wcout << std::endl; }
		}
	}


	void PrintSMBIOS() {
		std::wcout << L"Manufacturer:\t\t" << HWID.SMBIOS.Manufacturer << std::endl;
		std::wcout << L"Product:\t\t" << HWID.SMBIOS.Product << std::endl;
		std::wcout << L"Version:\t\t" << HWID.SMBIOS.Version << std::endl;
		std::wcout << L"Serial Number:\t\t" << HWID.SMBIOS.SerialNumber << std::endl;
	}


	void PrintGPUs() {
		for (int i = 0; i < HWID.GPU.size(); i++) {
			std::wcout << L"Name:\t\t\t" << HWID.GPU.at(i).Name << std::endl;
			std::wcout << L"Driver Version:\t\t" << HWID.GPU.at(i).DriverVersion << std::endl;
			std::wcout << L"Memory:\t\t\t" << HWID.GPU.at(i).Memory << L" GB" << std::endl;
			std::wcout << L"Resolution:\t\t" << HWID.GPU.at(i).XResolution << L"x" << HWID.GPU.at(i).YResolution << std::endl;
			std::wcout << L"Refresh Rate:\t\t" << HWID.GPU.at(i).RefreshRate << std::endl;

			if (i + 1 < HWID.GPU.size()) { std::wcout << std::endl; }
		}
	}


	void PrintCPU() {
		std::wcout << L"Processor Id:\t\t" << HWID.CPU.ProcessorId << std::endl;
		std::wcout << L"Manufacturer:\t\t" << HWID.CPU.Manufacturer << std::endl;
		std::wcout << L"Name:\t\t\t" << HWID.CPU.Name << std::endl;
		std::wcout << L"Cores:\t\t\t" << HWID.CPU.Cores << std::endl;
		std::wcout << L"Threads:\t\t" << HWID.CPU.Threads << std::endl;
	}


	void PrintNetwork() {
		for (int i = 0; i < HWID.NetworkAdapter.size(); i++) {
			std::wcout << L"Name:\t\t\t" << HWID.NetworkAdapter.at(i).Name << std::endl;
			std::wcout << L"MAC Address:\t\t" << HWID.NetworkAdapter.at(i).MAC << std::endl;

			if (i + 1 < HWID.NetworkAdapter.size()) { std::wcout << std::endl; }
		}
	}


	void PrintSystem() {
		std::wcout << L"System Name:\t\t" << HWID.System.Name << std::endl;
		std::wcout << L"Hypervisor Present:\t" << (HWID.System.IsHypervisorPresent ? L"Yes" : L"No") << std::endl;
		std::wcout << L"OS Title:\t\t" << HWID.System.OSName << std::endl;
		std::wcout << L"OS Version:\t\t" << HWID.System.OSVersion << std::endl;
		std::wcout << L"OS Architecture:\t" << HWID.System.OSArchitecture << std::endl;
		std::wcout << L"OS Serial Number:\t" << HWID.System.OSSerialNumber << std::endl;
	}


	void PrintPhysicalMemory() {
		std::wcout << L"Part Number:\t\t" << HWID.PhysicalMemory.PartNumber << std::endl;
	}


	void PrintRegistry() {
		std::wcout << L"Computer Hardware Id:\t" << HWID.Registry.ComputerHardwareId << std::endl;
	}


	std::vector <std::wstring> ValidSubCommands() {
		switch (ParsedCommand.CommandIndex) {
			case eDisk: {
				return DiskSubCommands;
			} break;

			case eSMBIOS: {
				return SMBIOSSubCommands;
			} break;

			case eGPU: {
				return GPUSubCommands;
			} break;

			case eCPU: {
				return CPUSubCommands;
			} break;

			case eNetwork: {
				return NetworkSubCommands;
			} break;

			case eSystem: {
				return SystemSubCommands;
			} break;

			case ePhysicalMemory: {
				return PhysicalMemorySubCommands;
			} break;

			case eRegistry: {
				return RegistrySubCommands;
			} break;
		}

		return {};
	}


	void ParseCommand() {
		bool bFound{ false };

		auto OrderVector{ [](std::vector <int> ToOrder) -> std::vector <int> {
			struct MinToMax {
				MinToMax(const std::vector <int>& Vector) : _Vector(Vector) {}

				bool operator () (int Index, int Index2) {
					return _Vector[Index] < _Vector[Index2];
				}

				const std::vector <int>& _Vector{};
			};

			std::vector <int> OrderedElements{};
			OrderedElements.resize(ToOrder.size());

			for (int i = 0; i < ToOrder.size(); ++i) {
				OrderedElements.at(i) = i;
			}

			std::sort(OrderedElements.begin(), OrderedElements.end(), MinToMax(ToOrder));
			return OrderedElements;
		} };


		// Convert the command to lowercase to avoid any case issues

		for (int i = 0; i < CurCmd.size(); i++) {
			CurCmd.at(i) = std::tolower(CurCmd.at(i));
		}


		// example of a valid command: "disk get serialnumber, model"
		// "disk" is the main command
		// "get" is the keyword required to request fields/members
		// "serialnumber" and "model" are sub-commands containing the requested fields


		// Determine if the start of the command contains a valid main command by looping through our vector of valid commands

		for (; ParsedCommand.CommandIndex < ValidCommands.size(); ParsedCommand.CommandIndex++) {
			if (CurCmd.find(ValidCommands.at(ParsedCommand.CommandIndex)) == 0) {
				bFound = true;
				break;
			}
		}


		if (!bFound) {
			// The command is invalid
			ParsedCommand.CommandIndex = eInvalid;
			return;
		}


		// Determine if the command contains a "get" request by checking for the "get" keyword
		// Additionally, make sure the command isn't just "[main command] get" by checking 
		// if it's larger than the size of the main command + the size of "get"

		if (CurCmd.find(L"get") != std::wstring::npos && CurCmd.size() != ValidCommands.at(ParsedCommand.CommandIndex).size() + 3) {

			// Determine if "get" is directly after the main command. If it's not, it's an invalid command 
			// e.g. "disk get serialnumber" is valid, "get disk serialnumber" is not

			if (CurCmd.find(L"get") != ValidCommands.at(ParsedCommand.CommandIndex).size()) {
				ParsedCommand.CommandIndex = eInvalid;
				return;
			}


			// Check if we have valid sub-commands 
			// If we do, we construct our vector of sub-command indices; however, they won't be in the same order as the command request
			// If the full command is "disk get model, serialnumber", the requested fields will be returned in the order of our sub-command vector
			// In this example, the disk serial number(s) will be returned before the model(s) because serial number is before model in our vector
			// even though they specified the model before the serial number.
			// So we must keep track of the starting offset of the sub-commands by storing each one in an additional vector
			// The smallest offsets are closest to the "get" keyword in the command, 
			// so we'll use the indices of them to sort our sub-command index vector

			std::vector <std::wstring> SubCmds{ ValidSubCommands() };
			std::vector <int> UnorderedSubIndex{};
			std::vector <int> Offsets{};

			for (int i = 0; i < SubCmds.size(); i++) {
				auto Offset{ CurCmd.find(SubCmds.at(i)) };

				if (Offset != std::wstring::npos) {
					UnorderedSubIndex.push_back(i);
					Offsets.push_back(Offset);
				}
			}


			if (UnorderedSubIndex.size() == 0 || Offsets.size() == 0) {

				// No valid sub-commands, but "get" is present - invalid command

				ParsedCommand.CommandIndex = eInvalid;
				return;
			}


			// We now call our lambda which will return a vector of the original offset indices 
			// ordered in correspondence with the offset values sorted from min to max

			auto OrderedOffsets{ OrderVector(Offsets) };


			// Check if the closest sub-command to the "get" keyword is directly after it
			// If it's not, we have invalid junk between "get" and the sub-commands (e.g. disk get afgxsdf serialnumber)

			if (Offsets.at(OrderedOffsets.at(0)) != ValidCommands.at(ParsedCommand.CommandIndex).size() + 3) {
				ParsedCommand.CommandIndex = eInvalid;
				return;
			}


			// Sort our sub-command indices from closest to farthest relative to "get"

			ParsedCommand.SubCommandIndex.resize(UnorderedSubIndex.size());

			for (int i = 0; i < UnorderedSubIndex.size(); i++) {
				ParsedCommand.SubCommandIndex.at(i) = UnorderedSubIndex.at(OrderedOffsets.at(i));
			}
		}
		else if (CurCmd.size() > ValidCommands.at(ParsedCommand.CommandIndex).size()) {

			// There's a valid main command present, but there are no sub-commands and the total size of the command is larger than 
			// the size of the valid main command it contains
			// We have an invalid command (e.g. diskgkdfmkm)

			ParsedCommand.CommandIndex = eInvalid;
		}
	}


	void RespondCommand() {
		ParseCommand();

		// handle sub-commands if present
		if (ParsedCommand.SubCommandIndex.size()) {

			std::wcout << std::endl;

			switch (ParsedCommand.CommandIndex) {

				// Disk Sub-Command (e.g. "disk get serialnumber")
				case eDisk: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {


						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).SerialNumber, eDisk, j);
								}
							} break;

							case 2: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).Model, eDisk, j);
								}
							} break;

							case 3: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).Interface, eDisk, j);
								}
							} break;

							case 4: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).DriveLetter, eDisk, j);
								}
							} break;

							case 5: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).Size, eDisk, j);
								}
							} break;

							case 6: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).FreeSpace, eDisk, j);
								}
							} break;

							case 7: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).MediaType, eDisk, j);
								}
							} break;

							case 8: {
								for (int j = 0; j < HWID.Disk.size(); j++) {
									FmtPrint(HWID.Disk.at(j).IsBootDrive, eDisk, j);
								}
							} break;
						}
					}
				} break;


					// SMBIOS Sub-Command (e.g. "smbios get manufacturer")
				case eSMBIOS: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {
						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								std::wcout << HWID.SMBIOS.SerialNumber << std::endl;
							} break;

							case 2: {
								std::wcout << HWID.SMBIOS.Manufacturer << std::endl;
							} break;

							case 3: {
								std::wcout << HWID.SMBIOS.Product << std::endl;
							} break;

							case 4: {
								std::wcout << HWID.SMBIOS.Version << std::endl;
							} break;
						}
					}
				} break;


					// GPU Sub-Command (e.g. "gpu get resolution")
				case eGPU: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {
						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								for (int i = 0; i < HWID.GPU.size(); i++) {
									FmtPrint(HWID.GPU.at(i).Name, eGPU, i);
								}
							} break;

							case 2: {
								for (int i = 0; i < HWID.GPU.size(); i++) {
									FmtPrint(HWID.GPU.at(i).DriverVersion, eGPU, i);
								}
							} break;

							case 3: {
								for (int i = 0; i < HWID.GPU.size(); i++) {
									std::wstring Res(std::to_wstring(HWID.GPU.at(i).XResolution));
									Res.append(L"x").append(std::to_wstring(HWID.GPU.at(i).YResolution));

									FmtPrint(Res, eGPU, i);
								}
							} break;

							case 4: {
								for (int i = 0; i < HWID.GPU.size(); i++) {
									FmtPrint(HWID.GPU.at(i).RefreshRate, eGPU, i);
								}
							} break;

							case 5: {
								for (int i = 0; i < HWID.GPU.size(); i++) {
									FmtPrint(HWID.GPU.at(i).Memory, eGPU, i, L" GB");
								}
							} break;
						}
					}
				} break;


					// CPU Sub-Command (e.g. "cpu get processorid")
				case eCPU: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {
						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								std::wcout << HWID.CPU.ProcessorId << std::endl;
							} break;

							case 2: {
								std::wcout << HWID.CPU.Manufacturer << std::endl;
							} break;

							case 3: {
								std::wcout << HWID.CPU.Name << std::endl;
							} break;

							case 4: {
								std::wcout << HWID.CPU.Cores << std::endl;
							} break;

							case 5: {
								std::wcout << HWID.CPU.Threads << std::endl;
							} break;
						}
					}

				} break;


					// Network Sub-Command (e.g. "network get mac")
				case eNetwork: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {
						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								for (int i = 0; i < HWID.NetworkAdapter.size(); i++) {
									FmtPrint(HWID.NetworkAdapter.at(i).Name, eNetwork, i);
								}
							} break;

							case 2: {
								for (int i = 0; i < HWID.NetworkAdapter.size(); i++) {
									FmtPrint(HWID.NetworkAdapter.at(i).MAC, eNetwork, i);
								}
							} break;
						}
					}
				} break;


					// System Sub-Command (e.g. "system get osversion")
				case eSystem: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {
						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								std::wcout << HWID.System.Name << std::endl;
							} break;

							case 2: {
								std::wcout << HWID.System.IsHypervisorPresent << std::endl;
							} break;

							case 3: {
								std::wcout << HWID.System.OSVersion << std::endl;
							} break;

							case 4: {
								std::wcout << HWID.System.OSName << std::endl;
							} break;

							case 5: {
								std::wcout << HWID.System.OSArchitecture << std::endl;
							} break;

							case 6: {
								std::wcout << HWID.System.OSSerialNumber << std::endl;
							} break;
						}
					}

				} break;


					// Physcial Memory Sub-Command (e.g. "physicalmemory get partnumber")
				case ePhysicalMemory: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {
						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								std::wcout << HWID.PhysicalMemory.PartNumber << std::endl;
							} break;
						}
					}

				} break;


					// Registry Sub-Command (e.g. "registry get computerhardwareid")
				case eRegistry: {
					for (int i = 0; i < ParsedCommand.SubCommandIndex.size(); i++) {
						switch (ParsedCommand.SubCommandIndex.at(i)) {
							case 1: {
								std::wcout << HWID.Registry.ComputerHardwareId << std::endl;
							} break;
						}
					}

				} break;
			}

			goto ClearCmd;
		}


		// No sub-commands, handle main command
		switch (ParsedCommand.CommandIndex) {

			case eDisk: {
				std::wcout << std::endl;
				PrintDisks();
			} break;

			case eSMBIOS: {
				std::wcout << std::endl;
				PrintSMBIOS();
			} break;

			case eGPU: {
				std::wcout << std::endl;
				PrintGPUs();
			} break;

			case eCPU: {
				std::wcout << std::endl;
				PrintCPU();
			} break;

			case eNetwork: {
				std::wcout << std::endl;
				PrintNetwork();
			} break;

			case eSystem: {
				std::wcout << std::endl;
				PrintSystem();
			} break;

			case ePhysicalMemory: {
				std::wcout << std::endl;
				PrintPhysicalMemory();
			} break;

			case eRegistry: {
				std::wcout << std::endl;
				PrintRegistry();
			} break;

			case eAll: {
				std::wcout << L"\n-------------\n";
				std::wcout << L"[+] Disks [+] \n";
				std::wcout << L"-------------\n\n";
				PrintDisks();

				std::wcout << L"\n--------------\n";
				std::wcout << L"[+] SMBIOS [+]\n";
				std::wcout << L"--------------\n\n";
				PrintSMBIOS();

				std::wcout << L"\n------------\n";
				std::wcout << L"[+] GPUs [+]\n";
				std::wcout << L"------------\n\n";
				PrintGPUs();


				std::wcout << L"\n-----------\n";
				std::wcout << L"[+] CPU [+]\n";
				std::wcout << L"-----------\n\n";
				PrintCPU();


				std::wcout << L"\n---------------\n";
				std::wcout << L"[+] Network [+] \n";
				std::wcout << L"---------------\n\n";
				PrintNetwork();


				std::wcout << L"\n--------------\n";
				std::wcout << L"[+] System [+]\n";
				std::wcout << L"--------------\n\n";
				PrintSystem();


				std::wcout << L"\n-----------------------\n";
				std::wcout << L"[+] Physical Memory [+]\n";
				std::wcout << L"-----------------------\n\n";
				PrintPhysicalMemory();


				std::wcout << L"\n----------------\n";
				std::wcout << L"[+] Registry [+]\n";
				std::wcout << L"----------------\n\n";
				PrintRegistry();

			} break;

			case eHelp: {
				ShellExecuteW(nullptr, nullptr, L"http://www.github.com/paradoxwastaken", nullptr, nullptr, SW_SHOW);
			} break;

			case eExit: {
				exit(0);
			} break;

			default: {
				std::wcout << L"Invalid Command\n";
			} break;
		}


ClearCmd:

		ParsedCommand.CommandIndex = eInvalid;
		ParsedCommand.SubCommandIndex.resize(0);
	}


	void Create(HardwareId& _out) {
		auto NewLine{ []() -> void {
			std::wcout << CmdName;
			std::getline(std::wcin, CurCmd);

			HardwareId::RemoveWhitespaces(CurCmd);
			if (CurCmd.empty()) { return; }

			bool bFmt{
				(CurCmd.find(L"diskget") != std::wstring::npos && HWID.Disk.size() > 1
				|| CurCmd.find(L"gpuget") != std::wstring::npos && HWID.GPU.size() > 1
				|| CurCmd.find(L"networkget") != std::wstring::npos && HWID.NetworkAdapter.size() > 1)
			};

			RespondCommand();
			if (!bFmt) { std::wcout << std::endl; }
		} };

		HWID = HardwareId();
		_out = HWID;
		return;
		for (;;) {
			NewLine();
		}
	}
};

#pragma endregion CMD



#include <codecvt>


namespace archt {
	namespace system_info {


		HardwareId id;
		MEMORYSTATUSEX memStat;

		

		//std::string ws2s(const std::wstring& wstr) {
		//	return std::string(wstr.begin(), wstr.end());
		//}

		std::string convert(const std::wstring& wstr) {
			std::string ret;
			std::string buff(MB_CUR_MAX, '\0');

			for (wchar_t const& wc : wstr) {
				int mbCharLen = std::wctomb(&buff[0], wc);

				if (mbCharLen < 1) { break; }

				for (int i = 0; i < mbCharLen; ++i) {
					ret += buff[i];
				}
			}

			return ret;
		}

		void renderFunc(bool* open, GuiWindow_s* handle) {

			ImGui::Begin("Cpu");
			ImGui::Text("Name :\t\t%s", convert(id.CPU.Name).c_str());
			ImGui::Text("Manufacturer :\t%s", convert(id.CPU.Manufacturer).c_str());
			ImGui::Text("Cores  :\t\t%i", id.CPU.Cores);
			ImGui::Text("ProcessorId :\t%s", convert(id.CPU.ProcessorId).c_str());
			ImGui::Text("Threads\t\t%i", id.CPU.Threads);
			ImGui::End();

			ImGui::Begin("OS");
			ImGui::Text("Name :\t\t%s", convert(id.System.Name).c_str());
			ImGui::Text("Os name :\t%s", convert(id.System.OSName).c_str());
			ImGui::Text("Architecture  :\t\t%s", convert(id.System.OSArchitecture).c_str());
			ImGui::Text("Version :\t%s", convert(id.System.OSVersion).c_str());
			ImGui::Text("Serial number :\t%s", convert(id.System.OSSerialNumber).c_str());
			ImGui::End();



			ImGui::Begin("Bios");
			ImGui::Text("Manufacturer :\t\t%s", convert(id.SMBIOS.Manufacturer).c_str());
			ImGui::Text("Product :\t%s", convert(id.SMBIOS.Product).c_str());
			ImGui::Text("Version  :\t\t%s", convert(id.SMBIOS.Version).c_str());
			ImGui::Text("Serialnumber :\t%s", convert(id.SMBIOS.SerialNumber).c_str());
			ImGui::End();


			std::string dataMetric = "bytes";


			ImGui::Begin("Memory");
			ImGui::RadioButton("Bytes", true);
			ImGui::RadioButton("Kilo bytes", false);

			ImGui::Text("Percent used :\t\t%i %", memStat.dwMemoryLoad);
			ImGui::Text("Total :\t\t%i bytes", memStat.ullTotalPhys / DIV);
			ImGui::Text("Free :\t\t%i", memStat.ullAvailPhys / DIV);
			ImGui::Text("Total paging :\t\t%i", memStat.ullTotalPageFile / DIV);
			ImGui::Text("Free paging :\t\t%i", memStat.ullAvailPageFile / DIV);
			ImGui::Text("Total virtual :\t\t%*I64x", WIDTH, memStat.ullTotalVirtual / DIV, divisor);
			ImGui::Text("Free virtual :\t\t%*I64x", WIDTH, memStat.ullAvailVirtual / DIV, divisor);
			ImGui::Text("Free extended virtual :\t\t%i", memStat.ullAvailExtendedVirtual / DIV);
			ImGui::End();


			
		}

		void createSysteminfoWindow() {
			getMemoryMetrics(memStat);
			CommandLine::Create(id);

			Gui_s::getInstance()->addGuiWindow_void(renderFunc);
	
		}

	}
}