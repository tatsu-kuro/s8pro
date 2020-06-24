//int DBS[5]={8192*2,8192,4096};//,2048};//data block size
//int NNN[5]={4096*2,4096,2048};//,1024};//fft number of data
//int DMAX[5]={1024,512,256};//,128};
#include "stdafx.h"
#include "stdafx.h"
#include "Syaku8.h"
#include "Syaku8Doc.h"
#include "Syaku8View.h"
#include "math.h"
#include "tuner.h"
#include <mmsystem.h>
HGLOBAL			hGMemRecBuf;
HWAVEIN			hWave			= NULL;

BUFTYPE			*wavebuf		= NULL;
LPWAVEHDR		wh[MAX_BUFFERS];
bool			bWaveOpened		= false;

/////////////////////////////////////////////////////////////////////////////
// CS8tunerView �N���X�̕`��
#if 0
int cnt=0;

void CS8tunerView::OnDraw(CDC* pDC)
{
	CS8tunerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int i,s;
	CPen *oldpen,mypen;
	static POINT po[260],oldpo[260];
	for(i=0;i<256;i++)po[i].x=oldpo[i].x=2*i;
	// �I�V���X�R�[�v�`��
	s = BUFSZ-DRAWWIDTH*(NYQUIST/11025)*4-1; // �o�b�t�@�̍Ōォ��f�[�^�𒊏o�i�x���h�~��j
	for(i=0; i<DRAWWIDTH; i++) {
		po[i].y= (((int)wavebuf[s]+32768)/128+1)*DRAWHEIGHT/512;
		// ��̂Q�s�́u+1�v�̓m�C�Y�����炷����256�̈ʂŎl�̌ܓ����Ă���
		if(po[i].y>DRAWHEIGHT) po[i].y=DRAWHEIGHT;
		s += NYQUIST/11025*4;
	}
	oldpen=(CPen*)pDC->SelectStockObject(WHITE_PEN);
	pDC->Polyline(oldpo,DRAWWIDTH);
	mypen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	pDC->SelectObject(&mypen);
	pDC->Polyline( po ,DRAWWIDTH);
	pDC->SelectObject(oldpen);
	mypen.DeleteObject();
	for(i=0;i<256;i++)oldpo[i].y=po[i].y;
	char b[100];
	sprintf(b,"%d",cnt);
	pDC->TextOut(0,300,b);
}


/////////////////////////////////////////////////////////////////////////////
// CS8tunerView �N���X�̃��b�Z�[�W �n���h��
int CS8tunerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	int i;
	// �T���v�����O�o�b�t�@���m��
	wavebuf = (BUFTYPE *) GlobalAlloc( GPTR, BUFSZ * sizeof(BUFTYPE) );
	for(i=0; i<MAX_BUFFERS; i++) wh[i] = NULL;
	return 0;
}
void CS8tunerView::OnDestroy() 
{
	CView::OnDestroy();

	if(bWaveOpened)CloseWaveInputDevice();

	// �m�ۂ��Ă������������J��
	GlobalFree( wavebuf );
	
}
void CS8tunerView::OnRecstop() 
{
	// �ꎞ��~
	if(bWaveOpened) {
		CloseWaveInputDevice();
	}
}

void CS8tunerView::OnRecstart() 
{
	// �J�n
	if(!bWaveOpened) {
		if(bWaveOpened = OpenWaveInputDevice(m_hWnd))
			StartRecording();
	}
}

LRESULT CS8tunerView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
//	CDC *pDC;
//	CPoint po;
	switch(message)
	{
		case WIM_DATA:
			MSG		 msg;
			MMRESULT rc;

			if(wavebuf != NULL && bWaveOpened) {
				int i,j;

				for(i=0; i<MAX_BUFFERS; i++) {
					if(((LPWAVEHDR)lParam)->lpData == wh[i]->lpData) break;
				}
				for(j=0; j<BUFDIV; j++) {
					CopyMemory(wavebuf+BUFSZ/BUFDIV*j,
						   wh[(i-(BUFDIV-1)+j)&(MAX_BUFFERS-1)]->lpData,
						   BUFSZ/BUFDIV*sizeof(BUFTYPE));
				}

				// �f�[�^�o�b�t�@�u���b�N���ė��p����
				rc = waveInPrepareHeader(hWave, (LPWAVEHDR)lParam, sizeof(WAVEHDR));
				if (rc == MMSYSERR_NOERROR)
					rc = waveInAddBuffer(hWave, (LPWAVEHDR)lParam, sizeof(WAVEHDR));
				if (rc != MMSYSERR_NOERROR) {
					CloseWaveInputDevice();
				}
				cnt++;
				// �`�揈��
//				PlotSpectrum(hWnd, PlotMode);

				// �`�揈�����ɗ��܂����`��v�����폜����
				while( PeekMessage(&msg,m_hWnd, WIM_DATA, WIM_DATA, PM_REMOVE) ) {
					// �f�[�^�o�b�t�@�u���b�N���ė��p����
					rc = waveInPrepareHeader(hWave, (LPWAVEHDR)msg.lParam, sizeof(WAVEHDR));
					if (rc == MMSYSERR_NOERROR)
						rc = waveInAddBuffer(hWave, (LPWAVEHDR)msg.lParam, sizeof(WAVEHDR));
					if (rc != MMSYSERR_NOERROR) {
						CloseWaveInputDevice();
						break;
					}
				}
			}
//			InvalidateRect(hWnd, NULL, TRUE);
			InvalidateRect(NULL);
			break;
	}

	return CView::WindowProc(message, wParam, lParam);
}
#endif
//*******************************************************************
// �^���f�o�C�X���J�� - ����������TRUE��Ԃ�
//*******************************************************************

bool OpenWaveInputDevice(
	HWND hWnd)
{
	WAVEFORMATEX wfx;
	MMRESULT rc;
	char err[200];
	wfx.wFormatTag		= WAVE_FORMAT_PCM;		// PCM without any compression
	wfx.nChannels		= 1;					// 1channel (Monaural)
	wfx.nSamplesPerSec	= NYQUIST * 2;			// (Hz)  22050  NYQUIST			11025
	wfx.wBitsPerSample	= sizeof(BUFTYPE) * 8;	// (bits/channel) 16  BUFSZ			8192
	wfx.nBlockAlign		= (WORD)(wfx.nChannels * wfx.wBitsPerSample / 8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize			= 0;

	hWave = NULL;
//	return ( waveInOpen(&hWave, WAVE_MAPPER, &wfx, (DWORD)(void*)WaveInProc, (DWORD)hWnd, CALLBACK_FUNCTION)
//			 == MMSYSERR_NOERROR );
	rc=waveInOpen(&hWave, WAVE_MAPPER, &wfx, (DWORD)hWnd,NULL,CALLBACK_WINDOW);
	if (rc != MMSYSERR_NOERROR){
		waveInGetErrorText(rc, err, 128);
		MessageBox(NULL, err,"waveInOpenErr", MB_OK);
		return false;
	}
	return true;
}

//*******************************************************************
// �^���J�n
//*******************************************************************
//HGLOBAL			hGMemRecBuf;
void StartRecording(void)
{
	MMRESULT rc;
	int		 i;
	LPSTR	 h;

	//*******************************************************************
	// �^���o�b�t�@���m�ۂ���
	//*******************************************************************
	if( ( hGMemRecBuf = GlobalAlloc( GPTR, ( sizeof(WAVEHDR) + BUFSZ/BUFDIV*sizeof(BUFTYPE) ) * MAX_BUFFERS ) )	== NULL ) {
		waveInClose(hWave);
		bWaveOpened = false;
		return;
	}

	h = (LPSTR) hGMemRecBuf;
	for (i = 0; i < MAX_BUFFERS; i++) {
	   wh[i] = (LPWAVEHDR) h;
	   h += sizeof(WAVEHDR);
	   wh[i]->lpData = h;
	   h += ( wh[i]->dwBufferLength = BUFSZ / BUFDIV * sizeof(BUFTYPE) );
	}

	// �o�b�t�@�u���b�N���������āA���̓L���[�ɒǉ�����
	for (i=0; i<MAX_BUFFERS; i++) {
		rc = waveInPrepareHeader(hWave, wh[i], sizeof(WAVEHDR));

		// ���̓L���[�Ƀo�b�t�@��ǉ�����
		if (rc == MMSYSERR_NOERROR)
			rc = waveInAddBuffer(hWave, wh[i], sizeof(WAVEHDR));

	}

	// �^�����J�n����
	if (rc == MMSYSERR_NOERROR)
		rc = waveInStart(hWave);

	if (rc != MMSYSERR_NOERROR) {
		CloseWaveInputDevice();	 // ���蓖�Ă�ꂽ���������������
		return;
	}
}

//*******************************************************************
// �^���f�o�C�X�����
//*******************************************************************
void CloseWaveInputDevice(void)
{
	int i;

	// �^�����~����
	waveInStop(hWave);
	waveInReset(hWave);

	// �w�b�_��񏀔���Ԃɂ���
	for(i=0; i<MAX_BUFFERS; i++) {//MAX_BUFFERS 8
		waveInUnprepareHeader(hWave, wh[i], sizeof(WAVEHDR));
	}

	waveInClose(hWave);

	//*******************************************************************
	// ���蓖�Ă��^���o�b�t�@�����
	//*******************************************************************
	if( hGMemRecBuf != NULL ) GlobalFree( hGMemRecBuf );
	for (i = 0; i < MAX_BUFFERS; i++) {
		wh[i] = NULL;
	}

	bWaveOpened = false;
}

#if 0



//*******************************************************************
// �֐��v���g�^�C�v
//*******************************************************************
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
void DrawGauge(HWND,HBITMAP,DWORD);
void CALLBACK WaveInProc(HWAVEIN,UINT,DWORD,DWORD,DWORD);
void PlotSpectrum(HWND,DWORD);

//*******************************************************************
// MainWndProc - ���C���E�B���h�E�̃��b�Z�[�W����
	  // �R���X�g���N�^
	  case WM_CREATE: {
		TOOLINFO	 ti;
		LPLOGPALETTE lpPal;
		LPDWORD		 dp;
		LPDWORD		 sp;
		int			 i;
		RECT		 r;
		HDC			 hDC;


		// �T���v�����O�o�b�t�@���m��
		wavebuf = (BUFTYPE *) GlobalAlloc( GPTR, BUFSZ * sizeof(BUFTYPE) );
		for(i=0; i<MAX_BUFFERS; i++) wh[i] = NULL;

		{
			// "SNDVOL32.EXE" �̑��݃`�F�b�N
			LPTSTR result;
			result = (LPTSTR)GlobalAlloc( GMEM_FIXED, 1024 );
			if( (UINT)FindExecutable( TEXT("SNDVOL32.EXE"), NULL, result ) < 32 ) {
				SendMessage(hToolWnd, TB_SETSTATE, IDM_VOLCTRL, TBSTATE_INDETERMINATE);
			}
			GlobalFree( result );
		}
		// �ꎞ��~��Ԃ���������
		SendMessage(hToolWnd, TB_SETSTATE, IDM_PAUSE, TBSTATE_ENABLED);
		PostMessage(hWnd, WM_COMMAND, IDM_PAUSE, 0);


	  // �f�X�g���N�^
	  case WM_DESTROY:
		// �v���Z�X�̗D�揇�ʂ� NORMAL �ɖ߂�
		// �i�����Ȃ��΃E�B���h�E��������܂łɎ��Ԃ������邱�Ƃ�����j
		SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );

		// WAVE�f�o�C�X�����
		PostQuitMessage(0);
		break;

	  // �^���o�b�t�@����t�ɂȂ������̏���
	  // �c�[���o�[�̃{�^���ɉ������鏈��
	  case WM_COMMAND:
		switch( LOWORD(wParam) ) {

		  // �u��e�v�{�^���̏���
		  case IDM_LOCKWND: {
			if( SendMessage(hToolWnd, TB_GETSTATE, LOWORD(wParam), 0) & TBSTATE_CHECKED ) {
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 1);
				// Window���g�őO�ʕ\���h�ɐݒ�
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				// �v���Z�X�̗D�揇�ʂ� HIGH �ɐݒ�
				SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
			} else {
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 0);
				// �g�őO�ʕ\���h������
				SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				// �v���Z�X�̗D�揇�ʂ� NORMAL �ɐݒ�
				SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );
			}
			break;
		  }

		  // �u�d���v�{�^���̏���
		  /* �� �u�ꎞ��~�v�{�^���ɕύX
		  case IDM_POWER: {
			if( SendMessage(hToolWnd, TB_GETSTATE, LOWORD(wParam), 0) & TBSTATE_CHECKED ) {
				// �d���n�m
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 3);
				if(!bWaveOpened) {
					bWaveOpened = OpenWaveInputDevice(hWnd);
					if(!bWaveOpened) {
						SendMessage(hToolWnd, TB_SETSTATE, LOWORD(wParam),TBSTATE_ENABLED);
						SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 2);
						break;
					}
					StartRecording();
				}
			} else {
				// �d���n�e�e
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 2);
				if(bWaveOpened) {
					CloseWaveInputDevice();
				}
			}
			break;
		  }
		  */

		  // �u�ꎞ��~�v�{�^���̏���


		  // �u�^�����x�������v�{�^���̏���
		  case IDM_VOLCTRL:
			break;


//*******************************************************************
// �X�y�N�g����DIB�o�b�t�@�ɕ`�悷��
//	�ȉ��̃O���[�o���ϐ����g�p�F
//		BUFTYPE wavebuf[BUFSZ]		�����T���v�����O�f�[�^
//		double fnWFilter[BUFSZ/2]	���֐��e�[�u��
//		int ipTab[2+SQRT_BUFSZ2]	FFT sin/cos table
//		double wTab[BUFSZ/2]		FFT sin/cos table
//		double aFFT[BUFSZ]			FFT data
//		int ipTab2[2+SQRT_BUFSZ2]	FFT sin/cos table
//		double wTab2[BUFSZH/2]		FFT sin/cos table
//		double aFFT2[BUFSZH]		FFT data
//		int logtbl[DRAWDIVS+1]		���K�ϊ��p�ΐ��e�[�u��
//		LPBYTE lpBits				DIB�o�b�t�@�C���[�W���ւ̃|�C���^
//*******************************************************************
void PlotSpectrum(
	HWND hWnd,
	DWORD PlotMode // PM_NOTATE | PM_OSCILLO
) {
	int i, j, s, d;
	double dt;

}


//*******************************************************************
// WaveIn�R�[���o�b�N�֐�
// (��) ���̊֐����ł͌���ꂽAPI�����Ăׂ����}�ɕ��A����K�v������
//*******************************************************************




void CS8tunerView::OnRecvolume() 
{
	ShellExecute( NULL, NULL, TEXT("SNDVOL32.EXE"), TEXT("/RECORD"), NULL, SW_SHOWNORMAL );
}
#endif