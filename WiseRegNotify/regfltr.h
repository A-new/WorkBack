#pragma once

//#include <ntifs.h>
//#include <ntstrsafe.h>
#include "common.h"

//#define REGFLTR_MEMORY_TAG          'wISe'

#define STATUS_INVALID_CONFIG_FILE          0xE000001L
#define STATUS_INVALID_POSITION             0xE000002L
#define STATUS_MEMORY_TOO_SMALL             0xE000003L

//
// Registry callback version
//
extern ULONG g_MajorVersion;
extern ULONG g_MinorVersion;

typedef struct _FILR_NOTIFY_DATA {   //ע����������
	KEVENT hEvent;
	REGISTER_NOTIFY_DATA_OUT Data;
}FILR_NOTIFY_DATA, *PFILR_NOTIFY_DATA;

typedef struct _NOTIFY_DATA_STACK{   //ע��������ջ
	struct _NOTIFY_DATA_STACK* pNext;
	PFILR_NOTIFY_DATA pNotifyData;
}NOTIFY_DATA_STACK, *PNOTIFY_DATA_STACK;

typedef struct _REG_FILTER_KEY {  
	ULONG KeySize;
	ULONG ValueSize;
	PWSTR RegKey;                         
	PWSTR ValueName;                      
}REG_FILTER_KEY, *PREG_FILTER_KEY;

typedef struct _CALLBACK_CONTEXT {  //�ص�����Context
	PKEVENT UserEvent;              //֪ͨ�û������Event
	//HANDLE hEvent;
	HANDLE hProcess;                //���ûص������Ľ���ID 
	CALLBACK_NOTIFY Denied;         //
	SPIN_LOCK StackLock;            //֪ͨ���ݶ�ջ������
	PNOTIFY_DATA_STACK NotifyStack; //��⵽ע���������ݶ�ջ���ȴ��û������ȡ��
} CALLBACK_CONTEXT, *PCALLBACK_CONTEXT;

typedef struct _EXCLUDE_FILE{   //�ļ��������б�
	CALLBACK_NOTIFY Denied;
	ULONG nNameSize;
	WCHAR Filename[0];
}EXCLUDE_FILE, *PEXCLUDE_FILE;

typedef struct _EXCLU_FILES_STACK{   //�ļ��������б�
	struct _EXCLU_FILES_STACK* pNext;
	PEXCLUDE_FILE pExcludeFile;
}EXCLU_FILES_STACK, *PEXCLU_FILES_STACK;

typedef struct _EXCLU_PID_STACK{   //PID�����б�
	struct _EXCLU_PID_STACK* pNext;
	HANDLE pid;
}EXCLU_PID_STACK, *PEXCLU_PID_STACK;

typedef struct _POSITION_KEY{
	USHORT KeySize;
	ULONG Key;
	USHORT ValueSize;
	ULONG Value;
}POSITION_KEY, *PPOSITION_KEY;

typedef struct _REGFILTER_POSITION {  
	USHORT id;                 
	BOOLEAN SubKeys;               
	ULONG NotifyClass;             
	UCHAR keyCount;
	USHORT NameSize;
	ULONG Name;
	POSITION_KEY Keys[1];
} REGFILTER_POSITION, *PREGFILTER_POSITION;

typedef struct _EXCLUDE_POSITION_STATCK {
	struct _EXCLUDE_POSITION_STATCK* pNext;
	CALLBACK_NOTIFY Denied;
	ULONG positionId;
}EXCLUDE_POSITION_STATCK, *PEXCLUDE_POSITION_STATCK;

EX_CALLBACK_FUNCTION Callback;   //

NTSTATUS RegisterCallback( //ע��ص��������û�������ã�
	ULONG InputBufferLength, PREGISTER_CALLBACK_IN pInput);
NTSTATUS UnRegisterCallback();
NTSTATUS GetRegNotifyData(  //��ȡע���������ݣ��û�������ã�
	ULONG OutputBufferLength, PREGISTER_NOTIFY_DATA_OUT pOut, PULONG_PTR pRetlen);
NTSTATUS SetRegNotifyResult( //����ע����������ֵ�������ܾ������û�������ã�
	ULONG InputBufferLength, PREGISTER_CALLBACK_RESULT_IN Input);
NTSTATUS AddExcludeFile(  //��������б�
	ULONG Length, PEXCLUDE_ADD_IN pInput);
NTSTATUS RemoveExcludeFile( //�Ƴ��������
	ULONG Length, PEXCLUDE_ADD_IN pInput);
NTSTATUS AddExcludePosition( //�û�����ı�ص�������Ϊ��֪ͨ��ֱ�Ӿܾ���ֱ������
	ULONG Length, PEXCLUDE_POSITION_IN pInput);

NTSTATUS UnRegisterAll();  //��ע��ȫ���ص�����������ж��ʱ���ã�
VOID UnRegisterProcess(HANDLE ProcessId);  //��ע�����ע��Ļص��������û�����CloseHandleʱ���ã�

NTSTATUS GetCallbackVersion(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp);
NTSTATUS GetPositions(ULONG OutputBufferLength, PGET_POSITION_OUT pOut, PULONG_PTR pRetlen);
NTSTATUS SetPositions(ULONG InputBufferLength, PVOID pData);
NTSTATUS GetExcludeFiles(ULONG OutputBufferLength, PGET_EXCLUDE_FILE_OUT pOut, PULONG_PTR pRetlen);
//------------------------------------------------------------------------------------
NTSTATUS InitGlobalData();
NTSTATUS FreeGlobalData();
VOID LoadRegsiterExcludePositions();

NTSTATUS InitCallbackContext(HANDLE UserEvent);
// NTSTATUS PushCallbackContext( //Contextѹջ
// 	PCALLBACK_CONTEXT* pCallbackCtx);
VOID FreeCallbackContext();
VOID FreeNotifyData(PFILR_NOTIFY_DATA pNotifyData);

PCALLBACK_CONTEXT* FindContextByCookie(ULONG64 Cookie);
VOID RemoveContextStack(PCALLBACK_CONTEXT* pCallbackCtx);
VOID RemoveAllContextStack();
PCALLBACK_CONTEXT* FindCallbackContextByPID(HANDLE ProcessId);
ULONG FindPositionIDByData(
	REG_NOTIFY_CLASS NotifyClass, PVOID pRegObject, PUNICODE_STRING pCompleteName, PUNICODE_STRING pValueName, PUNICODE_STRING* pNewKeyname, PUNICODE_STRING *pNewValuename);

PNOTIFY_DATA_STACK PushCallbackNotifyData(PFILR_NOTIFY_DATA pNotifyData);
PFILR_NOTIFY_DATA InitRegNotifyData(
	ULONG positionId, REG_NOTIFY_CLASS NotifyClass, PUNICODE_STRING pKeyName,  PUNICODE_STRING pSubKeyname, PUNICODE_STRING pValueName, PUNICODE_STRING pNewName, PUNICODE_STRING pProcessName);
VOID RemoveWaitNotifyDataStck(PNOTIFY_DATA_STACK pStack);
PFILR_NOTIFY_DATA PopCallbackNotifyData();
PFILR_NOTIFY_DATA SetWaitNotifyData(ULONG64 DataCookie, CALLBACK_NOTIFY Denied);

CALLBACK_NOTIFY IsExcludeFile(PWSTR pFilename, ULONG nNameSize);
PEXCLU_FILES_STACK InExcludeProcess(HANDLE pid);

NTSTATUS UnRegisterCallbackByContext(PCALLBACK_CONTEXT* pContext);
NTSTATUS PushExcludeFile(PWSTR Filename, ULONG Filesize, CALLBACK_NOTIFY Notify);
NTSTATUS PopExcludeFile(PWSTR Filename, ULONG Namesize);
PUNICODE_STRING ExractKeyName(PUNICODE_STRING pFullKeyname);
CALLBACK_NOTIFY IsExcludePosition(ULONG positionId);
NTSTATUS PushExcludePosition(ULONG positionId, CALLBACK_NOTIFY Denied);
NTSTATUS PushExcludePositionByName(PWSTR Posname, ULONG Namesize, CALLBACK_NOTIFY Notify);

extern PCALLBACK_CONTEXT g_Context;