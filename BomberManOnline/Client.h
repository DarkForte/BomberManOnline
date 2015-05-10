#pragma once

#include "Message.h"

// ����deprecation����
#pragma warning(disable: 4996)

// ����������(8*1024�ֽ�)
#define MAX_BUFFER_LEN 8196    
#define DEFAULT_PORT          12345                      // Ĭ�϶˿�
#define DEFAULT_IP            _T("127.0.0.1")            // Ĭ��IP��ַ
#define DEFAULT_THREADS       1                          // Ĭ�ϲ����߳���

class CClient;

// ���ڷ������ݵ��̲߳���
typedef struct _tagThreadParams_WORKER
{
	CClient* pClient;                               // ��ָ�룬���ڵ������еĺ���
	SOCKET   sock;                                  // ÿ���߳�ʹ�õ�Socket
	int      nThreadNo;                             // �̱߳��
	char     szBuffer[MAX_BUFFER_LEN];

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER;  

// ����Socket���ӵ��߳�
typedef struct _tagThreadParams_CONNECTION
{
	CClient* pClient;                               // ��ָ�룬���ڵ������еĺ���

} THREADPARAMS_CONNECTION,*PTHREADPARAM_CONNECTION; 

enum ClientState
{
	DISCONNECT, CONNECT
};

class CClient
{
public:
	CClient(void);
	~CClient(void);

public:

	// ����Socket��
	bool LoadSocketLib();
	// ж��Socket��
	void UnloadSocketLib() { WSACleanup(); }

	// ��ʼ
	bool Start();
	//	ֹͣ
	void Stop();

	// ��ñ�����IP��ַ
	CString GetLocalIP();

	// ��������IP��ַ
	void SetIP( const CString& strIP ) { m_strServerIP=strIP; }
	// ���ü����˿�
	void SetPort( const int& nPort )   { m_nPort=nPort; }
	// ���ò����߳�����
	void SetThreads(const int& n)      { m_nThreads=n; }

	CMessage _SendMessage(CMessage msg);

	ClientState state;

	CString GetIp(){ return m_strServerIP; }
	int GetPort(){ return m_nPort; }

private:

	// ��������
	bool EstablishConnections();
	// ���������������
	bool ConnetToServer( SOCKET *pSocket, CString strServer, int nPort );
	// ���ڽ������ӵ��߳�
	static DWORD WINAPI _ConnectionThread(LPVOID lpParam);

	// �ͷ���Դ
	void CleanUp();

private:

	CString   m_strServerIP;                                // �������˵�IP��ַ
	CString   m_strLocalIP;                                 // ����IP��ַ
	int       m_nPort;                                      // �����˿�
	int       m_nThreads;                                   // �����߳�����

	HANDLE    *m_phWorkerThreads;
	HANDLE    m_hConnectionThread;                          // �������ӵ��߳̾��
	HANDLE    m_hShutdownEvent;                             // ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�

	THREADPARAMS_WORKER      *m_pParamsWorker;              // �̲߳���
};
