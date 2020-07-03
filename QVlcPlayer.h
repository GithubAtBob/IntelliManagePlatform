#ifndef QVLCPLAYER_H
#define QVLCPLAYER_H
#include <QWidget>
#include <string>

using namespace std;

struct libvlc_instance_t;
struct libvlc_media_player_t;

#pragma once
class QVlcPlayer
{
public:
	QVlcPlayer(void);
	~QVlcPlayer(void);

	bool Play(HWND hwnd,string strPath,bool bLocal=false);             // ����·��ΪstrPath���ļ�
	void Resume ();                                 // ���ţ���������
	void Pause();                                   // ��ͣ
	void Stop ();                                   // ֹͣ
	void Volume(int nVol);                          // ��������ΪnVol
	void VolumeIncrease();                          // ��������
	void VolumeReduce();                            // ������С

	void SeekTo(int nPos);                          // ����ָ��λ��nPos��0-100
	void SeekForward();                             // ����
	void SeekBackward();                            // ����

	//void SetCallback(pfnPosChanged pfn);            // �����ļ�λ�øı�ʱ�Ļص�����
	//pfnPosChanged GetCallback();                    // ��ȡ�ļ�λ�øı�ʱ�Ļص�����

	//bool    IsOpen();                               // �ļ��Ƿ�����
	//bool    IsPlaying();                            // �ļ��Ƿ����ڲ���
	//bool    IsPause();                              // �Ƿ���ͣ
	//int     GetPos();                               // ��ȡ�ļ���ǰ���ŵ�λ��
	//qint64 GetTime();                               // ��ȡʱ��
	//void SetPlayTime(qint64 time);                  // ���ò���ʱ��
	//qint64 GetLength();                             // ��ȡ����
	//int GetPlayStatue();                            // ��ȡ����״̬
	//const char* getLibVlcVersion();

    void RemoveBlack();                                //设置画面比例
private:
	libvlc_instance_t     *m_pVLC_Inst;             // VLCʵ��
	libvlc_media_player_t *m_pVLC_Player;           // VLC������
	//pfnPosChanged          m_pfn;                   // �ļ�λ�øı�ʱ�Ļص�����

    QString m_filePath;                             // �ļ�·��
	void Init();                                    // ��ʼ��
	//CString UnicodeToUTF8(const CString &strWide);  // ����ת��
	void Release();                                 // �����ڴ�
};

#endif

