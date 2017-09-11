#ifndef _ALL_INFO_H_
#define _ALL_INFO_H_

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ( ( FILE_DEVICE_SCSI << 16 ) + 0x0501 )

#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

#define  IDENTIFY_BUFFER_SIZE  512
#define  SENDIDLENGTH  ( sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE )

#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088


#include <afx.h>
#include <windows.h> 

#include <Setupapi.h>
#include <Regstr.h>
#include <Rpc.h>

#include <Iphlpapi.h>
#include <cfgmgr32.h>
#include <vector>
#pragma comment ( lib, "Iphlpapi.lib" )
#pragma comment( lib, "Setupapi.lib" )
#pragma comment( lib, "Rpcrt4.lib" )
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <atlbase.h>
#include <atlconv.h>

#pragma warning(disable:4996)
char* GetAllMacInfo()
{
	PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;	
	
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc( 2*sizeof(IP_ADAPTER_INFO) );
    ULONG ulOutBufLen = 2*sizeof(IP_ADAPTER_INFO);
	
    if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) 
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);

	static char szMacInfo[256];
	memset(szMacInfo, 0, sizeof(szMacInfo));
    if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
    {
        pAdapter = pAdapterInfo;
		while (pAdapter)
        {
			strcat(szMacInfo, pAdapter->AdapterName);
			for (UINT i = 0; i < pAdapter->AddressLength; i++)
			{
				char szTmp[20] = {0};
				sprintf_s(szTmp, "%02X%c",pAdapter->Address[i],
					i == pAdapter->AddressLength - 1 ? '\n' : '-');
				strcat(szMacInfo, szTmp);
			}
            pAdapter = pAdapter->Next;       
		}
    }
	return szMacInfo;
}

char* GetSingleMacInfo(void)
{
	PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;	
	
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc( 2*sizeof(IP_ADAPTER_INFO) );
    ULONG ulOutBufLen = 2*sizeof(IP_ADAPTER_INFO);
	
    if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) 
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
	
	static char szMacInfo[256];
	memset(szMacInfo, 0, sizeof(szMacInfo));
    if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
    {
        pAdapter = pAdapterInfo;
		while (pAdapter)
        {
			for (UINT i = 0; i < pAdapter->AddressLength; i++)
			{
				char szTmp[20] = {0};
				sprintf_s(szTmp, "%02X%c",pAdapter->Address[i],
					i == pAdapter->AddressLength - 1 ? '\n' : '-');
				strcat(szMacInfo, szTmp);
			}
            break;
		}
    }
	return szMacInfo;
}

char* GetOsVersion()
{
	static char osinfo[256];
	char m_stInfo[256];
	char m_stMinorVer[256];
	char m_stServPack[256];
	char m_stBuildNumber[256];
	memset(osinfo, 0, sizeof(osinfo));
	memset(m_stInfo, 0, sizeof(m_stInfo));
	memset(m_stMinorVer, 0, sizeof(m_stMinorVer));
	memset(m_stServPack, 0, sizeof(m_stServPack));
	memset(m_stBuildNumber, 0, sizeof(m_stBuildNumber));

	int stat = 0;
	char buffer[20];
	TCHAR data [64];
	memset(buffer, 0, sizeof(buffer));
	memset(data, 0, sizeof(data));
	DWORD dataSize;
	DWORD win95Info;
	OSVERSIONINFO versionInfo;
	HKEY hKey;
	LONG result;

	// set the size of OSVERSIONINFO, before calling the function

	versionInfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);

	// Get the version information

	if (!::GetVersionEx (&versionInfo)) 
	{
		return false;
	}

	// If NT ,then check if its server or workstation.

	if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {

		strcpy(m_stInfo, "Windows NT");

		dataSize = sizeof (data);
		
		result = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
			"System\\CurrentControlSet\\Control\\ProductOptions",
			0, KEY_QUERY_VALUE, &hKey);

		// If there is error in opening the registry key, return

		if (result != ERROR_SUCCESS) {
			SetLastError (result);
			return false;
		}

		result = ::RegQueryValueEx (hKey, _T("ProductType"), NULL, NULL,
			(LPBYTE) data, &dataSize);

		// Make sure to close the reg key

		RegCloseKey (hKey);

		if (result != ERROR_SUCCESS) {
			SetLastError (result);
			return false;
		}

		// Check what string has been returned

		if (lstrcmpi (data, "WinNT") == 0) {
			strcpy(m_stInfo, "Windows NT Workstation");
		}
		else if (lstrcmpi (data, "ServerNT") == 0) {
			strcpy(m_stInfo, "Windows NT Server");
		}
		else {
			strcpy(m_stInfo, "Windows NT Server - Domain Controller");
		}

		// Check the version number

		if (versionInfo.dwMajorVersion == 3 || versionInfo.dwMinorVersion == 51) {
			strcpy(m_stMinorVer, "3.51");
		}
		else {
			strcpy(m_stMinorVer, "4.0");
		}

		// Get the build number.

		_itoa_s( versionInfo.dwBuildNumber, buffer, 10 );
		strcpy(m_stBuildNumber, buffer);
	}
	else if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		strcpy(m_stInfo, "Windows 95");
		if ((versionInfo.dwMajorVersion > 4) || ((versionInfo.dwMajorVersion == 4)
			&& (versionInfo.dwMinorVersion > 0))) {
			strcpy(m_stInfo, "Windows 98");
		}

		// For win95, build number is low order word
		
		win95Info = (DWORD)(LOBYTE(LOWORD(versionInfo.dwBuildNumber)));

		// Get the build number.

		_itoa_s( win95Info, buffer, 10 );
		strcpy(m_stBuildNumber, buffer);
	}
	else {
		strcpy(m_stInfo, "Windows 3.1");
	}

	// Get service pack information.

	strcpy(m_stServPack, versionInfo.szCSDVersion);

	sprintf_s(osinfo, "OS%s Ver:%s Build:%s %s",
		m_stInfo,
		m_stMinorVer,
		m_stBuildNumber,
		m_stServPack);
	return osinfo;
}

char* GetMemInfo()
{
	MEMORYSTATUS memStatus;
	static char memsta[256];
	memset(memsta, 0, sizeof(memsta));
	memStatus.dwLength=sizeof(MEMORYSTATUS);
	
	GlobalMemoryStatus(&memStatus);
	
	DWORD tom=memStatus.dwTotalPhys;
	DWORD mem=memStatus.dwAvailPhys;
	DWORD res=memStatus.dwAvailVirtual;
	
	tom=(tom/1024)/1024;
	mem=(mem/1024)/1024;
	res=(res/1024)/1024;
	sprintf_s(memsta, "Total Mem:%ld MB, CanUse:%ld MB, Virtual:%ld MB",
		tom,
		mem,
		res);
	return memsta;
}

int GetCpuID(char *CpuSerialNum) 
{ 
	char szCPUID[256]; 
	char szTmp[128]; 
	memset(szCPUID, 0, sizeof(szCPUID));
	memset(szTmp, 0, sizeof(szTmp));
	_asm 
	{ 
		mov eax,2 
			CPUID 
	} 
	unsigned long s1 = 0,s2=0; 
	_asm 
	{	
		mov eax,01h
			xor edx,edx
			cpuid 
			mov s1,edx
			mov s2,eax 
	}
	sprintf_s(szTmp, "%08X%08X", s1, s2); 
	strcpy(szCPUID, szTmp);
	_asm 
	{ 
		mov eax,03h 
			xor ecx,ecx 
			xor edx,edx
			cpuid 
			mov s1,edx 
			mov s2,ecx 
	} 
	memset(szTmp, 0, sizeof(szTmp));
	sprintf_s(szTmp, "%08X%08X", s1, s2); 
	strcat(szCPUID, szTmp); 
	//生产厂家
	int mycpuid[20] = {0}; 
	__asm{
		mov eax,0
			cpuid

			mov mycpuid[0],ebx
			mov mycpuid[4],edx
			mov mycpuid[8],ecx
	}
	memset(szTmp, 0, sizeof(szTmp));
	sprintf_s(szTmp,"manufacturer:%s",mycpuid);
	strcat(szCPUID, szTmp);
	strcpy(CpuSerialNum, szCPUID);
	return 0; 
} 

typedef struct _IDSECTOR
{
    USHORT  wGenConfig;
    USHORT  wNumCyls;
    USHORT  wReserved;
    USHORT  wNumHeads;
    USHORT  wBytesPerTrack;
    USHORT  wBytesPerSector;
    USHORT  wSectorsPerTrack;
    USHORT  wVendorUnique[3];
    CHAR    sSerialNumber[20];
    USHORT  wBufferType;
    USHORT  wBufferSize;
    USHORT  wECCSize;
    CHAR    sFirmwareRev[8];
    CHAR    sModelNumber[40];
    USHORT  wMoreVendorUnique;
    USHORT  wDoubleWordIO;
    USHORT  wCapabilities;
    USHORT  wReserved1;
    USHORT  wPIOTiming;
    USHORT  wDMATiming;
    USHORT  wBS;
    USHORT  wNumCurrentCyls;
    USHORT  wNumCurrentHeads;
    USHORT  wNumCurrentSectorsPerTrack;
    ULONG   ulCurrentSectorCapacity;
    USHORT  wMultSectorStuff;
    ULONG   ulTotalAddressableSectors;
    USHORT  wSingleWordDMA;
    USHORT  wMultiWordDMA;
    BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

typedef struct _DRIVERSTATUS

{
    BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
    BYTE  bIDEStatus;    //  Contents of IDE Error register.
    //  Only valid when bDriverError is SMART_IDE_ERROR.
    BYTE  bReserved[2];  //  Reserved for future expansion.
    DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

typedef struct _SENDCMDOUTPARAMS
{
    DWORD         cBufferSize;   //  Size of bBuffer in bytes
    DRIVERSTATUS  DriverStatus;  //  Driver status structure.
    BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

typedef struct _SRB_IO_CONTROL
{
    ULONG HeaderLength;
    UCHAR Signature[8];
    ULONG Timeout;
    ULONG ControlCode;
    ULONG ReturnCode;
    ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

typedef struct _IDEREGS
{
    BYTE bFeaturesReg;       // Used for specifying SMART "commands".
    BYTE bSectorCountReg;    // IDE sector count register
    BYTE bSectorNumberReg;   // IDE sector number register
    BYTE bCylLowReg;         // IDE low order cylinder value
    BYTE bCylHighReg;        // IDE high order cylinder value
    BYTE bDriveHeadReg;      // IDE drive/head register
    BYTE bCommandReg;        // Actual IDE command.
    BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;

typedef struct _SENDCMDINPARAMS
{
    DWORD     cBufferSize;   //  Buffer size in bytes
    IDEREGS   irDriveRegs;   //  Structure with drive register values.
    BYTE bDriveNumber;       //  Physical drive number to send 
    //  command to (0,1,2,3).
    BYTE bReserved[3];       //  Reserved for future expansion.
    DWORD     dwReserved[4]; //  For future use.
    BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

typedef struct _GETVERSIONOUTPARAMS
{
    BYTE bVersion;      // Binary driver version.
    BYTE bRevision;     // Binary driver revision.
    BYTE bReserved;     // Not used.
    BYTE bIDEDeviceMap; // Bit map of IDE devices.
    DWORD fCapabilities; // Bit mask of driver capabilities.
    DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//////////////////////////////////////////////////////////////////////

//结构定义 
typedef struct _UNICODE_STRING 
{ 
    USHORT  Length;//长度 
    USHORT  MaximumLength;//最大长度 
    PWSTR  Buffer;//缓存指针 
} UNICODE_STRING,*PUNICODE_STRING; 

typedef struct _OBJECT_ATTRIBUTES 
{ 
    ULONG Length;//长度 18h 
    HANDLE RootDirectory;//  00000000 
    PUNICODE_STRING ObjectName;//指向对象名的指针 
    ULONG Attributes;//对象属性00000040h 
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR，0 
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE，0 
} OBJECT_ATTRIBUTES; 
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES; 

//函数指针变量类型
typedef DWORD  (__stdcall *ZWOS )( PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES); 
typedef DWORD  (__stdcall *ZWMV )( HANDLE,HANDLE,PVOID,ULONG,ULONG,PLARGE_INTEGER,PSIZE_T,DWORD,ULONG,ULONG); 
typedef DWORD  (__stdcall *ZWUMV )( HANDLE,PVOID); 
BOOL WinNTHDSerialNumAsScsiRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen )
{
    BOOL bInfoLoaded = FALSE;
    
    for( int iController = 0; iController < 2; ++ iController )
    {
        HANDLE hScsiDriveIOCTL = 0;
        char   szDriveName[256];
        
        //  Try to get a handle to PhysicalDrive IOCTL, report failure
        //  and exit if can't.
        sprintf_s( szDriveName, "\\\\.\\Scsi%d:", iController );

        //  Windows NT, Windows 2000, any rights should do
        hScsiDriveIOCTL = CreateFile( szDriveName,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            OPEN_EXISTING, 0, NULL);

        // if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
        //    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
        //            controller, GetLastError ());
        
        if( hScsiDriveIOCTL != INVALID_HANDLE_VALUE )
        {
            int iDrive = 0;
            for( iDrive = 0; iDrive < 2; ++ iDrive )
            {
                char szBuffer[sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH] = { 0 };

                SRB_IO_CONTROL* p = ( SRB_IO_CONTROL* )szBuffer;
                SENDCMDINPARAMS* pin = ( SENDCMDINPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
                DWORD dwResult;

                p->HeaderLength = sizeof( SRB_IO_CONTROL );
                p->Timeout = 10000;
                p->Length = SENDIDLENGTH;
                p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
                strncpy_s( ( char* )p->Signature, 8 ,"SCSIDISK", 8 );

                pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
                pin->bDriveNumber = iDrive;
                
                if( DeviceIoControl( hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
                    szBuffer,
                    sizeof( SRB_IO_CONTROL ) + sizeof( SENDCMDINPARAMS ) - 1,
                    szBuffer,
                    sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH,
                    &dwResult, NULL ) )
                {
                    SENDCMDOUTPARAMS* pOut = ( SENDCMDOUTPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
                    IDSECTOR* pId = ( IDSECTOR* )( pOut->bBuffer );
                    if( pId->sModelNumber[0] )
                    {
                        if( * puSerialLen + 20U <= uMaxSerialLen )
                        {
                            // 序列号
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 10, 20 );

                            // Cut off the trailing blanks
							UINT i = 20;
                            for( ; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
                            {}
                            * puSerialLen += i;

                            // 型号
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 27, 40 );
                            // Cut off the trailing blanks
                            for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
                            {}
                            * puSerialLen += i;

                            bInfoLoaded = TRUE;
                        }
                        else
                        {
                            ::CloseHandle( hScsiDriveIOCTL );
                            return bInfoLoaded;
                        }
                    }
                }
            }
            ::CloseHandle( hScsiDriveIOCTL );
        }
    }
    return bInfoLoaded;
}

BOOL DoIdentify( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned )
{
    // Set up data structures for IDENTIFY command.
    pSCIP->cBufferSize                  = IDENTIFY_BUFFER_SIZE;
    pSCIP->irDriveRegs.bFeaturesReg     = 0;
    pSCIP->irDriveRegs.bSectorCountReg  = 1;
    pSCIP->irDriveRegs.bSectorNumberReg = 1;
    pSCIP->irDriveRegs.bCylLowReg       = 0;
    pSCIP->irDriveRegs.bCylHighReg      = 0;
    
    // calc the drive number.
    pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ( ( bDriveNum & 1 ) << 4 );

    // The command can either be IDE identify or ATAPI identify.
    pSCIP->irDriveRegs.bCommandReg = bIDCmd;
    pSCIP->bDriveNumber = bDriveNum;
    pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
    
    return DeviceIoControl( hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
        ( LPVOID ) pSCIP,
        sizeof( SENDCMDINPARAMS ) - 1,
        ( LPVOID ) pSCOP,
        sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1,
        lpcbBytesReturned, NULL );
}

BOOL WinNTHDSerialNumAsPhysicalRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen )
{
	#define  DFP_GET_VERSION 0x00074080
    BOOL bInfoLoaded = FALSE;

    for( UINT uDrive = 0; uDrive < 4; ++ uDrive )
    {
        HANDLE hPhysicalDriveIOCTL = 0;

        //  Try to get a handle to PhysicalDrive IOCTL, report failure
        //  and exit if can't.
        char szDriveName [256];
        sprintf_s( szDriveName, "\\\\.\\PhysicalDrive%d", uDrive );

        //  Windows NT, Windows 2000, must have admin rights
        hPhysicalDriveIOCTL = CreateFile( szDriveName,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            OPEN_EXISTING, 0, NULL);

        if( hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE )
        {
            GETVERSIONOUTPARAMS VersionParams = { 0 };
            DWORD               cbBytesReturned = 0;

            // Get the version, etc of PhysicalDrive IOCTL
            if( DeviceIoControl( hPhysicalDriveIOCTL, DFP_GET_VERSION,
                NULL, 
                0,
                &VersionParams,
                sizeof( GETVERSIONOUTPARAMS ),
                &cbBytesReturned, NULL ) )
            {
                // If there is a IDE device at number "i" issue commands
                // to the device
                if( VersionParams.bIDEDeviceMap != 0 )
                {
                    BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
                    SENDCMDINPARAMS  scip = { 0 };

                    // Now, get the ID sector for all IDE devices in the system.
                    // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
                    // otherwise use the IDE_ATA_IDENTIFY command
                    bIDCmd = ( VersionParams.bIDEDeviceMap >> uDrive & 0x10 ) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
                    BYTE IdOutCmd[sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1] = { 0 };

                    if( DoIdentify( hPhysicalDriveIOCTL, 
                        &scip, 
                        ( PSENDCMDOUTPARAMS )&IdOutCmd, 
                        ( BYTE )bIDCmd,
                        ( BYTE )uDrive,
                        &cbBytesReturned ) )
                    {
                        if( * puSerialLen + 20U <= uMaxSerialLen )
                        {
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 10, 20 );  // 序列号

                            // Cut off the trailing blanks
							UINT i = 20;
                            for( ; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {}
                            * puSerialLen += i;

                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 27, 40 ); // 型号

                            // Cut off the trailing blanks
                            for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {}
                            * puSerialLen += i;

                            bInfoLoaded = TRUE;
                        }
                        else
                        {
                            ::CloseHandle( hPhysicalDriveIOCTL );
                            return bInfoLoaded;
                        }
                    }
                }
            }
            CloseHandle( hPhysicalDriveIOCTL );
        }
    }
    return bInfoLoaded;
}

UINT FindAwardBios( BYTE** ppBiosAddr )
{
    BYTE* pBiosAddr = * ppBiosAddr + 0xEC71;

    BYTE szBiosData[128];
    CopyMemory( szBiosData, pBiosAddr, 127 );
    szBiosData[127] = 0;
    
    int iLen = lstrlen( ( char* )szBiosData );
    if( iLen > 0 && iLen < 128 )
    {
        //AWard:         07/08/2002-i845G-ITE8712-JF69VD0CC-00 
        //Phoenix-Award: 03/12/2002-sis645-p4s333
        if( szBiosData[2] == '/' && szBiosData[5] == '/' )
        {
            BYTE* p = szBiosData;
            while( * p )
            {
                if( * p < ' ' || * p >= 127 )
                {
                    break;
                }
                ++ p;
            }
            if( * p == 0 )
            {
                * ppBiosAddr = pBiosAddr;
                return ( UINT )iLen;
            }
        }
    }
    return 0;
}

UINT FindAmiBios( BYTE** ppBiosAddr )
{
    BYTE* pBiosAddr = * ppBiosAddr + 0xF478;
    
    BYTE szBiosData[128];
    CopyMemory( szBiosData, pBiosAddr, 127 );
    szBiosData[127] = 0;
    
    int iLen = lstrlen( ( char* )szBiosData );
    if( iLen > 0 && iLen < 128 )
    {
        // Example: "AMI: 51-2300-000000-00101111-030199-"
        if( szBiosData[2] == '-' && szBiosData[7] == '-' )
        {
            BYTE* p = szBiosData;
            while( * p )
            {
                if( * p < ' ' || * p >= 127 )
                {
                    break;
                }
                ++ p;
            }
            if( * p == 0 )
            {
                * ppBiosAddr = pBiosAddr;
                return ( UINT )iLen;
            }
        }
    }
    return 0;
}

UINT FindPhoenixBios( BYTE** ppBiosAddr )
{
    UINT uOffset[3] = { 0x6577, 0x7196, 0x7550 };
    for( UINT i = 0; i < 3; ++ i )
    {
        BYTE* pBiosAddr = * ppBiosAddr + uOffset[i];

        BYTE szBiosData[128];
        CopyMemory( szBiosData, pBiosAddr, 127 );
        szBiosData[127] = 0;

        int iLen = lstrlen( ( char* )szBiosData );
        if( iLen > 0 && iLen < 128 )
        {
            // Example: Phoenix "NITELT0.86B.0044.P11.9910111055"
            if( szBiosData[7] == '.' && szBiosData[11] == '.' )
            {
                BYTE* p = szBiosData;
                while( * p )
                {
                    if( * p < ' ' || * p >= 127 )
                    {
                        break;
                    }
                    ++ p;
                }
                if( * p == 0 )
                {
                    * ppBiosAddr = pBiosAddr;
                    return ( UINT )iLen;
                }
            }
        }
    }
    return 0;
}

char* GetPhysicalSerialNum()
{
    BYTE szSystemInfo[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
    UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度
	OSVERSIONINFO ovi = { 0 };
	ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	//memset(&ovi, 0, sizeof(OSVERSIONINFO));
	memset(szSystemInfo, 0, sizeof(szSystemInfo));
	GetVersionEx( &ovi );
	
	if( ovi.dwPlatformId != VER_PLATFORM_WIN32_NT )
	{
		// Only Windows 2000, Windows XP, Windows Server 2003...
		return FALSE;
	}
	else
	{
		if( !WinNTHDSerialNumAsPhysicalRead( szSystemInfo, &uSystemInfoLen, 1024 ) )
		{
			WinNTHDSerialNumAsScsiRead( szSystemInfo, &uSystemInfoLen, 1024 );
		}
    }

	static char TmpMem[256];
	memset(&TmpMem, 0, sizeof(TmpMem));
	memcpy(TmpMem, szSystemInfo, sizeof(TmpMem));
	return TmpMem;
}

char* GetBoisId()
{	
	BYTE szSystemInfo[4096]; 
    UINT uSystemInfoLen = 0; 
	OSVERSIONINFO ovi = { 0 };
	ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	memset(szSystemInfo, 0, sizeof(szSystemInfo));
	SIZE_T ssize; 	
	LARGE_INTEGER so; 
	so.LowPart=0x000f0000;
	so.HighPart=0x00000000; 
	ssize=0xffff; 
	wchar_t strPH[30]=L"\\device\\physicalmemory"; 
	DWORD ba=0;
	
	UNICODE_STRING struniph; 
	struniph.Buffer=strPH; 
	struniph.Length=0x2c; 
	struniph.MaximumLength =0x2e; 
	
	OBJECT_ATTRIBUTES obj_ar; 
	obj_ar.Attributes =64;
	obj_ar.Length =24;
	obj_ar.ObjectName=&struniph;
	obj_ar.RootDirectory=0; 
	obj_ar.SecurityDescriptor=0; 
	obj_ar.SecurityQualityOfService =0; 
	
	HMODULE hinstLib = LoadLibrary("ntdll.dll"); 
	ZWOS ZWopenS=(ZWOS)GetProcAddress(hinstLib,"ZwOpenSection"); 
	ZWMV ZWmapV=(ZWMV)GetProcAddress(hinstLib,"ZwMapViewOfSection"); 
	ZWUMV ZWunmapV=(ZWUMV)GetProcAddress(hinstLib,"ZwUnmapViewOfSection"); 
	
	//调用函数，对物理内存进行映射 
	HANDLE hSection; 
	static char TmpMem[256];
	memset(&TmpMem, 0, sizeof(TmpMem));
	if( 0 == ZWopenS(&hSection,4,&obj_ar) && 
		0 == ZWmapV( 
		( HANDLE )hSection,   //打开Section时得到的句柄 
		( HANDLE )0xFFFFFFFF, //将要映射进程的句柄， 
		&ba,                  //映射的基址 
		0,
		0xFFFF,               //分配的大小 
		&so,                  //物理内存的地址 
		&ssize,               //指向读取内存块大小的指针 
		1,                    //子进程的可继承性设定 
		0,                    //分配类型 
		2                     //保护类型 
		) )
        //执行后会在当前进程的空间开辟一段64k的空间，并把f000:0000到f000:ffff处的内容映射到这里 
        //映射的基址由ba返回,如果映射不再有用,应该用ZwUnmapViewOfSection断开映射 
	{
		BYTE* pBiosSerial = ( BYTE* )ba;
		UINT uBiosSerialLen = FindAwardBios( &pBiosSerial );
		if( uBiosSerialLen == 0U )
		{
			uBiosSerialLen = FindAmiBios( &pBiosSerial );
			if( uBiosSerialLen == 0U )
			{
				uBiosSerialLen = FindPhoenixBios( &pBiosSerial );
			}
		}
		if( uBiosSerialLen != 0U )
		{
			CopyMemory( szSystemInfo + uSystemInfoLen, pBiosSerial, uBiosSerialLen );
			uSystemInfoLen += uBiosSerialLen;
			strcat(TmpMem,"BiosID:");
			strcat(TmpMem, (char*)pBiosSerial);
		}
		else
		{
			strcat(TmpMem,"BiosSerial:");
			strcat(TmpMem, (char*)pBiosSerial);
		}
		ZWunmapV( ( HANDLE )0xFFFFFFFF, ( void* )ba );
	}
	return TmpMem;
}

#include<map>
typedef std::map<int , std::string> SoftInfoMap;
void GetAllSoftInfo(SoftInfoMap& InfoMap)   
{   
	static SoftInfoMap tmpSoft;
	tmpSoft.clear();
	HKEY   m_key,m_key2;   
	char   m_SubKey[255]="SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";   
	DWORD   m_index=0, count = 0;   
	char   m_name[200],m_displayname[200],m_uninstall[200],uninstall[200];   
	DWORD   m_namelen=200,m_displaynamelen=200,   m_uninstalllen=200;   
	DWORD   m_attr=REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ; 

	if   (RegOpenKeyEx(HKEY_LOCAL_MACHINE,m_SubKey,0,KEY_ALL_ACCESS,&m_key)==ERROR_SUCCESS)   
	{   
		while   (RegEnumKeyEx(m_key,m_index,m_name,&m_namelen,0,NULL,NULL,0)!=ERROR_NO_MORE_ITEMS)   
		{   
			m_index++;   
			if   (strcmp(m_name,"")!=NULL)   
			{ 
				strcpy_s(uninstall,m_SubKey);   
				strcat(uninstall,"\\");   
				strcat(uninstall,m_name);
				m_displaynamelen=200;     
				memset(m_displayname,0,200);   
				m_uninstalllen=200;
				memset(m_uninstall,0,200);    
				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,uninstall,0,KEY_ALL_ACCESS,&m_key2)==ERROR_SUCCESS)   
				{   
					RegQueryValueEx(m_key2,"DisplayName",0,&m_attr,LPBYTE(m_displayname),&m_displaynamelen);   
					RegQueryValueEx(m_key2,"UninstallString",0,&m_attr,LPBYTE(m_uninstall),&m_uninstalllen);   

					if ((strcmp(m_displayname,"")!=NULL) || strcmp(m_uninstall,"") != NULL) 
					{
						char tmpCh[512];
						memset(tmpCh, 0, sizeof(tmpCh));
						strcmp(tmpCh, m_displayname);
						strcat(tmpCh, m_name);
						strcat(tmpCh, m_uninstall);
						tmpSoft[count] = tmpCh;
						count++;
					}

					m_displaynamelen=200;
					memset(m_displayname,0,200);   
					m_uninstalllen=200;
					memset(m_uninstall,0,200);   
				}   

			}   
			m_namelen=200;   
			memset(m_name,0,200);   
		}//end   while   
		RegCloseKey(m_key);   
	}   //end   if   
	InfoMap = tmpSoft;
}
#if 0
int CLICINFO::GetMacInfo(char *szMacInfo)
 {
	int nRet = -1;
    ULONG ulOutBufLen = 0;
    PIP_ADAPTER_INFO pAdapterInfo = NULL;
    if ( ERROR_SUCCESS != GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) )
    {
        pAdapterInfo = ( PIP_ADAPTER_INFO )new BYTE[ ulOutBufLen ];
        if ( NULL == pAdapterInfo )
        {
            return nRet;
        }
    }
	
    DWORD dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen );
    if ( ERROR_SUCCESS == dwRetVal )
    {
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            if (MIB_IF_TYPE_ETHERNET == pAdapter->Type && 
				strstr(pAdapter->Description,"Virtual") == 0)
            {
				sprintf(szMacInfo, "%s-", pAdapter->AdapterName);  
				for (UINT i = 0; i < pAdapter->AddressLength; i++)
				{
					char szTmp[20] = {0};
					sprintf(szTmp, "%02X%c",pAdapter->Address[i],
						i == pAdapter->AddressLength - 1 ? '\n' : '-');
					strcat(szMacInfo, szTmp);
				}
				nRet = 0;
            }
            pAdapter = pAdapter->Next;
        }
    }	
    delete[] pAdapterInfo;
	return nRet;
// 	int nRet = -1;
// 	
// 	PIP_ADAPTER_INFO pAdapterInfo;
//     PIP_ADAPTER_INFO pAdapter = NULL;
//     DWORD dwRetVal = 0;	
// 	
//     pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
// 	//IP_ADAPTER_INFO tmp = {0};
// 	//pAdapterInfo = &tmp;
// 
//     ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
// 	
//     if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) 
//     {
//         //GlobalFree (pAdapterInfo);
//         pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
//     }
// 	
//     if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
//     {
//         pAdapter = pAdapterInfo;
//         while (pAdapter) 
//         {
// 			if(strstr(pAdapter->Description,"Virtual") ==  0)	//去除虚拟网卡
//             {
// 				sprintf(szMacInfo, "%s-", pAdapter->AdapterName);  
// 				for (UINT i = 0; i < pAdapter->AddressLength; i++)
//                 {
// 					char szTmp[20] = {0};
// 					sprintf(szTmp, "%02X%c",pAdapter->Address[i],
// 						i == pAdapter->AddressLength - 1 ? '\n' : '-');
// 					strcat(szMacInfo, szTmp);
//                 }							
// 				nRet = 0;
// 				break;
//             }
//             pAdapter = pAdapter->Next;
//         }
//     }
// 
// 	return nRet;

// 	SYSTEMTIME time1, time2;
// 	memset(&time1, 0, sizeof(SYSTEMTIME));
// 	memset(&time2, 0, sizeof(SYSTEMTIME));
// 	GetSystemTime(&time1);
// 	SetSystemTime(&time2);
// 	CoInitialize(NULL);   
// 	GUID guid; 
// 	CoCreateGuid(&guid);   
// 	WCHAR wszGuid[39];   
// 	StringFromGUID2(guid, wszGuid, 39);   
// 	WideCharToMultiByte(CP_ACP, 0, wszGuid, -1, szMacInfo, 128, NULL, NULL); 
// 
// 	char tmp[256];	
// 	memset(tmp, 0, sizeof(tmp));
// 	sprintf(tmp,"%d", guid.Data1);
// 	OutputDebugString(tmp);
// 	
// 	memset(tmp, 0, sizeof(tmp));
// 	sprintf(tmp,"%d", guid.Data2);
// 	OutputDebugString(tmp);
// 
// 	
// 	memset(tmp, 0, sizeof(tmp));
// 	sprintf(tmp,"%d", guid.Data3);
// 	OutputDebugString(tmp);
// 	
// 	sprintf(tmp,"%d", guid.Data4);
// 	OutputDebugString(tmp);
// 	OutputDebugString("\n");
//     CoUninitialize();
// //	SetSystemTime(&time1);
// 	return 0;
}
#endif
#endif
