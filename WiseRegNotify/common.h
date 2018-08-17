
#pragma once

#ifdef _ANTI_
#define DRIVER_NAME             L"WiseAntiReg"
#define NT_DEVICE_NAME          L"\\Device\\WiseAntiReg"
#define DOS_DEVICES_LINK_NAME   L"\\DosDevices\\WiseAntiReg"
#define WIN32_DEVICE_NAME       L"\\\\.\\WiseAntiReg"
//#define NOTIFY_EVENT_NAME       L"\\WiseAntiRegEvent"
#else
#define DRIVER_NAME             L"WiseRegNotify"
#define NT_DEVICE_NAME          L"\\Device\\WiseRegNotify"
#define DOS_DEVICES_LINK_NAME   L"\\DosDevices\\WiseRegNotify"
#define WIN32_DEVICE_NAME       L"\\\\.\\WiseRegNotify"
#define NOTIFY_EVENT_NAME       L"\\WiseRegNotifyEvent"
#endif

#define IOCTL_REGISTER_CALLBACK        CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 1), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_UNREGISTER_CALLBACK      CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 2), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_CALLBACK_VERSION     CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 3), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_CONTEXT_DATA         CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 4), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_NOTIFY_RESULT        CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 5), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ADD_EXCLUDE              CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 6), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_EXCLUDE           CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 7), METHOD_BUFFERED, FILE_ANY_ACCESS)
//#define IOCTL_GET_EXCLUDE              CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 8), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CHANGE_CALLBACK          CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 9), METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_GETFILTER_POSITIONS      CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 10), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GETEXCLUDE_FILES         CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 11), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SETFILTER_POSITIONS      CTL_CODE (FILE_DEVICE_UNKNOWN, (0x800 + 12), METHOD_BUFFERED, FILE_ANY_ACCESS)

// typedef enum _NOTIFY_CLASS { //ע�������������� ��WinXPû��ǰ��֮�֣�
// 	NotifyClassNone       = 0x0000,  //������
// 	PreDeleteKey          = 0x0001,   //ɾ��Key��ǰ��
// 	PostDeleteKey         = 0x0002,	  //ɾ��Key����
// 	PreRenameKey          = 0x0003,	  //������Key��ǰ��
// 	PostRenameKey         = 0x0004,	  //������Key����
// 	PreCreateKey          = 0x0005,	  //�����Key��ǰ��
// 	PostCreateKey         = 0x0006,	  //�����Key����
// 	PreSetValueKey        = 0x0007,	  //��ӻ��޸�Key����ֵ��ǰ��
// 	PostSetValueKey       = 0x0008,	  //��ӻ��޸�Key����ֵ����
// 	PreDeleteValueKey     = 0x0009,	  //ɾ��Key����ֵ��ǰ��
// 	PostDeleteValueKey    = 0x000A,	   //ɾ��Key����ֵ����
// 	PreSetInformationKey  = 0x000B,
// 	PostSetInformationKey = 0x000C,
// } NOTIFY_CLASS;

#pragma pack(push, 1)

typedef enum _CALLBACK_NOTIFY{ //0: ֪ͨ�û�����1��ֱ�Ӿܾ���2��ֱ������
	RN_NOTIFY = 0, 
	RN_ALLOW  = 1,
	RN_DENY   = 2
}CALLBACK_NOTIFY;

typedef struct _REGISTER_CALLBACK_IN {  //ע��ص������������
	ULONG positionId;                           //�����ܳ���
	ULONG UserEvent;                       //Event Handle���в�������⵽ͨ����֪ͨ�û�����
	//REGFILTER_KEY Keys[0];
} REGISTER_CALLBACK_IN, *PREGISTER_CALLBACK_IN;

typedef struct _REGISTER_CALLBACK_RESULT_IN {  //����ע����������������
	ULONG64 DataCookie;                    //��������Cookie��ʵ��Ϊ����ָ��
	CALLBACK_NOTIFY Denied;                        //�Ƿ�ܾ�
} REGISTER_CALLBACK_RESULT_IN, *PREGISTER_CALLBACK_RESULT_IN;

typedef struct _REGISTER_NOTIFY_DATA_OUT {  //��ȡע���������ݵ����������
	ULONG nSize;                            //�����ܳ���     
	ULONG64 Cookie;      
	union{
		ULONG positionId;                     //������ʹ�ã��Ƿ�ܾ���־
		CALLBACK_NOTIFY Denied;
	};
	ULONG NotifyClass;                      //ע�������
	ULONG FileName;                         //�ļ���
	ULONG KeyName;                          //������Key�ַ�����ŵ�ƫ��λ��
	ULONG SubKeyName;                       //�Ӽ����ַ�����ŵ�ƫ��λ��
	ULONG ValueName;                        //�������ַ�����ŵ�ƫ��λ��
	ULONG NewName;
} REGISTER_NOTIFY_DATA_OUT, *PREGISTER_NOTIFY_DATA_OUT;

typedef struct _EXCLUDE_ADD_IN {            //��������б�	
	ULONG Namesize;                            
	ULONG pid;                              //����ID
	ULONG Denied;                           //������/�ܾ���
	WCHAR Filename[0];                      //�����ļ���
} EXCLUDE_ADD_IN, *PEXCLUDE_ADD_IN;

typedef struct _EXCLUDE_POSITION_IN {       //����λ��
	USHORT positionId;
	CALLBACK_NOTIFY Denied;                          //�Ƿ�ܾ�
} EXCLUDE_POSITION_IN, *PEXCLUDE_POSITION_IN;

typedef struct _GET_POSITION_OUT {         //��ȡ���м��λ��
	USHORT id;
	ULONG name;                          
	CALLBACK_NOTIFY Denied;
} GET_POSITION_OUT, *PGET_POSITION_OUT;

typedef struct _GET_EXCLUDE_FILE_OUT {     //��ȡ�����������  
	CALLBACK_NOTIFY Denied;
	ULONG name;                          
} GET_EXCLUDE_FILE_OUT, *PGET_EXCLUDE_FILE_OUT;

#pragma pack(pop)