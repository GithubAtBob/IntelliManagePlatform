#include "QVlcPlayer.h"


#include "libvlc.h"
#include "libvlc_media.h"
#include "libvlc_media_player.h"

QVlcPlayer::QVlcPlayer(void)
{
    m_pVLC_Inst = NULL;
    m_pVLC_Player = NULL;
    m_filePath = "";
}


QVlcPlayer::~QVlcPlayer(void)
{
    Release();
}

void QVlcPlayer::Init()
{
    //VLC实例初始化
    if (!m_pVLC_Inst)
    {
        m_pVLC_Inst = libvlc_new(0,NULL);
    }

    m_filePath = "";
}

void QVlcPlayer::Release()
{
    //释放
    if (m_pVLC_Inst)
    {
        //libvlc_release(m_pVLC_Inst);
    }
}

void QVlcPlayer::RemoveBlack()
{
        //可以设置视频的比例
        //    "4:3"
        //    "1:1"
        //    "16:10"
        //    "2.21:1"
        //    "2.35:1"
        //    "2.39:1"
        //    "5:4"
        if(!m_pVLC_Player)
            return;

        libvlc_video_set_aspect_ratio(m_pVLC_Player,"16:10");
}

bool QVlcPlayer::Play(HWND hwnd,string strPath,bool bLocal)
{
    if (!m_pVLC_Inst)
    {
        this->Init();
    }

    if ( strPath.length() == 0 || !m_pVLC_Inst)
    {
        return false;
    }

    Stop();

    bool bRet = false;
    //通过文件路径创建一个媒体实例
    libvlc_media_t *vlc_media = NULL;
    if (bLocal)
        vlc_media = libvlc_media_new_path(m_pVLC_Inst,strPath.c_str());     //本地
    else
        vlc_media = libvlc_media_new_location(m_pVLC_Inst,strPath.c_str()); //网络

    if (vlc_media)
    {

        m_pVLC_Player = libvlc_media_player_new_from_media(vlc_media);
        if (m_pVLC_Player)
        {
            libvlc_media_player_set_hwnd(m_pVLC_Player,hwnd); //设置播放窗口给播放器

            libvlc_media_player_play(m_pVLC_Player);

            libvlc_set_fullscreen(m_pVLC_Player,1);

            libvlc_video_set_mouse_input(m_pVLC_Player,false);
            bRet = true;
        }
        libvlc_media_release(vlc_media);
    }
    return bRet;
}

void QVlcPlayer::Resume()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_play(m_pVLC_Player);
    }
}

void QVlcPlayer::Pause()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_pause(m_pVLC_Player);
    }
}

void QVlcPlayer::Stop()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop(m_pVLC_Player);    /*stop playing*/
        libvlc_media_player_release(m_pVLC_Player); /*Free the media_player*/
        m_pVLC_Player = NULL;
    }
}

void QVlcPlayer::SeekTo(int nPos)
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_set_position(m_pVLC_Player, nPos/(float)100.0);
    }
}

void QVlcPlayer::SeekForward()
{
    if (m_pVLC_Player)
    {
        float fIndex = 1.0;
        float frate = libvlc_media_player_get_rate(m_pVLC_Player);
        frate = frate + fIndex;
        libvlc_media_player_set_rate(m_pVLC_Player,frate);
    }
}

void QVlcPlayer::SeekBackward()
{
    if (m_pVLC_Player)
    {
        float fIndex = 1.0;
        float frate = libvlc_media_player_get_rate(m_pVLC_Player);
        frate = frate - fIndex;
        libvlc_media_player_set_rate(m_pVLC_Player,frate);
    }
}

